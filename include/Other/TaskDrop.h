#ifndef _TASKDROP_H_
#define _TASKDROP_H_

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

class Param2;

class TaskDrop
{
public:
    TaskDrop(const TaskDrop\& a2);

    int32_t nCount;
    std::list<Param2> lstJobGroup;
};

#endif // _TASKDROP_H_