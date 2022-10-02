void config_load() { 
  if (!SPIFFS.begin()) {
    Serial.println("Error mounting the file system");
    return;
  } 
 
  File file = SPIFFS.open("/config.json", "r");
 
  if (!file) {
    Serial.println("Failed to open config for loading");
    return;
  }

  size_t filesize = file.size();
  char output[filesize + 1];

  file.read((uint8_t *)output, sizeof(output));
  file.close();
  output[filesize] = '\0';
  Serial.println(output);  

  DynamicJsonDocument doc(1024);
  DeserializationError err = deserializeJson(doc, output);
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.f_str());
  } 

  strcpy(ssid, doc["ssid"]);
  strcpy(password, doc["password"]);
  strcpy(color, doc["color"]);

  leds = atoi(doc["leds"]);
  gpio = atoi(doc["gpio"]);
  lightmode = atoi(doc["lightmode"]);

  file.close();
}

void config_write() {
  if (!SPIFFS.begin()) {
    Serial.println("Error mounting the file system");
    return;
  } 
  
  File file = SPIFFS.open("/config.json", "w");
 
  if (!file) {
    Serial.println("Failed to open config for writing");
    return;
  }

  DynamicJsonDocument doc(1024);

  doc["ssid"] = "";
  doc["password"] = "";
  doc["color"] = "green";
  doc["gpio"] = "2";
  doc["leds"] = "5";
  doc["lightmode"] = "2";

  char input[300];
  
  serializeJson(doc, input);

  int bytesWritten = file.print(input);
 
  if (bytesWritten > 0) {
    Serial.println("Config was written");
 
  } else {
    Serial.println("Config write failed");
  }
 
  file.close();
  for (int i = 5; i > 0; i--) {
    Serial.print("Restarting in ");
    Serial.println(i);
    delay(1000);
  }
  Serial.println("Restarting now!");
  ESP.restart();
}
