String processor(const String& var) {
  //Serial.println(var);
  if (var == "ssid") {
    return ssid;
  }
  else if (var == "cpip") {
    return WiFi.localIP().toString();
  } 
  else if (var == "conn_status") {
    return conn_status;
  }

  return String();
}


void controlerSite() {
}

void configSite() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/status.html", String(), false, processor);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/status.html", String(), false, processor);
  });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/jquery-3.6.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery-3.6.1.min.js", "text/javascript");
  });
  
  server.on("/loader.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/loader.css", "text/css");
  });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest * request) {
    int params = request->params();
    for (int i = 0; i < params; i++) {
      AsyncWebParameter* p = request->getParam(i);
      if (p->isPost()) {
        if (p->name() == "ssid") {
          strcpy(ssid, p->value().c_str());
          Serial.print("SSID set to: ");
          Serial.println(ssid);
        }
        if (p->name() == "password") {
          strcpy(password, p->value().c_str());
          Serial.print("Password set to: ");
          Serial.println(password);
        }
      }
    }

    test_credentials = true;
    request->redirect("/");
  });

  server.on("/restart", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("esp32 restarted");
    request->redirect("/");
  });

  server.begin();
}
