#!/usr/bin/env python3
import cgi
import os
from http import cookies
import sys

# get the form values
form = cgi.FieldStorage()
new_text = form.getvalue('textColor')
new_bg = form.getvalue('bgColor')

# Read cookies from environment
cookie = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))

# Extract cookie values safely
text_color = cookie.get('textColor')
bg_color = cookie.get('bgColor')
text_value = text_color.value if text_color else '#ff0000'
bg_value = bg_color.value if bg_color else '#00ff00'

# If user submitted new values, set cookies and redirect
if new_text is not None or new_bg is not None:
    headers = "HTTP/1.1 303 See Other\r\n"
    if new_text and text_value != new_text:
        headers += f"Set-Cookie: textColor={new_text}; Path=/\r\n"
    if new_bg and bg_value != new_bg:
        headers += f"Set-Cookie: bgColor={new_bg}; Path=/\r\n"
    headers += "Location: /cgi-bin/cookie_set.py\r\n"
    headers += "\r\n"
    sys.stdout.write(headers)
    sys.exit(0)

body = f"""<html>
<head>
    <title>Color Cookie Setter</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            background-color: {bg_value};
            color: {text_value};
            height: 100vh;
            margin: 0;
            display: flex;
            align-items: center;
            justify-content: center;
        }}

        form {{
            background: rgba(255, 255, 255, 0.9);
            padding: 40px;
            border-radius: 15px;
            box-shadow: 0 8px 25px rgba(0, 0, 0, 0.2);
            display: flex;
            flex-direction: column;
            align-items: center;
            min-width: 300px;
            border: 2px solid {text_value};
            backdrop-filter: blur(10px);
        }}

        h1 {{
            text-align: center;
            width: 100%;
            margin-bottom: 30px;
            font-size: 24px;
            color: {text_value};
            font-weight: bold;
        }}

        label {{
            display: flex;
            align-items: center;
            margin-bottom: 20px;
            font-size: 16px;
            color: {text_value};
            width: 100%;
            justify-content: space-between;
        }}

        label span {{
            font-weight: 500;
        }}

        input[type="color"] {{
            border: 2px solid {text_value};
            border-radius: 8px;
            width: 50px;
            height: 35px;
            cursor: pointer;
            transition: all 0.3s ease;
        }}

        input[type="color"]:hover {{
            border-color: {bg_value};
            transform: scale(1.05);
        }}

        input[type="submit"] {{
            padding: 12px 30px;
            font-size: 16px;
            border: 2px solid {text_value};
            border-radius: 25px;
            background-color: {text_value};
            color: {bg_value};
            cursor: pointer;
            margin-top: 10px;
            transition: all 0.3s ease;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.3);
        }}

        input[type="submit"]:hover {{
            background-color: {bg_value};
            color: {text_value};
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(0, 0, 0, 0.4);
        }}

        .demo-text {{
            position: absolute;
            top: 20px;
            left: 20px;
            font-size: 18px;
            font-weight: bold;
            color: {text_value};
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.3);
        }}
    </style>
</head>
<body>
    <form method="POST">
        <h1>Customize Colors</h1>
        <label><span>Text Color:</span>
            <input type="color" name="textColor" value="{text_value}">
        </label>
        <label><span>Background Color:</span>
            <input type="color" name="bgColor" value="{bg_value}">
        </label>
        <input type="submit" value="Apply">
    </form>
</body>
</html>"""

headers = "HTTP/1.1 200 OK\r\n"
headers += "Content-Type: text/html\r\n"
headers += f"Content-Length: {len(body.encode('utf-8'))}\r\n"
headers += "\r\n"

sys.stdout.write(headers + body)