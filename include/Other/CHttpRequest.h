#ifndef _CHTTPREQUEST_H_
#define _CHTTPREQUEST_H_

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

class CHttpRequest
{
public:
    CHttpRequest();
    ~CHttpRequest();

    void run();
    void Init(std::string *p_ip_0, std::string *p_path);
    void Stop();
    void SendGetRequest();
    std::string UrlEncode(std::string * str);
    unsigned __int8 ToHex(unsigned __int8 x);
    bool isValidIpAddress(char *ipAddress);
    bool GetIpByDomainName(char *szHost, char *szIp);

private:
        // TODO: 确认类型 m_GetStringLock
        // TODO: 确认类型 m_ServerIp
        // TODO: 确认类型 m_ServerPath
        // TODO: 确认类型 m_bRunning
        // TODO: 确认类型 m_lGetReqString
};

#endif // _CHTTPREQUEST_H_
