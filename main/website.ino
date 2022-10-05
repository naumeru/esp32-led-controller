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
  else if (var == "gpio") {
    return String(gpio);
  }
  else if (var == "leds") {
    return String(leds);
  }
  else if (var == "brightness") {
    return String(brightness);
  }

  return String();
}


void controlerSite() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  server.on("/setting", HTTP_POST, [](AsyncWebServerRequest * request) {
    request->redirect("/settings");
  });
  
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/settings.html", String(), false, processor);
  });

  server.on("/jquery-3.6.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery-3.6.1.min.js", "text/javascript");
  });
  
  server.on("/savesettings", HTTP_POST, [](AsyncWebServerRequest * request) {
    int params = request->params();
    for (int i = 0; i < params; i++) {
      AsyncWebParameter* p = request->getParam(i);
      if (p->isPost()) {
        if (p->name() == "gpio") {
          gpio = atoi(p->value().c_str());
          Serial.print("GPIO set to: ");
          Serial.println(gpio);
        }
        if (p->name() == "leds") {
          leds = atoi(p->value().c_str());
          Serial.print("LEDs set to: ");
          Serial.println(leds);
        }
      }
    }
    config_write();
    request->redirect("/settings");
  });

  server.on("/back", HTTP_POST, [](AsyncWebServerRequest * request) {
    request->redirect("/");
  });
  
  server.on("/disconnect", HTTP_POST, [](AsyncWebServerRequest * request) {
    strcpy(ssid, "");
    strcpy(password, "");
    config_write();
    ESP.restart();
    
    /* 
    ESP.restart();
    request->send(200);*/
  });
  
  server.serveStatic("/", SPIFFS, "/");
  
  server.begin();
}

void configSite() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/config.html", String(), false, processor);
  });

  server.on("/jquery-3.6.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery-3.6.1.min.js", "text/javascript");
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
    ESP.restart();
    request->send(200);
  });

  server.begin();
}
