#!/usr/bin/env python3
import cgi
import cgitb
import os
from http import cookies

cgitb.enable()

print("Content-type: text/html")

cookie = cookies.SimpleCookie()
cookie.load(os.environ.get('HTTP_COOKIE', ''))

form = cgi.FieldStorage()
text_color = form.getvalue('textColor', '')
bg_color = form.getvalue('bgColor', '')

if text_color and bg_color:
    print(f"Set-Cookie: textColor={text_color}; Path=/")
    print(f"Set-Cookie: bgColor={bg_color}; Path=/")
    cookie = cookies.SimpleCookie()
    cookie['textColor'] = text_color
    cookie['bgColor'] = bg_color

current_text = cookie.get('textColor', cookies.SimpleCookie()).value if 'textColor' in cookie else '#000000'
current_bg = cookie.get('bgColor', cookies.SimpleCookie()).value if 'bgColor' in cookie else '#ffffff'

html = f"""
<!DOCTYPE html>
<html>
<head>
    <title>Color Cookie Setter</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            max-width: 800px;
            margin: 0 auto;
            padding: 20px;
            color: {current_text};
            background-color: {current_bg};
            transition: all 0.3s ease;
        }}
        .form-group {{
            margin-bottom: 15px;
        }}
        label {{
            display: inline-block;
            width: 120px;
        }}
        .result {{
            padding: 10px;
            margin: 15px 0;
            border-radius: 4px;
        }}
        .success {{
            background-color: #d4edda;
            color: #155724;
        }}
        .error {{
            background-color: #f8d7da;
            color: #721c24;
        }}
    </style>
</head>
<body>
    <h1>Color Settings</h1>
"""

if text_color and bg_color:
    html += f'''
    <div class="result success">
        Colors set successfully!<br>
        Text: {current_text}<br>
        Background: {current_bg}
    </div>
    '''
elif text_color or bg_color:
    html += '<div class="result error">Please provide both text and background colors</div>'

html += f"""
    <form method="POST" action="">
        <div class="form-group">
            <label for="textColor">Text Color:</label>
            <input type="color" id="textColor" name="textColor" value="{current_text}">
        </div>
        <div class="form-group">
            <label for="bgColor">Background Color:</label>
            <input type="color" id="bgColor" name="bgColor" value="{current_bg}">
        </div>
        <button type="submit">Apply Colors</button>
    </form>
    <div style="margin-top: 30px;">
        <h2>Current Colors:</h2>
        <p>This text is in <span style="font-weight: bold;">{current_text}</span></p>
        <p>Page background is <span style="font-weight: bold;">{current_bg}</span></p>
    </div>
</body>
</html>
"""

print(html)