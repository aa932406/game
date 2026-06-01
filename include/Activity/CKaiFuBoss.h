#ifndef _CKAIFUBOSS_H_
#define _CKAIFUBOSS_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CKaiFuBoss : public CActivity
{
public:
    CKaiFuBoss(const CfgActivity* cfgActivity);
    ~CKaiFuBoss();

    void reset();
    void broadcastReady();
    void broadcastStart();
    void broadcastTimeEnd();
    void sendPlayerScore(Player *player);
    void onTimeEnd();
    bool checkData();
    bool checkWeek();
    bool isSpecialDay(int32_t nStartDays);
    int32_t getNextStartTime();
    void onMonsterAdd(MonsterActivity *pMonster);

private:
        // TODO: 确认类型 m_Boss
};

#endif // _CKAIFUBOSS_H_

