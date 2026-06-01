// CExtFightChecker.h
#ifndef CEXTFIGHTCHECKER_H
#define CEXTFIGHTCHECKER_H

#include "CExtSystemBase.h"

class CExtFightChecker : public CExtSystemBase
{
public:
    CExtFightChecker();
    virtual ~CExtFightChecker();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    void SetInPvP(int64_t nCurTick);
    void CheckInFight(int64_t nCurTick);
    void OnLeaveFight();
    void OnEnterFight();
    void ChangeBuleName(bool bFlag);
    bool IsBuleName();
    void enterPvP();
    void leavePvP();
    void UpdateFightState();
    void UpdatePvPState();
    
private:
    bool m_bIsInFight;
    bool m_IsBuleName;
    bool m_bIsInPvP;
    int64_t m_nFightTick;
    int64_t m_nBuleNameTick;
    int64_t m_nPvPTick;
};

#endif