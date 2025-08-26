#ifndef JIYU_ATTACK
#define JIYU_ATTACK

#include <vector>
#include <string>
#include <regex>
#include "JiYuAtk_lib.h"
#include "Logger.h"
#include "ISocket.h"
#include <windows.h>

class JiYu_Attack {
    public:
        JiYu_Attack();
        static const std::vector<BYTE> cmdCodePrefix[4];
        static const int cmdContentBegin[2];
        enum cmdType {
            MSG = 0,
            CMD,
            REBOOT,
            SHUTDOWN
        };
        static const std::string nc_ps_url;
        ISocket* client;
        std::vector<std::string> IPParser(std::string rawIP);
        int sendCmd(std::string rawIP, int port, std::string cmd);
        int sendMsg(std::string rawIP, int port, std::string msg);
        int sendShutdown(std::string rawIP, int port);
        int sendReboot(std::string rawIP, int port);
        int netcat(std::string IP, int port, int ncport);
        int breakScreenControl();
        int continueScreenControl();
    
    private:
        Logger* logger;
};

DWORD WINAPI netcat_remote(LPVOID lpParameter);

#endif // JIYU_ATTACK
