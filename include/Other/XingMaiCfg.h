#ifndef _XINGMAICFG_H_
#define _XINGMAICFG_H_

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

class XingMaiCfg
{
public:
    XingMaiCfg();
    ~XingMaiCfg();
    XingMaiCfg(const XingMaiCfg& other);

private:
    int32_t Level;
    int32_t PlayerLevel;
    std::list<ItemData> CostItems;
    std::list<AddAttribute> AttrList;
    int32_t GongGaoId;
    int32_t NeedBossScore;
    int32_t BossScoreLimit;
};

#endif // _XINGMAICFG_H_
