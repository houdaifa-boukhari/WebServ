# Webserv - Custom HTTP Server

## **Overview**
Webserv is a lightweight HTTP/1.1 server written in **C++98**, designed to handle multiple client connections efficiently.  
The project follows the HTTP protocol and allows serving static files, handling client requests, and executing CGI scripts.  
It is developed without external libraries (except for allowed system calls) and aims to mimic the behavior of a real web server like **NGINX**.

---

## **Features**
### **1. Server Core & Logging**
- Implements a **non-blocking** HTTP server using `poll()`, allowing multiple client connections.
- Manages sockets (`socket()`, `bind()`, `listen()`, `accept()`).
- Handles client requests and responses efficiently.
- Implements a **logging system** that records server events, requests, and errors.

### **2. Configuration & Response Handling**
- Parses a **configuration file** (`webserv.conf`) to define server behavior.
- Supports multiple **virtual servers** running on different ports.
- Handles **HTTP response generation** with correct status codes.
- Serves static files and manages error pages.

### **3. CGI & Request Processing**
- Implements request parsing for **GET, POST, and DELETE** methods.
- Supports **CGI execution** (PHP, Python, etc.) for dynamic content.
- Allows **file uploads** and **directory listing** based on configuration settings.

---