#ifndef _CLIBRARYITEM_H_
#define _CLIBRARYITEM_H_

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

class CLibraryItem
{
public:
    CLibraryItem();
    CLibraryItem(const CLibraryItem& a2);

    int32_t IsGet;
    int32_t nProbability;
    std::vector<MemChrBag> Items;
};

#endif // _CLIBRARYITEM_H_
