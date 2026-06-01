#ifndef _ACTIVESKILL_H_
#define _ACTIVESKILL_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Common/CommonTypes.h"

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

class ActiveSkill
{
public:
    ActiveSkill();
    ActiveSkill(int32_t skillId, EntityId_t entityId, int32_t type);

    int32_t nSkillId;
    EntityId_t nEntityId;
    int32_t nType;
};

#endif // _ACTIVESKILL_H_
