import requests

r = requests.get('http://help.websiteos.com/websiteos/example_of_a_simple_html_page.htm', stream=True)
print(f"Status: {r.raw.status} {r.raw.reason}", end="\r\n")
for i, (k, v) in enumerate(r.raw.headers.items()):
	if k == "Content-Encoding" or k == "Transfer-Encoding":
		continue
	elif i == len(r.raw.headers) - 1:
		print(f"{k}: {v}", end="\r\n\r\n")
	else:
		print(f"{k}: {v}", end="\r\n")
print(r.text)