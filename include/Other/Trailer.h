#ifndef _TRAILER_H_
#define _TRAILER_H_

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

class Trailer
{
public:
    void** _vptr_Entity;        // vtable pointer (decompiler stub)

    Trailer();
    ~Trailer();

    void reset();
    bool refresh();
    FamilyId_t GetFamilyId();
    void postDamage(int32_t damge, UnitHandle launcher, int32_t Mid);
    bool appendInfo(Answer::NetPacket *packet);
    void LeaveMap();
    void EnterMap(Map *pMap, int32_t tx, int32_t ty);
    void SendInfo(Player *player);
    void SendPos(Player *player);
    bool isLifeEnd();
    void CheckTrailerEnd();
    float GetRate();
    int32_t GetRewardTimes();

private:
        int64_t m_DieTick;
        // TODO: 确认类型 m_Level
        // TODO: 确认类型 m_Name
        // TODO: 确认类型 m_Owner
        int64_t m_RewardTimes;
        int32_t m_RobberyId;
        // TODO: 确认类型 m_RobberyName
        int32_t m_State;
        const CfgActivity* m_cfgTrailer;  // TODO: 确认配置类型
        int64_t m_nEndTime;
        // TODO: 确认类型 m_pMap
};

#endif // _TRAILER_H_
