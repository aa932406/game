#ifndef _CFGCFGGOLDEGGTABLE_H_
#define _CFGCFGGOLDEGGTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgGoldEgg.h"


/**
 * @brief CfgGoldEggTable - Config data class
 */
class CfgGoldEggTable
{
public:
    CfgGoldEggTable();
    ~CfgGoldEggTable();

    void Add(const CfgGoldEgg& stu);

public:
    std::map<int, CfgGoldEgg> m_mGoldEgg;
};

#endif // _CFGCFGGOLDEGGTABLE_H_
