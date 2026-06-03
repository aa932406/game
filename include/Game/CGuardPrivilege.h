#ifndef _CGUARDPRIVILEGE_H_
#define _CGUARDPRIVILEGE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;

class CGuardPrivilege
{
public:
    CGuardPrivilege();
    ~CGuardPrivilege();

    void OnUpdate(int64_t curTick);
    void GetInterestsProtocol(ProcIdList * procList);
    int32_t onRequestInfo(Answer::NetPacket *inPacket);
    int32_t onGetReward(Answer::NetPacket *inPacket);
    int32_t onDBGetReward(Answer::NetPacket *inPacket);
    void sendInfo();
    bool isOpen();
    int8_t getTimeState();
    void GetIconState(IconStateList * iconList);
    void getIconState(ShowIcon *__return_ptr retstr);
    void sendIconState();

private:
        int64_t m_nLastTick;
        int32_t m_nState;
        Player* m_pPlayer;
};

#endif // _CGUARDPRIVILEGE_H_