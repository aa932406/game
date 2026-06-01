#ifndef _CSHIZHUANG_H_
#define _CSHIZHUANG_H_

#include "Character/CExtSystemBase.h"

class CShiZhuang : public CExtSystemBase
{
public:
    CShiZhuang();
    virtual ~CShiZhuang();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* const dbData) override;
    void OnSaveToDB(PlayerDBData* const dbData) override;
    void GetInterestsProtocol(ProcIdList* const procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t onRequestInfo(Answer::NetPacket* inPacket);
    int32_t onPeiYang(Answer::NetPacket* inPacket);
    int32_t onWear(Answer::NetPacket* inPacket);
    int32_t onLevelUp(Answer::NetPacket* inPacket);
    void sendShiZhuangPeiYangResult(int8_t nType, int32_t nExp, int8_t nCrit);
    void PackShiZhuangInfo(Answer::NetPacket* packet);
    void appendShiZhuangInfo(int8_t nType, Answer::NetPacket* packet);
    void sendShiZhuangInfo(int8_t nType);
    void SendShiZhuangInfo();
    int32_t GetWearId(int8_t nType);
    void AddPlayerAttr();
    int32_t GetShiZhuangLevel(int8_t nType);
    void addShiZhuangExp(int8_t nType, int32_t nAddExp);
    bool ActiveWithItem(int32_t nId, int32_t nAddTime);
    void wear(int8_t nType, int32_t nId);
    bool IsForever(int8_t nType, int32_t nId);
    int32_t GetNewShiZhuangLevel(int32_t nId);
    void CheckSuitInfo();
    void CheckSuitEffect();
};

#endif // _CSHIZHUANG_H_

