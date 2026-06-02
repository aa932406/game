#ifndef _CFGCFGFAMILYDUNGEONTABLE_H_
#define _CFGCFGFAMILYDUNGEONTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>

#ifndef CFG_FAMILY_DUNGEON_DEFINED
#define CFG_FAMILY_DUNGEON_DEFINED
struct CfgFamilyDungeon {
    int nHard;
    int nDungeonId;
    int nFamilyLevel;
    int nCostMoney;
    int nBaoKuFuBen;
    int nNpcId;
    int X;
    int Y;
    int nTime;
};
#endif /* CFG_FAMILY_DUNGEON_DEFINED */


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
