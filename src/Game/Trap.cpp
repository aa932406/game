#include "common.h"
#include "Game/Trap.h"

#include <cstring>
#include <algorithm>
#include <string>

Trap::Trap()
{
    StaticObj::StaticObj(this, EntityType::ET_TRAP);
      this->_vptr_Entity = (int (**)(...))off_8F3A30;
      CfgTrap::CfgTrap(&this->m_cfgTrap);
      this->m_user = 0;
      this->m_state = TrapState::TS_STANDBY;
      this->m_stateTick = 0;
}

Trap::~Trap()
{
    this->_vptr_Entity = (int (**)(...))off_8F3A30;
      CfgTrap::~CfgTrap(&this->m_cfgTrap);
      StaticObj::~StaticObj(this);
}



void Trap::init(Map *pMap, Position pos, const CfgTrap *const cfgTrap)
{
    if ( pMap )
      {
        this->m_cfgTrap = cfgTrap;
        this->m_pMap = pMap;
        this->m_currentTile = pos;
        this->m_currentPos = Map::tileToPixel(pos.x, pos.y);
        this->m_user = 0;
        this->m_state = TrapState::TS_STANDBY;
        this->m_stateTick = Map::getTick(this->m_pMap);
        if ( this->m_cfgTrap.life > 0 )
          this->m_liftTime = Map::getNow(this->m_pMap) + this->m_cfgTrap.life;
      }
}

void Trap::reset()
{
    StaticObj::reset(this);
      this->m_user = 0;
      this->m_state = TrapState::TS_STANDBY;
      this->m_stateTick = 0;
      this->m_liftTime = 0;
}

void Trap::checkState()
{
    TrapState m_state; // eax

      if ( this->m_pMap )
      {
        if ( this->m_liftTime > 0 && this->m_state != TrapState::TS_DIE && Map::getNow(this->m_pMap) > this->m_liftTime )
        {
          Trap::setState(this, TrapState::TS_DIE);
        }
        else
        {
          m_state = this->m_state;
          if ( m_state == TrapState::TS_EFFECT )
          {
            if ( Map::getTick(this->m_pMap) - this->m_stateTick >= this->m_cfgTrap.delay )
              Trap::effect(this);
          }
          else if ( m_state == TrapState::TS_COOLDOWN
                 && Map::getTick(this->m_pMap) - this->m_stateTick >= this->m_cfgTrap.cd )
          {
            Trap::setState(this, TrapState::TS_STANDBY);
          }
        }
      }
}

int32_t Trap::onUse(Player *player)
{
    if ( !this->m_pMap || !player )
        return 10002;
      if ( this->m_state )
        return 10005;
      if ( this->m_cfgTrap.item_cost && !Player::hasItem(player, this->m_cfgTrap.item_cost, 1, 1) )
        return 10002;
      if ( (this->m_cfgTrap.event_type == 4 || this->m_cfgTrap.event_type == 6) && Player::IsTeamLeader(player) )
        return 10002;
      Trap::setState(this, TrapState::TS_EFFECT);
      this->m_user = Player::getCid(player);
      return 0;
}

bool Trap::appendInfo(Answer::NetPacket *packet)
{
    int8_t Type; // dl
      int64_t v4; // rdx

      if ( !packet )
        return 0;
      Type = Entity::getType(this);
      Answer::NetPacket::writeInt8(packet, Type);
      v4 = (*((int64_t (__fastcall **)(Trap *const))this->_vptr_Entity + 2))(this);
      Answer::NetPacket::writeInt64(packet, v4);
      Answer::NetPacket::writeInt32(packet, this->m_cfgTrap.id);
      Answer::NetPacket::writeInt16(packet, this->m_currentTile.x);
      Answer::NetPacket::writeInt16(packet, this->m_currentTile.y);
      return 1;
}

void Trap::broadcastState()
{
    GameService *v1; // rax
      int64_t v2; // rdx
      uint32_t WOffset; // edx
      Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

      if ( this->m_pMap )
      {
        v1 = Answer::Singleton<GameService>::instance();
        packet = GameService::GetInstance()->popNetpacket(v1, Answer::PackType::PACK_DISPATCH, 0x2741u);
        if ( packet )
        {
          v2 = (*((int64_t (__fastcall **)(Trap *const))this->_vptr_Entity + 2))(this);
          Answer::NetPacket::writeInt64(packet, v2);
          Answer::NetPacket::writeInt32(packet, this->m_state);
          WOffset = Answer::NetPacket::getWOffset(packet);
          Answer::NetPacket::setSize(packet, WOffset);
          Map::broadcast(this->m_pMap, packet);
        }
      }
}

void Trap::broadcastLeaveMap()
{
    if ( this->m_pMap )
        Map::broadcastLeaveMap(this->m_pMap, this);
}

void Trap::setState(TrapState state)
{
    if ( this->m_pMap )
      {
        this->m_state = state;
        this->m_stateTick = Map::getTick(this->m_pMap);
        if ( this->m_state == TrapState::TS_DIE )
          Trap::broadcastLeaveMap(this);
        else
          Trap::broadcastState(this);
      }
}

void Trap::effect()
{
    {
}
