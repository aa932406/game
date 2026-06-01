#include "Other/User.h"
#include "Other/TencentInfo.h"

void User::init(int64_t uid, int32_t sid)
{
    m_uid = uid;
    m_sid = sid;
}

void User::reset()
{
    m_uid = 0;
    m_sid = 0;
    m_cid = 0;
    m_player = 0;
}

void User::SetTencentInfo(const TencentInfo* info)
{
    m_tencentInfo = *info;
}

void User::GetTencentInfo(TencentInfo& retstr)
{
    retstr = m_tencentInfo;
}
