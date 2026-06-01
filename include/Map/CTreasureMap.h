#pragma once
#include <cstdint>
#include <map>

class CExtSystemBase {
public:
    CExtSystemBase() {}
    virtual ~CExtSystemBase() {}
protected:
    int (**_vptr_IExtSystem)(...);
};

class SpecialTreasureMapRandCfg;
class TreasureMapData;
namespace Answer {
    class NetPacket {};
    class InterestsProtocol {};
}

class CTreasureMap : public CExtSystemBase
{
public:
    CTreasureMap();
    virtual ~CTreasureMap();

    void OnCleanUp();
    Answer::InterestsProtocol *GetInterestsProtocol();
    void DispatchNetDatas(Answer::NetPacket *packet);
    void OnGetTreasureMapReward(Answer::NetPacket *packet);
    void OnAskTreasureMapInfo(Answer::NetPacket *packet);
    void SendTreasureInfo(Player *player);
    int32_t GetUseMaxTime(int32_t diff);
    void OnAskSpecialTreasureMapInfo(Answer::NetPacket *packet);
    void OnGetSpecialTreasureMapReward(Answer::NetPacket *packet);
    void SendSpecialTreasureInfo(Player *player);
    void SendGetItemInfo(Player *player, int32_t itemId);
    int32_t GetDidff(int32_t level);
    void SendEndInfo(Player *player);

protected:
    SpecialTreasureMapRandCfg m_Items[6][6];
    int32_t m_SpecialX;
    std::map<int, TreasureMapData> m_TreasureMapDataMap;
};
