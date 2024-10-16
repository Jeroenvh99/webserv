import requests
import os
import filecmp

BASE_URL = "http://localhost:1100"

# def test_headers_1():
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

#     response = requests.post(f'{BASE_URL}/', headers={'Content Type': 'application/json', 'Content-Length': '27'}, data='{ "title":"foo","body":"bar","id":1 }')

#     assert response.status_code == 400
# 	assert response.text == expected_body

# def test_headers_2():
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

#     response = requests.post(f'{BASE_URL}/', headers={'': 'application/json', 'Content-Length': '27'}, data='{ "title":"foo","body":"bar","id":1 }')

#     assert response.status_code == 400
# 	assert response.text == expected_body

# def test_headers_3():
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

#     response = requests.post(f'{BASE_URL}/', headers={'Content-Type': 'application/json', 'Content-Length': '-1'}, data='{ "title":"foo","body":"bar","id":1 }')

#     assert response.status_code == 400
# 	assert response.text == expected_body

# def test_headers_4():
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

#     response = requests.post(f'{BASE_URL}/', headers={'Content-Type': 'application/json', 'Content-Length': '27', 'Host': 'localhost', 'Host': 'globalhost'}, data='{ "title":"foo","body":"bar","id":1 }')

#     assert response.status_code == 400
# 	assert response.text == expected_body

# def test_headers_5():
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

#     response = requests.post(f'{BASE_URL}/', headers={'Content-Type': 'application/json', 'Content-Length': '27', 'Host': 'localhost', 'Host': 'localhost'}, data='{ "title":"foo","body":"bar","id":1 }')

#     assert response.status_code == 400
# 	assert response.text == expected_body

# def test_headers_6():
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

#     response = requests.post(f'{BASE_URL}/', headers={'Content-Type': 'application/json', 'Content-Length': '27', 'Host': 'localhost@here.com'}, data='{ "title":"foo","body":"bar","id":1 }')

#     assert response.status_code == 400
# 	assert response.text == expected_body

# def test_max_body_len():
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

# 	long_text = 'a' * 1000001
# 	response = requests.post(f'{BASE_URL}/', headers={'Content-Type': 'text/txt', 'Content-Length': '100001000'}, data=f'{long_text}')

#     assert response.status_code == 400
# 	assert response.text == expected_body

# def test_not_found():
#     expected_body = '''\
# <!DOCTYPE html>
# <html lang="en-US">
# 	<head>
# 		<meta charset="utf-8" />
# 			<title>404 Not found</title>
# 	</head>
# 	<body>
# 		<h1 align="center">What you're looking for doesn't seem to exist</h1>
# 		<hr />
# 		<p align="center">webserv</p>
# 	</body>
# </html>
# '''

#     response = requests.get(f'{BASE_URL}/not-found')
    
#     assert response.status_code == 404
#     assert response.text == expected_body

# def test_index():
#     expected_body = '''\
# <!DOCTYPE html>
# <html lang="en-US">
# 	<head>
# 		<meta charset="utf-8" />
# 			<title>Index</title>
# 	</head>
# 	<body>
# 		<a href="hi.txt">hi.txt</a>
# 	</body>
# </html>
# '''

#     response = requests.get(f'{BASE_URL}/')
    
#     assert response.status_code == 200
#     assert response.text == expected_body
#     assert response.headers['Content-Type'] == 'text/html'
#     assert response.headers['Content-Length'] == '159'

# def test_unallowed_method():
#     response = requests.get(f'{BASE_URL}/not-allowed')

#     assert response.status_code == 405


# def test_reponse_headers():
#     response = requests.get(f'{BASE_URL}/')

#     assert response.headers['Server'] == 'webserv'
#     assert response.headers['Content-Type'] == 'text/html'
#     assert response.headers['Content-Length'] == '3473'
#     assert response.headers['Last-Modified'] == 'Tue, 27 Aug 2024 17:02:14 GMT'
#     assert response.headers['Connection'] == 'keep-alive'

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

#     response = requests.post(f'{BASE_URL}/', headers={'Content-Type': 'application/json', 'Content-Length': 'no'}, data='{ "title":"foo","body":"bar","id":1 }')

#     assert response.status_code == 400
# 	assert response.text == expected_body

# def test_cgi():
#     response = requests.get(f'{BASE_URL}/cgi/env.py')
    
#     assert response.status_code == 200
#     assert response.headers['Content-Type'] == 'text/html'

# def test_post():
# 	files = {'upload_file': open('/home/jvan-hal/Desktop/webserv/www/default/cgi/deleterandom.py','rb')}
#     response = requests.post(f'{BASE_URL}/upload.txt', files=files)

#     assert response.status_code == 201
# 	assert os.path.exists('/home/jvan-hal/Desktop/webserv/www/default/upload.txt')

# def test_delete():
# 	file.create(f'{BASE_URL}/data/user/fulltest.sh')
# 	response = requests.delete(f'{BASE_URL}/data/user/fulltest.sh')

# 	assert response.status_code == 200

# def test_multipart():
# 	files = {'upload_file': open('/home/jvan-hal/Desktop/webserv/www/default/cgi/deleterandom.py','rb')}
# 	response = requests.post(f'{BASE_URL}/upload.txt', files=files)

# 	assert filecmp.cmp("/home/jvan-hal/Desktop/webserv/www/default/data/user/deleterandom.py", "/home/jvan-hal/Desktop/webserv/www/default/cgi/deleterandom.py", shallow=False)


files = {'upload_file': open('/home/jvan-hal/Desktop/webserv/www/default/cgi/deleterandom.py','rb')}
response = requests.post(f'{BASE_URL}/upload.txt', files=files)

assert filecmp.cmp("/home/jvan-hal/Desktop/webserv/www/default/data/user/deleterandom.py", "/home/jvan-hal/Desktop/webserv/www/default/cgi/deleterandom.py", shallow=False)
# response = requests.post(f'{BASE_URL}/s.t', headers={'Content-Type': 'application/json', 'Content-Length': '37'}, data='{ "title":"foo","body":"bar","id":1 }')
# print(response.status_code)
# print(response.text)

