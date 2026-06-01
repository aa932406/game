#ifndef _CHARWISHINFO_H_
#define _CHARWISHINFO_H_

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

class CharWishInfo
{
public:
    CharWishInfo();
    CharWishInfo(const CharWishInfo& other);

    int32_t nId;
    int32_t nItemId;
    int32_t nStartTime;
    int32_t nGetReward;
    std::string strInfo;
};

#endif // _CHARWISHINFO_H_