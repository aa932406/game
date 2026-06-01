#ifndef _CLITTLEHELPERDATA_H_
#define _CLITTLEHELPERDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Character/CharLittlerHelper.h"

namespace Answer { class NetPacket; }

class CLittleHelperData
{
public:
    CLittleHelperData();
    ~CLittleHelperData();

    void CleanUp();
    std::string getRewardState();
    void SaveRewardState(std::string *p_String);
    void PackageData(Answer::NetPacket *packet);

private:
    std::map<int32_t, ActLittleHelperInfo> m_ActLittleHelperInfoMap;
    int32_t m_LittleHelperId;
};

#endif
