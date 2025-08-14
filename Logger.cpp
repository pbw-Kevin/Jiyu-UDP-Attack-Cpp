#include "Logger.h"
#include <ctime>
#include <cstdio>
#include <string>

Logger::Logger(FILE* fp) : fp(fp) {};

Logger::~Logger() {
    fclose(fp);
}

std::string Logger::getTime() {
	time_t rawtime;
	time(&rawtime);
	char tmstr[9];
	strftime(tmstr, 9, "%H:%M:%S", localtime(&rawtime));
	return (std::string)tmstr;
}

int Logger::getLevel() {
    return curLevel;
}

void Logger::setLevel(int level) {
    curLevel = level;
}

template<typename... Args>
void Logger::log(int level, std::string content, Args... args) {
    if(level < curLevel) return;
    std::string levelStr;
    if(level == DEBUG) levelStr = "DEBUG";
    else if(level == INFO) levelStr = "INFO";
    else if(level == WARNING) levelStr = "WARNING";
    else if(level == IERROR) levelStr = "ERROR";
    else if(level == CRITICAL) levelStr = "CRITICAL";
    fprintf(fp, ("[" + getTime() + "|" + levelStr + "] " + content + "\n").c_str(), args...);
}
