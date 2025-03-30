// include/FSHelpers.h
#ifndef FS_HELPERS_H
#define FS_HELPERS_H

#include <FS.h>

void initializeFileSystem();
void listFiles();
void readFile(const char* path);
void checkFileSystem();

#endif