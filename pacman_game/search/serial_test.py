import requests

ESP_IP = "172.20.10.3"
URL = f"http://{ESP_IP}/update"

payload = {
    "tMs": 123456,
    "score": 99,
    "px": 5,
    "py": 7,
    "desiredDir": "North",
    "currentDir": "East",
    "raw": "TEST LINE"
}

r = requests.post(URL, json=payload, timeout=2)
print(r.status_code, r.text)
