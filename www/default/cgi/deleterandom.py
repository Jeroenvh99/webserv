import os
import random

print('delete a random file from the tests folder')
testfiles = []
for root, dirs, files in os.walk("/home/jvan-hal/Desktop/webserv/tests"):
	for file in files:
		testfiles.append(os.path.join(root, file))
print("Files in the tests folder")
print(testfiles)
deleteindex = random.randint(0, len(testfiles))
os.remove(testfiles[deleteindex])