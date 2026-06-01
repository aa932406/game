#ifndef _CYABIAO_H_
#define _CYABIAO_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CYaBiao : public CActivity
{
public:
    CYaBiao(const CfgActivity* cfgActivity);
    ~CYaBiao();

    void broadcastStart();
    void broadcastTimeEnd();
    void broadcastReady();

private:
};

#endif // _CYABIAO_H_

