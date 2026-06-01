#ifndef _QIQINGCFG_H_
#define _QIQINGCFG_H_

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

class QiQingCfg
{
public:
    QiQingCfg();
    ~QiQingCfg();
    QiQingCfg(const QiQingCfg& other);

    int32_t nLevel;
    std::list<ItemData> CostItems;
    std::list<AddAttribute> Attr;
    int32_t GongGaoId;
};

#endif // _QIQINGCFG_H_
