#ifndef _TIANLINGCFG_H_
#define _TIANLINGCFG_H_

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
struct AddAttribute;

class TianLingCfg
{
public:
    TianLingCfg();
    ~TianLingCfg();
    TianLingCfg(const TianLingCfg& other);

private:
    int32_t Level;
    std::list<ItemData> CostItems;
    std::list<AddAttribute> AttrList;
    int32_t GongGaoId;
    int32_t CostMoney;
    int32_t CostCoin;
};

#endif // _TIANLINGCFG_H_
