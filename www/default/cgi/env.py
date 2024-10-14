#!/usr/bin/python3

import os

def element(tag, *nested):
	body = "<" + tag + ">"
	for e in nested:
		body += e
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

def get_payload():
	html = "<!DOCTYPE html>"
	html += element("html",
		element("head",
			element("title", "CGI Execution Environment"),
			"<meta charset=\"utf-8\"> \
			<link rel=\"stylesheet\" href=\"/nested.css\">"
		),
		element("body",
			element("h1", "CGI: Execution Environment"),
			element("p", "Directory: ", element("code", os.getcwd())),
			element("ul",
				element("li", "Your IP address: ", element("code", environ_value('REMOTE_HOST'))),
				element("li", "User agent: ", element("code", environ_value('HTTP_USER_AGENT'))),
				element("li", "Request method: ", element("code", environ_value('REQUEST_METHOD'))),
				element("li", "Server software: ", element("code", environ_value("SERVER_SOFTWARE"))),
				element("li", "Server protocol: ", element("code", environ_value("SERVER_PROTOCOL"))),
				element("li", "Server port: ", element("code", environ_value("SERVER_PORT")))
			)
		)
	)
	return html

payload = get_payload()
print("Content-Type: text/html")
print("Content-Length:", len(payload))
print("")
print(payload)
