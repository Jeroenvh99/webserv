#!/bin/bash

if { echo "he"; sleep 1; } | telnet 127.0.0.1 1100 | grep -q '<!DOCTYPE html><html lang="en-US"><head><meta charset="utf-8" /><title>400 Bad Request</title></head><body><h1 align="center">400 Bad Request</h1><hr /><p align="center">webserv</p></body></html>'; then
  echo "matched"
fi