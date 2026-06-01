#ifndef _CVIPCLUB_H_
#define _CVIPCLUB_H_

#include <cstdint>
#include <map>
#include <vector>

#include "Other/ClubData.h"

namespace Answer { class NetPacket; }

class CVipClub
{
public:
    CVipClub();
    ~CVipClub();

    void Init(int32_t line);
    int32_t CalculationClubLevel();
    void CalculationClubWallInfo();
    void AppendClubWallInfo(Answer::NetPacket* packet);

private:
    std::map<int64_t, ClubData> m_ClubDataMap;
    int32_t m_ClubLevel;
    std::map<int, int32_t> m_LevelCountMap;
    ClubData m_Vip10ClubData;
    std::vector<ClubData> m_Vip7ClubDataVt;
};

#endif // _CVIPCLUB_H_
