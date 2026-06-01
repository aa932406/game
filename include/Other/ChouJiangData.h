#ifndef _CHOUJIANSDATA_H_
#define _CHOUJIANSDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Character/CExtCharBag.h"

namespace Answer { class NetPacket; }

struct ChouJiangRecord
{
    MemChrBag CJItem;   // 抽奖物品
    int32_t Time;       // 抽奖时间戳
};

class ChouJiangData
{
public:
    ChouJiangData();
    ~ChouJiangData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    std::list<ChouJiangRecord> m_RecordList;
    int32_t   m_LuckyPoint;
    int32_t   m_Score;
    char      m_ItemList[0x3D40];
};

#endif
