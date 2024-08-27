import requests

BASE_URL = "http://localhost:1100"

def test_not_found():
    expected_body = '''\
<!DOCTYPE html>
<html lang="en-US">
	<head>
		<meta charset="utf-8" />
			<title>404 Not found</title>
	</head>
	<body>
		<h1 align="center">What you're looking for doesn't seem to exist</h1>
		<hr />
		<p align="center">webserv</p>
	</body>
</html>
'''

    response = requests.get(f'{BASE_URL}/not-found')
    
    assert response.status_code == 404
    assert response.text == expected_body

def test_index():
    expected_body = '''\
<!DOCTYPE html>
<html lang="en-US">
	<head>
		<meta charset="utf-8" />
			<title>Index</title>
	</head>
	<body>
		<a href="hi.txt">hi.txt</a>
	</body>
</html>
'''

    response = requests.get(f'{BASE_URL}/')
    
    assert response.status_code == 200
    assert response.text == expected_body
    assert response.headers['Content-Type'] == 'text/html'
    assert response.headers['Content-Length'] == '159'

def test_unallowed_method():
    response = requests.get(f'{BASE_URL}/not-allowed')

    assert response.status_code == 405


# def test_reponse_headers():
#     response = requests.get(f'{BASE_URL}/')
#
#     assert response.headers['Server'] == 'webserv'
#     assert response.headers['Date']
#     assert response.headers['Content-Type'] == 'text/html'
#     assert response.headers['Content-Length'] == '159'
#     assert response.headers['Last-Modified'] == 'Tue, 27 Aug 2024 17:02:14 GMT'
    # assert response.headers['Last-Modified']

# def test_bad_request():
#     expected_body = '''\
# <html lang="en-US">
#     <head>
#         <meta charset="utf-8" />
#         <title>400 Bad Request</title>
#     </head>
#     <body>
#         <h1 align="center">
#             400 Bad Request</h1>
#         <hr />
#         <p align="center">webserv</p>
#     </body>
# </html>
# '''
#
#     response = requests.post(f'{BASE_URL}/', headers={'Content-Type': 'application/json', 'Content-Length': 'no'}, data='{ "title":"foo","body":"bar","id":1 }')
#
#     assert response.status_code == 400
