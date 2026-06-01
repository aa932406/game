#ifndef _CMOLINGRUQIN_H_
#define _CMOLINGRUQIN_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CMoLingRuQin : public CActivity
{
public:
    CMoLingRuQin(const CfgActivity* cfgActivity);
    ~CMoLingRuQin();

    void CheckActivity();
    void FlushMonsters();
    void startActivity();
    void stopActivity();
    void broadcastReady();
    void broadcastStart();
    void onTimeEnd();

private:
        int64_t m_LastFlushTime;
        int64_t m_RefreshTime;
        int32_t m_nState;
        int32_t m_refreshCount;
};

#endif // _CMOLINGRUQIN_H_

