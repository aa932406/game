#ifndef _SYSTEMOPENGIFT_H_
#define _SYSTEMOPENGIFT_H_

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

class SystemOpenGift
{
public:
    SystemOpenGift(const SystemOpenGift\& a2);

    int32_t Id;
    int32_t TypeId;
    std::vector<MemChrBag> Gifts;
    int32_t Star;
};

#endif // _SYSTEMOPENGIFT_H_