#ifndef _DROPLOG_H_
#define _DROPLOG_H_

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

class DropLog
{
public:
    ~DropLog();
    DropLog();

    std::string sKillerName;
    std::string sName;
};

#endif // _DROPLOG_H_
