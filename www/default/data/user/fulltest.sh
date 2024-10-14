-----------------------------14318359941972859170663619504
Content-Disposition: form-data; name="filePost"; filename="fulltest.sh"
Content-Type: application/x-shellscript

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

# post request
curl -H 'Content-Type: application/json' \
	-d '{ "title":"foo","body":"bar","id":1 }' \
	-X POST \
	http://localhost:1100/curl.txt
if cat ./www/default/curl.txt | grep -q '{ "title":"foo","body":"bar","id":1 }'; then
  echo "matched"
fi
if curl http://localhost:1100/curl.txt | grep -q '{ "title":"foo","body":"bar","id":1 }'; then
  echo "matched"
fi
rm -rf ./www/default/curl.txt
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

# content-length
if curl -H 'Content-Type: application/json' \
	-H 'Content-Length: no' \
	-d '{ "title":"foo","body":"bar","id":1 }' \
	-X POST \
	http://localhost:1100/curl.txt | grep -q '<html lang="en-US"><head><meta charset="utf-8" /><title>400 Bad Request</title></head><body><h1 align="center">400 Bad Request</h1><hr /><p align="center">webserv</p></body></html>'; then
  echo "matched, content-length badly formatted"
fi

# get request
if curl http://localhost:1100/hi.txt | grep -q "hei
das ist ein test"; then
  echo "matched"
fi

# delete request
touch ./www/default/curl.txt
curl -X DELETE http://localhost:1100/curl.txt
if cat ./www/default/curl.txt 2>&1 | grep -q "cat: ./www/default/curl.txt: No such file or directory"; then
  echo "success, the file is gone"
else
  rm -rf ./www/default/curl.txt
fi

# no host
if curl hi.txt | grep -q "curl: (6) Could not resolve host: hi.txt"; then
  echo "success, this host is not recognized"
fi

# no host
if curl http://hi.txt | grep -q "curl: (6) Could not resolve host: hi.txt"; then
  echo "success, this host is not recognized"
fi

# incorrect host
if curl http://fake:2000/hi.txt | grep -q "curl: (6) Could not resolve host: fake"; then
  echo "success, this host is not recognized"
fi


# double host
if curl -H "Host: another.com" http://localhost:1100/hi.txt | grep -q "curl: (6) Could not resolve host: fake"; then
  echo "success, this host is not recognized"
fi

-----------------------------14318359941972859170663619504--
