const int TRIG_PIN = 6; 
const int ECHO_PIN = 7; 
const int LED_PIN  = 10; 
const int BUZZER_PIN = 8; 

const int LED_DISTANCE_THRESHOLD = 20;  
const int BUZZER_DISTANCE_THRESHOLD = 10; 

float duration_us, distance_cm;

void setup() {
  Serial.begin(9600);       
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);   
  pinMode(LED_PIN, OUTPUT);  
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); 
}

void loop() {
  // Send trigger pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo pulse duration
  duration_us = pulseIn(ECHO_PIN, HIGH);

  // Convert to distance in cm
  distance_cm = 0.017 * duration_us;

  // LED control
  if(distance_cm < LED_DISTANCE_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH); 
  } else {
    digitalWrite(LED_PIN, LOW);  
  }

  // Buzzer control
  if(distance_cm <= BUZZER_DISTANCE_THRESHOLD) {
    digitalWrite(BUZZER_PIN, LOW); // buzzer ON
  } else {
    digitalWrite(BUZZER_PIN, HIGH);  // buzzer OFF
  }

  // Debug print
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(200); // smaller delay for faster reaction
}
