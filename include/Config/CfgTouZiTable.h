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
    const SevenTouZi* GetSevenTouZi( int32_t nId);
    bool IsAllGetSevenDay( int32_t SevenDayRecord);
    const MonthTouZi* GetMonthTouZi( int32_t nDay);
    bool IsAllGetMonthTouZi( int32_t MonthTouZiRecord);
    int32_t GetSevenTouZiCount();
    int32_t GetMonthTouZiCount();
    void Clear();

public:
    std::map<int32_t, SevenTouZi> m_mSevenTouZi;
    std::map<int32_t, MonthTouZi> m_mMonthTouZi;
};

#endif // _CFGTOUZITABLE_H_