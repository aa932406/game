#ifndef _NEWSERVERFAVORABLE_H_
#define _NEWSERVERFAVORABLE_H_

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
struct CfgEquipItem;
struct CfgGongGao;

class NewServerFavorable
{
public:
    NewServerFavorable();
    ~NewServerFavorable();
    NewServerFavorable(const NewServerFavorable& other);

private:
    int32_t Index;
    int32_t NeedGold;
    std::vector<CfgEquipItem> ItemVector;
    std::list<CfgGongGao> GongGaoInfo;
    int32_t nLimitTime;
};

#endif // _NEWSERVERFAVORABLE_H_
