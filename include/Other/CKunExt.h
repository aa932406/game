#ifndef _CKUNEXT_H_
#define _CKUNEXT_H_

#include "Character/CExtSystemBase.h"
#include <map>

class CKunExt : public CExtSystemBase
{
public:
    CKunExt();
    virtual ~CKunExt();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnKunLing(Answer::NetPacket* inPacket);
    int32_t OnLingZhu(Answer::NetPacket* inPacket);
    int32_t OnDanTian(Answer::NetPacket* inPacket);
    int32_t OnKunLingUpgrade(Answer::NetPacket* inPacket);
    int32_t OnLingZhuUpgrade(Answer::NetPacket* inPacket);
    int32_t OnDanTianUpgrade(Answer::NetPacket* inPacket);
    void SendKunExtInfo();
    void SendKunLingInfo();
    void SendLingZhuInfo();
    void SendDanTianInfo();

private:
    int32_t m_nKunLingLevel;
    int32_t m_nKunLingExp;
    int32_t m_nLingZhuLevel;
    int32_t m_nLingZhuExp;
    int32_t m_nDanTianLevel;
    int32_t m_nDanTianExp;
};

#endif // _CKUNEXT_H_
