#ifndef _SPECIAEQUIPCFG_H_
#define _SPECIAEQUIPCFG_H_

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

class SpeciaEquipCfg
{
public:
    SpeciaEquipCfg();
    ~SpeciaEquipCfg();
    SpeciaEquipCfg(const SpeciaEquipCfg& other);

private:
    int32_t nIndex;
    std::list<AddAttribute> nAttrList;
    std::list<AddAttribute> nAttrList2;
    int32_t nNeedVipLevel;
    int32_t nContinueLogin;
    int32_t nNeedGold;
    int32_t nGongGaoId;
};

#endif // _SPECIAEQUIPCFG_H_
