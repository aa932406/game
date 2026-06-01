#ifndef _MEMCHRDEPOTDATA_H_
#define _MEMCHRDEPOTDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

struct DepotItem
{
    int32_t itemId;
    int8_t itemClass;
    int32_t itemCount;
    int8_t bind;
    int32_t endTime;
    int64_t srcId;
};

class MemChrDepotData
{
public:
    MemChrDepotData();
    ~MemChrDepotData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);
    std::string GetCurrencyString();
    void InitCurrency(std::string *p_CurrencyString);

private:
    std::string Password;
    std::string SecondPassword;
    DepotItem gambleDepot[560];
    int64_t Currency[12];
    int32_t OpneCount;
};

#endif // _MEMCHRDEPOTDATA_H_
