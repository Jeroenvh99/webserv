#!/usr/bin/env python3

import sys
import os
import re

def get_payload():
    # Read the incoming data from the environment
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    content_type = os.environ.get('CONTENT_TYPE', '')

    # Check if the content type is multipart/form-data
    if 'multipart/form-data' in content_type:
        # Extract the boundary string from the content-type
        boundary = re.search(r'boundary=(.*)', content_type).group(1)
        boundary = boundary.strip()

        # Read the multipart form data
        data = sys.stdin.read(content_length)

        # Split the data based on the boundary
        parts = data.split('--' + boundary)

        # Iterate through the parts and look for the "toSquare" field
        for part in parts:
            if 'name="toSquare"' in part:
                # Extract the value of the input box
                input_value = re.search(r'\r\n\r\n(.*)\r\n', part, re.DOTALL)
                if input_value:
                    input_value = input_value.group(1).strip()

                    # Try to square the input if it's an integer
                    try:
                        number = float(input_value)
                        squared_value = number ** 2
                        return f"""
                        <html>
                        <body>
                            <h1>Result</h1>
                            <p>The square of {number} is {squared_value}.</p>
                        </body>
                        </html>
                        """
                    except ValueError:
                        return f"""
                        <html>
                        <body>
                            <h1>Error</h1>
                            <p>That doesn't seem to be a number.</p>
                        </body>
                        </html>
                        """
        return """
        <html>
        <body>
            <h1>Error</h1>
            <p>No input found in the form submission.</p>
        </body>
        </html>
        """
    else:
        # If not multipart/form-data, return an error
        return f"""
        <html>
        <body>
            <h1>Error</h1>
            <p>Invalid content type. Please submit the form using multipart/form-data.</p>
        </body>
        </html>
        """

payload = get_payload()

# Set the content-type for the response (HTML in this case)
print("Content-Type: text/html")
print(f"Content-Length: {len(payload)}")
print()  # Blank line to indicate the end of the headers
print(payload)
