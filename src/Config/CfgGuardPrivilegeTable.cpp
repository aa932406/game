#include "Config/CfgGuardPrivilegeTable.h"
#include "Answer/Logger.h"

CfgGuardPrivilegeTable::CfgGuardPrivilegeTable()
{
    m_mGuardPrivilege.clear();
    m_nStartTime = 0;
    m_nEndTime = 0;
}

CfgGuardPrivilegeTable::~CfgGuardPrivilegeTable()
{
    m_mGuardPrivilege.clear();
}

bool CfgGuardPrivilegeTable::Add(const CfgGuardPrivilege& stu)
{
    // CfgGuardPrivilege is defined in CfgData.h - use index as key
    int32_t nIndex = 1; // default index
    m_mGuardPrivilege[nIndex] = stu;
    return true;
}

void CfgGuardPrivilegeTable::SetTime(int32_t nStartTime, int32_t nEndTime)
{
    m_nStartTime = nStartTime;
    m_nEndTime = nEndTime;
}

void CfgGuardPrivilegeTable::CleanUp()
{
    m_mGuardPrivilege.clear();
    m_nStartTime = 0;
    m_nEndTime = 0;
}
