#ifndef _GMBROADCAST_H_
#define _GMBROADCAST_H_

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

class GMBroadcast
{
public:
    GMBroadcast();
    GMBroadcast(const GMBroadcast& other);

    int32_t nId;
    int32_t nType;
    std::string strText;
    int32_t nStartTime;
    int32_t nInterval;
    int32_t nTimes;
    int32_t nLastTime;
    int32_t nCount;
};

#endif // _GMBROADCAST_H_