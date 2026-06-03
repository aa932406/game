#ifndef _CFUNCTIONOPEN_H_
#define _CFUNCTIONOPEN_H_

#include "Character/CExtSystemBase.h"
#include <list>

class CFunctionOpen : public CExtSystemBase
{
public:
    CFunctionOpen();
    virtual ~CFunctionOpen();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnGetSysOpenGift(Answer::NetPacket* inPacket);
    void GetSysOpenIcon(IconStateList* IconList);
    void SendSysOpenIcon();
    void GetSysOpenIcon(ShowIcon* retstr);
    bool IsAllRewardGet();
    int32_t CanGetRewardCount();
    void InitFunctionOpen(int32_t TaskId, int32_t Level);
    void CheckFunctionOpne(int32_t TaskId, int32_t Level);
    void FunctionInit(int32_t FunctionId);
    bool IsOpened(int32_t FunctionId);
    static bool IsOpened(CFunctionOpen* func, int32_t FunctionId) { return func ? func->IsOpened(FunctionId) : false; }
    void InitFunctionOpenMailByLevel(int32_t nLevel);
    void InitFunctionOpenMailByKaiFuDay(int32_t KaiFuDay);
    void CheckFunctionOpenMailByLevel(int32_t nLevel);
    void CheckFunctionOpenMailByKaiFuDay(int32_t nKaiFuDay);
    void AddSendMailList(int32_t nId);
    bool IsSendedMail(int32_t Id);

private:
    std::list<int> m_OpenedList;
    std::list<int> m_SendMailList;
};

#endif // _CFUNCTIONOPEN_H_

