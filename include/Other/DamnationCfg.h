#ifndef _DAMNATIONCFG_H_
#define _DAMNATIONCFG_H_

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

class DamnationCfg
{
public:
    DamnationCfg();
    ~DamnationCfg();
    DamnationCfg(const DamnationCfg& other);

private:
    int32_t Level;
    std::list<ItemData> CostItems;
    int32_t CostMoney;
    std::list<AddAttribute> AttrList;
    int32_t Probability;
    int32_t HpPecent;
    int32_t GongGaoId;
};

#endif // _DAMNATIONCFG_H_
