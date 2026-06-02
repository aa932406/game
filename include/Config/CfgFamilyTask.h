#ifndef _CFGCFGFAMILYTASK_H_
#define _CFGCFGFAMILYTASK_H_

#include "Common/CommonTypes.h"
#include <cstdint>


/**
 * @brief CfgFamilyTask - Config data class
 */
class CfgFamilyTask
{
public:
    int32_t nId;
    int32_t nType;
    int32_t nCondition;
    int32_t nConditionValue;
    int32_t nRewardCount;
    int32_t nUnitReward;

    static void AddFamilyTask(void*, ...) {}

    CfgFamilyTask() = default;
    ~CfgFamilyTask() = default;
    CfgFamilyTask(const CfgFamilyTask& a2);
    CfgFamilyTask& operator=(const CfgFamilyTask& a2);
};

#endif // _CFGCFGFAMILYTASK_H_
