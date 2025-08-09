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
    print("here kkkk")
    sys.exit(0)

# Otherwise, show the form with current colors
# body = f"""<!DOCTYPE html>
# <html>
# <head>
#     <title>Color Cookie Setter</title>
# </head>
# <body style="color:{text_value}; background-color:{bg_value};">
#     <h1> text_value: {text_value} bg_value: {bg_value} </h1>
#     <form method="POST">
#         <label>Teext Color: <input type="color" name="textColor" value="{text_value}"></label><br><br>
#         <label>Background Color: <input type="color" name="bgColor" value="{bg_value}"></label><br><br>
#         <input type="submit" value="Save Colors">
#     </form>
# </body>
# </html>"""


body = f"""<html>
<head>
    <title>Color Cookie Setter</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: {text_value};
            height: 100vh;
            margin: 0;
            display: flex;
            align-items: center;
            justify-content: center;
        }}

        form {{
            background: linear-gradient(145deg, #ffffff 0%, #f8f9ff 100%);
            padding: 40px;
            border-radius: 15px;
            box-shadow: 0 8px 25px rgba(102, 126, 234, 0.3);
            display: flex;
            flex-direction: column;
            align-items: center;
            min-width: 300px;
            border: 1px solid rgba(102, 126, 234, 0.2);
        }}

        h1 {{
            text-align: center;
            width: 100%;
            margin-bottom: 30px;
            font-size: 24px;
            background: linear-gradient(45deg, #667eea, #764ba2);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            background-clip: text;
            font-weight: bold;
        }}

        label {{
            display: flex;
            align-items: center;
            margin-bottom: 20px;
            font-size: 16px;
            color: #555;
            width: 100%;
            justify-content: space-between;
        }}

        label span {{
            font-weight: 500;
        }}

        input[type="color"] {{
            border: 2px solid #e1e8ff;
            border-radius: 8px;
            width: 50px;
            height: 35px;
            cursor: pointer;
            transition: all 0.3s ease;
        }}

        input[type="color"]:hover {{
            border-color: #667eea;
            transform: scale(1.05);
        }}

        input[type="submit"] {{
            padding: 12px 30px;
            font-size: 16px;
            border: none;
            border-radius: 25px;
            background: linear-gradient(45deg, #667eea, #764ba2);
            color: white;
            cursor: pointer;
            margin-top: 10px;
            transition: all 0.3s ease;
            box-shadow: 0 4px 15px rgba(102, 126, 234, 0.4);
        }}

        input[type="submit"]:hover {{
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(102, 126, 234, 0.6);
        }}
    </style>
</head>
<body>
    zaba w chta saba
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

# if new_text and new_bg:
#     # Print headers one by one with proper line endings
#     print("Status: 303 See Other")
#     if new_text:
#         print(f"Set-Cookie: textColor={new_text}; Path=/")
#     if new_bg:
#         print(f"Set-Cookie: bgColor={new_bg}; Path=/")
#     print("Location: /cgi-bin/cookie_set.py")
#     print("Content-Type: text/html")
#     print("Content-Length: 0")
#     print()  # Empty line to separate headers from body
# else:
#     html = f"""<!DOCTYPE html>
# <html>
# <head>
#     <title>Set Colors</title>
#     <style>
#         body {{
#             font-family: Arial, sans-serif;
#             color: {text_value};
#             background-color: {bg_value};
#             padding: 2em;
#         }}
#     </style>
# </head>
# <body>
#     <h1>Choose Colors</h1>
#     <form method="POST">
#         <label>Text Color: <input type="color" name="textColor" value="{text_value}"></label><br><br>
#         <label>Background Color: <input type="color" name="bgColor" value="{bg_value}"></label><br><br>
#         <input type="submit" value="Save Colors">
#     </form>
# </body>
# </html>"""
    
#     html_bytes = html.encode('utf-8')
#     print("Content-Type: text/html")
#     print(f"Content-Length: {len(html_bytes)}")
#     print()  # Empty line to separate headers from body
#     print(html)