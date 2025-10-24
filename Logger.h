#ifndef LOGGER
#define LOGGER

#include <ctime>
#include <cstdio>
#include <string>

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

#endif // LOGGER
