#ifndef _LOG360_H_
#define _LOG360_H_

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

class Log360
{
public:
    Log360();
    ~Log360();

    int32_t     UseSid;
    std::string passport;
    std::string LogString;
    std::string Interface;
};

#endif // _LOG360_H_
