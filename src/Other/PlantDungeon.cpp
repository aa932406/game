#include "Other/PlantDungeon.h"

#include "Game/Dungeon.h"
#include "Game/Plant.h"
#include "Config/CfgPlant.h"
#include "Config/CfgMapPlant.h"

PlantDungeon::PlantDungeon()
    : m_dungeon(nullptr)
{
}

PlantDungeon::~PlantDungeon()
{
}

int32_t PlantDungeon::onEndGather(Player *player)
{
    if (!player)
        return 10002;
    int32_t err = Plant::onEndGather(this, player);
    if (!err)
        Dungeon::onPlantGather(m_dungeon, this);
    return err;
}

int32_t PlantDungeon::onBeginGather(Player *player)
{
    if (!player)
        return 10002;
    int32_t err = Dungeon::onBeginGather(m_dungeon, player);
    if (!err)
        return Plant::onBeginGather(this, player);
    return err;
}
