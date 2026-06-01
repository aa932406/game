#ifndef _CFGGOBLINTABLEDATA_H_
#define _CFGGOBLINTABLEDATA_H_

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
class cfgGoblinData;

class cfgGoblinTableData
{
public:
    cfgGoblinTableData();

    void Add(cfgGoblinData* stu);
    const cfgGoblinData* getGoblinData(int32_t index) const;

private:
    std::map<int, cfgGoblinData> m_GoblinDataMap;
};

#endif // _CFGGOBLINTABLEDATA_H_
