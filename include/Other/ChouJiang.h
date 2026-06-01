#ifndef _CHOUJIANG_H_
#define _CHOUJIANG_H_

#include "Character/CExtSystemBase.h"
#include <list>

class ChouJiang;

class ChouJiang : public CExtSystemBase
{
public:
    ChouJiang();
    virtual ~ChouJiang();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnLoadFromDB(const PlayerDBData* const dbData) override;
    void OnSaveToDB(PlayerDBData* const dbData) override;
    void GetInterestsProtocol(ProcIdList* const procList) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnAskChouJiang(Answer::NetPacket* inPacket);
    int8_t GetChouJiangType();
    int32_t GetChouJiangSpecialTime();
    int32_t OnChouJiang(Answer::NetPacket* inPacket);
    int32_t OnAskChouJiangRecord(Answer::NetPacket* inPacket);
    int32_t ToGlobalAskChouJiangRecord();
    int32_t OnGetItem(Answer::NetPacket* inPacket);
    int32_t OnTidy(Answer::NetPacket* inPacket);
    int32_t OnGetWeekReward(Answer::NetPacket* inPacket);
    void SendWeekRewardInfo();
    void SendChouJiangRecord();
    void AddChouJiangRecord(MemChrBag* const CJItem, int32_t Time);
    void AddServerRecord(MemChrBag* const CJItem);
    void SendNewRecord(MemChrBag* const CJItem, int32_t Time);
    void SendChouJiangItemChang(std::list<int>* const ItemList);
    void SendGetItem(std::list<int>* const ItemList);
    void SendChouJiangItem();
    bool AddItem(MemChrBagVector* p_Items);
    void GetItem(int32_t Slot);
    int32_t GetFreeCount();
    void GetChouJiangStu(IconStateList* const IconList);
    void GetShowIconStu(ShowIcon* __return_ptr retstr);
    void SendHuoDongIcon();
    void CleanBag();

private:
    int32_t m_LuckyPoint;
    int32_t m_WeekReward;
    std::list<ChouJiangRecord> m_RecordList;
    MemChrBag m_ItemList[490];
};

#endif // _CHOUJIANG_H_
