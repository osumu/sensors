const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int PIR_PIN = 2;
const int LIGHT_PIN = A0;

unsigned long lastPirOnTime = 0;
const unsigned long PIR_HOLD_TIME = 5000; 

int lastLightValue = -1; 

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

    float distance;
    if ((d1 <= d2 && d2 <= d3) || (d3 <= d2 && d2 <= d1)) distance = d2;
    else if ((d2 <= d1 && d1 <= d3) || (d3 <= d1 && d1 <= d2)) distance = d1;
    else distance = d3;

    int rawPirState = digitalRead(PIR_PIN);
    unsigned long currentMillis = millis();
  
    if (rawPirState == HIGH) {
        lastPirOnTime = currentMillis;
    }
  
    int pirState = (currentMillis - lastPirOnTime < PIR_HOLD_TIME) ? HIGH : LOW;
    int currentLightValue = analogRead(LIGHT_PIN);
    int lightDrop = 0;
    if (lastLightValue != -1) {
        lightDrop = lastLightValue - currentLightValue;
    }
    lastLightValue = currentLightValue;

    String result = "正常・変化なし";

    if (distance < 200.0 && pirState == HIGH) {
        result = "人が近づいた";
    }
    if (distance < 200.0 && pirState == LOW) {
        result = "物が動いた";
    }
    else if (distance < 200.0 && pirState == LOW) {
        result = "物体 または 止まった人";
    }
    else if (distance > 150.0 && lightDrop > 150) {
        result = "照明が消えた";
    }

    Serial.print("距離: ");
    Serial.print(distance, 1);
    Serial.print("cm | PIR: ");
    Serial.print(pirState == HIGH ? "ON " : "OFF");
    Serial.print(" | 光: ");
    Serial.print(currentLightValue);
    Serial.print(" | ");
    Serial.println(result);

    delay(500);
}