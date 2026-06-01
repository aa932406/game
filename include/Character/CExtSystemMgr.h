// CExtSystemMgr.h
#ifndef CEXTSYSTEMMGR_H
#define CEXTSYSTEMMGR_H

#include "CExtSystemBase.h"
#include <map>
#include <list>

class CExtSystemMgr : public CExtSystemBase
{
public:
    CExtSystemMgr();
    virtual ~CExtSystemMgr();
    
    void Register(CExtSystemBase* pExtSystem);
    void Unregister(CExtSystemBase* pExtSystem);
    CExtSystemBase* GetHandler(ProcId_t procId);
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual void InitSystem() override;
    
    int32_t DispatchProtocol(ProcId_t nProcId, Answer::NetPacket* inPacket);
    
private:
    void registProtocolHandler(ProcId_t procId, CExtSystemBase* pHanlder);
    
    std::map<ProcId_t, CExtSystemBase*> m_mapProcHandler;
    std::list<CExtSystemBase*> m_lstExtSystems;
};

#endif