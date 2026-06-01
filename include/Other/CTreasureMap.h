#ifndef _CTREASUREMAP_H_
#define _CTREASUREMAP_H_

#include "Character/CExtSystemBase.h"

class CTreasureMap : public CExtSystemBase
{
public:
    CTreasureMap();
    virtual ~CTreasureMap();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnGetTreasureMapReward(Answer::NetPacket* inPacket);
    int32_t OnAskTreasureMapInfo(Answer::NetPacket* inPacket);
    void SendTreasureInfo(int32_t Id);
    int32_t GetUseMaxTime();
    int32_t OnAskSpecialTreasureMapInfo(Answer::NetPacket* inPacket);
    int32_t OnGetSpecialTreasureMapReward(Answer::NetPacket* inPacket);
    void SendSpecialTreasureInfo(int32_t nSlot, int32_t ItemId);
    void SendGetItemInfo(int32_t X, int32_t Y);
    int32_t GetDidff(int32_t X, int32_t Y);
    void SendEndInfo();
};

#endif // _CTREASUREMAP_H_

