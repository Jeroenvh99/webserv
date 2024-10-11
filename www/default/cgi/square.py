import cgi

print("Content-Type: text/html\n")

# Parse the form data
form = cgi.FieldStorage()

# Extract the content of the input box using its name attribute
if "textInput" in form:
    user_input = form.getvalue("toSquare")
    print(f"<p>You entered: {user_input}</p>")
else:
    print("<p>No input received!</p>")