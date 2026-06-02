#ifndef _CFGDRAW_H_
#define _CFGDRAW_H_

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


/**
 * @brief CfgDraw - Config data class
 */
class CfgDraw
{
public:
        CfgDraw();
    CfgDraw(const CfgDraw& a2);
    CfgDraw& operator=(const CfgDraw& a2);

    void Add( CfgDrawReward * reward);

private:
        int32_t m_nMaxRate;
        int32_t m_nType;
        std::list<CfgDrawReward*> m_rewards;
};

#endif // _CFGDRAW_H_