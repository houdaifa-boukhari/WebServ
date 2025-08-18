#!/usr/bin/env python3
"""
Webserv automatic tester for 42 Webserv project (mandatory features).
Save as webserv_tester.py and run with Python 3.8+.

Requirements:
    pip install requests

Usage examples:
    python3 webserv_tester.py --base http://127.0.0.1:8080
    python3 webserv_tester.py --base http://127.0.0.1:8080 --start-cmd "./webserv conf/default.conf" --wait 1
    python3 webserv_tester.py --base http://127.0.0.1:8080 --ports 8080 8081

Notes / limitations:
 - This script tests external behaviour only (correct HTTP status codes, uploads, chunked requests,
   CGI response, multiple ports reachable, directory listing, etc.). It CANNOT directly verify that
   your server uses poll()/epoll()/kqueue or that it is non-blocking internally — instead it
   performs concurrency/stress tests to increase confidence in non-blocking behavior.
 - Adjust endpoints/paths below to match your configuration (upload route, CGI path, etc.).
"""
import argparse
import subprocess
import sys
import time
import socket
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
import requests
import random
import string

# ---------- Configuration (edit if your config uses different paths) ----------
DEFAULT_BASE = "http://127.0.0.1:8080"
UPLOAD_PATH = "/upload"        # route that accepts file uploads (adjust if needed)
STATIC_TEST_PATH = "/index.html"
DIR_LISTING_PATH = "/files/"   # a directory route used to test directory listing
CGI_TEST_PATH = "/cgi-files/test.py"  # an example CGI script path you should provide in config
DELETE_TEST_PATH = "/tmp-to-delete.txt"  # URL path used for testing DELETE (script will create + delete)
MAX_BODY_TEST_PATH = "/big"    # route that accepts POST body (adjust if needed)
# -----------------------------------------------------------------------------

TIMEOUT = 5  # seconds for requests


def random_text(n=16):
    return "".join(random.choice(string.ascii_letters + string.digits) for _ in range(n))


def start_server(start_cmd, wait):
    proc = subprocess.Popen(start_cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    print(f"[+] Started server using: {start_cmd} (pid {proc.pid}), waiting {wait}s for it to come up")
    time.sleep(wait)
    return proc


def stop_server(proc):
    if proc:
        proc.terminate()
        try:
            proc.wait(timeout=3)
        except Exception:
            proc.kill()


def assert_ok(cond, message):
    if cond:
        print(f"  ✅ {message}")
        return True
    else:
        print(f"  ❌ {message}")
        return False


def test_connection(base):
    print("Test: connection and root GET")
    try:
        r = requests.get(base + "/", timeout=TIMEOUT)
        return assert_ok(r.status_code in (200, 301, 302, 404), f"HTTP GET / returned {r.status_code}")
    except Exception as e:
        return assert_ok(False, f"Connection to {base} failed: {e}")


def test_static_file(base, path=STATIC_TEST_PATH):
    print(f"Test: static file GET {path}")
    try:
        r = requests.get(base + path, timeout=TIMEOUT)
        ok = (r.status_code == 200 and len(r.content) > 0)
        return assert_ok(ok, f"GET {path} -> {r.status_code}, {len(r.content)} bytes")
    except Exception as e:
        return assert_ok(False, f"GET {path} failed: {e}")


def test_directory_listing(base, path=DIR_LISTING_PATH):
    print(f"Test: directory listing at {path}")
    try:
        r = requests.get(base + path, timeout=TIMEOUT)
        # If listing enabled you'll typically see filenames or '<a href=' in HTML. If disabled server might return 403 or custom page.
        if r.status_code == 200 and (b"<a href" in r.content or b"Index of" in r.content):
            return assert_ok(True, f"Directory listing seems enabled ({r.status_code})")
        elif r.status_code in (403, 404):
            return assert_ok(True, f"Directory listing disabled or not found ({r.status_code})")
        else:
            return assert_ok(False, f"Unexpected response for directory listing: {r.status_code}")
    except Exception as e:
        return assert_ok(False, f"Directory listing test failed: {e}")


def test_upload(base, path=UPLOAD_PATH):
    print(f"Test: file upload to {path}")
    content = b"hello-webserv-test-" + random_text(8).encode()
    files = {"file": ("testfile.txt", content)}
    try:
        r = requests.post(base + path, files=files, timeout=TIMEOUT)
        # After upload, try to fetch by name. Behavior depends on your config: many configs save to a directory that is served.
        ok = r.status_code in (200, 201, 204, 302)
        return assert_ok(ok, f"Upload POST returned {r.status_code}")
    except Exception as e:
        return assert_ok(False, f"Upload failed: {e}")


def test_delete(base, path=DELETE_TEST_PATH):
    print(f"Test: create a file via POST and DELETE it at {path}")
    # Create a file by uploading to a path or by POSTing content to that resource
    test_content = random_text(64)
    target_url = base + path
    try:
        # Try PUT-like create via POST (server implementations vary). If your server needs a particular route, adapt this.
        r_create = requests.post(target_url, data=test_content, timeout=TIMEOUT)
        created = r_create.status_code in (200, 201, 204, 302)
        if not created:
            print("  ⚠️ Create step returned", r_create.status_code, "; continuing to attempt DELETE anyway.")
        r_del = requests.delete(target_url, timeout=TIMEOUT)
        ok = r_del.status_code in (200, 202, 204, 404)  # 404 might mean it wasn't created — accept as non-crash
        return assert_ok(ok, f"DELETE {path} -> {r_del.status_code}")
    except Exception as e:
        return assert_ok(False, f"DELETE test failed: {e}")


def send_raw_chunked(host, port, path, body_bytes):
    """Open raw socket and send Transfer-Encoding: chunked request. Return (status, body)."""
    s = socket.create_connection((host, port), timeout=TIMEOUT)
    try:
        req_lines = [
            f"POST {path} HTTP/1.1",
            f"Host: {host}:{port}",
            "User-Agent: webserv-tester",
            "Transfer-Encoding: chunked",
            "Connection: close",
            "", ""
        ]
        s.sendall("\r\n".join(req_lines).encode())
        # send body in chunks of random sizes
        idx = 0
        while idx < len(body_bytes):
            chunk = body_bytes[idx: idx + 16]
            s.sendall(("{:x}\r\n".format(len(chunk))).encode())
            s.sendall(chunk)
            s.sendall(b"\r\n")
            idx += len(chunk)
        # send zero-length chunk to finish
        s.sendall(b"0\r\n\r\n")
        # read response
        resp = b""
        while True:
            part = s.recv(4096)
            if not part:
                break
            resp += part
        # parse status line
        first_line = resp.split(b"\r\n", 1)[0]
        try:
            status = int(first_line.split()[1])
        except Exception:
            status = None
        # body after double CRLF
        body = resp.split(b"\r\n\r\n", 1)[1] if b"\r\n\r\n" in resp else b""
        return status, body
    finally:
        s.close()


def test_chunked(base, path="/chunked"):
    print("Test: chunked Transfer-Encoding request")
    # parse host/port from base
    if not base.startswith("http://"):
        return assert_ok(False, "Chunked test only supports http:// base url")
    host_port = base[len("http://"):]
    if "/" in host_port:
        host_port = host_port.split("/", 1)[0]
    if ":" in host_port:
        host, port = host_port.split(":", 1)
        port = int(port)
    else:
        host = host_port
        port = 80
    body = b"The quick brown fox jumps over the lazy dog." * 10
    try:
        status, body_out = send_raw_chunked(host, port, path, body)
        ok = status in (200, 201, 204, 413)  # 413 if server checks size
        return assert_ok(ok, f"Chunked request returned {status}")
    except Exception as e:
        return assert_ok(False, f"Chunked request failed: {e}")


def test_cgi(base, path=CGI_TEST_PATH):
    print(f"Test: CGI execution at {path}")
    try:
        r = requests.get(base + path, timeout=TIMEOUT)
        ok = (r.status_code == 200 and len(r.text.strip()) > 0)
        return assert_ok(ok, f"CGI {path} -> {r.status_code}, length {len(r.text)}")
    except Exception as e:
        return assert_ok(False, f"CGI test failed: {e}")


def test_max_body_size(base, path=MAX_BODY_TEST_PATH, limit_bytes=1024*1024):
    print("Test: max body size (send large body expecting 413 or configured rejection)")
    big = b"x" * (limit_bytes + 100)  # slightly over
    try:
        r = requests.post(base + path, data=big, timeout=TIMEOUT)
        ok = r.status_code in (413, 400, 200)  # 413 preferred; some servers handle differently
        return assert_ok(ok, f"POST large body -> {r.status_code}")
    except Exception as e:
        return assert_ok(False, f"Max body size test failed: {e}")


def test_keepalive(base):
    print("Test: persistent connection (Connection: keep-alive) with two sequential requests on same socket")
    # open raw socket connection and send two simple GETs with Connection: keep-alive and read responses
    if not base.startswith("http://"):
        return assert_ok(False, "Keep-alive test only supports http:// base url")
    host_port = base[len("http://"):]
    if "/" in host_port:
        host_port = host_port.split("/", 1)[0]
    if ":" in host_port:
        host, port = host_port.split(":", 1)
        port = int(port)
    else:
        host = host_port
        port = 80
    try:
        s = socket.create_connection((host, port), timeout=TIMEOUT)
        req = f"GET / HTTP/1.1\r\nHost: {host}\r\nConnection: keep-alive\r\n\r\n"
        s.sendall(req.encode())
        resp1 = s.recv(65536)
        # send second request on same socket
        s.sendall(req.encode())
        resp2 = s.recv(65536)
        s.close()
        ok = (b"HTTP/1.1" in resp1 and b"HTTP/1.1" in resp2)
        return assert_ok(ok, "Multiple requests on same socket succeeded")
    except Exception as e:
        return assert_ok(False, f"Keep-alive test failed: {e}")


def stress_test(base, path="/", concurrency=50, requests_count=200):
    print(f"Stress test: {requests_count} requests with concurrency {concurrency}")
    def do_one(i):
        try:
            r = requests.get(base + path, timeout=TIMEOUT)
            return r.status_code
        except Exception:
            return None
    counts = {"ok": 0, "failed": 0}
    with ThreadPoolExecutor(max_workers=concurrency) as ex:
        futures = [ex.submit(do_one, i) for i in range(requests_count)]
        for f in as_completed(futures):
            st = f.result()
            if st and st < 500:
                counts["ok"] += 1
            else:
                counts["failed"] += 1
    ok = counts["failed"] == 0
    print(f"  -> ok: {counts['ok']}, failed: {counts['failed']}")
    return assert_ok(ok, f"Stress test: {requests_count} total requests; failed {counts['failed']}")


def test_ports(base, ports):
    print("Test: multiple ports reachability")
    ok_all = True
    for p in ports:
        try:
            r = requests.get(base.replace(":"+base.split("://")[1].split(":")[1], f":{p}") if ":" in base.split("://")[1] else base.split("//")[0] + f"//{base.split('//')[1].split(':')[0]}:{p}", timeout=TIMEOUT)
            ok = r.status_code < 600
            ok_all = ok_all and ok
            assert_ok(ok, f"Port {p} responded with {r.status_code}")
        except Exception as e:
            ok_all = False
            print(f"  ❌ Port {p} connect failed: {e}")
    return ok_all


def main():
    parser = argparse.ArgumentParser(description="Automated Webserv tester")
    parser.add_argument("--base", "-b", default=DEFAULT_BASE, help="Base URL of server, e.g. http://127.0.0.1:8080")
    parser.add_argument("--start-cmd", help="Optional: command to start server (shell).")
    parser.add_argument("--wait", type=float, default=1.0, help="Seconds to wait after starting server.")
    parser.add_argument("--ports", nargs="+", type=int, help="List of ports to check for listening servers.")
    parser.add_argument("--no-stress", action="store_true", help="Skip stress test (long).")
    parser.add_argument("--concurrency", type=int, default=30)
    parser.add_argument("--requests", type=int, default=150)
    args = parser.parse_args()

    proc = None
    results = []

    try:
        if args.start_cmd:
            proc = start_server(args.start_cmd, args.wait)

        # sequence of tests
        results.append(("connection", test_connection(args.base)))
        results.append(("static_file", test_static_file(args.base)))
        results.append(("directory_listing", test_directory_listing(args.base)))
        results.append(("upload", test_upload(args.base)))
        results.append(("delete", test_delete(args.base)))
        results.append(("chunked", test_chunked(args.base)))
        results.append(("cgi", test_cgi(args.base)))
        results.append(("max_body", test_max_body_size(args.base)))
        results.append(("keep-alive", test_keepalive(args.base)))

        if not args.no_stress:
            results.append(("stress", stress_test(args.base, "/", concurrency=args.concurrency, requests_count=args.requests)))
        else:
            print("Skipping stress test (--no-stress)")

        if args.ports:
            results.append(("ports", test_ports(args.base, args.ports)))

        # summary
        print("\nTest summary:")
        ok_count = sum(1 for _,v in results if v)
        for name, val in results:
            print(f" - {name}: {'PASS' if val else 'FAIL'}")
        print(f"\n{ok_count}/{len(results)} tests passed.")

        if ok_count != len(results):
            sys.exit(2)
    finally:
        stop_server(proc)


if __name__ == "__main__":
    main()
