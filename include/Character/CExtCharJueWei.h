// CExtCharJueWei.h
#ifndef CEXTCHARJUEWEI_H
#define CEXTCHARJUEWEI_H

#include "CExtSystemBase.h"

class CExtCharJueWei : public CExtSystemBase
{
public:
    CExtCharJueWei();
    virtual ~CExtCharJueWei();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onRequestJueWeiInfo(Answer::NetPacket* inPacket);
    int32_t onAskJueWeiLevelUp(Answer::NetPacket* inPacket);
    
    void GongGao();
    void AddCharAttr();
    void sendJueWeiInfo();
    
private:
    int32_t m_nJueWei;
};

#endif