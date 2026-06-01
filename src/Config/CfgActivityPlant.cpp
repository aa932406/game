#include "Config/CfgActivityPlant.h"


CfgActivityPlant::CfgActivityPlant(const CfgActivityPlant& a2)
{
    id = a2.id;
    activity_id = a2.activity_id;
    plant_id = a2.plant_id;
    count = a2.count;
    wave = a2.wave;
    region_id = a2.region_id;
    whoplant = a2.whoplant;
    life_time = a2.life_time;
    EnterPosVector = a2.EnterPosVector;
}

CfgActivityPlant& CfgActivityPlant::operator=(const CfgActivityPlant& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        activity_id = a2.activity_id;
        plant_id = a2.plant_id;
        count = a2.count;
        wave = a2.wave;
        region_id = a2.region_id;
        whoplant = a2.whoplant;
        life_time = a2.life_time;
        EnterPosVector = a2.EnterPosVector;
    }
    return *this;
}
