#!/bin/bash

./webserv > /dev/null 2>&1 & 

source venv/bin/activate

pytest tests/all-tests.py
