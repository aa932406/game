#ifndef _CFGLIMITTIMETABLE_H_
#define _CFGLIMITTIMETABLE_H_

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
 * @brief CfgLimitTimeTable - Config data class
 */
class CfgLimitTimeTable
{
public:

    void AddLimitTime( CfgLimitTime * cfg);
    int32_t GetLimitTime( int32_t nIndex);

private:
        int64_t m_mLimitTime;
};

#endif // _CFGLIMITTIMETABLE_H_