#define PINO_LED 12

void setup() {
  pinMode(PINO_LED, OUTPUT);
}

void loop() {
  digitalWrite(PINO_LED, HIGH);
  delay(1000);
  digitalWrite(PINO_LED, LOW);
  delay(1000);
}
