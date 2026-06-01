#include "Other/CLibraryTable.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

CLibraryTable::CLibraryTable()
{
}

CLibraryTable::~CLibraryTable()
{
}

void CLibraryTable::AddCfgLibraryItem(CfgLibraryItem stu)
{
    m_CfgLibraryItemMap[stu.nId] = stu;
}

void CLibraryTable::AddCfgLibraryQuality(CfgLibraryQuality stu)
{
    m_CfgLibraryQualityMap[stu.Quality] = stu;
}

void CLibraryTable::AddCfgLibraryCost(CfgLibraryCost stu)
{
    auto key = std::make_pair(stu.Times, stu.nDunGeonId);
    m_CfgLibraryCostMap[key] = stu;
}

const CfgLibraryQuality* CLibraryTable::GetCfgLibraryQuality(int32_t Quality)
{
    auto it = m_CfgLibraryQualityMap.find(Quality);
    if (it != m_CfgLibraryQualityMap.end())
    {
        return &it->second;
    }
    return nullptr;
}
