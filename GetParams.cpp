#include "GetParams.h"

#include <string>
#include <vector>

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
                    if(i < argc - 1) {
                        i++;
                        if(argv[i][0] != '-') {
                            ret.value = argv[i];
                        }
                        else ret.value = opt.defaultvalue;
                    }
                    break;
                }
            }
        }
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