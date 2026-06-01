#ifndef _CWAN360_H_
#define _CWAN360_H_

#include "Character/CExtSystemBase.h"

class CWan360 : public CExtSystemBase
{
public:
    CWan360();
    virtual ~CWan360();

    void OnCleanUp() override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnUpdate(int64_t curTick) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnGetJiaSuQiuReward(Answer::NetPacket* inPacket);
    int32_t OnGetGameAppReward(Answer::NetPacket* inPacket);
    int32_t OnLoginFromGameApp(Answer::NetPacket* inPacket);
    int32_t OnLoginFromSouGouSkin(Answer::NetPacket* inPacket);
    void SendSouGouSkinIcon();
    void GetSouGouSkinIconState(IconStateList* IconList);
    void GetSouGouSkinIconStu(ShowIcon* __return_ptr retstr, int32_t nIcon);
    void SendGameAppIcon();
    void GetGameAppIconState(IconStateList* IconList);
    void GetGameAppIconStu(ShowIcon* __return_ptr retstr);
    void SendJiaShuQiuIcon();
    void GetJiaShuQiuIconState(IconStateList* IconList);
    void GetJiaShuQiuIconStu(ShowIcon* __return_ptr retstr);
    void SendWeiXinIcon();
    void GetWeiXinIconState(IconStateList* IconList);
    void GetWeiXinIconStu(ShowIcon* __return_ptr retstr);
    int32_t OnGetSpeed360Reward(Answer::NetPacket* inPacket);
    void SendSpeed360Icon();
    void GetSpeed360State(IconStateList* IconList);
    void GetSpeed360IconStu(ShowIcon* __return_ptr retstr);

    bool InSpeed360Time();

    int32_t IsGameAppLogin;
    int32_t IsSpeed360Start;
    int32_t m_bSouGouSkinLogin;
    int64_t LastUpdateTiem;
};

#endif // _CWAN360_H_


