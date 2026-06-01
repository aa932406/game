#include "Other/NpcDungeon.h"

#include "Game/Dungeon.h"
#include "Game/Npc.h"
#include "Config/CfgNpc.h"

NpcDungeon::NpcDungeon()
    : m_dungeon(nullptr)
{
}

NpcDungeon::~NpcDungeon()
{
}

void NpcDungeon::init(Dungeon *dungeon, const CfgNpc *const cfgNpc)
{
    if (dungeon)
    {
        m_dungeon = dungeon;
        Npc::init(cfgNpc, 0);
    }
}

int32_t NpcDungeon::onEndNpc(Player *player, int8_t choice)
{
    if (!player)
        return 10002;
    Dungeon::OnNpcEnd(m_dungeon, this, choice);
    return 0;
}
