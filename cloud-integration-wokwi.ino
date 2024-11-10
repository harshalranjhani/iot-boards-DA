#include <WiFi.h>
#include <HTTPClient.h>

const int led = 13;
const int pirdata = 15;

int pirstate = LOW;
int value = 0;

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* thingspeak_url = "https://api.thingspeak.com/alerts/send";
const char* alerts_api_key = "<--API-KEY-->";

unsigned long lastAlertTime = 0;
const unsigned long alertDelay = 60000;

void setup() {
  Serial.begin(9600);

  pinMode(led, OUTPUT);
  pinMode(pirdata, INPUT);

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password, 6);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void sendAlert(String subject, String body) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(thingspeak_url);
    
    http.addHeader("ThingSpeak-Alerts-API-Key", alerts_api_key);
    http.addHeader("Content-Type", "application/json");

    String jsonBody = "{\"subject\": \"" + subject + "\", \"body\": \"" + body + "\"}";

    int httpResponseCode = http.POST(jsonBody);

    if (httpResponseCode == 202) {
      Serial.println("Alert sent successfully.");
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Wi-Fi not connected");
  }
}

void loop() {
  value = digitalRead(pirdata);

  if (value == HIGH) {
    digitalWrite(led, HIGH);

    if (pirstate == LOW) {
      Serial.println("Motion detected!");
      pirstate = HIGH;

      unsigned long currentTime = millis();
      if (currentTime - lastAlertTime > alertDelay) {
        sendAlert("Motion Alert", "Motion detected!");
        lastAlertTime = currentTime;
      }
    }
  } else {
    digitalWrite(led, LOW);

    if (pirstate == HIGH) {
      Serial.println("Motion ended!");
      pirstate = LOW;

      unsigned long currentTime = millis();
      if (currentTime - lastAlertTime > alertDelay) {
        sendAlert("Motion Alert", "Motion ended!");
        lastAlertTime = currentTime;
      }
    }
  }

  delay(100);
}
