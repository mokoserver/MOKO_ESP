#include <ESP8266WiFi.h>
#include <EEPROM.h>

// Прототипы функций
bool connectToWiFi();
void startAccessPoint();
void initWiFi();

// Стандартные настройки (если EEPROM пуста)
const char* DEFAULT_SSID = "ESP8266_Default";
const char* DEFAULT_PASS = "12345678";

const char* AP_SSID = "ESP_Config";
const char* AP_PASS = "12345678";

String currentSSID;
String currentPassword;

// Объявление функций
void startAccessPoint() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP_Config", "12345678");
  Serial.println("Точка доступа запущена:");
  Serial.println("SSID: ESP_Config");
  Serial.println("IP: " + WiFi.softAPIP().toString());
}

// Чтение данных из EEPROM
String readFromEEPROM(int offset, int maxLen) {
  String data;
  for (int i = 0; i < maxLen; ++i) {
    char c = EEPROM.read(offset + i);
    if (c == 0) break;
    data += c;
  }
  return data;
}

// Запись данных в EEPROM
void writeToEEPROM(int offset, const String &data) {
  int len = data.length();
  for (int i = 0; i < len; ++i) {
    EEPROM.write(offset + i, data[i]);
  }
  EEPROM.write(offset + data.length(), 0); // Null-терминатор
  EEPROM.commit();
}

// Подключение к Wi-Fi
bool connectToWiFi() {
  WiFi.begin(currentSSID.c_str(), currentPassword.c_str());
  Serial.print("Подключение к ");
  Serial.print(currentSSID);

  int timeout = 20;
  while (WiFi.status() != WL_CONNECTED && timeout-- > 0) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nУспешно! IP: " + WiFi.localIP().toString());
    return true;
  } else {
    Serial.println("\nОшибка подключения! Запускаю точку доступа...");
    startAccessPoint();
    return false;
  }
}


void initWiFi() {
  EEPROM.begin(512);
  currentSSID = readFromEEPROM(0, 32);
  currentPassword = readFromEEPROM(32, 64);

  if (currentSSID.isEmpty()) {
    currentSSID = DEFAULT_SSID;
    currentPassword = DEFAULT_PASS;
  }

  // Всегда запускаем точку доступа ПАРАЛЛЕЛЬНО с клиентом
  WiFi.mode(WIFI_AP_STA);
  
  // Подключаемся к роутеру
  WiFi.begin(currentSSID.c_str(), currentPassword.c_str());
  
  // Запускаем точку доступа
  WiFi.softAP("ESP_Config", "12345678");

  Serial.print("Подключение к ");
  Serial.println(currentSSID);
  
  int timeout = 20;
  while (WiFi.status() != WL_CONNECTED && timeout-- > 0) {
    delay(500);
    Serial.print(".");
  }

  // Выводим оба IP
  Serial.println("\nAP IP: " + WiFi.softAPIP().toString());
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("STA IP: " + WiFi.localIP().toString());
  }
}