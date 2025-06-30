#!/usr/bin/env python3

import http.cookies
import datetime

# Create a cookie
cookie = http.cookies.SimpleCookie()
cookie["session_id"] = "abc123"
cookie["session_id"]["path"] = "/"
cookie["session_id"]["max-age"] = 3600  # 1 hour

# Optional: Set expiration date
expires = (datetime.datetime.utcnow() + datetime.timedelta(hours=1))
cookie["session_id"]["expires"] = expires.strftime("%a, %d-%b-%Y %H:%M:%S GMT")

# Output HTTP headers
print(cookie.output())
print("Content-Type: text/html")
print()
print("<html><body><h1>Cookie has been set!</h1></body></html>")
