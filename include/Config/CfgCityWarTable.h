#ifndef _CFGCITYWARTABLE_H_
#define _CFGCITYWARTABLE_H_

#include "Common/CommonTypes.h"
#include "Config/CfgCityWarContReward.h"
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>


/**
 * @brief CfgCityWarTable - Config data class
 */
#ifndef CFG_CITY_WAR_TABLE_DEFINED
#define CFG_CITY_WAR_TABLE_DEFINED
class CfgCityWarTable
{
public:

    void AddContReward(const CfgCityWarContReward& stu);
    const CfgCityWarContReward* GetContReward(int32_t id);

private:
    std::list<CfgCityWarContReward> m_lContReward;
};
#endif // CFG_CITY_WAR_TABLE_DEFINED

#endif // _CFGCITYWARTABLE_H_