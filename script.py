import socket
import time

request_lines = [
    "POST /your/path HTTP/1.1\r\n",
    "Host: localhost\r\n",
    "Transfer-Encoding: chunked\r\n",
    "Content-Type: text/plain\r\n",
    "\r\n",
    "5\r\n",
    "Hello6\r\n",   # Intentional error: declared 5, sending 6 bytes
    "4\r\n",
    "Test\r\n",
    "0\r\n",
    "\r\n"
]

with socket.create_connection(("localhost", 8080)) as sock:
    for line in request_lines:
        sock.sendall(line.encode())
        print(f"Sent: {line.strip()}")
        time.sleep(0.5)  # delay to simulate streaming
    response = sock.recv(4096)
    print("Response from server:")
    print(response.decode())
