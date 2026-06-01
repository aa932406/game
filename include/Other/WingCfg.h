#ifndef _WINGCFG_H_
#define _WINGCFG_H_

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
struct AttrAddon;

class WingCfg
{
public:
    WingCfg();
    ~WingCfg();
    WingCfg(const WingCfg& other);

private:
    int32_t Level;
    std::list<ItemData> ConstItems;
    std::vector<AttrAddon> AddonVector;
    int32_t StartPoints;
    int32_t SuccessPoints;
    int32_t MaxPoints;
    int32_t Rate;
    int32_t FailAddPoints;
};

#endif // _WINGCFG_H_
