#ifndef _CFGTRIGSKILL_H_
#define _CFGTRIGSKILL_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>


/**
 * @brief CfgTrigSkill - Config data class
 */
class CfgTrigSkill
{
public:
    int32_t id;
    int32_t groupid;
    int32_t trigType;
    std::string trigParam;
    int32_t targetType;
    int32_t trigRate;
    int32_t cdTime;
    int32_t trigBuff;
    int32_t special;
    int32_t specialParam;
    int32_t IsPvp;

    CfgTrigSkill() = default;
    CfgTrigSkill(const CfgTrigSkill& a2);
    ~CfgTrigSkill() = default;
    CfgTrigSkill& operator=(const CfgTrigSkill& a2);
};

#endif // _CFGTRIGSKILL_H_
