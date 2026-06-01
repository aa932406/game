#ifndef _BACKLIMITINFO_H_
#define _BACKLIMITINFO_H_

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

class BackLimitInfo
{
public:
    BackLimitInfo();
    BackLimitInfo(const BackLimitInfo& a2);

    int32_t nId;
    int32_t nCount;
    std::string sLastName;
};

#endif // _BACKLIMITINFO_H_
