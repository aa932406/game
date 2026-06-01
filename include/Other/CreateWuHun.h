#ifndef _CREATEWUHUN_H_
#define _CREATEWUHUN_H_

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
struct RateItem;

class CreateWuHun
{
public:
    CreateWuHun();
    ~CreateWuHun();
    CreateWuHun(const CreateWuHun& other);

private:
    int32_t nId;
    std::list<ItemData> ConstItem;
    std::list<RateItem> GetItemRate;
    std::list<RateItem> GetItemRate2;
    int32_t SpecialCost;
};

#endif // _CREATEWUHUN_H_
