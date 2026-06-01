#ifndef _CFGCFGTALENTTABLE_H_
#define _CFGCFGTALENTTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include <utility>
#include <vector>
#include "Config/CfgTalent.h"
#include "Config/CfgTalentPage.h"


/**
 * @brief CfgTalentTable - Config data class
 */
class CfgTalentTable
{
public:
    CfgTalentTable();
    ~CfgTalentTable();

    void AddTalent(const CfgTalent& stu);
    void AddTalentPage(const CfgTalentPage& stu);

    const CfgTalent* GetTalent(int32_t id, int32_t level);
    int32_t GetTalentMaxLevel(int32_t id);
    const CfgTalentPage* GetTalentPage(int32_t job);
    std::vector<int32_t> GetAllTalentIds();
    bool IsTalentExist(int32_t id);
    void Clear();

public:
    std::map<int32_t, std::map<int32_t, CfgTalent>> m_mTalent;  // [id][level]
    std::map<int32_t, CfgTalentPage> m_mTalentPage;  // [job]
};

#endif // _CFGCFGTALENTTABLE_H_
