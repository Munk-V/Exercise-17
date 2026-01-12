#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// WiFi
const char* WIFI_SSID = "ASUS_48";
const char* WIFI_PASS = "wanglta210";

// ThingSpeak
WiFiClient client;
unsigned long CHANNEL_ID = 3226621;
const char* WRITE_API_KEY = "2L0NUIRK1NVJHE8J";

// External LED on breadboard
const int EXT_LED_PIN = D2;   // GPIO5 on NodeMCU
int ledState = 0;             // 0/1

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting to WiFi");
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
    // optional: timeout after 30 sec
    if (millis() - t0 > 30000) {
      Serial.println("\nWiFi connect timeout, retrying...");
      WiFi.disconnect();
      delay(500);
      WiFi.begin(WIFI_SSID, WIFI_PASS);
      t0 = millis();
    }
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(EXT_LED_PIN, OUTPUT);
  digitalWrite(EXT_LED_PIN, LOW); // OFF (pin->LOW)

  connectWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  // Reconnect if WiFi drops
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost, reconnecting...");
    connectWiFi();
  }

  // 1) Make data
  long rssi = WiFi.RSSI();        // Field 1 (e.g. -55)
  ledState = !ledState;           // toggle Field 2 (0/1)

  // External LED: HIGH = ON with wiring D1->R->LED->GND
  digitalWrite(EXT_LED_PIN, ledState ? HIGH : LOW);

  // 2) Send to ThingSpeak
  ThingSpeak.setField(1, rssi);
  ThingSpeak.setField(2, ledState);

  int code = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);

  if (code == 200) {
    Serial.print("Upload OK | RSSI=");
    Serial.print(rssi);
    Serial.print(" | LED=");
    Serial.println(ledState);
  } else {
    Serial.print("Upload failed HTTP=");
    Serial.println(code);
  }

  delay(20000); // >= 15 sec for ThingSpeak free
}
