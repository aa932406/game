#ifndef _ENTITY_H_
#define _ENTITY_H_

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

enum EntityType : int32_t;

class Entity
{
public:
    Entity(EntityType type);
    virtual ~Entity();

    void generateEntityId();

private:
    int64_t m_entityId;
    EntityType m_type;

    static int64_t m_nextEntityId;
};

#endif // _ENTITY_H_