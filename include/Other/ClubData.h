#ifndef _CLUBDATA_H_
#define _CLUBDATA_H_

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

class ClubData
{
public:
    ClubData();
    ClubData(const ClubData& a2);

    int32_t nCid;
    std::string strName;
    int32_t nVipLevel;
    int32_t nVip7Time;
    int32_t nVip10Time;
};

#endif // _CLUBDATA_H_
