#include "Other/EnergyCfg.h"

EnergyCfg::EnergyCfg()
    : MaxEnergy(0)
    , EnergyTime(0)
    , BuyAddEnergy(0)
{
}

EnergyCfg::EnergyCfg(const EnergyCfg& a2)
    : MaxEnergy(a2.MaxEnergy)
    , EnergyTime(a2.EnergyTime)
    , EnergyPrice(a2.EnergyPrice)
    , BuyAddEnergy(a2.BuyAddEnergy)
{
}
