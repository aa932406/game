#ifndef _CHANGEITEMCFG_H_
#define _CHANGEITEMCFG_H_

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

class ChangeItemCfg
{
public:
    ChangeItemCfg();
    ~ChangeItemCfg();
    ChangeItemCfg(const ChangeItemCfg& other);

private:
    std::list<ItemData> lCostItem;
    int32_t nCostGold;
    std::vector<MemChrBag> GetItems;
    int32_t Times;
};

#endif // _CHANGEITEMCFG_H_
