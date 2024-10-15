#!/usr/bin/env python3

import sys
import os
import io
import email
import cgitb

# Enable debugging for CGI errors
cgitb.enable()

# Set the content-type for the response (HTML in this case)
print("Content-Type: text/html")
print()  # Blank line to indicate the end of the headers

# Read the incoming data from the environment
content_length = int(os.environ.get('CONTENT_LENGTH', 0))
content_type = os.environ.get('CONTENT_TYPE', '')

# Check if the content type is multipart/form-data
if 'multipart/form-data' in content_type:
    # Read the multipart form data
    data = sys.stdin.read(content_length)
    
    # Use the email.parser to parse the multipart data
    msg = email.message_from_bytes(data.encode('utf-8'), policy=email.policy.HTTP)

    # Extract the value of the input box (assuming name="toSquare")
    for part in msg.iter_parts():
        if part.get_content_disposition() == 'form-data' and part.get_param('name') == 'toSquare':
            input_value = part.get_payload(decode=True).decode('utf-8').strip()
            
            # Now handle the logic for squaring the input if it's a valid integer
            try:
                number = int(input_value)
                squared_value = number ** 2
                print(f"""
                <html>
                <body>
                    <h1>Result</h1>
                    <p>The square of {number} is {squared_value}.</p>
                </body>
                </html>
                """)
            except ValueError:
                print(f"""
                <html>
                <body>
                    <h1>Error</h1>
                    <p>Please enter a valid integer.</p>
                </body>
                </html>
                """)
            break
else:
    # If not multipart/form-data, return an error
    print(f"""
    <html>
    <body>
        <h1>Error</h1>
        <p>Invalid content type. Please submit the form using multipart/form-data.</p>
    </body>
    </html>
    """)
