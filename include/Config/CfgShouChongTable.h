#ifndef _CFGCFGSHOUCHONGTABLE_H_
#define _CFGCFGSHOUCHONGTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgShouChong.h"


/**
 * @brief CfgShouChongTable - Config data class
 */
class CfgShouChongTable
{
public:
    CfgShouChongTable();
    ~CfgShouChongTable();

    void Add(const CfgShouChong& stu);

    const CfgShouChong* GetShouChong(int32_t nIndex);
    int32_t GetMaxIndex();
    bool IsExist(int32_t nIndex);
    void Clear();

public:
    std::map<int, CfgShouChong> m_mShouChong;
};

#endif // _CFGCFGSHOUCHONGTABLE_H_
