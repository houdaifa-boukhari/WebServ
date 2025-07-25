import os

# print("hello from cgi_file.py \n")

for key, value in os.environ.items():
    print(f"{key}={value}")