#ifndef _CCARDGROUPBOXMANAGER_H_
#define _CCARDGROUPBOXMANAGER_H_

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
#include "Other/CCardGroupBox.h"

class CCardGroupBoxManager
{
public:
    int32_t GetCard(int32_t nId);
    void Add(int32_t nGroupId, std::list<CCardGroupBox>* cardList);

    std::list<CCardGroupBox> m_lCardGroupBox;
};

#endif // _CCARDGROUPBOXMANAGER_H_
