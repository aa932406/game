#ifndef _CFGTOUZITABLE_H_
#define _CFGTOUZITABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Other/SevenTouZi.h"
#include "Other/MonthTouZi.h"

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


/**
 * @brief CfgTouZiTable - Config data class
 */
class CfgTouZiTable
{
public:
    CfgTouZiTable();
    ~CfgTouZiTable();

    void AddSevenTouZi( SevenTouZi *p_stu);
    void AddMonthTouZi( MonthTouZi *p_stu);
    void GetSevenTouZi( int32_t nId);
    bool IsAllGetSevenDay( int32_t SevenDayRecord);
    void GetMonthTouZi( int32_t nDay);
    bool IsAllGetMonthTouZi( int32_t MonthTouZiRecord);

private:
        // TODO: 确认类型 m_MonthTouZiMap
        // TODO: 确认类型 m_SevenTouZiMap
};

#endif // _CFGTOUZITABLE_H_