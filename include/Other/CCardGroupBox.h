#ifndef _CCARDGROUPBOX_H_
#define _CCARDGROUPBOX_H_

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

class CardGroup;
class CardGroupList;

class CCardGroupBox
{
public:
    CCardGroupBox();

    void Init(int32_t nId, CardGroupList* lst);
    int32_t GetCard();
    void resetCardGroup();
    void initCardGroup(CardGroup* group);
    int32_t getFirstEmptyIndex(int32_t nMin, int32_t nMax);

    int32_t m_nId;
    int32_t m_nIndex;
    std::list<int32_t> m_lstCards;
    std::vector<CardGroup> m_vCardGroup;
};

#endif // _CCARDGROUPBOX_H_
