#ifndef _CSQIDERQUEEN_H_
#define _CSQIDERQUEEN_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CSqiderQueen : public CActivity
{
public:
    CSqiderQueen(const CfgActivity* cfgActivity);
    ~CSqiderQueen();

    void StopActivityBySqiderQueenDie();
    void broadcastStart();
    void broadcastTimeEnd();
    void broadcastKillAllSqiderQueen();

private:
};

#endif // _CSQIDERQUEEN_H_

