import time
import requests
import os

ESP_IP = "172.20.10.3"
FILE_PATH = "telemetry_log.txt"
URL = f"http://{ESP_IP}/update"
SEND_INTERVAL = 0.04

def parse_csv_line(line: str):
    line = line.strip()
    if not line:
        return None
    if line.lower().startswith("ts,"):
        return None

    parts = [p.strip() for p in line.split(",")]
    if len(parts) != 6:
        return None

    try:
        ts = float(parts[0])
        px = int(parts[1])
        py = int(parts[2])
        score = float(parts[3])
        desired = parts[4]
        current = parts[5]
    except:
        return None

    return {
        "tMs": int(ts * 1000),
        "score": score,
        "px": px,
        "py": py,
        "desiredDir": desired,
        "currentDir": current,
        "raw": line
    }

def main():
    print("CWD:", os.getcwd())
    print("Reading:", os.path.abspath(FILE_PATH))

    sent = 0
    parsed = 0

    with open(FILE_PATH, "r", encoding="utf-8", errors="ignore") as f:
        for i, line in enumerate(f, start=1):
            payload = parse_csv_line(line)
            if payload is None:
                continue

            parsed += 1
            if parsed <= 3:
                print("PARSED SAMPLE:", payload)

            r = requests.post(URL, json=payload, timeout=2)
            if r.status_code != 200:
                print("BAD POST:", r.status_code, r.text)
                return

            sent += 1
            if sent % 50 == 0:
                print("sent", sent)

            time.sleep(SEND_INTERVAL)

    print("done, parsed", parsed, "sent", sent)

if __name__ == "__main__":
    main()
