#ifndef _CREFRESHMONSTER_H_
#define _CREFRESHMONSTER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CRefreshMonster : public CActivity
{
public:
    CRefreshMonster(const CfgActivity* cfgActivity);
    ~CRefreshMonster();

    void reset();
    void CheckActivity();
    void FlushMonsters(int32_t NowTime);
    void stopActivity();
    void broadcastStart();
    void FreshObjGongGao(int32_t GongGaoId);

private:
        // TODO: 确认容器类型 m_RefreshCfgList
        int64_t m_StateTime;
        int32_t m_nState;
};

#endif // _CREFRESHMONSTER_H_

