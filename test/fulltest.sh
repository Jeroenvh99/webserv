#!/bin/bash

# bad request
if { echo "he"; sleep 1; } | telnet 127.0.0.1 1100 | grep -q '<!DOCTYPE html><html lang="en-US"><head><meta charset="utf-8" /><title>400 Bad Request</title></head><body><h1 align="center">400 Bad Request</h1><hr /><p align="center">webserv</p></body></html>'; then
  echo "matched"
fi

POST /test HTTP/1.1
Host: foo.example
Content-Type: application/x-www-form-urlencoded
Content-Length: 27

field1=value1&field2=value2

# method not allowed
if { echo "POST /test HTTP/1.1"; echo "Host: localhost"; echo "Content-Type: application/x-www-form-urlencoded"; echo "Content-Length: 27"; echo "field1=value1&field2=value2"; sleep 1; } | telnet 127.0.0.1 1100 | grep -q '<!DOCTYPE html><html lang="en-US"><head><meta charset="utf-8" /><title>405 Method Not Allowed</title></head><body><h1 align="center">405 Method Not Allowed</h1><hr /><p align="center">webserv</p></body></html>'; then
  echo "matched"
fi