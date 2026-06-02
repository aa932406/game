#ifndef _MAILINFO_H_
#define _MAILINFO_H_

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
struct MemChrBag;

class MailInfo
{
public:
    MailInfo();
    ~MailInfo();
    MailInfo(const MailInfo& other);

    int32_t MailId;
    int32_t SysMailId;
    int64_t SenderId;
    std::string SenderName;
    int64_t ReceiveId;
    std::string ReceiveName;
    int32_t SendTime;
    int8_t HasRead;
    int8_t Extract;
    std::string MailTitle;
    std::string MailContent;
    int32_t nReason;
    std::string Param;
    MemChrBag Item[6];
};

#endif // _MAILINFO_H_
