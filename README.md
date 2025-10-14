# WebServ

A lightweight HTTP/1.1 web server written in modern C++ (C++98 standard in this project) using non-blocking sockets and poll-based I/O. It supports multiple virtual servers, static file serving, directory listing, uploads, basic CGI, redirects, and custom error pages.

## Features

- Multiple servers per config: host, ports, and server_name
- HTTP methods: GET, POST, DELETE
- Static files with proper Content-Type and chunked transfer for large files
- Autoindex (directory listing) or index files per location
- File uploads (multipart/form-data or raw body) to a configured directory
- File deletion via DELETE (by query parameter)
- CGI execution for scripts (e.g., Python, PHP)
- Redirects (e.g., 301)
- Custom error pages
- Keep-Alive connections and basic timeouts

## Project structure

- `main.cpp` – entry point, config parsing, server boot
- `Makefile` – build rules
- `configFiles/config.conf` – example configuration
- `CoreServer/` – sockets, connections, event loop
  - `include/Server.hpp` and `src/Server.cpp`
  - `include/Connection.hpp` and `src/Connection.cpp`
  - `include/logger.hpp` and `src/Logger.cpp`
- `configParsing/` – config parser and validators
- `ParseRequest/` – HTTP request parsing and CGI execution
- `response/` – response builder and helpers
- `pages/` – static files and examples (errors, uploads, etc.)
- `cgi_files/` – sample CGI scripts (Python, PHP)

## Build

Requirements:
- Linux or macOS
- C++ compiler with C++98 support (g++/clang++)

Build the binary:

```bash
make
```

Outputs a binary named `webserv`.

Clean builds:

```bash
make clean   # object files
make fclean  # object files + binary
make re      # full rebuild
```

## Run

Run with a configuration file:

```bash
./webserv configFiles/config.conf
```

Run with no arguments to use a safe default config (127.0.0.1:8080 serving `./pages/`):

```bash
./webserv
```

Then open a browser at http://127.0.0.1:8080

## Configuration

The configuration format is nginx-like. Top-level blocks are `server { ... }`, each containing `location { ... }` blocks.

Server directives:
- `listen <port> [<port> ...];` – one or more ports
- `host <ip-or-hostname>;`
- `server_name <name> [<name> ...];`
- `client_max_body_size <size>` – e.g. `1M`, `100mb`
- `error_page <code> <path>;`

Location directives:
- `root <path>;` – document root for that location
- `index <file> [<file> ...];`
- `allowed_methods <GET|POST|DELETE ...>;`
- `autoindex on|off;`
- `return <code> <to>;` – e.g. `return 301 /new;`
- `cgi <extension> <interpreter>;` – mark location as CGI-enabled and set interpreter
  - Example: `cgi .py /usr/bin/python3;` or `cgi .php /usr/bin/php;`
- `upload <path>;` – mark location as upload-enabled (optional)

Notes:
- This project also supports extension-based `location` names like `location *.py { ... }`. The matching picks the most specific location, preferring an extension wildcard when appropriate.
- CGI requests are expected under the `/cgi-bin/` URL prefix and are mapped to files under the `cgi_files/` folder.

### Example

```nginx
server {
    listen 8080 8081;
    host 127.0.0.1;
    server_name example.com;

    client_max_body_size 100mb;

    error_page 404 ./pages/errors/404.html;
    error_page 500 ./pages/errors/500.html;
    error_page 413 ./pages/errors/413.html;

    location / {
        root ./pages/;
        index index.html;
        allowed_methods GET;
        autoindex off;
    }

    location /upload.html {
        root ./pages/uploads/;
        index upload.html;
        allowed_methods GET;
        autoindex off;
    }

    location /uploads {
        root ./pages/uploads/;
        allowed_methods GET POST DELETE;
        autoindex on;
    }

    location /new {
        root ./pages/new/;
        index new.html;
        autoindex off;
    }

    location /old {
        root ./pages;
        return 301 /new;
    }

    # Serve a small demo video
    location /video {
        root ./pages/;
        index video.mp4;
        autoindex off;
    }

    # Example DELETE UI page
    location /delete.html {
        root ./pages/;
        index delete.html;
        allowed_methods GET;
        autoindex off;
    }

    # Example DELETE endpoint (uses query parameter ?path=)
    location /toDelete {
        root ./pages/uploads/;
        allowed_methods DELETE;
        autoindex off;
    }

    # CGI: Python and PHP, accessed via /cgi-bin/<script>
    location *.py {
        root ./cgi_files/;
        allowed_methods GET POST;
        cgi .py /usr/bin/python3;
    }

    location *.php {
        root ./cgi_files/;
        allowed_methods GET;
        cgi .php /usr/bin/php;
    }
}
```

Adjust interpreters (e.g., `/usr/bin/python3`, `/usr/bin/php`) to your system.

## Usage examples (curl)

- Static file:

```bash
curl -i http://127.0.0.1:8080/
```

- Directory listing (if `autoindex on;`):

```bash
curl -i http://127.0.0.1:8080/uploads/
```

- Upload a file to `/uploads`:

```bash
# multipart/form-data
curl -i -F "file=@README.md" http://127.0.0.1:8080/uploads

# or a raw body
curl -i --data-binary @README.md http://127.0.0.1:8080/uploads
```

- Delete a file by query param (server expects `?path=`):

```bash
curl -i -X DELETE "http://127.0.0.1:8080/toDelete?path=/uploads/README.md"
```

- Run a CGI script (maps `/cgi-bin/` to `cgi_files/`):

```bash
# Assumes cgi_files/cgi_info.py exists and Python interpreter is configured
curl -i "http://127.0.0.1:8080/cgi-bin/cgi_info.py?foo=bar"
```

## Implementation highlights

- Non-blocking sockets and `poll(2)` for multiplexing
- Per-connection state machine (`Connection`) with keep-alive support
- Streaming static responses using chunked transfer encoding
- Request parsing including cookies, query parameters, and multipart parsing for uploads
- CGI execution via `fork/execve`, passing relevant environment variables:
  - `HTTP_HOST`, `CONTENT_LENGTH`, `CONTENT_TYPE`, `REQUEST_METHOD`, `PATH_INFO`, `SCRIPT_NAME`, and combined `HTTP_COOKIE`


## Contributing / Developing

- Code style: C++98, warnings enabled (`-Wall -Wextra -Werror`)
- Build with `make`
- Test with curl or your browser

## Authors

- yel-moun
- hel-bouk
- aet-tale 
