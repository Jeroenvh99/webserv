#!/bin/bash

./webserv ./tests/test.conf > /dev/null 2>&1 & 

source venv/bin/activate

pytest tests/all-tests.py
