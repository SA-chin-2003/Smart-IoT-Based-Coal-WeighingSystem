#include "HX711.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
const char* server = "http://your-api-url.com/upload"; // Replace with your endpoint

HX711 scale;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  scale.begin(D3, D2);  // HX711 connections
  scale.set_scale(2280.f); // Calibrate this
  scale.tare(); // Set current weight as 0
}

void loop() {
  float weight = scale.get_units(5);
  Serial.print("Weight: ");
  Serial.println(weight);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(server);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"wagon_id\":\"001\", \"weight\":" + String(weight) + "}";
    http.POST(jsonData);
    http.end();
  }

  delay(5000);
}
