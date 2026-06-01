#ifndef _XINMOCFG_H_
#define _XINMOCFG_H_

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

struct AddAttribute;

class XinMoCfg
{
public:
    XinMoCfg(XinMoCfg *const this, const XinMoCfg *const a2);

    int32_t XinMoLevel;
    int32_t CostMoney;
    int32_t ConstShenYaoBi;
    int32_t ConstExp;
    int32_t NeedQiQingLevel;
    std::list<AddAttribute> Attr;
    int32_t GongGaoId;
};

#endif // _XINMOCFG_H_