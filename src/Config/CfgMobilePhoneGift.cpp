#include "Config/CfgMobilePhoneGift.h"


CfgMobilePhoneGift::CfgMobilePhoneGift(const CfgMobilePhoneGift& a2)
{
    vItem = a2.vItem;
    nIcon = a2.nIcon;
}

CfgMobilePhoneGift& CfgMobilePhoneGift::operator=(const CfgMobilePhoneGift& a2)
{
    if (this != &a2)
    {
        vItem = a2.vItem;
        nIcon = a2.nIcon;
    }
    return *this;
}
