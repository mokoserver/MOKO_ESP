#include "WiFiConfig.h"

// Определяем константы только здесь
const char* DEFAULT_SSID = "ESP8266_Default";
const char* DEFAULT_PASS = "12345678";
const char* AP_SSID = "ESP_Config";
const char* AP_PASS = "12345678";

// Определяем глобальные переменные
String currentSSID;
String currentPassword;

String readFromEEPROM(int offset, int maxLen) {
  String data;
  for (int i = 0; i < maxLen; ++i) {
    char c = EEPROM.read(offset + i);
    if (c == 0) break;
    data += c;
  }
  return data;
}

bool writeToEEPROM(int offset, const String &data, int maxLen) {
  if (data.length() > static_cast<unsigned int>(maxLen - 1)) {
    Serial.println("Ошибка: данные слишком длинные для EEPROM");
    return false;
  }
  
  for (unsigned int i = 0; i < data.length(); ++i) {
    EEPROM.write(offset + i, data[i]);
  }
  EEPROM.write(offset + data.length(), 0);
  return EEPROM.commit();
}

bool saveWiFiCredentials(const String &ssid, const String &pass) {
  if (!writeToEEPROM(0, ssid, MAX_SSID_LEN) || !writeToEEPROM(32, pass, MAX_PASS_LEN)) {
    Serial.println("Ошибка сохранения в EEPROM");
    return false;
  }
  return true;
}

bool connectToWiFi() {
  if (currentSSID.isEmpty() || currentPassword.isEmpty()) {
    Serial.println("SSID или пароль не заданы");
    return false;
  }

  WiFi.begin(currentSSID.c_str(), currentPassword.c_str());
  Serial.print("Подключение к ");
  Serial.print(currentSSID);

  int timeout = 20;
  while (WiFi.status() != WL_CONNECTED && timeout-- > 0) {
    delay(500);
    Serial.print(".");
  }

  return WiFi.status() == WL_CONNECTED;
}

void startAccessPoint() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println("Точка доступа запущена:");
  Serial.println("SSID: " + String(AP_SSID));
  Serial.println("IP: " + WiFi.softAPIP().toString());
}

void setWiFiMode(bool apMode) {
  if (apMode) {
    startAccessPoint();
  } else {
    WiFi.mode(WIFI_STA);
    if (connectToWiFi()) {
      Serial.println("Успешно подключено!");
      Serial.println("IP: " + WiFi.localIP().toString());
    } else {
      Serial.println("Ошибка подключения! Переключаюсь в режим AP");
      startAccessPoint();
    }
  }
}

void initWiFi() {
  EEPROM.begin(512);
  currentSSID = readFromEEPROM(0, MAX_SSID_LEN);
  currentPassword = readFromEEPROM(32, MAX_PASS_LEN);

  if (currentSSID.isEmpty()) {
    currentSSID = DEFAULT_SSID;
    currentPassword = DEFAULT_PASS;
  }

  // Пытаемся подключиться к сети
  setWiFiMode(false);
}