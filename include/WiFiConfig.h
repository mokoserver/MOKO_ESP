#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <ESP8266WiFi.h>
#include <EEPROM.h>

// Максимальные длины (оставляем в заголовочном файле, так как это константы времени компиляции)
const int MAX_SSID_LEN = 32;
const int MAX_PASS_LEN = 64;

// Объявляем константы как extern
extern const char* DEFAULT_SSID;
extern const char* DEFAULT_PASS;
extern const char* AP_SSID; 
extern const char* AP_PASS;

// Объявляем переменные как extern
extern String currentSSID;
extern String currentPassword;

// Прототипы функций
bool connectToWiFi();
void startAccessPoint();
void initWiFi();
bool saveWiFiCredentials(const String &ssid, const String &pass);
void setWiFiMode(bool apMode);
String readFromEEPROM(int offset, int maxLen);
bool writeToEEPROM(int offset, const String &data, int maxLen);

#endif