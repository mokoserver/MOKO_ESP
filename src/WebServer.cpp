#include "WebServer.h"
#include "WiFiConfig.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

// Определяем сервер
ESP8266WebServer server(80);
WebSocketsServer webSocket(81);
String lastRequestJson = "Waiting for requests...";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
      
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
      // Отправляем текущее состояние новому клиенту
      webSocket.sendTXT(num, lastRequestJson);
      break;
    }
      
    case WStype_TEXT:
      // Можно добавить обработку входящих сообщений
      break;
      
    default:
      // Остальные типы не обрабатываем
      break;
  }
}

void handleApiPost() {
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "No data received");
    return;
  }

  lastRequestJson = server.arg("plain");
  Serial.printf("Received JSON: %s\n", lastRequestJson.c_str());
  
  // Отправляем подтверждение
  server.send(200, "application/json", "{\"status\":\"success\"}");

  // Рассылаем всем подключенным клиентам
  webSocket.broadcastTXT(lastRequestJson);
}


void handleEvents() {
  Serial.println("SSE Client connected");
  
  server.sendHeader("Content-Type", "text/event-stream");
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Connection", "keep-alive");
  
  String data = "data: " + lastRequestJson + "\n\n";
  server.sendContent(data);
}

void initWebServer() {
  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/save", handleSave);
  
  server.on("/api/log", HTTP_POST, []() {
    server.collectHeaders("Content-Type", "Content-Length");
    handleApiPost();
  });
  
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("HTTP server and WebSocket started");
}

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