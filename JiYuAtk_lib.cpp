#include "JiYuAtk_lib.h"
#include <cstdio>
#include <string>
#include <cstring>
#include <windows.h>
#include <tlhelp32.h>
#include "Logger.h"

int strToInt(std::string str) {
    if(str.empty()) return 0;
    int ret = 0, f = 1;
    if(str[0] == '-') f = -1;
    for(auto i: str) {
        if(isdigit(i)) ret = ret * 10 + i - '0';
    }
    return ret * f;
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

    if(Process32First(snapshot, &entry)) {
        do {
            if(processName == entry.szExeFile) pid.push_back(entry.th32ProcessID);
        } while(Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pid;
}

std::string IPDwordToString(DWORD ip) {
    WORD hiWord = HIWORD(ip);
    WORD loWord = LOWORD(ip);
    char ret[16];
    sprintf(ret, "%d.%d.%d.%d", LOBYTE(loWord), HIBYTE(loWord), LOBYTE(hiWord), HIBYTE(hiWord));
    return ret;
}
