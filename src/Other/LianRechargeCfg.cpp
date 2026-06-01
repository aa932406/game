#include "Other/LianRechargeCfg.h"

LianRechargeCfg::LianRechargeCfg()
    : nValues(0)
{
}

LianRechargeCfg::LianRechargeCfg(const LianRechargeCfg& a2)
    : nValues(a2.nValues)
    , vGiftVector(a2.vGiftVector)
{
}
