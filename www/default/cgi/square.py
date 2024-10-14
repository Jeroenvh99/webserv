#!/usr/bin/python3

# Note: this script requires the use of the `requests-toolbelt` package. If you're unable to install it, you're out of luck.

import sys
import os
import io
import email

print("Content-Type: text/html\n")

content_length = int(os.environ.get('CONTENT_LENGTH', 0))
content_type = os.environ.get('CONTENT_TYPE', '')

if "multipart/form-data" in content_type:
	data = sys.stdin.read(content_length)
	msg = email.message_from_bytres(data.encode('utf-8'), policy=email.policy.HTTP)
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