#ifndef _DROPRECORD_H_
#define _DROPRECORD_H_

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

class DropRecord
{
public:
    DropRecord();
    DropRecord(const DropRecord& a2);

    std::string strName;
    int32_t nCharId;
    int32_t nMapId;
    int32_t nMid;
    int32_t nRecord;
    int32_t nTime;
    int32_t nSpecial;
};

#endif // _DROPRECORD_H_
