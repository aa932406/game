#ifndef _CFGDUNGEONDROPGROUP_H_
#define _CFGDUNGEONDROPGROUP_H_

#include "Common/CommonTypes.h"
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
struct CfgDungeonDrop;


/**
 * @brief CfgDungeonDropGroup - Config data class
 */
class CfgDungeonDropGroup
{
public:
    CfgDungeonDropGroup();

    void CleanUp();
    void Add(CfgDungeonDrop * drop);
    const CfgDungeonDrop* RandDrop() const;

private:
        std::list<CfgDungeonDrop*> m_lstDropItems;
        int32_t m_nMaxWight;
};

#endif // _CFGDUNGEONDROPGROUP_H_