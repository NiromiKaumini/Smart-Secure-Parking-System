// PIR sensor test - PIR OUT -> D3, VCC -> 5V, GND -> GND
const uint8_t PIR_PIN = 3;
const uint8_t LED_PIN = 11;

int lastState = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // ms

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.println("PIR sensor test starting...");
  Serial.println("Give the PIR sensor ~60-90 seconds to stabilise if powered first time.");
  delay(2000); // allow PIR to stabilise a bit
  Serial.println("Ready.");
}

void loop() {
  int reading = digitalRead(PIR_PIN);
  // simple debounce on changes
  if (reading != lastState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the reading has been stable longer than debounceDelay then take it as actual state
    static int stableState = LOW;
    if (reading != stableState) {
      stableState = reading;
      if (stableState == HIGH) {
        Serial.print("Motion detected at ");
        Serial.print(millis());
        Serial.println(" ms");
        digitalWrite(LED_PIN, HIGH);
      } else {
        Serial.print("Motion ended at ");
        Serial.print(millis());
        Serial.println(" ms");
        digitalWrite(LED_PIN, LOW);
      }
    }
  }

  lastState = reading;
  delay(10); // small loop delay
}
