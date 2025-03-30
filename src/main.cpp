#include <Arduino.h>
#include "WiFiConfig.h"
#include "WebServer.h"
#include "FSHelpers.h"

void setup() {
  Serial.begin(115200);
  
  if (!initializeFileSystem()) {
    Serial.println("Не удалось инициализировать файловую систему. Работа невозможна.");
    while (true) {}
  }
  listFiles();  // Выведет список файлов в Serial Monitor
  initWiFi();
  initWebServer();

  Serial.println("\nВеб-сервер запущен");
  
  if (WiFi.getMode() & WIFI_AP) {
    Serial.println("AP interface: http://" + WiFi.softAPIP().toString());
  } else if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Web interface: http://" + WiFi.localIP().toString());
  }
}

void loop() {
  server.handleClient();
  
  static unsigned long lastIPprint = 0;
  if (millis() - lastIPprint > 60000) {
    if (WiFi.getMode() & WIFI_AP) {
      Serial.println("AP interface: http://" + WiFi.softAPIP().toString());
    } else if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Web interface: http://" + WiFi.localIP().toString());
    }
    lastIPprint = millis();
  }
}
