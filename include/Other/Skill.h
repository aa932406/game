#ifndef _SKILL_H_
#define _SKILL_H_

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

class Skill
{
public:
    Skill();
    Skill(const CfgActiveSkill* pCfgSkill, int32_t nSkillFlag);

    int32_t calBaseRevive(int32_t atkMin, int32_t atkMax, int32_t sklModify);

private:
    const CfgActiveSkill* m_cfg;
    int32_t m_nSkillFlag;
};

#endif // _SKILL_H_