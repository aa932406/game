#include "Config/CfgItemBase.h"

CfgItemBase::CfgItemBase(const CfgItemBase& a2)
{
    name = a2.name;
    desc = a2.desc;
    url = a2.url;
    drop_url = a2.drop_url;
}

CfgItemBase& CfgItemBase::operator=(const CfgItemBase& a2)
{
    if (this != &a2)
    {
        name = a2.name;
        desc = a2.desc;
        url = a2.url;
        drop_url = a2.drop_url;
    }
    return *this;
}
