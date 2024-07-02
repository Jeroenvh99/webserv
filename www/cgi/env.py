#!/usr/bin/python3

import os

def body():
	body = "<!DOCTYPE html>"
	body += "<html>"
	body += "<head><title>ENV CGI</title></head>"
	body += "<body>"
	body += "<h1>ENV CGI</h1>"
	body += "<p>Host address is:" + os.environ['REMOTE_HOST'] + "</p>"
	# body += "<p>User Agent:" + os.environ['HTTP_USER_AGENT'] + "</p>"
	body += "<p>Request Method:" + os.environ['REQUEST_METHOD'] + "</p>"
	body += "<p>Server Software:" + os.environ['SERVER_SOFTWARE'] + "</p>"
	body += "<p>Server Protocol:" + os.environ['SERVER_PROTOCOL'] + "</p>"
	body += "<p>Server Port:" + os.environ['SERVER_PORT'] + "</p>"
	body += "</body>"
	body += "</html>"
	return body

payload = body()
print("Content-Type: text/html")
print("Content-Length:", len(payload))
print("")
print(payload)
