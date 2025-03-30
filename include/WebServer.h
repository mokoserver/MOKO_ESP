#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server(80);

void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (file) {
    server.streamFile(file, "text/html");
    file.close();
  } else {
    server.send(500, "text/plain", "Error loading HTML file");
  }
}

void handleCSS() {
  File file = SPIFFS.open("/style.css", "r");
  if (file) {
    server.streamFile(file, "text/css");
    file.close();
  } else {
    server.send(500, "text/plain", "Error loading CSS file");
  }
}

void handleSave() {
  String newSSID = server.arg("ssid");
  String newPass = server.arg("password");

  if (newSSID.length() > 0 && newPass.length() > 0) {
    writeToEEPROM(0, newSSID);
    writeToEEPROM(32, newPass);
    
    server.send(200, "text/plain", "Settings saved! Rebooting...");
    delay(1000);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Error: SSID and password cannot be empty!");
  }
}

void initWebServer() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/save", handleSave);
  
  server.begin();
  Serial.println("HTTP server started");
}