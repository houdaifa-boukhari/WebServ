import threading
import requests
import time

def make_request(i):
	try:
		r = requests.get("http://127.0.0.1:8080/", timeout=5)
		print(f"Client {i} status: {r.status_code}")
	except Exception as e:
		print(f"Client {i} error: {type(e).__name__} - {e}")

threads = []

for i in range(50):
	t = threading.Thread(target=make_request, args=(i,))
	threads.append(t)
	t.start()
	time.sleep(0.1)  # Add a small delay to avoid overwhelming the server

for t in threads:
	t.join()