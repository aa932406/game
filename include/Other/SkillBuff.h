#ifndef _SKILLBUFF_H_
#define _SKILLBUFF_H_

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

class SkillBuff
{
public:
    ~SkillBuff();

    void interval();
    void broadcastIntervalEffect(int32_t hpchange, int32_t attackState);

private:
        // TODO: 确认类型 m_Mid
        const CfgActivity* m_cfgBuff;  // TODO: 确认配置类型
        // TODO: 确认类型 m_launcher
        // TODO: 确认类型 m_nAtkMax
        // TODO: 确认类型 m_skillid
        // TODO: 确认类型 m_unit
};

#endif // _SKILLBUFF_H_
