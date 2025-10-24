#ifndef GETPARAMS
#define GETPARAMS

#include <string>
#include <vector>

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

#endif // GETPARAMS