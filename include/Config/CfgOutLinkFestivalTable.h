#ifndef _CFGCFGOUTLINKFESTIVALTABLE_H_
#define _CFGCFGOUTLINKFESTIVALTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgOutLinkFestival.h"


/**
 * @brief CfgOutLinkFestivalTable - Config data class
 */
class CfgOutLinkFestivalTable
{
public:
    CfgOutLinkFestivalTable();
    ~CfgOutLinkFestivalTable();

    void Add(const CfgOutLinkFestival& stu);

public:
    std::map<int, CfgOutLinkFestival> m_mOutLinkFestival;
};

#endif // _CFGCFGOUTLINKFESTIVALTABLE_H_
