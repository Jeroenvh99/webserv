#!/usr/bin/python3

import os

print("Content-type: text/html\r\n")
print("<!DOCTYPE html>")
print("<html>")
print("<head><title>ENV CGI</title></head>")
print("<body>")

print("<h1>ENV CGI</h1>")
print("<p>Host address is:", os.environ['REMOTE_HOST'] + "</p>")
print("<p>User Agent:", os.environ['HTTP_USER_AGENT'] + "</p>")
print("<p>Request Method:", os.environ['REQUEST_METHOD'] + "</p>")
print("<p>Server Software:", os.environ['SERVER_SOFTWARE'] + "</p>")
print("<p>Server Protocol:", os.environ['SERVER_PROTOCOL'] + "</p>")
print("<p>Server Port:", os.environ['SERVER_PORT'] + "</p>")

print("</body>")
print("</html>")