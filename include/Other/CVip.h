#ifndef _CVIP_H_
#define _CVIP_H_

#include "Character/CExtSystemBase.h"

// 公会掉落数据结构
struct VipLuckyDrop
{
    int32_t nType;
    int32_t nTimes;
    
    VipLuckyDrop() : nType(0), nTimes(0) {}
};

class CVip : public CExtSystemBase
{
public:
    CVip();
    virtual ~CVip();

    void OnCleanUp() override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    bool AddVipLuckyDrop(int32_t Type, int32_t Times);
    int32_t OnBuyVipCard(Answer::NetPacket* inPacket);
    int32_t OnEnterClub(Answer::NetPacket* inPacket);
    int32_t OnBuyDropTimes(Answer::NetPacket* inPacket);
    int32_t OnBuyAllVipCard(Answer::NetPacket* inPacket);
    bool GetVipFlg(int8_t VipType);
    static bool GetVipFlg(CVip* self, int8_t VipType) { return self ? self->GetVipFlg(VipType) : false; }
    static int32_t GetStorePage(CVip* self) { return self ? self->GetStorePage() : 0; }
    int32_t GetVipEquipBackRate();
    int32_t OnGetVipCardGift(Answer::NetPacket* inPacket);
    void BuyGongGao(int32_t GongGaoId, int8_t VipType, int8_t VipLevel);
    int32_t OnGetVipGift(Answer::NetPacket* inPacket);
    void AddVipExp(int32_t VipExp);
    bool CanSiteRevive();
    void GetClubDrop(int32_t Mid);
    void GetLuckDropType(int32_t Mid);
    void AddDropTimes(int32_t Type, int32_t VipLevel);
    int32_t GetDropVipLevel(int32_t Type);
    int32_t GetTreasureTimes();
    int32_t GetCycleTowerTimes();
    void SendLuckDrop(int32_t nType, MemChrBag stu, int32_t Mid);
    int8_t GetVipLevel();
    int32_t GetVipExp();
    void AddVipTime(int8_t VipType, int32_t Seconds);
    int8_t GetVipType();
    void AddVipAttr();
    void SendVipInfo();
    void OnLoadVipEndTime(const std::string* VipEndTime);
    std::string OnSaveVipEndTime();
    void CheckVipEnd(int64_t Tick);
    void LoadDropString(std::string* p_String);
    std::string SaveDropString();
    bool HaveVipPrivilege();
    bool CanAuction();
    int32_t GetStorePage();
    void AddClubDropTime();
    void SendClubInfo();

    int32_t m_PlatinumVipEndTime;
    int32_t m_DiamVipEndTime;
    int32_t m_StarVipEndTime;
    int32_t m_Club;
    int32_t m_ClubDropTimes;
    int32_t m_BuyTimes;
    std::map<int, VipLuckyDrop> m_VipLuckyDropMap;
};

#endif // _CVIP_H_

