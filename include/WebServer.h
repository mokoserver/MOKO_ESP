#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WebServer.h>

// Только объявление, определение в WebServer.cpp
extern ESP8266WebServer server;

void handleRoot();
void handleCSS();
void handleSave();
void initWebServer();

#endif