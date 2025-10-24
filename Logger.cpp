#include "Logger.h"
#include <ctime>
#include <cstdio>
#include <string>

Logger::Logger(FILE* fp, int level) : fp(fp) {
    if(level >= Debug && level <= None) this->curLevel = level;
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
