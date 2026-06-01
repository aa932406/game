#include "Config/CfgTalentPage.h"


CfgTalentPage::CfgTalentPage(const CfgTalentPage& a2)
{
    job = a2.job;
    talents = a2.talents;
}

CfgTalentPage& CfgTalentPage::operator=(const CfgTalentPage& a2)
{
    if (this != &a2)
    {
        job = a2.job;
        talents = a2.talents;
    }
    return *this;
}
