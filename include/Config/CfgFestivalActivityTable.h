#ifndef _CFGCFGFESTIVALACTIVITYTABLE_H_
#define _CFGCFGFESTIVALACTIVITYTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgFestivalActivity.h"


/**
 * @brief CfgFestivalActivityTable - Config data table for festival activities
 */
class CfgFestivalActivityTable
{
public:
    CfgFestivalActivityTable() = default;
    ~CfgFestivalActivityTable() = default;

    void Add(const CfgFestivalActivity& stu) { m_mFestivalActivity[stu.nIcon] = stu; }
    const std::map<int, CfgFestivalActivity>* GetAll() const { return &m_mFestivalActivity; }

public:
    std::map<int, CfgFestivalActivity> m_mFestivalActivity;
};

#endif // _CFGCFGFESTIVALACTIVITYTABLE_H_
