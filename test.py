import socket
import time

def send_chunked_request():
    # Define the server address and port
    server_address = ('127.0.0.1', 8080)  # Replace with your server's IP and port

    # Create a socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        client_socket.connect(server_address)

        # Define the chunked HTTP request
        request_lines = [
            "POST / HTTP/1.1\r\n",
            "Host: example.com\r\n",
            "Transfer-Encoding: chunked\r\n",
            "\r\n",
            "3\r\n",  # Chunk size (4 bytes)
            "Wiki\r\n",  # Chunk data
            "5\r\n",  # Chunk size (5 bytes)
            "pedia\r\n",  # Chunk data
            "0\r\n",  # End of chunks
            "\r"
        ]

        # Send each line with a delay
        for line in request_lines:
            client_socket.sendall(line.encode('utf-8'))
            print(f"Sent: {line.strip()}")
            time.sleep(2)  # Wait 2 seconds before sending the next line

        # Receive the server's response
        response = client_socket.recv(4096)
        print("Server response:")
        print(response.decode('utf-8'))

if __name__ == "__main__":
    send_chunked_request()