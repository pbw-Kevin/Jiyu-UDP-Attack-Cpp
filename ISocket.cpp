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
#pragma comment(lib, "iphlpapi.lib")
#endif // _MSC_VER

ISocket::ISocket(Logger* logger): logger(logger) {
    logger->log(Logger::Info, "Initializing ISocket...");
    if(WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
        logger->log(Logger::Error, "执行 WSAStartup 失败。");
        return;
    }

    client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(client <= 0) {
        logger->log(Logger::Error, "Socket 客户端启动失败。");
        return;
    }
    setsockopt(client, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(int));

    auto localIPs = getLocalIPs();
    auto studentPorts = getStudentPorts();
    for(auto port: studentPorts) {
        if(std::find(localIPs.begin(), localIPs.end(), port.ip) != localIPs.end()) {
            localIP = port.ip;
            break;
        }
    }
    if(localIP == ""){
        logger->log(Logger::Warning, "未能获取学生端监听端口对应的 IP，尝试使用其它本机 IP 代替……");
        if(localIPs.size()) localIP = localIPs[0];
        else logger->log(Logger::Warning, "未能获取合适的 IP。");
    }
    logger->log(Logger::Info, "ISocket initialized.");
}

ISocket::~ISocket() {
    WSACleanup();
}

std::vector<std::string> ISocket::getLocalIPs() {
    char host[100] = {};
    std::vector<std::string> ret;

    if(gethostname(host, sizeof(host)) == SOCKET_ERROR) {
        logger->log(Logger::Error, "执行 gethostname 失败。");
        return ret;
    }

    struct hostent *hp;
    if((hp = gethostbyname(host)) == NULL) {
        logger->log(Logger::Error, "执行 gethostbyname 失败。");
        return ret;
    }

    for(int i = 0; hp -> h_addr_list[i] != NULL; i++) {
        ret.push_back(inet_ntoa(*(struct in_addr*)hp->h_addr_list[i]));
    }
    return ret;
}

std::vector<StudentPort> ISocket::getStudentPorts() {
    std::vector<StudentPort> ret;
    auto studentMainPid = getProcessIdByName("StudentMain.exe");
    if(studentMainPid.empty()) {
        logger->log(Logger::Warning, "进程 StudentMain.exe 未找到。返回空结果。");
        return ret;
    }
    logger->log(Logger::Info, "Getting %d Student Terminal pid(s)...", (int)studentMainPid.size());
    DWORD dwBufferSize = 0;
    GetExtendedUdpTable(NULL, &dwBufferSize, true, AF_INET, UDP_TABLE_OWNER_PID, 0);
    MIB_UDPTABLE_OWNER_PID *pMibUdpTable = (MIB_UDPTABLE_OWNER_PID*)malloc(dwBufferSize);
    DWORD dwRet = GetExtendedUdpTable(pMibUdpTable, &dwBufferSize, true, AF_INET, UDP_TABLE_OWNER_PID, 0);
    if(dwRet != NO_ERROR) {
        logger->log(Logger::Error, "获取 UDP 端口列表失败。错误码：%lu", dwRet);
        return ret;
    }
    for(int i = 0; i < (int)pMibUdpTable->dwNumEntries; i++) {
        if(std::find(
            studentMainPid.begin(),
            studentMainPid.end(),
            pMibUdpTable->table[i].dwOwningPid
        ) != studentMainPid.end()) {
            ret.push_back({
                (int)pMibUdpTable->table[i].dwOwningPid,
                IPDwordToString(pMibUdpTable->table[i].dwLocalAddr),
                ntohs(pMibUdpTable->table[i].dwLocalPort)
            });
        }
    }
    return ret;
}

int ISocket::send(std::string IP, int port, std::vector<BYTE> data) {
    SOCKADDR_IN dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(IP.c_str());

    int sendRes;
    sendRes = sendto(client, (const char*)&data[0], data.size(), 0, (sockaddr*)&dest_addr, sizeof(sockaddr));
    if(sendRes == -1) {
        logger->log(Logger::Error, "发送失败。函数 sendto 出现问题。");
        return 1;
    }
    return 0;
}

#else
#error "Invalid system. Windows required."
#endif // _WIN32
