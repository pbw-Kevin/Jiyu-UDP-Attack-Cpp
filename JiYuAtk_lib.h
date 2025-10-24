#ifndef JIYUATK_LIB
#define JIYUATK_LIB

#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <windows.h>
#include "Logger.h"

std::string execCmd(std::string cmd, Logger* logger);

int strToInt(std::string str);

std::vector<BYTE> formatANSIString(const std::string str);

#endif // JIYUATK_LIB
