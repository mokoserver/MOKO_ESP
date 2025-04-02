#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

extern ESP8266WebServer server;
extern String lastRequestJson;
extern WebSocketsServer webSocket;

void handleRoot();
void handleCSS();
void handleSave();
void handleApiPost();
void handleEvents();
void initWebServer();
void initWebSocket();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

#endif