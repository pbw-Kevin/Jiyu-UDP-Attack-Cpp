#ifndef ISOCKET
#define ISOCKET

#ifdef _WIN32
#include <winsock2.h>
#include <string>
#include <vector>
#include <regex>
#include "Logger.h"
#include "JiYuAtk_lib.h"

struct StudentPort {
    int pid;
    std::string ip;
    int port;
};

class ISocket {
    public:
        ISocket(Logger* logger);
        ~ISocket();
        std::string localIP = "";
        std::vector<std::string> getLocalIPs();
        std::vector<StudentPort> getStudentPorts();
        int send(std::string IP, int port, std::vector<BYTE> data);
    private:
        WSADATA wsd;
        int optval = 1;
        SOCKET client;
        Logger* logger;
};

#else
#error "Invalid system. Windows required."
#endif // _WIN32

#endif // ISOCKET
