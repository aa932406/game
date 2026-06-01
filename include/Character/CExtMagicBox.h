// CExtMagicBox.h
#ifndef CEXTMAGICBOX_H
#define CEXTMAGICBOX_H

#include "CExtSystemBase.h"
#include <list>

class CExtMagicBox : public CExtSystemBase
{
public:
    CExtMagicBox();
    virtual ~CExtMagicBox();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    
    int32_t onRequestInfo(Answer::NetPacket* inPacket);
    int32_t onItemDecompose(Answer::NetPacket* inPacket);
    int32_t onItemCombi(Answer::NetPacket* inPacket);
    int32_t onOrnamentCombi(Answer::NetPacket* inPacket);
    int32_t onEquipCombi(Answer::NetPacket* inPacket);
    
    int32_t randomIndex(const void* vParam);
    bool AddPoints(int32_t nValue);
    bool ActiveScroll(int32_t nId);
    void SendMagicBoxInfo();
    void SendItemCombiPoint();
    void broadcastItemCombi(int32_t ItemId, int8_t ItemClass);
    void sendItemCombiResult(int32_t nId, int32_t nSuccess, int32_t nFail);
    int32_t getMaxPoints();
    int32_t getPointsReviveValue();
    bool isActive(int32_t nId);
    
private:
    std::string saveString(const void* inList);
    void loadString(void* outList, const std::string* inStr);
    
    int32_t m_nCombiPoints;
    int32_t m_nLastReviveTime;
    std::list<int32_t> m_lActiveScrolls;
    std::list<int32_t> m_lSuccessIds;
};

#endif