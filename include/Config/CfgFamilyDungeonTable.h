#ifndef _CFGCFGFAMILYDUNGEONTABLE_H_
#define _CFGCFGFAMILYDUNGEONTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgFamilyDungeonTable - Config data class
 */
class CfgFamilyDungeonTable
{
public:
    CfgFamilyDungeonTable();
    ~CfgFamilyDungeonTable();

    void AddDungeon(const CfgFamilyDungeon& stu);

public:
    std::map<int8_t, CfgFamilyDungeon> m_mDungeon;
};

#endif // _CFGCFGFAMILYDUNGEONTABLE_H_
