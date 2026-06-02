#ifndef _CFGMYSJREWARDTABLE_H_
#define _CFGMYSJREWARDTABLE_H_

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
 * @brief CfgMYSJRewardTable - Config data class
 */
class CfgMYSJRewardTable
{
public:
    CfgMYSJRewardTable();
    ~CfgMYSJRewardTable();

    bool Add( int32_t nId, CfgMYSJReward * reward);
    int32_t GetGroupId( int32_t nDungeonId);

private:
        // TODO: 确认类型 m_mDungeonGroup
        // TODO: 确认类型 m_mGroupReward
        // TODO: 确认类型 m_mReward
};

#endif // _CFGMYSJREWARDTABLE_H_