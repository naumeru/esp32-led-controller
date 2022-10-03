void controlerSite() {
}

void configSite() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/setup.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

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
        if (p->name() == "pass") {
          strcpy(password, p->value().c_str());
          Serial.print("Password set to: ");
          Serial.println(password);
        }
      }
    }

    test_credentials = true;
    request->send(200, "text/plain", "redirect to status page");
  });

  server.begin();
}
