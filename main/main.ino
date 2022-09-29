#include <SPIFFS.h>
#include <ArduinoJson.h>
 
void setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  File file = SPIFFS.open("/config.json");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  StaticJsonDocument<100> doc;
  
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* ssid = doc["ssid"]; // nullptr
  const char* password = doc["password"]; // nullptr

  Serial.println(ssid);
  Serial.println(password);
  
  file.close();
}
 
void loop() {

}
