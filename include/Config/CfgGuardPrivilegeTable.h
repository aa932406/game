#ifndef _CFGGUARDPRIVILEGETABLE_H_
#define _CFGGUARDPRIVILEGETABLE_H_

#include "Common/CommonTypes.h"
#include "Config/CfgGuardPrivilege.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgGuardPrivilegeTable - Guard privilege config table
 */
class CfgGuardPrivilegeTable
{
public:
    CfgGuardPrivilegeTable();
    ~CfgGuardPrivilegeTable();

    bool Add(const CfgGuardPrivilege& stu);
    void SetTime(int32_t nStartTime, int32_t nEndTime);
    void CleanUp();

    int64_t GetStartTime() const { return m_nStartTime; }
    int64_t GetEndTime() const { return m_nEndTime; }

private:
    std::map<int32_t, CfgGuardPrivilege> m_mGuardPrivilege;
    int64_t m_nStartTime;
    int64_t m_nEndTime;
};

#endif // _CFGGUARDPRIVILEGETABLE_H_
