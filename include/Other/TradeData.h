#ifndef _TRADEDATA_H_
#define _TRADEDATA_H_

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

class TradeData
{
public:

    void OnClanup();

    int32_t BagSlot;
    MemChrBag CharBagData;
};

#endif // _TRADEDATA_H_
