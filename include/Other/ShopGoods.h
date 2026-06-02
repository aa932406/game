#ifndef _SHOPGOODS_H_
#define _SHOPGOODS_H_

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

class ShopGoods
{
public:
    ShopGoods(const ShopGoods\& a2);

    std::vector<MemChrBag> vItem;
    int32_t nPrice;
};

#endif // _SHOPGOODS_H_