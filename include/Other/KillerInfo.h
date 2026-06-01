#ifndef _KILLERINFO_H_
#define _KILLERINFO_H_

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

class KillerInfo
{
public:
    KillerInfo();
    KillerInfo(const KillerInfo& a2);

    int32_t nCharId;
    std::string strName;
    int32_t nTime;
};

#endif // _KILLERINFO_H_
