#include "JiYuAtk_lib.h"
#include <cstdio>
#include <string>
#include <cstring>
#include <windows.h>
#include <tlhelp32.h>
#include "Logger.h"

int strToInt(std::string str) {
    int ret = 0;
    for(auto i: str) {
        ret = ret * 10 + i - '0';
    }
    return ret;
}

std::vector<BYTE> formatANSIString(const std::string str) {
    int num = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    wchar_t *wide = new wchar_t[num];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wide, num);
    std::vector<BYTE> ret((num - 1) * sizeof(wchar_t));
    memcpy(&ret[0], wide, (num - 1) * sizeof(wchar_t));
    return ret;
}

std::vector<DWORD> getProcessIdByName(const std::string& processName) {
    std::vector<DWORD> pid;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(snapshot, &entry)) {
        do {
            if (processName == entry.szExeFile) {
                pid.push_back(entry.th32ProcessID);
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pid;
}

std::string IPDwordToString(DWORD ip) {
    WORD hiWord=HIWORD(ip);
    WORD loWord=LOWORD(ip);
    char ret[20];
    sprintf(ret, "%d.%d.%d.%d", LOBYTE(loWord), HIBYTE(loWord), LOBYTE(hiWord), HIBYTE(hiWord));
    return ret;
}
