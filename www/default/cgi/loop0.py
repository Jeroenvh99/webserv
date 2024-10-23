#!/usr/bin/python3

import os
import time

def get_payload():
	return "You will never see me, I'm afraid..."

payload = get_payload()
print(f"Content-Type: text/html\nContent-Length: {len(payload)}\n", flush=True)
time.sleep(60)
print(payload)