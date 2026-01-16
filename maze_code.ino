const int X_PIN = A0;
const int Y_PIN = A1;

const int DEAD = 120;   
const int MID  = 512;

char lastCmd = 'S';

void setup() {
  Serial.begin(115200);
}

void loop() {
  int x = analogRead(X_PIN) - MID;
  int y = analogRead(Y_PIN) - MID;

  char cmd = 'S';
  if (x > DEAD) cmd = 'R';
  else if (x < -DEAD) cmd = 'L';
  else if (y > DEAD) cmd = 'D';
  else if (y < -DEAD) cmd = 'U';

  if (cmd != lastCmd) {
    Serial.write(cmd);
    lastCmd = cmd;
  }

  delay(10);
}
