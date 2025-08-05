#!/usr/bin/env python3
import cgi
import cgitb
import os
from http import cookies

cgitb.enable()

# Load cookies from browser
cookie = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))

# Fallback to default colors if cookie doesn't exist
current_text = cookie.get('textColor')
current_bg = cookie.get('bgColor')

text_value = current_text.value if current_text else '#000000'
bg_value = current_bg.value if current_bg else '#ffffff'


form = cgi.FieldStorage()
text_color = form.getvalue('textColor')
bg_color = form.getvalue('bgColor')

print("Content-type: text/html")
# Always print cookies if values were submitted
if text_color:
    print(f"Set-Cookie: textColor={text_color}; Path=/")
    text_value = text_color
if bg_color:
    print(f"Set-Cookie: bgColor={bg_color}; Path=/")
    bg_value = bg_color

# Start HTML
print("Status: 303 See Other")
print("Location: /cgi-bin/coo.py")
print()
print(f"""<!DOCTYPE html>
<html>
<head>
    <title>Color Cookie Setter</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            max-width: 800px;
            margin: 0 auto;
            padding: 20px;
            color: {text_value};
            background-color: {bg_value};
        }}
    </style>
</head>
<body>
    <h1>Color Settings</h1>
    <form method="POST">
        <label>Text Color: <input type="color" name="textColor" value="{text_value}"></label><br><br>
        <label>Background Color: <input type="color" name="bgColor" value="{bg_value}"></label><br><br>
        <input type="submit" value="Apply">
    </form>
</body>
</html>
""")