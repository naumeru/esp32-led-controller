#include <SPIFFS.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

char ssid[50];
char password[50];
char color[50];
unsigned short gpio;
unsigned short leds;
unsigned short lightmode;

bool test_credentials = false;
char conn_status[40] = "standby";



AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  config_load();

  if (attemptConnection()) {
    controlerSite();
  } else {
    WiFi.mode(WIFI_AP_STA);
    Serial.println("Launching AP: " + String(WiFi.softAP("esp32-led-controller") ? "Success" : "Fail")); // Launch the Access Point and print the status in serial monitor
    configSite();
  }
}

void loop() {
  if (test_credentials) {
    WiFi.begin(ssid, password);

    unsigned int attempts = 0;
    Serial.print("Connecting to WiFi ..");

    int timestamp = esp_timer_get_time() / 1000000;
    int previous_tick = esp_timer_get_time() / 1000000;
    strcpy(conn_status, "Connecting..");

    while (WiFi.status() != WL_CONNECTED && test_credentials == true) {
      if (esp_timer_get_time() / 1000000 - timestamp >= 10) {
        Serial.println(" Unsuccessful");
        strcpy(conn_status, "Failed!");
        test_credentials = false;
        return;
      }
      if (esp_timer_get_time() / 1000000 - previous_tick >= 1) {
        Serial.print(".");
        previous_tick = esp_timer_get_time() / 1000000;
        strcat(conn_status, ".");
      }
    }

    Serial.println(" Successful");
    Serial.print("Control panel IP: ");
    Serial.println(WiFi.localIP());
    strcpy(conn_status, "Success");
    test_credentials = false;
  }
}
