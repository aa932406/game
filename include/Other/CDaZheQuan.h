#ifndef _CDAZHEQUAN_H_
#define _CDAZHEQUAN_H_

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
#include "Other/ItemEffect.h"

class CDaZheQuan : public ItemEffect
{
public:
    CDaZheQuan();
    virtual ~CDaZheQuan();

    bool parseEffect(int32_t id, const std::string& strEffect);

private:
    int32_t Index;
};

#endif // _CDAZHEQUAN_H_
