# content-length
if curl -H 'Content-Type: application/json' \
	-H 'Content-Length: 27' \
	-d '{ "title":"foo","body":"bar","id":1 }' \
	-X POST \
	http://localhost:1100/cgi/curl.txt
