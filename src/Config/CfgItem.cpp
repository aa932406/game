#include "Config/CfgItem.h"

CfgItem::CfgItem(const CfgItem& a2)
{
    effect = a2.effect;
    use_method = a2.use_method;
    RongHeReceovery = a2.RongHeReceovery;
}

CfgItem& CfgItem::operator=(const CfgItem& a2)
{
    if (this != &a2)
    {
        CfgItemBase::operator=(a2);
        effect = a2.effect;
        use_method = a2.use_method;
        RongHeReceovery = a2.RongHeReceovery;
    }
    return *this;
}
