#include <Arduino.h>
#include "WiFiConfig.h"
#include "WebServer.h"
#include "FSHelpers.h"

void setup() {
  Serial.begin(115200);
  
  // Инициализация файловой системы
  initializeFileSystem();

  initWiFi();       // Инициализация Wi-Fi
  initWebServer();  // Запуск веб-сервера

  // Вывод текущего режима
  if (WiFi.getMode() & WIFI_AP) {
    Serial.println("\nРежим: Точка доступа");
    Serial.println("SSID: ESP_Config");
    Serial.println("IP: " + WiFi.softAPIP().toString());
  } else {
    Serial.println("\nРежим: Клиент");
    Serial.println("Подключено к: " + WiFi.SSID());
    Serial.println("IP: " + WiFi.localIP().toString());
  }

  Serial.println("\nВеб-сервер запущен");
  Serial.println("Откройте в браузере: http://" + (WiFi.getMode() & WIFI_AP ? WiFi.softAPIP() : WiFi.localIP()).toString());
}

void loop() {
  server.handleClient();
  
  static unsigned long lastIPprint = 0;
  if (millis() - lastIPprint > 60000) { // Каждую минуту
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Web interface: http://" + WiFi.localIP().toString());
    }
    Serial.println("AP interface: http://" + WiFi.softAPIP().toString());
    lastIPprint = millis();
  }
}
