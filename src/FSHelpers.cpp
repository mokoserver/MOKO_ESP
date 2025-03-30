// src/FSHelpers.cpp
#include "FSHelpers.h"
#include <Arduino.h>

void initializeFileSystem() {
  if (!SPIFFS.begin()) {
    Serial.println("Ошибка монтирования файловой системы!");
    return;
  }
  Serial.println("Файловая система инициализирована");
  checkFileSystem();
}

void listFiles() {
  Serial.println("\nСодержимое файловой системы:");
  Dir dir = SPIFFS.openDir("/");
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
  
  if (!SPIFFS.exists(path)) {
    Serial.println("  Файл не найден!");
    return;
  }

  File file = SPIFFS.open(path, "r");
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
    if (SPIFFS.exists(file)) {
      File f = SPIFFS.open(file, "r");
      Serial.printf("%s найден (%d байт)\n", file, f.size());
      f.close();
    } else {
      Serial.printf("%s ОТСУТСТВУЕТ\n", file);
    }
  }
  
  FSInfo fs_info;
  if (SPIFFS.info(fs_info)) {
    Serial.printf("\nВсего места: %d байт\n", fs_info.totalBytes);
    Serial.printf("Использовано: %d байт\n", fs_info.usedBytes);
  }
}