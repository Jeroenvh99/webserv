#!/usr/bin/python3

import os

def element(tag, content):
	body = "<" + tag + ">"
	body += content
	body += "</" + tag + ">"
	return body

def environ_value(key):
	try:
		value = os.environ[key]
		if (value == ""):
			return "&lt;null&gt;"
		return value
	except KeyError:
		return "&lt;undefined&gt;"

def body():
	body = "<!DOCTYPE html>"
	body += "<html>"
	body += "<head><title>ENV CGI</title></head>"
	body += "<body>"
	body += "<h1>ENV CGI</h1>"
	body += element("p", "Directory: " + os.getcwd())
	body += "<p>Host address is: " + environ_value('REMOTE_HOST') + "</p>"
	body += "<p>User Agent: " + environ_value('HTTP_USER_AGENT') + "</p>"
	body += "<p>Request Method: " + environ_value('REQUEST_METHOD') + "</p>"
	body += "<p>Server Software: " + environ_value('SERVER_SOFTWARE') + "</p>"
	body += "<p>Server Protocol: " + environ_value('SERVER_PROTOCOL') + "</p>"
	body += "<p>Server Port: " + environ_value('SERVER_PORT') + "</p>"
	body += "</body>"
	body += "</html>"
	return body

payload = body()
print("Content-Type: text/html")
print("Content-Length:", len(payload))
print("")
print(payload)
