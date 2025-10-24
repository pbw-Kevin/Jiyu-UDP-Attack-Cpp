#ifndef JIYUATK_LIB
#define JIYUATK_LIB

#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <windows.h>
#include <iphlpapi.h>
#include "Logger.h"

int strToInt(std::string str);

std::vector<BYTE> formatANSIString(const std::string str);

std::vector<DWORD> getProcessIdByName(const std::string& processName);

std::string IPDwordToString(DWORD ip);

#endif // JIYUATK_LIB
