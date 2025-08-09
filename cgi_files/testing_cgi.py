#!/usr/bin/env python3

# HTML body
body = """<!DOCTYPE html>
<html>
<head>
    <title>Color Cookie Setter</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            max-width: 800px;
            margin: 0 auto;
            padding: 20px;
            color: #0aa00f;  /* Default text color */
            background-color: #ffffff;
        }
    </style>
</head>
<body>
    <h1>Color Settings</h1>
</body>
</html>"""

# Prepare headers
headers = "http/1.1 200 OK\r\n"
headers += "Content-Type: text/html\r\n"
headers += "Set-Cookie: zaba=wchta%20saba; Path=/\r\n"
headers += "Content-Length: " + str(len(body.encode("utf-8"))) + "\r\n"
headers += "\r\n"  # End of headers

# Output headers + body
print(headers + body, end="")
