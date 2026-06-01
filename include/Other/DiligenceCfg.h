#ifndef _DILIGENCECFG_H_
#define _DILIGENCECFG_H_

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
struct RateItem;

class DiligenceCfg
{
public:
    DiligenceCfg();
    DiligenceCfg(const DiligenceCfg& other);

    void RandItem(RateItem* retstr);
    bool IsInItemList(int32_t ItemId, int8_t ItemCalss);

private:
    int32_t nId;
    int32_t nValues;
    std::list<RateItem> lItems;
};

#endif // _DILIGENCECFG_H_
