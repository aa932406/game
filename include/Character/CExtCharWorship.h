// CExtCharWorship.h
#ifndef CEXTCHARWORSHIP_H
#define CEXTCHARWORSHIP_H

#include "CExtSystemBase.h"
#include "Common/CommonTypes.h"
#include <list>
#include <string>

class CExtCharWorship : public CExtSystemBase
{
public:
    CExtCharWorship();
    virtual ~CExtCharWorship();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onSocialWorship(Answer::NetPacket* inPacket);
    void sendSocialWorship(CharId_t nCharId);
    void SendWorshipInfo();
    
    bool isTimesLimit();
    bool isWorshiped(CharId_t nCharId);
    
private:
    void loadCharList(const std::string* str);
    std::string saveCharList();
    
    int32_t m_nWorshipTimes;
    std::list<CharId_t> m_lstChars;
};

#endif