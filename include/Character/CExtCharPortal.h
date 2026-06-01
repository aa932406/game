// CExtCharPortal.h
#ifndef CEXTCHARPOTAL_H
#define CEXTCHARPOTAL_H

#include "CExtSystemBase.h"
#include <list>

struct PortalInfo
{
    int32_t nId;
    int32_t nMapId;
    int32_t nPosX;
    int32_t nPosY;
    int32_t nDungeon;
    int32_t nStartTime;
    int32_t nDuration;
    bool bClose;
};

typedef std::list<PortalInfo> PortalInfoList;

class CExtCharPortal : public CExtSystemBase
{
public:
    CExtCharPortal();
    virtual ~CExtCharPortal();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onRequestInfo(Answer::NetPacket* inPacket);
    int32_t onUsePortal(Answer::NetPacket* inPacket);
    
    void SendPortalInfo(const PortalInfo* info);
    void SendPortalInfo(const PortalInfoList* lst);
    void SendPortalClose(int32_t nId);
    int32_t getPortalId();
    void checkPortalTime(bool bInit);
    void BroadcastPortal();
    
private:
    int32_t m_nPortalId;
    PortalInfoList m_lstPortal;
    int64_t m_nLastTick;
};

#endif