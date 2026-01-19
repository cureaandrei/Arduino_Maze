const int X_PIN = A0;
const int Y_PIN = A1;
const int SW_PIN = 2;

void setup() {
  pinMode(SW_PIN, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  int x = analogRead(X_PIN);   
  int y = analogRead(Y_PIN);   
  int b = (digitalRead(SW_PIN) == LOW) ? 1 : 0;

  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.println(b);

  delay(10);
}
