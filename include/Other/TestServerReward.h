#ifndef _TESTSERVERREWARD_H_
#define _TESTSERVERREWARD_H_

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

class TestServerReward
{
public:
    TestServerReward(TestServerReward *const this, const TestServerReward *const a2);

    int32_t nId;
    int32_t nType;
    int32_t nParm;
    std::vector<MemChrBag> vItems;
    int32_t nGongId;
};

#endif // _TESTSERVERREWARD_H_