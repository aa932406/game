#ifndef _CFGCFGSEARCHBACKTABLE_H_
#define _CFGCFGSEARCHBACKTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgSearchBack.h"


/**
 * @brief CfgSearchBackTable - Config data class
 */
class CfgSearchBackTable
{
public:
    CfgSearchBackTable();
    ~CfgSearchBackTable();

    void Add(const CfgSearchBack& stu);

public:
    std::map<int, CfgSearchBack> m_mSearchBack;
};

#endif // _CFGCFGSEARCHBACKTABLE_H_
