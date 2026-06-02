#ifndef _VIPEQUIPPOSSUIT_H_
#define _VIPEQUIPPOSSUIT_H_

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

class VipEQuipPosSuit
{
public:
    VipEQuipPosSuit(const VipEQuipPosSuit\& a2);

    int32_t nIndex;
    int32_t nLevel;
    std::list<AddAttribute> lAddAttrs;
    int32_t nTalentId;
    int32_t TalentLevel;
};

#endif // _VIPEQUIPPOSSUIT_H_