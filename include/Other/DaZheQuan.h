#ifndef _DAZHEQUAN_H_
#define _DAZHEQUAN_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Common/CommonTypes.h"

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

class DaZheQuan
{
public:
    DaZheQuan();
    DaZheQuan(const DaZheQuan& a2);

    int32_t Index;
    int32_t CurrencyType;
    int32_t CurrencyValues;
    std::vector<MemChrBag> Items;
};

#endif // _DAZHEQUAN_H_
