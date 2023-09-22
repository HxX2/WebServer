import requests

r = requests.get('https://www.w3schools.com/w3css/tryw3css_templates_gourmet_catering.htm', stream=True)
print(f"HTTP/{r.raw.version/10} {r.raw.status} {r.raw.reason}", end="\r\n")
for i, (k, v) in enumerate(r.raw.headers.items()):
	if k == "Content-Encoding":
		continue
	elif i == len(r.raw.headers) - 1:
		print(f"{k}: {v}", end="\r\n\r\n")
	else:
		print(f"{k}: {v}", end="\r\n")
print(r.text)