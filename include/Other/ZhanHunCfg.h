#ifndef _ZHANHUNCFG_H_
#define _ZHANHUNCFG_H_

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

class ZhanHunCfg
{
public:
    ZhanHunCfg(const ZhanHunCfg\& a2);

    int32_t nLevel;
    int32_t nMoney;
    int32_t nSyb;
    std::list<AddAttribute> lAddAttrs;
    int32_t nGongGaoId;
};

#endif // _ZHANHUNCFG_H_