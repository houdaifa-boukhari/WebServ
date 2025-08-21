import requests

def test_chunked(base_url):
    """
    Test the server's ability to handle chunked requests.
    """
    url = f"{base_url}/chunked"
    
    def chunked_data():
        # Simulate sending chunks of data
        chunks = ["Hello", " ", "World", "!"]
        for chunk in chunks:
            yield chunk
        yield ""  # End of chunks

    try:
        # Send a POST request with chunked encoding
        response = requests.post(url, data=chunked_data(), headers={"Transfer-Encoding": "chunked"})
        
        # Check if the server responded with a success status code
        if response.status_code == 200:
            print("Chunked request test passed.")
            return True
        else:
            print(f"Chunked request test failed. Status code: {response.status_code}")
            return False
    except Exception as e:
        print(f"Chunked request test failed with exception: {e}")
        return False
    
test_chunked("http://localhost:8080")  # Replace with your server's base URL