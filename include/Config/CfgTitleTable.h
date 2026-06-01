#ifndef _CFGCFGTITLETABLE_H_
#define _CFGCFGTITLETABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include <vector>
#include "Config/CfgTitle.h"


/**
 * @brief CfgTitleTable - Config data class
 */
class CfgTitleTable
{
public:
    CfgTitleTable();
    ~CfgTitleTable();

    void Add(const CfgTitle& stu);

    const CfgTitle* GetTitle(int32_t nId);
    const std::vector<int32_t>* GetTitleIdsByType(int32_t nType);
    bool IsTitleExist(int32_t nId);
    int32_t GetTitlePriority(int32_t nId);
    int32_t GetTitleType(int32_t nId);
    void Clear();

public:
    std::map<int, CfgTitle> m_mTitle;
    std::map<int32_t, std::vector<int32_t>> m_mTitleByType;
};

#endif // _CFGCFGTITLETABLE_H_
