#ifndef _CFGBUFF_H_
#define _CFGBUFF_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgBuff - Config data class
 */
class CfgBuff
{
public:
    int32_t id;
    int32_t groupid;
    int32_t attack_type;
    int32_t level;
    int32_t beneficial;
    int32_t special;
    int32_t modify;
    int32_t hpValue;
    int32_t hpPecent;
    int32_t control;
    int32_t duration;
    int32_t interval;
    std::vector<BuffAttr> buffAttr;
    int32_t isShow;
    int32_t shieldValue;
    int32_t shieldRatio;
    int32_t effectType;
    int32_t effectTimes;
    int32_t canRemove;
    int32_t canRecover;
    int32_t angry;
    int32_t battle;

    CfgBuff() = default;
    CfgBuff(const CfgBuff& a2);
    ~CfgBuff() = default;
    CfgBuff& operator=(const CfgBuff& a2);
};

#endif // _CFGBUFF_H_
