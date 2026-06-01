#ifndef _LUCKDROPTABLE_H_
#define _LUCKDROPTABLE_H_

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
class LuckDrop;

class LuckDropTable
{
public:
    LuckDropTable();

    void AddLuckDrop(LuckDrop *p_stu);
    int32_t GetLuckRate(int32_t Type, int32_t VipLevel);

private:
    std::map<std::pair<int,int>, LuckDrop> m_LuckDropMap;
};

#endif // _LUCKDROPTABLE_H_
