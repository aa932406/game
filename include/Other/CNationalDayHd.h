#ifndef _CNATIONALDAYHD_H_
#define _CNATIONALDAYHD_H_

#include "Character/CExtSystemBase.h"
#include <map>
#include <string>

class Param2;

class CNationalDayHd : public CExtSystemBase
{
public:
    CNationalDayHd();
    virtual ~CNationalDayHd();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void paraseNational(std::string* p_NationalInfo);
    std::string SaveNationalInfo();
    bool GetNationalExp(int32_t nType, int32_t nId);
    bool UnLockHighReward();
    bool GetSeniorReward(int32_t nLevel);
    bool GetOrdinaryReward(int32_t nLevel);
    bool BuyLevel(int32_t BuyLv);
    void AddExp(int32_t AddExp);
    void CheckLevelUp();
    void SendLevelInfo();
    void PackHdInfo(Answer::NetPacket* packet);
    int32_t CanGetCount();

private:
    int32_t m_Level;
    int32_t m_Exp;
    int32_t m_UnLockReward;
    int32_t m_OrdinaryReward;
    int32_t m_SeniorReward;
    std::map<int, Param2> m_NationalInfo;
};

#endif // _CNATIONALDAYHD_H_
