#ifndef _CMINGGEEXT_H_
#define _CMINGGEEXT_H_

#include "Character/CExtSystemBase.h"
#include <map>

class CMingGeExt : public CExtSystemBase
{
public:
    CMingGeExt();
    virtual ~CMingGeExt();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnMingGeRefine(Answer::NetPacket* inPacket);
    int32_t OnLeiMingRefine(Answer::NetPacket* inPacket);
    int32_t OnEquipRefine(Answer::NetPacket* inPacket);
    int32_t OnEquipSuitRefine(Answer::NetPacket* inPacket);
    void SendMingGeInfo();
    void SendLeiMingInfo();
    void SendEquipRefineInfo();

private:
    int32_t m_nMingGeLevel;
    int32_t m_nMingGeExp;
    int32_t m_nLeiMingLevel;
    int32_t m_nLeiMingExp;
    int32_t m_nEquipRefineLevel;
    std::map<int, int> m_EquipSuitRefineMap;
};

#endif // _CMINGGEEXT_H_
