#ifndef _CHATVALIDATE_H_
#define _CHATVALIDATE_H_

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

class ChatValidate
{
public:
    ChatValidate();
    ChatValidate(const ChatValidate& a2);

    int32_t SendPacket;
    int32_t Time;
    int32_t CharId;
    int32_t Channel;
    int32_t FamilyId;
    int32_t SendCharId;
    std::list<int32_t> NeedSender;
};

#endif // _CHATVALIDATE_H_
