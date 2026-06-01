#ifndef _MAPBOSSINFO_H_
#define _MAPBOSSINFO_H_

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

class MapBossInfo
{
public:
    MapBossInfo();
    MapBossInfo(const MapBossInfo& a2);

    int32_t nId;
    std::list<int32_t> BossMapList;
};

#endif // _MAPBOSSINFO_H_
