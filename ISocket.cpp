#include "ISocket.h"

#ifdef _WIN32
#include <winsock2.h>
#include <string>
#include <vector>
#include <regex>
#include "Logger.h"
#include "JiYuAtk_lib.h"

#ifdef _MSC_VER
#pragma comment(lib,"ws2_32.lib")
#endif // _MSC_VER

ISocket::ISocket(Logger logger): logger(logger) {
    if(WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
        logger.log(Logger::IERROR, "执行 WSAStartup 失败。");
        return;
    }

    client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(client <= 0) {
        logger.log(Logger::IERROR, "Socket 客户端启动失败。");
        return;
    }
    setsockopt(client, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(int));

    std::vector<std::string> localIPs = getLocalIPs();
    for(auto IP: localIPs) {
        if(getStudentPorts(IP).size()) {
            localIP = IP;
            break;
        }
    }
    if(localIP != "" && localIPs.size()) localIP = localIPs[0];
}

ISocket::~ISocket() {
    WSACleanup();
}

std::vector<std::string> ISocket::getLocalIPs() {
    char host[100] ={0};
    std::vector<std::string> ret;

    if(gethostname(host, sizeof(host)) == SOCKET_ERROR) {
        logger.log(Logger::IERROR, "执行 gethostname 失败。");
        return ret;
    }

    struct hostent *hp;
    if((hp = gethostbyname(host)) == NULL) {
        logger.log(Logger::IERROR, "执行 gethostbyname 失败。");
        return ret;
    }

    for(int i = 0; hp -> h_addr_list[i] != NULL; i++) {
        ret.push_back(inet_ntoa(*(struct in_addr*)hp->h_addr_list[i]));
    }
    return ret;
}

std::vector<int> ISocket::getStudentPorts(std::string IP) {
    if(IP == "") IP = localIP;
    std::vector<int> ret;

    std::string taskStudent = execCmd("tasklist | find \"Student\"", logger);
    std::regex pattern("[e]\\s*\\d{1,5}\\s*[C]");
    std::smatch matches;
    if(!std::regex_search(taskStudent, matches, pattern)){
        logger.log(Logger::WARNING, "进程 StudentMain.exe 未找到。返回空结果。");
        return ret;
    }
    std::string studentPID = matches[0];
    studentPID = studentPID.substr(1, studentPID.size() - 2);
    while(!isprint(studentPID.front())) studentPID.erase(studentPID.begin());
    while(!isprint(studentPID.back())) studentPID.pop_back();

    std::string netstat = execCmd("netstat -ano | find \"" + studentPID + "\"", logger);
    pattern = std::regex(IP + ":\\d{1,5}\\s*[*]");
    while(std::regex_search(netstat, matches, pattern)) {
        std::string portStr = matches[0];
        portStr = portStr.substr(IP.size() + 1, portStr.size() - IP.size() - 2);
        while(!isprint(portStr.back())) portStr.pop_back();
        int port = 0;
        for(auto i: portStr) port = port * 10 + i - '0';
        ret.push_back(port);
        netstat = matches.suffix().str();
    }
    return ret;
}

int ISocket::send(std::string IP, int port, std::vector<BYTE> data) {
    SOCKADDR_IN dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.S_un.S_addr = inet_addr(IP.c_str());

    int sendRes;
    sendRes = sendto(client, (const char*)&data[0], data.size() * sizeof(BYTE), 0, (sockaddr*)&dest_addr, sizeof(sockaddr));
    if(sendRes == -1) {
        logger.log(Logger::IERROR, "发送失败。函数 sendto 出现问题。");
        return 1;
    }
    return 0;
}

#else
#error "Invalid system. Windows required."
#endif // _WIN32
