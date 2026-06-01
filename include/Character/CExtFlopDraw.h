// CExtFlopDraw.h
#ifndef CEXTFLOPDRAW_H
#define CEXTFLOPDRAW_H

#include "CExtSystemBase.h"
#include <map>

class Player;

class CExtFlopDraw : public CExtSystemBase
{
public:
    CExtFlopDraw();
    virtual ~CExtFlopDraw();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    bool OnFlopDraw(int32_t nType, int32_t nFlopId);
    int32_t GetFlopType(int32_t nType, int32_t nFlopId);
    bool DrawAndCost(int32_t nType, int32_t nFlopId);
    bool IsGet(int32_t nType, int32_t nId);
    bool IsFloped(int32_t nType, int32_t nFlopId);
    void addDrawTimes(int32_t nType, int32_t nFlopId, int32_t FlopResult);
    void SendFlopRecord(Player* player, int32_t nType);
    void SendFlopOneRecord(Player* player, int32_t nType, int32_t nFlopId, int32_t FlopResult);
    
private:
    std::map<int32_t, std::map<int32_t, int32_t>> m_FlopDrawRecordMap;
};

#endif