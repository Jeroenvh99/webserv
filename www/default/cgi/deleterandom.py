#!/usr/bin/python3

import os
import random

def body():
	body = "<!DOCTYPE html>"
	body += "<html>"
	body += "<head><title>Random delete</title></head>"
	body += "<body>"
	body += "<h1>Delete a random file</h1>"
	body += "<p>delete a random file from the tests folder</p>"
	testfiles = []
	for root, dirs, files in os.walk("/home/jvan-hal/Desktop/webserv/tests"):
		for file in files:
			testfiles.append(os.path.join(root, file))
	deleteindex = random.randint(0, len(testfiles))
	os.remove(testfiles[deleteindex])
	body += "</body>"
	body += "</html>"
	return body

payload = body()
print("Content-Type: text/html")
print("Content-Length:", len(payload))
print("")
print(payload)