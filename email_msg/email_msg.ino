#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>
#include <time.h>  // For NTP time

#define PIR_PIN D5       // Connect PIR sensor OUT pin to D5
#define LED_PIN D6       // LED pin (you can change if needed)

// WiFi credentials
const char* ssid = "Yasheena";
const char* password = "123456789";

// SMTP credentials
#define AUTHOR_EMAIL "testniro2001@gmail.com"
#define AUTHOR_PASSWORD "mujuffojegwwcpcg"  // Gmail App Password (no spaces)
#define RECIPIENT_EMAIL "testniro2001@gmail.com"
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465  // SSL port

SMTPSession smtp;
ESP_Mail_Session session;

unsigned long lastMotionTime = 0;       // track last motion time
const unsigned long cooldown = 60000;   // 1 minute = 60000 ms

// Callback to track email sending status
void smtpCallback(SMTP_Status status) {
  Serial.println(status.info());
  if (status.success()) {
    Serial.println("✅ Email sent successfully!");
  } else {
    Serial.println("❌ Email failed: " + smtp.errorReason());
  }
}

// Function to get current formatted time
String getTimeString() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // LED off at start

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ WiFi connected!");

  // Set NTP time (GMT+5:30 for Sri Lanka)
  configTime(5.5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("⏳ Syncing time...");
  while (time(nullptr) < 100000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Time synced!");

  // Configure SMTP session
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = ""; // usually empty for Gmail
}

void sendEmail() {
  String timeStr = getTimeString();
  String ipAddress = WiFi.localIP().toString();
  unsigned long uptimeSec = millis() / 1000;
  unsigned long hours = uptimeSec / 3600;
  unsigned long minutes = (uptimeSec % 3600) / 60;
  unsigned long seconds = uptimeSec % 60;

  // Build email body with details
  String body = "🚨 Motion Detected!\n\n";
  body += "📅 Date & Time: " + timeStr + "\n";
  body += "📡 WiFi SSID: " + String(ssid) + "\n";
  body += "🌐 Device IP: " + ipAddress + "\n";
  body += "⏱️ Uptime: " + String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s\n";
  body += "\n⚠️ PIR sensor detected motion near your device.";

  SMTP_Message message;
  message.sender.name = "ESP8266";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "PIR Motion Alert 🚨";
  message.addRecipient("User", RECIPIENT_EMAIL);
  message.text.content = body.c_str();

  smtp.debug(1);
  smtp.callback(smtpCallback);

  if (!smtp.connect(&session)) {
    Serial.println("❌ SMTP connection failed!");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("❌ Error sending email: " + smtp.errorReason());
  } else {
    Serial.println("📧 Email sent with details:\n" + body);
  }
}

void loop() {
  int val = digitalRead(PIR_PIN);

  if (val == HIGH) {
    digitalWrite(LED_PIN, HIGH); // Turn LED ON when motion detected
    if (millis() - lastMotionTime > cooldown) {
      String detectedAt = getTimeString();
      Serial.println("🚨 Motion detected at " + detectedAt + " → Sending email...");
      sendEmail();
      lastMotionTime = millis();  // reset cooldown
    }
  } else {
    digitalWrite(LED_PIN, LOW); // Turn LED OFF when no motion
  }

  delay(500);
}
