#include "JiYuAtk_lib.h"
#include <cstdio>
#include <string>
#include <cstring>
#include "Logger.h"

std::string execCmd(std::string cmd, Logger* logger) {
    char buf_ps[1024] = {};
    char ps[1024] = {0};
    char result[2048] = {};
    auto ptr = new FILE;
    strcpy(ps, cmd.c_str());
    if((ptr = _popen(ps, "r")) != NULL) {
        while(fgets(buf_ps, 1024, ptr) != NULL) {
            strcat(result, buf_ps);
            if(strlen(result) > 1024) break;
        }
        _pclose(ptr);
        ptr = NULL;
        return result;
    }
    else {
        logger->log(Logger::Error, "Failed to popen %s", ps);
        return "";
    }
}

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
