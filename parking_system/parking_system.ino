// --- Ultrasonic sensor pins ---
const int TRIG_PIN = 6;
const int ECHO_PIN = 7;

// --- LED & Buzzer for ultrasonic ---
const int US_LED_PIN = 10;
const int BUZZER_PIN = 8;

const int LED_DISTANCE_THRESHOLD = 20;    // cm
const int BUZZER_DISTANCE_THRESHOLD = 10; // cm

// --- Variables ---
float duration_us, distance_cm;

void setup() {
  Serial.begin(9600);

  // Ultrasonic setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(US_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(US_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, HIGH); // buzzer off (active LOW type)

  Serial.println("System starting...");
  delay(2000);
  Serial.println("Ready!");
}

void loop() {
  // --- ULTRASONIC SENSOR HANDLING ---
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;

  // LED control
  if (distance_cm < LED_DISTANCE_THRESHOLD) {
    digitalWrite(US_LED_PIN, HIGH);
  } else {
    digitalWrite(US_LED_PIN, LOW);
  }

  // Buzzer control (active LOW)
  if (distance_cm <= BUZZER_DISTANCE_THRESHOLD) {
    digitalWrite(BUZZER_PIN, LOW);  // buzzer ON
  } else {
    digitalWrite(BUZZER_PIN, HIGH); // buzzer OFF
  }

  // Debug output
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(200);
}
