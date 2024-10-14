-----------------------------70599581513633856662066317535
Content-Disposition: form-data; name="filePost"; filename="run-tests.sh"
Content-Type: application/x-shellscript

#!/bin/bash

./webserv ./tests/test.conf > /dev/null 2>&1 & 

source venv/bin/activate

pytest tests/all-tests.py

-----------------------------70599581513633856662066317535--
