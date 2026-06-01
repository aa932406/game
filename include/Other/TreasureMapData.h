#ifndef _TREASUREMAPDATA_H_
#define _TREASUREMAPDATA_H_

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

class TreasureMapData
{
public:
    TreasureMapData(TreasureMapData *const this);

    Position Pos;
};

#endif // _TREASUREMAPDATA_H_
