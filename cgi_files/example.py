#!/usr/bin/env python3
import os
import sys
import tempfile
import uuid
import cgi

def main():
    # Read cookies
    cookies = os.environ.get("HTTP_COOKIE", "")
    session_id = None
    if "SESSION_ID=" in cookies:
        for c in cookies.split(";"):
            key, _, value = c.strip().partition("=")
            if key == "SESSION_ID":
                session_id = value
                break

    # If no session, create one
    if not session_id:
        session_id = str(uuid.uuid4())
        set_cookie = f"Set-Cookie: SESSION_ID={session_id}; Path=/; HttpOnly\r\n"
    else:
        set_cookie = ""

    # Use FieldStorage to parse POST and GET parameters
    form = cgi.FieldStorage()
    message = form.getvalue("message", "")

    # Save message to a temp file if present
    tmp_file_path = None
    if message:
        fd, tmp_file_path = tempfile.mkstemp(prefix="cgi_body_", dir="/tmp")
        with os.fdopen(fd, "w") as f:
            f.write(message)

    # Gradient background
    bg_gradient = "rgb(17 24 39 / var(--tw-bg-opacity, 1))"

    # Build HTML response
    body = f"""
    <html>
    <head>
        <title>Advanced CGI</title>
        <style>
            body {{
                background: {bg_gradient};
                font-family: Arial, sans-serif;
                padding: 20px;
                color: #fff;
            }}
            h1 {{
                color: #fff;
                text-align: center;
            }}
            .box {{
                background: rgba(255, 255, 255, 0.15);
                border-radius: 10px;
                padding: 20px;
                margin: auto;
                width: 60%;
                backdrop-filter: blur(10px);
                box-shadow: 0 4px 15px rgba(0,0,0,0.2);
            }}
            .info {{
                margin: 10px 0;
                padding: 10px;
                background: rgba(255,255,255,0.25);
                border-radius: 5px;
                color: #fff;
            }}
            .body-content {{
                margin: 10px 0;
                padding: 10px;
                background: rgba(255,255,255,0.2);
                border-radius: 5px;
                white-space: pre-wrap;
                color: #fff;
            }}
            form {{
                margin-top: 20px;
                text-align: center;
            }}
            input[type="text"] {{
                padding: 8px;
                width: 70%;
                border: 1px solid #fff;
                border-radius: 5px;
                background: rgba(255,255,255,0.2);
                color: #fff;
            }}
            input::placeholder {{
                color: #eee;
            }}
            button {{
                padding: 8px 16px;
                border: none;
                background: #fff;
                color: #3b82f6;
                font-weight: bold;
                border-radius: 5px;
                cursor: pointer;
            }}
            button:hover {{
                background: #e0e0ff;
            }}
        </style>
    </head>
    <body>
        <div class="box">
            <h1>Advanced CGI Script</h1>
            <div class="info"><b>Session ID:</b> {session_id}</div>
            <div class="info"><b>Cookies:</b> {cookies or "(none)"}</div>
            <div class="info"><b>POST message saved at:</b> {tmp_file_path if tmp_file_path else "No message received"}</div>
            <div class="body-content"><b>Submitted Content:</b>\n{message if message else "(empty)"}</div>
            <form method="POST">
                <input type="text" name="message" placeholder="Type something here">
                <button type="submit">Submit</button>
            </form>
        </div>
    </body>
    </html>
    """

    # Build headers
    headers = "HTTP/1.1 200 OK\r\n"
    headers += "Content-Type: text/html\r\n"
    headers += "Connection: close\r\n"
    headers += set_cookie
    headers += f"Content-Length: {len(body.encode('utf-8'))}\r\n"
    headers += "\r\n"

    # Output response
    sys.stdout.write(headers + body)

if __name__ == "__main__":
    main()
