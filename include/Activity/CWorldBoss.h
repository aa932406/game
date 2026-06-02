#ifndef _CWORLDBOSS_H_
#define _CWORLDBOSS_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Npc;
class CActivity;

enum BossType : int8_t
{
    BOSS_TYPE_NORMAL = 0,
    BOSS_TYPE_WORLD = 1,
    BOSS_TYPE_RUINS = 2,
};

class CWorldBoss
{
public:
    CWorldBoss();
    ~CWorldBoss();

    void Init(int32_t line);
    void InitDBInfo();
    Position GetRevivePos(int32_t BossId);
    void PacketBossInfo(Answer::NetPacket *packet, int8_t nBossType);
    void broadcastBossRevive(int32_t Mid, int32_t BossId, int32_t MapId);
    void GetWorldBossIcon(IconStateList * IconList);
    void SendWorldBossIcon(Player *pPlayer);
    void Update(int64_t CurTick);
    int32_t GetBossLevel(int32_t id);
    void adjustBossAttr(CfgMonster * cfgMonster, int32_t nLevel);
    Position OnBossRevive(int32_t nBossId, Map *pMap, Monster *pMonster);
    void OnBossSummon(int32_t nBossId, Map *pMap, Monster *pMonster);
    void UpdateWorldBossInfo(WorldBossInfo * info);
    void saveBossInfo(WorldBossInfo * info);
    void InitDropRecord();
    void SendDropRecord(int8_t connid, int16_t nGateIndex);
    int32_t GetBossRevie(int32_t mId);
    void SendRuinsBossInfo(Player *player, BossType nBossType, int32_t nMapId);

private:
        // TODO: 确认类型 m_Lock
        // TODO: 确认类型 m_RecordLock
        // TODO: 确认类型 m_dropRecords
        // TODO: 确认类型 m_dropRecordsSpecial
        int64_t m_lastUpdateTick;
        // TODO: 确认类型 m_mBossMap
};

#endif // _CWORLDBOSS_H_
