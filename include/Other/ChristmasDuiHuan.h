#ifndef _CHRISTMASDUIHUAN_H_
#define _CHRISTMASDUIHUAN_H_

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
struct ItemData;
struct MemChrBag;

class ChristmasDuiHuan
{
public:
    ChristmasDuiHuan();
    ~ChristmasDuiHuan();
    ChristmasDuiHuan(const ChristmasDuiHuan& other);

private:
    int32_t nIndx;
    std::vector<MemChrBag> Items;
    std::list<ItemData> CostItems;
    int32_t nLimitCount;
    int64_t nStartTime;
    int64_t nEndTime;
};

#endif // _CHRISTMASDUIHUAN_H_
