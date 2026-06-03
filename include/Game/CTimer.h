#ifndef _CTIMER_H_
#define _CTIMER_H_

#include <cstdint>
#include <ctime>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Answer/Mutex.h"

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

namespace Answer {
    class Mutex;
}

class CTimer
{
public:
    static CTimer* GetInstance();
    static CTimer* instance() { return GetInstance(); }

    CTimer();
    ~CTimer();

    void run();
    void Start();
    void Stop();
    int64_t GetTick();
    int32_t GetNow();
    static int32_t GetNow(CTimer* timer) { return timer ? timer->GetNow() : 0; }
    tm GetLocalNow();
    void GetLocalNow(tm *retstr);
    bool BetweenDate(int32_t begin_date, int32_t end_date);
    int32_t GetWeekDay();
    void updateTime();
    bool isRightLocalNow(tm * localnow);
    void checkNewMinute(tm * localnow);
    void onNewMinute(int32_t nMinute);
    void onDaySwitch();
    void onWeekSwitch();

private:
    bool m_bRunning;
    int32_t m_nNow;
    int64_t m_nTick;
    int32_t m_nWeekDay;
    int32_t m_nextAnnoucement;
    Answer::Mutex m_timeLock;
    tm m_tmLastLocalNow;
    tm m_tmLocalNow;
};

#endif // _CTIMER_H_