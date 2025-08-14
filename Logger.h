#ifndef LOGGER
#define LOGGER

#include <ctime>
#include <cstdio>
#include <string>

class Logger {
    public:
        Logger(FILE* fp);
        ~Logger();
        enum LogLevel {
            DEBUG = 1,
            INFO,
            WARNING,
            IERROR,
            CRITICAL,
            NONE
        };;
        static std::string getTime();
        void setLevel(int level);
        int getLevel();
        template<typename... Args>
        void log(int level, std::string content, Args... args);

    private:
        FILE* fp;
        int curLevel = WARNING;
};

#endif // LOGGER
