#ifndef _CFGCFGPETTABLE_H_
#define _CFGCFGPETTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgPetData.h"


/**
 * @brief CfgPetTable - Config data class
 */
class CfgPetTable
{
public:
    CfgPetTable();
    ~CfgPetTable();

    void Add(const CfgPetData& stu);

    const CfgPetData* GetPetData(int32_t nPetId);
    bool IsPetExist(int32_t nPetId);
    int32_t GetPetCount();
    void Clear();

public:
    std::map<int, CfgPetData> m_mPetData;
};

#endif // _CFGCFGPETTABLE_H_
