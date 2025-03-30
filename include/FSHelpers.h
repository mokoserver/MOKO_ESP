#ifndef FS_HELPERS_H
#define FS_HELPERS_H

#include <LittleFS.h>

bool initializeFileSystem();
void listFiles();
void readFile(const char* path);
void checkFileSystem();

#endif