#include "Other/MailInfo.h"

#include <cstring>
#include <algorithm>

MailInfo::MailInfo()
    : MailId(0), SysMailId(0), SenderId(0), ReceiveId(0), SendTime(0)
{
}

MailInfo::~MailInfo()
{
}

MailInfo::MailInfo(const MailInfo& other)
    : MailId(other.MailId)
    , SysMailId(other.SysMailId)
    , SenderId(other.SenderId)
    , SenderName(other.SenderName)
    , ReceiveId(other.ReceiveId)
    , ReceiveName(other.ReceiveName)
    , SendTime(other.SendTime)
{
}
