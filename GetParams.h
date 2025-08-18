#ifndef GETPARAMS
#define GETPARAMS

#include <string>
#include <vector>

struct ParamOpt {
    std::string id = "";
    char opt = 0;
    std::string longopt = "";
    bool withParam = false;
    std::string defaultvalue = "";
};

struct ParamRet {
    std::string id = "";
    bool exists = false;
    std::string value = "";
};

std::vector<ParamRet> GetParams(int argc, char *argv[], std::vector<ParamOpt> opts);
ParamRet GetParamfromParams(std::string id, std::vector<ParamRet> rets);

#endif // GETPARAMS