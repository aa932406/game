#ifndef _ACTSTRUCT_H_
#define _ACTSTRUCT_H_

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

class ActStruct
{
public:
    ActStruct();
    ~ActStruct();
    ActStruct(const ActStruct& other);

private:
    std::string name;
    std::string passport;
    int32_t nSid;
    int64_t Time;
    int32_t connid;
};

#endif // _ACTSTRUCT_H_
