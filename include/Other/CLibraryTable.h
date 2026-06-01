#ifndef _CLIBRARYTABLE_H_
#define _CLIBRARYTABLE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Npc;
class CActivity;
struct CfgLibraryItem;
struct CfgLibraryQuality;
struct CfgLibraryCost;

class CLibraryTable
{
public:
    CLibraryTable();
    ~CLibraryTable();

    void AddCfgLibraryItem(CfgLibraryItem stu);
    void AddCfgLibraryQuality(CfgLibraryQuality stu);
    void AddCfgLibraryCost(CfgLibraryCost stu);
    const CfgLibraryQuality* GetCfgLibraryQuality(int32_t Quality);

private:
    std::map<int, CfgLibraryItem> m_CfgLibraryItemMap;
    std::map<int, CfgLibraryQuality> m_CfgLibraryQualityMap;
    std::map<int, int> m_QualityMaxProbabilityMap;
    std::map<std::pair<int, int>, CfgLibraryCost> m_CfgLibraryCostMap;
};

#endif // _CLIBRARYTABLE_H_
