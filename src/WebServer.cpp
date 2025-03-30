#include "WebServer.h"
#include "WiFiConfig.h"
#include <LittleFS.h>

// Определяем сервер
ESP8266WebServer server(80);

void handleRoot() {
  if (!LittleFS.exists("/index.html")) {
    server.send(500, "text/plain", "HTML file not found, please upload it");
    return;
  }
  
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    server.send(500, "text/plain", "Error opening HTML file");
    return;
  }
  
  server.streamFile(file, "text/html");
  file.close();
}

void handleCSS() {
  if (!LittleFS.exists("/style.css")) {
    server.send(500, "text/plain", "CSS file not found, please upload it");
    return;
  }
  
  File file = LittleFS.open("/style.css", "r");
  if (!file) {
    server.send(500, "text/plain", "Error opening CSS file");
    return;
  }
  
  server.streamFile(file, "text/css");
  file.close();
}

void handleSave() {
  String newSSID = server.arg("ssid");
  String newPass = server.arg("password");

  if (newSSID.isEmpty() || newPass.isEmpty()) {
    server.send(400, "text/plain", "Error: SSID and password cannot be empty!");
    return;
  }

  if (!saveWiFiCredentials(newSSID, newPass)) {
    server.send(500, "text/plain", "Failed to save settings");
    return;
  }
  
  server.send(200, "text/plain", "Settings saved! Device will try to connect to new network...");
  
  // Обновляем текущие учетные данные
  currentSSID = newSSID;
  currentPassword = newPass;
  
  // Пытаемся подключиться к новой сети
  setWiFiMode(false);
}

void initWebServer() {
  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/save", handleSave);
  
  server.begin();
  Serial.println("HTTP server started");
}