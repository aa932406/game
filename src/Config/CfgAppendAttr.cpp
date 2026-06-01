#include "Config/CfgAppendAttr.h"

CfgAppendAttr::CfgAppendAttr(const CfgAppendAttr& a2)
{
    Id = a2.Id;
    Job = a2.Job;
    AttrList = a2.AttrList;
}

CfgAppendAttr& CfgAppendAttr::operator=(const CfgAppendAttr& a2)
{
    if (this != &a2)
    {
        Id = a2.Id;
        Job = a2.Job;
        AttrList = a2.AttrList;
    }
    return *this;
}
