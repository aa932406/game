#ifndef _CFRIENDEXPREWARD_H_
#define _CFRIENDEXPREWARD_H_

#include "Character/CExtSystemBase.h"
#include "Other/DieRecord.h"
#include <string>
#include <list>

class CFriendExpReward : public CExtSystemBase
{
public:
    CFriendExpReward();
    virtual ~CFriendExpReward();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void SendDieBoard();
    void SendOneDieRecord(DieRecord* Stu);
    void SendDieRecord();
    int32_t OnZuiZhong(Answer::NetPacket* pData);
    void AddDieRecord(int64_t Cid, std::string* p_Name, int32_t MapId, int32_t Mid);

private:
    std::list<DieRecord> m_DieRecord;
};

#endif // _CFRIENDEXPREWARD_H_

