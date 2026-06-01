#ifndef _DIERECORD_H_
#define _DIERECORD_H_

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

class DieRecord
{
public:
    DieRecord();
    DieRecord(const DieRecord& other);

    int64_t Cid;
    int32_t KillTime;
    int32_t MapId;
    int32_t MonsterId;
    std::string Name;
};

#endif // _DIERECORD_H_