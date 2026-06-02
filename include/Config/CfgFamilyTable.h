#ifndef _CFGCFGFAMILYTABLE_H_
#define _CFGCFGFAMILYTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgFamily.h"
#include "Other/FamilyMedal.h"
#include "Other/FamilySelfMedal.h"


/**
 * @brief CfgFamilyTable - Config data class
 */
class CfgFamilyTable
{
public:
    CfgFamilyTable();
    ~CfgFamilyTable();

    void Add(const CfgFamily& stu);
    void CleanUp();

public:
    CfgFamily m_vFamily;
    std::map<int, FamilyMedal> m_FamilyMedalMap;
    std::map<int, FamilySelfMedal> m_FamilySelfMedalMap;
    std::map<int, CfgFamilyBoss> m_mFamilyBoss;
    std::map<int, CfgFamilyBoss> m_mFamilyBossByMid;
};

#endif // _CFGCFGFAMILYTABLE_H_
