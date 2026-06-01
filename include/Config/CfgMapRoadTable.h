#ifndef _CFGCFGMAPROADTABLE_H_
#define _CFGCFGMAPROADTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgMapRoad.h"


/**
 * @brief CfgMapRoadTable - Config data class
 */
class CfgMapRoadTable
{
public:
    CfgMapRoadTable();
    ~CfgMapRoadTable();

    void Add(const CfgMapRoad& stu);

public:
    std::map<int, CfgMapRoad> m_mMapRoad;
};

#endif // _CFGCFGMAPROADTABLE_H_
