import http.cookies
import datetime

# Create a cookie
cookie = http.cookies.SimpleCookie()
cookie["session_id"] = "blue"
cookie["session_id"]["path"] = "/"
cookie["session_id"]["max-age"] = 7 * 24 * 60 * 60  # 7 hour

# Optional: Set expiration date
expires = (datetime.datetime.utcnow() + datetime.timedelta(days=7))
cookie["session_id"]["expires"] = expires.strftime("%a, %d-%b-%Y %H:%M:%S GMT")