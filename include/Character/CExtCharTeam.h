// CExtCharTeam.h
#ifndef CEXTCHARTEAM_H
#define CEXTCHARTEAM_H

#include "CExtSystemBase.h"
#include <list>
#include <string>

struct TeamMember
{
    CharId_t nCharId;
    std::string strName;
    int16_t nIndex;
};

class CExtCharTeam : public CExtSystemBase
{
public:
    CExtCharTeam();
    virtual ~CExtCharTeam();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    
    int32_t onQueryTeamsAround(Answer::NetPacket* inPacket);
    int32_t onSetTeamAutoOperate(Answer::NetPacket* inPacket);
    int32_t onUpdateTeamInfo(Answer::NetPacket* inPacket);
    
    void Broadcast(Answer::NetPacket* inPacket);
    void LeaveTeam();
    void BroadcastBasicData();
    void UpdateTeamSetting();
    bool IsInTeam();
    bool IsFunctionOpen();
    
private:
    int32_t m_nTeamId;
    int64_t m_nLastQueryTeamTick;
    CharId_t m_nLeaderId;
    std::string m_strLeaderName;
    std::list<TeamMember> m_lstMember;
};

#endif