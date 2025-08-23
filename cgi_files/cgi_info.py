import os

# Build headers
headers = "HTTP/1.1 200 OK\r\n"
headers += "Content-Type: text/html; charset=utf-8\r\n"
headers += "Connection: close\r\n"

# Build body with styled HTML
body = """<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>CGI Environment Variables</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: rgb(17 24 39 / var(--tw-bg-opacity, 1));
            color: #f1f5f9;
            padding: 20px;
        }
        h1 {
            color: #f9fafb;
            text-align: center;
            margin-bottom: 20px;
        }
        table {
            border-collapse: collapse;
            width: 90%;
            margin: 0 auto;
            margin-top: 20px;
            background: #1f2937;
            box-shadow: 0 4px 12px rgba(0,0,0,0.3);
            border-radius: 12px;
            overflow: hidden;
        }
        th, td {
            border: 1px solid #374151;
            padding: 12px 16px;
            text-align: left;
        }
        th {
            background: linear-gradient(135deg, #6366f1, #3b82f6, #06b6d4);
            color: #fff;
            font-weight: bold;
        }
        tr:nth-child(even) {
            background: #111827;
        }
        tr:nth-child(odd) {
            background: #1f2937;
        }
        tr:hover {
            background: #374151;
        }
    </style>
</head>
<body>
    <h1>CGI Environment Variables</h1>
    <table>
        <tr><th>Variable</th><th>Value</th></tr>
"""

# Add environment variables to the table
for key, value in os.environ.items():
    body += f"<tr><td>{key}</td><td>{value}</td></tr>\n"

body += """    </table>
</body>
</html>"""

# Add Content-Length
headers += f"Content-Length: {len(body.encode('utf-8'))}\r\n"
headers += "\r\n"

# Output full response
print(headers + body)
