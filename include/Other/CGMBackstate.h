#ifndef _CGMBACKSTATE_H_
#define _CGMBACKSTATE_H_

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

class CGMBackstate
{
public:
    CGMBackstate();
    ~CGMBackstate();

    void Init(int32_t line);
    void OnUpdate();
    void checkBroadcast();
    void sendGMBroadcast(std::string * text, int8_t type);
    int32_t GetBanChatLeftTime(CharId_t cid);
    int32_t GetSealLeftTime(CharId_t cid);
    void onUpdateGMBroadcast(Answer::NetPacket *inPacket);
    void onUpdateGMBanChat(Answer::NetPacket *inPacket);
    void onUpdateGMSeal(Answer::NetPacket *inPacket);

private:
        // TODO: 确认类型 m_mBanChat
        // TODO: 确认类型 m_mBroadcasts
        // TODO: 确认类型 m_mSeal
        // TODO: 确认类型 m_mutex
        int64_t m_nLastTick;
};

#endif // _CGMBACKSTATE_H_
