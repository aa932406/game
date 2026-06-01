#ifndef _FAMILYMEMBERCARD_H_
#define _FAMILYMEMBERCARD_H_

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
#include "Other/ItemEffect.h"

class FamilyMemberCard : public ItemEffect
{
public:
    FamilyMemberCard();
    virtual ~FamilyMemberCard();

private:
    int32_t m_addon;
};

#endif // _FAMILYMEMBERCARD_H_
