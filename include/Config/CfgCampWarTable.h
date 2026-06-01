#ifndef _CFGCFGCAMPWARTABLE_H_
#define _CFGCFGCAMPWARTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include <vector>
#include "Config/CfgCampWarReward.h"


/**
 * @brief CfgCampWarTable - Config data class
 */
class CfgCampWarTable
{
public:
    CfgCampWarTable();
    ~CfgCampWarTable();

    void Add(const CfgCampWarReward& stu);

    std::map<int, CfgCampWarReward> m_mReward;
    std::vector<int> m_vParams;
};

#endif // _CFGCFGCAMPWARTABLE_H_
