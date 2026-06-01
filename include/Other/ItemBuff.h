#ifndef _ITEMBUFF_H_
#define _ITEMBUFF_H_

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

class ItemBuff
{
public:
    ~ItemBuff();

    int32_t getGroupId();
    bool init(int32_t itemid, int32_t time);
    void extend(Buff *pBuff);
    int32_t getEndTime();
    int32_t getItemId();

private:
        const CfgActivity* m_cfgBuff;  // TODO: 确认配置类型
        const CfgActivity* m_cfgItem;  // TODO: 确认配置类型
        int64_t m_endTick;
        // TODO: 确认类型 m_unit
};

#endif // _ITEMBUFF_H_
