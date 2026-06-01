#include "Config/CfgFamilyTask.h"

CfgFamilyTask::CfgFamilyTask(const CfgFamilyTask& a2)
{
    nId = a2.nId;
    nType = a2.nType;
    nCondition = a2.nCondition;
    nConditionValue = a2.nConditionValue;
    nRewardCount = a2.nRewardCount;
    nUnitReward = a2.nUnitReward;
}

CfgFamilyTask& CfgFamilyTask::operator=(const CfgFamilyTask& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nType = a2.nType;
        nCondition = a2.nCondition;
        nConditionValue = a2.nConditionValue;
        nRewardCount = a2.nRewardCount;
        nUnitReward = a2.nUnitReward;
    }
    return *this;
}
