#!/usr/bin/env python3

import http.cookies
import datetime

# Create a cookie
cookie = http.cookies.SimpleCookie()
cookie["bg-color"] = "blue"
cookie["bg-color"]["path"] = "/"
cookie["bg-color"]["max-age"] = 3600  # 1 hour

# Optional: Set expiration date
expires = (datetime.datetime.utcnow() + datetime.timedelta(hours=1))
cookie["bg-color"]["expires"] = expires.strftime("%a, %d-%b-%Y %H:%M:%S GMT")

# Output HTTP headers
print(cookie.output())
# print("Content-Type: text/html")
# print()
# print("<html><body><h1>Cookie has been set!</h1></body></html>")
