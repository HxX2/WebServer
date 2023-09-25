import urllib.request

r = urllib.request.urlopen("https://www.w3schools.com/w3css/tryw3css_examples_newspaper.htm")

print("HTTP/1.1", r.status, r.reason, end="\r\n")
print("Content-Type: text/html", end="\r\n")
print("Content-Length:", r.length, end="\r\n")
print(end="\r\n")
print(r.read())