#ifndef _CFGCFGMAINTAINCOMPENSATETABLE_H_
#define _CFGCFGMAINTAINCOMPENSATETABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgMaintainCompensate.h"


/**
 * @brief CfgMaintainCompensateTable - Config data class
 */
class CfgMaintainCompensateTable
{
public:
    CfgMaintainCompensateTable();
    ~CfgMaintainCompensateTable();

    void Add(const CfgMaintainCompensate& stu);

public:
    std::map<int, CfgMaintainCompensate> m_mMaintainCompensate;
};

#endif // _CFGCFGMAINTAINCOMPENSATETABLE_H_
