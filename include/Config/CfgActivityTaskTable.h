#ifndef _CFGCFGACTIVITYTASKTABLE_H_
#define _CFGCFGACTIVITYTASKTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgActivityTaskTable - Config data class
 */
class CfgActivityTaskTable
{
public:
    CfgActivityTaskTable();
    ~CfgActivityTaskTable();

    void AddTask(int32_t nId, int32_t nMinLevel, int32_t nMaxLevel, int32_t nRatio);

public:
    std::map<int, CfgActivityTask> m_mTasks;
};

#endif // _CFGCFGACTIVITYTASKTABLE_H_
