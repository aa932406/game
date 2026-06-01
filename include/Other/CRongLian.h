#ifndef _CRONGLIAN_H_
#define _CRONGLIAN_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CRongLian : public CActivity
{
public:
    CRongLian(const CfgActivity* cfgActivity);
    ~CRongLian();

    void broadcastStart();
    void sendPlayerScore(Player *player);

private:
};

#endif // _CRONGLIAN_H_

