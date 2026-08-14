const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int PIR_PIN = 2;
const int LIGHT_PIN = A0;

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 400.0; 
  return (duration * 0.0343) / 2.0; 
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  float d1 = readDistance(); delay(10);
  float d2 = readDistance(); delay(10);
  float d3 = readDistance(); delay(10);

  float medianDistance;
  if ((d1 <= d2 && d2 <= d3) || (d3 <= d2 && d2 <= d1)) medianDistance = d2;
  else if ((d2 <= d1 && d1 <= d3) || (d3 <= d1 && d1 <= d2)) medianDistance = d1;
  else medianDistance = d3;

  int pirState = digitalRead(PIR_PIN);
  int lightValue = analogRead(LIGHT_PIN);

  Serial.print("超音波: ");
  Serial.print(medianDistance, 2);
  Serial.print(" cm");

  Serial.print(" | PIR: ");
  if (pirState == HIGH) {
    Serial.print("ON ");
  } else {
    Serial.print("OFF");
  }
  Serial.print(" | 光: ");
  Serial.println(lightValue);

  delay(500);
}