#!/bin/bash

# bad request
if curl http://localhost:1100/he | grep -q "<!DOCTYPE html>
<html lang=\"en-US\">
	<head>
		<meta charset=\"utf-8\" />
			<title>404 Not found</title>
	</head>
	<body>
		<h1 align=\"center\">What you're looking for doesn't seem to exist</h1>
		<hr />
		<p align=\"center\">webserv</p>
	</body>
</html>"; then
  echo "matched"
fi

curl -H 'Content-Type: application/json' \
	-d '{ "title":"foo","body":"bar","id":1 }' \
	-X POST \
	http://localhost:1100/curl.txt
if cat ./tests/default/curl.txt | grep -q '{ "title":"foo","body":"bar","id":1 }'; then
  echo "matched"
fi
if curl http://localhost:1100/curl.txt | grep -q '{ "title":"foo","body":"bar","id":1 }'; then
  echo "matched"
fi
rm -rf ./tests/default/curl.txt
if curl http://localhost:1100/curl.txt | grep -q "<!DOCTYPE html>
<html lang=\"en-US\">
	<head>
		<meta charset=\"utf-8\" />
			<title>404 Not found</title>
	</head>
	<body>
		<h1 align=\"center\">What you're looking for doesn't seem to exist</h1>
		<hr />
		<p align=\"center\">webserv</p>
	</body>
</html>"; then
  echo "matched"
fi

# get request
if curl http://localhost:1100/hi.txt | grep -q "hei
das ist ein test"; then
  echo "matched"
fi

# delete request
touch ./tests/default/curl.txt
curl -X DELETE http://localhost:1100/curl.txt
if cat ./tests/default/curl.txt 2>&1 | grep -q "cat: ./tests/default/curl.txt: No such file or directory"; then
  echo "success, the file is gone"
else
  echo "it doesn't fully work yet"
  rm -rf ./tests/default/curl.txt
fi
