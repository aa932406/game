#ifndef _CPLAYERMAIL_H_
#define _CPLAYERMAIL_H_

#include "Character/CExtSystemBase.h"

class CPlayerMail : public CExtSystemBase
{
public:
    CPlayerMail();
    virtual ~CPlayerMail();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnReadMail(Answer::NetPacket* inPacket);
    int32_t OnRequestMailList(Answer::NetPacket* inPacket);
    int32_t OnGetFuJian(Answer::NetPacket* inPacket);
    int32_t OnDeleteMail(Answer::NetPacket* inPacket);
    int32_t OnReciveNewMail(Answer::NetPacket* inPacket);
    void SendMailList();
    void SendMailContent(int32_t MailId);
    void PackParam(std::string* p_Param, Answer::NetPacket* inPacket);
    void SendNewMail();
};

#endif // _CPLAYERMAIL_H_

