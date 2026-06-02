#ifndef _SEVENTOUZI_H_
#define _SEVENTOUZI_H_

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

struct MemChrBag;

class SevenTouZi
{
public:
    SevenTouZi() = default;
    SevenTouZi(const SevenTouZi& a2);

    int32_t nId;
    int32_t nType;
    int32_t nCondition;
    std::vector<MemChrBag> vItem;
};

#endif // _SEVENTOUZI_H_