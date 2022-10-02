bool attemptConnection() {
   
  if(strlen(ssid) == 0 || strlen(password) == 0)  {
    Serial.println("the ssid or password is empty");
    return false;
  }

  unsigned int attempts = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    if(attempts == 10){
      Serial.println(" Unsuccessful");
      return false;
    }
    Serial.print('.');
    attempts++;
    delay(1000);
  }
  Serial.println(" Successful");
  Serial.print("Control panel IP: ");
  Serial.println(WiFi.localIP());
  
  return true;
}
