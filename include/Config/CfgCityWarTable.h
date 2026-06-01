#ifndef _CFGCITYWARTABLE_H_
#define _CFGCITYWARTABLE_H_

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
 * @brief CfgCityWarTable - Config data class
 */
class CfgCityWarTable
{
public:

    void AddContReward(CfgCityWarTable * this, CfgCityWarContReward * stu);
    void GetContReward(CfgCityWarTable * this, int32_t id);

private:
        std::list<CfgCityWarContReward*> m_lContReward;
};

#endif // _CFGCITYWARTABLE_H_