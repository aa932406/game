#ifndef _CFGCFGSEARCHBACKTABLE_H_
#define _CFGCFGSEARCHBACKTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include <vector>
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
    const CfgSearchBack* GetSearchBack(int32_t nId);
    const std::vector<CfgSearchBack>* GetSearchBackByType(int32_t nType);
    bool IsExist(int32_t nId);
    void Clear();

public:
    std::map<int32_t, CfgSearchBack> m_mSearchBack;
    std::map<int32_t, std::vector<CfgSearchBack>> m_mSearchBackByType;
};

#endif // _CFGCFGSEARCHBACKTABLE_H_
