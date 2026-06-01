#ifndef _CXINMO_H_
#define _CXINMO_H_

#include "Character/CExtSystemBase.h"
#include <map>

class CXinMo : public CExtSystemBase
{
public:
    CXinMo();
    virtual ~CXinMo();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void AddCharAttr();
    int32_t OnXinMoLevel(Answer::NetPacket* inPacket);
    int32_t OnQiQingLevelUp(Answer::NetPacket* inPacket);
    int32_t OnXinQing(Answer::NetPacket* inPacket);
    void SetYaoYiYaoFlag(int32_t nCount);
    int32_t GetFlagCount();
    void SetFlag(int32_t nId);
    void GetNoFlagIdList(Int32Vector* __return_ptr retstr);
    void AddExp(int32_t nValue);
    int32_t OnActiveXinMo(Answer::NetPacket* inPacket);
    int32_t OnJinHua(Answer::NetPacket* inPacket);
    void SendXiMoInfo();
    void SendXinQingInfo();
    void SendXinMoQiQingLevel(int32_t nType);
    int32_t OnGetItem(Answer::NetPacket* inPacket);
    int32_t OnDec(Answer::NetPacket* inPacket);
    int32_t OnTidy(Answer::NetPacket* inPacket);
    bool AddItem(MemChrBagVector* p_Items);
    void GetItem(int32_t Slot);
    void SendAllItem();
    void SendChangeItem(std::list<int>* ItemList);
    void CleanXinMoBag();
    void DieDropFromXinMoBag(MemChrBagVector* TmpVt);
    int32_t GetFreeSlotCount();
    int32_t ResetBagSlot(int32_t nSlot, int32_t Opway);
    void AddXinMoBagLog(MemChrBag item, int32_t falg, int32_t Opway);
    int32_t GetQiQingLevel(int32_t nType);
    int32_t GetLeftYaoYiYaoTimes();
    int32_t GetChangeXinQingTimes();
    void AddYaoYiYaoTimes();
    void AddChangeXinQingTimes();

    int32_t m_Exp;
    int32_t m_XinMoLevel;
    int32_t m_XinMoActiveLevel;
    int32_t m_QiQingInfo;
    std::map<int, int> m_QiQingLevel;
    MemChrBag m_ItemList[56];
};

#endif // _CXINMO_H_


