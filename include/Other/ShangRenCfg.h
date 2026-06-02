#ifndef _SHANGRENCFG_H_
#define _SHANGRENCFG_H_

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

class ShangRenCfg
{
public:
    ShangRenCfg(const ShangRenCfg\& a2);

    int32_t nId;
    std::vector<MemChrBag> vItem;
    int32_t nPrice;
};

#endif // _SHANGRENCFG_H_