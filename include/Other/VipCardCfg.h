#ifndef _VIPCARDCFG_H_
#define _VIPCARDCFG_H_

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

class VipCardCfg
{
public:
    VipCardCfg(VipCardCfg *const this, const VipCardCfg *const a2);

    int32_t VipCardId;
    int32_t AddVipTime;
    int32_t NeedGold;
    std::list<AddAttribute> AddAttr;
    int32_t VipLevel;
    int32_t ReNeedGold;
    int32_t Money;
};

#endif // _VIPCARDCFG_H_