#!/usr/bin/env python3
import cgi
import cgitb
import os
from http import cookies
import json
import hashlib
import time

cgitb.enable()

SESSION_FILE = "/tmp/sessions.json"
print("http/1.1 200 OK")

def load_sessions():
    if not os.path.exists(SESSION_FILE):
        return {}
    with open(SESSION_FILE, 'r') as f:
        return json.load(f)

def save_sessions(sessions):
    with open(SESSION_FILE, 'w') as f:
        json.dump(sessions, f)

def generate_session_id():
    return hashlib.sha256(str(time.time()).encode()).hexdigest()[:16]

sessions = load_sessions()
cookie = cookies.SimpleCookie()
cookie.load(os.environ.get('HTTP_COOKIE', ''))

new_session = False
if 'session_id' in cookie:
    session_id = cookie['session_id'].value
    session_data = sessions.get(session_id, {})
else:
    session_id = generate_session_id()
    session_data = {}
    new_session = True

# Process form data
form = cgi.FieldStorage()
session_data['color'] = form.getvalue('color') if 'color' in form else session_data.get('color', '#000000')
session_data['firstname'] = form.getvalue('firstname') if 'firstname' in form else session_data.get('firstname', '')
session_data['lastname'] = form.getvalue('lastname') if 'lastname' in form else session_data.get('lastname', '')
session_data['email'] = form.getvalue('email') if 'email' in form else session_data.get('email', '')

sessions[session_id] = session_data
save_sessions(sessions)

if new_session:
    print(f"Set-Cookie: session_id={session_id}; Path=/")

print("Content-type: text/html")  # end of headers

current_color = session_data.get('color', '#000000')
html = f"""
<!DOCTYPE html>
<html>
<head>
    <title>Session Example</title>
    <style>
        body {{
            font-family: 'Arial', sans-serif;
            padding: 0;
            margin: 0;
            min-height: 100vh;
            background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%);
            color: {{current_color}};
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
        }}
        
        .container {{
            background: rgba(255, 255, 255, 0.1);
            backdrop-filter: blur(10px);
            border-radius: 20px;
            padding: 40px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
            border: 1px solid rgba(255, 255, 255, 0.2);
            max-width: 400px;
            width: 90%;
        }}
        
        h1 {{
            text-align: center;
            margin-bottom: 20px;
            font-size: 28px;
            font-weight: bold;
            text-shadow: 0 2px 4px rgba(0, 0, 0, 0.3);
        }}
        
        p {{
            text-align: center;
            margin: 10px 0;
            font-size: 16px;
            opacity: 0.9;
        }}
        
        form {{
            margin-top: 30px;
        }}
        
        label {{
            display: block;
            margin-bottom: 8px;
            font-weight: 500;
            font-size: 14px;
            text-transform: uppercase;
            letter-spacing: 1px;
        }}
        
        input[type="color"] {{
            width: 100%;
            height: 50px;
            border: none;
            border-radius: 10px;
            cursor: pointer;
            margin-bottom: 20px;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
        }}
        
        input[type="text"],
        input[type="email"] {{
            width: 100%;
            padding: 15px;
            border: none;
            border-radius: 10px;
            background: rgba(255, 255, 255, 0.2);
            color: white;
            font-size: 16px;
            margin-bottom: 20px;
            box-sizing: border-box;
            backdrop-filter: blur(5px);
        }}
        
        input[type="text"]::placeholder,
        input[type="email"]::placeholder {{
            color: rgba(255, 255, 255, 0.7);
        }}
        
        input[type="text"]:focus,
        input[type="email"]:focus {{
            outline: none;
            background: rgba(255, 255, 255, 0.3);
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(0, 0, 0, 0.3);
        }}
        
        button {{
            width: 100%;
            padding: 15px;
            background: linear-gradient(45deg, #ff6b6b, #ee5a24);
            color: white;
            border: none;
            border-radius: 25px;
            font-size: 18px;
            font-weight: bold;
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 4px 15px rgba(238, 90, 36, 0.4);
            margin-top: 10px;
        }}
        
        button:hover {{
            transform: translateY(-3px);
            box-shadow: 0 8px 25px rgba(238, 90, 36, 0.6);
        }}
        
        .footer-text {{
            margin-top: 20px;
            font-size: 14px;
            opacity: 0.8;
            text-align: center;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>Session Color Picker</h1>
        <p>Your selected color: {current_color}</p>
        <p>Current session ID: {session_id}</p>
        <form method="POST">
            <label for="color">Choose a color:</label>
            <input type="color" id="color" name="color" value="{current_color}">
            <label for="firstname">First Name:</label>
            <input type="text" id="firstname" name="firstname" value="{session_data.get('firstname', '')}" required>
            <label for="lastname">Last Name:</label>
            <input type="text" id="lastname" name="lastname" value="{session_data.get('lastname', '')}" required>
            <label for="email">Email:</label>
            <input type="email" id="email" name="email" value="{session_data.get('email', '')}" required>
            <button type="submit">Save</button>
        </form>
        <p class="footer-text">This color will persist across page reloads.</p>
    </div>
</body>
</html>"""
html_bytes = html.encode('utf-8')
print(f"Content-Length: {len(html_bytes)}")
print()
print(html)