#ifndef _VIPCFG_H_
#define _VIPCFG_H_

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
struct AttrAddon;

class VipCfg
{
public:
    VipCfg();
    ~VipCfg();
    VipCfg(const VipCfg& other);

    int32_t VipLevel;
    int32_t NeedVipExp;
    std::vector<MemChrBag> Item;
    std::vector<MemChrBag> DayReward;
    std::vector<AttrAddon> AtttVector;
    int32_t Retroactive;
    int32_t Luck1;
};

#endif // _VIPCFG_H_
