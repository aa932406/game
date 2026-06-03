#include "common.h"
#include "Game/Npc.h"

#include <cstring>
#include <algorithm>
#include <string>

Npc::Npc()
    : Unit(EntityType::ET_NPC)
{
    this->_vptr_Entity = (int (**)(...))off_8E76F0;
    m_cfgNpc = nullptr;
    m_endTime = 0;
    m_nOwner = 0;
    reset();
}

Npc::~Npc()
{
    this->_vptr_Entity = (int (**)(...))off_8E76F0;
    m_cfgNpc = nullptr;
    m_endTime = 0;
    m_nOwner = 0;
}

void Npc::reset()
{
    Unit::reset();
    m_nOwner = 0;
}

bool Npc::isLifeEnd() const
{
    return m_endTime > 0 && Unit::getNow() >= m_endTime;
}

void Npc::onTalkByPlayer(Player *player)
{
    if ( player )
        player->taskTalkWithNpc(m_cfgNpc->id);
}

void Npc::init(const CfgNpc *const cfgNpc, int32_t EndTime)
{
    m_cfgNpc = cfgNpc;
    m_endTime = EndTime;
}

bool Npc::appendInfo(Answer::NetPacket *packet)
{
    int8_t Type; // dl
    int64_t v4; // rdx
    int32_t PosX; // edx
    int32_t PosY; // edx
    int64_t v7; // rdx

    if ( !packet )
        return 0;
    Type = Entity::getType();
    Answer::NetPacket::writeInt8(packet, Type);
    v4 = (*((int64_t (__fastcall **)(Npc *const))this->_vptr_Entity + 2))(this);
    Answer::NetPacket::writeInt64(packet, v4);
    Answer::NetPacket::writeInt32(packet, m_cfgNpc->id);
    PosX = StaticObj::GetPosX(this);
    Answer::NetPacket::writeInt32(packet, PosX);
    PosY = StaticObj::GetPosY(this);
    Answer::NetPacket::writeInt32(packet, PosY);
    Answer::NetPacket::writeInt32(packet, 500000);
    Answer::NetPacket::writeInt32(packet, 500000);
    Answer::NetPacket::writeInt32(packet, 0);
    v7 = (*((int64_t (__fastcall **)(Npc *const))this->_vptr_Entity + 4))(this);
    Answer::NetPacket::writeInt64(packet, v7);
    return 1;
}

Position Npc::GetStallPos()
{
    Position pos;
    Position::Position(&pos, -1, -1);
    if ( m_cfgNpc->func_extra != 1047 )
        return pos;
    if ( m_cfgNpc->params.size() == 2 )
    {
        pos.x = m_cfgNpc->params[0];
        pos.y = m_cfgNpc->params[1];
    }
    return pos;
}

bool Npc::CheckcheckPlatform(const std::string *const platform)
{
    return CfgNpc::checkPlatform(&m_cfgNpc, platform);
}

CfgDungeonNpcCost Npc::GetNpcCost(int8_t choice) const
{
    return m_cfgNpc->m_vNpcCost[choice];
}
