#include <WiFi.h>
#include <WebServer.h>

const char* WIFI_SSID = "Curea_Iphone";
const char* WIFI_PASS = "250720041984";

WebServer server(80);

volatile unsigned long lastUpdateMs = 0;

String lastRawLine = "no data yet";
int score = -1;
int px = -1, py = -1;                 
String desiredDir = "-";
String currentDir = "-";
unsigned long tMs = 0;                 

static const char INDEX_HTML[] PROGMEM = R"HTML(
<!doctype html>
<html>
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width,initial-scale=1" />
  <title>Pacman Telemetry (replay)</title>
  <style>
    body{font-family:system-ui,Arial;margin:20px;max-width:520px}
    h1{margin:0 0 14px 0}
    .card{border:1px solid #ddd;border-radius:12px;padding:14px;margin:10px 0}
    .k{opacity:.7;font-size:14px}
    .v{font-size:18px;font-weight:600}
    pre{background:#f7f7f7;padding:10px;border-radius:10px;overflow:auto}
  </style>
</head>
<body>
  <h1>Pacman Telemetry (replay)</h1>

  <div class="card">
    <div class="k">ESP IP</div>
    <div class="v" id="ip">-</div>
  </div>

  <div class="card">
    <div class="k">Last update (ms since boot)</div>
    <div class="v" id="lu">0</div>
  </div>

  <div class="card">
    <div class="k">Score</div>
    <div class="v" id="sc">-</div>

    <div class="k" style="margin-top:10px">Pacman position</div>
    <div class="v" id="pos">(-, -)</div>

    <div class="k" style="margin-top:10px">Desired / Current</div>
    <div class="v" id="dir">- / -</div>

    <div class="k" style="margin-top:10px">Timestamp</div>
    <div class="v" id="ts">-</div>
  </div>

  <div class="card">
    <div class="k">Last raw line</div>
    <pre id="raw">no data yet</pre>
  </div>

<script>
async function tick(){
  try{
    const r = await fetch('/data', {cache:'no-store'});
    const j = await r.json();
    document.getElementById('ip').textContent = j.ip;
    document.getElementById('lu').textContent = j.lastUpdateMs;
    document.getElementById('sc').textContent = j.score;
    document.getElementById('pos').textContent = `(${j.px}, ${j.py})`;
    document.getElementById('dir').textContent = `${j.desiredDir} / ${j.currentDir}`;
    document.getElementById('ts').textContent = j.tMs;
    document.getElementById('raw').textContent = j.lastRawLine;
  }catch(e){}
}
setInterval(tick, 250);
tick();
</script>
</body>
</html>
)HTML";

String jsonGetString(const String& body, const char* key) {
  String k = String("\"") + key + "\":";
  int i = body.indexOf(k);
  if (i < 0) return "";
  i += k.length();
  while (i < (int)body.length() && (body[i] == ' ')) i++;
  if (i < (int)body.length() && body[i] == '"') {
    int j = body.indexOf('"', i + 1);
    if (j < 0) return "";
    return body.substring(i + 1, j);
  }
  int j = body.indexOf(',', i);
  if (j < 0) j = body.indexOf('}', i);
  if (j < 0) j = body.length();
  String v = body.substring(i, j);
  v.trim();
  return v;
}

long jsonGetInt(const String& body, const char* key, long defv) {
  String s = jsonGetString(body, key);
  if (s.length() == 0) return defv;
  return s.toInt();
}

void handleIndex() {
  server.send(200, "text/html", INDEX_HTML);
}

void handleData() {
  String ip = WiFi.localIP().toString();

  String out = "{";
  out += "\"ip\":\"" + ip + "\",";
  out += "\"lastUpdateMs\":" + String(lastUpdateMs) + ",";
  out += "\"score\":" + String(score) + ",";
  out += "\"px\":" + String(px) + ",";
  out += "\"py\":" + String(py) + ",";
  out += "\"desiredDir\":\"" + desiredDir + "\",";
  out += "\"currentDir\":\"" + currentDir + "\",";
  out += "\"tMs\":" + String(tMs) + ",";
  out += "\"lastRawLine\":\"";

  String raw = lastRawLine;
  raw.replace("\\", "\\\\");
  raw.replace("\"", "\\\"");
  raw.replace("\n", "\\n");
  raw.replace("\r", "\\r");

  out += raw + "\"}";
  server.send(200, "application/json", out);
}

void handleUpdate() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "POST only");
    return;
  }

  String body = server.arg("plain");
  if (body.length() == 0) {
    server.send(400, "text/plain", "empty body");
    return;
  }

  tMs = (unsigned long)jsonGetInt(body, "tMs", tMs);
  score = (int)jsonGetInt(body, "score", score);
  px = (int)jsonGetInt(body, "px", px);
  py = (int)jsonGetInt(body, "py", py);

  String d = jsonGetString(body, "desiredDir");
  if (d.length()) desiredDir = d;

  String c = jsonGetString(body, "currentDir");
  if (c.length()) currentDir = c;

  String r = jsonGetString(body, "raw");
  if (r.length()) lastRawLine = r;

  lastUpdateMs = millis();
  server.send(200, "text/plain", "ok");
}

void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(". status=");
    Serial.println((int)WiFi.status());
  }

  Serial.println();
  Serial.print("Connected! IP=");
  Serial.println(WiFi.localIP());

  server.on("/", handleIndex);
  server.on("/data", handleData);
  server.on("/update", handleUpdate);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
