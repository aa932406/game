#ifndef _CFGCFGITEMCOMBITABLE_H_
#define _CFGCFGITEMCOMBITABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgItemCombi.h"


/**
 * @brief CfgItemCombiTable - Config data class
 */
class CfgItemCombiTable
{
public:
    CfgItemCombiTable();
    ~CfgItemCombiTable();

    void Add(const CfgItemCombi& stu);

public:
    std::map<int, CfgItemCombi> m_mItemCombi;
};

#endif // _CFGCFGITEMCOMBITABLE_H_
