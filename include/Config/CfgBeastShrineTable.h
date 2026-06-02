#ifndef _CFGBEASTSHRINETABLE_H_
#define _CFGBEASTSHRINETABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;

struct CfgBeastShrineCost {
    int32_t nTimes;
    std::list<ItemData> lCost;
};

/**
 * @brief CfgBeastShrineTable - Config data class
 */
class CfgBeastShrineTable
{
public:

    void GetEnterCost(int32_t nTimes);

private:
        std::map<int32_t, CfgBeastShrineCost*> m_mEnterCost;
};

#endif // _CFGBEASTSHRINETABLE_H_