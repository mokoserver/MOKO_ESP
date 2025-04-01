#include "FSHelpers.h"
#include <Arduino.h>

bool initializeFileSystem() {
  if (!LittleFS.begin()) {
    Serial.println(" ");
    Serial.println(" ");
    Serial.println("********************************* ");
    Serial.println(" ");
    Serial.println("Ошибка монтирования файловой системы!");
    return false;
  }
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("********************************* ");
  Serial.println(" ");
  Serial.println("Файловая система инициализирована");
  
  // Создаем обязательные файлы, если их нет
  if (!LittleFS.exists("/index.html")) {
    File f = LittleFS.open("/index.html", "w");
    if (f) {
      f.println("<html><body><h1>Default Page</h1></body></html>");
      f.close();
      Serial.println("Создан default index.html");
    }
  }
  
  if (!LittleFS.exists("/style.css")) {
    File f = LittleFS.open("/style.css", "w");
    if (f) {
      f.println("body { font-family: Arial; }");
      f.close();
      Serial.println("Создан default style.css");
    }
  }
  
  checkFileSystem();
  return true;
}

// Остальные функции остаются аналогичными, но с заменой SPIFFS на LittleFS

void listFiles() {
  Serial.println("\nСодержимое файловой системы:");
  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {
    Serial.print("  ");
    Serial.print(dir.fileName());
    File f = dir.openFile("r");
    Serial.print(" (");
    Serial.print(f.size());
    Serial.println(" байт)");
    f.close();
  }
}

void readFile(const char* path) {
  Serial.print("\nЧтение файла ");
  Serial.println(path);
  
  if (!LittleFS.exists(path)) {
    Serial.println("  Файл не найден!");
    return;
  }

  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.println("  Ошибка открытия файла!");
    return;
  }
  
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
  Serial.println("\n--- Конец файла ---");
}

void checkFileSystem() {
  Serial.println("\nПроверка файловой системы:");
  
  const char* requiredFiles[] = {"/index.html", "/style.css"};
  for (const char* file : requiredFiles) {
    if (LittleFS.exists(file)) {
      File f = LittleFS.open(file, "r");
      Serial.printf("%s найден (%d байт)\n", file, f.size());
      f.close();
    } else {
      Serial.printf("%s ОТСУТСТВУЕТ\n", file);
    }
  }
  
  FSInfo fs_info;
  if (LittleFS.info(fs_info)) {
    Serial.printf("\nВсего места: %d байт\n", fs_info.totalBytes);
    Serial.printf("Использовано: %d байт\n", fs_info.usedBytes);
  }
}