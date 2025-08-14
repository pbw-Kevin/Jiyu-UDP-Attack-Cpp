#ifndef JIYU_ATTACK
#define JIYU_ATTACK

#include <vector>
#include "JiYuAtk_lib.h"
#include "Logger.h"

class JiYu_Attack {
    public:
        static const std::vector<int> cmdCodePrefix[4];
        enum cmdType {
            MSG = 0,
            CMD,
            REBOOT,
            SHUTDOWN
        };
        JiYu_Attack();
    
    private:
        Logger logger;
};

#endif // JIYU_ATTACK
