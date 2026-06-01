// CExtCharTeamDungeon.h
#ifndef CEXTCHARTEAMDUNGEON_H
#define CEXTCHARTEAMDUNGEON_H

#include "CExtSystemBase.h"

class CExtCharTeamDungeon : public CExtSystemBase
{
public:
    CExtCharTeamDungeon();
    virtual ~CExtCharTeamDungeon();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onTeamDungeonCreate(Answer::NetPacket* inPacket);
    int32_t onTeamDungeonJoin(Answer::NetPacket* inPacket);
    int32_t onTeamDungeonStart(Answer::NetPacket* inPacket);
    
    void LeaveTeamDungeon();
    bool IsInTeamDungeon();
    bool checkTeamDungeon(const void* pCfgDungeon);
    
private:
    int32_t m_nId;
    int32_t m_nDungeonId;
    CharId_t m_nLeader;
};

#endif