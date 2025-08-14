#include "JiYuAtk_lib.h"
#include <cstdio>
#include <string>
#include "Logger.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

std::string execCmd(std::string cmd, Logger logger) {
    char buf_ps[1024] = {};
    char ps[1024] = {0};
    char result[2048] = {};
    FILE *ptr;
    strcpy(ps, cmd.c_str());
    if((ptr = popen(ps, "r")) != NULL) {
        while(fgets(buf_ps, 1024, ptr) != NULL) {
            strcat(result, buf_ps);
            if(strlen(result) > 1024) break;
        }
        pclose(ptr);
        ptr = NULL;
        return result;
    }
    else {
        logger.log(Logger::IERROR, "Failed to popen %s", ps);
        return "";
    }
}
