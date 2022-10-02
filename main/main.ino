#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

char ssid[50];
char password[50];
char color[50];
unsigned short gpio;
unsigned short leds;
unsigned short lightmode;

void setup() {
  Serial.begin(115200);
  config_load();
  if(attemptConnection()) {
    controlerSite();
  } else {
    WiFi.mode(WIFI_AP);
    Serial.println("Launching AP: " + String(WiFi.softAP("esp32-led-controller") ? "Success" : "Fail")); // Launch the Access Point and print the status in serial monitor
    configSite();
  }
}
 
void loop() {

}
