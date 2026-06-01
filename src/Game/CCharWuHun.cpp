#include "Game/CCharWuHun.h"

#include <cstring>
#include <algorithm>
#include <string>

CCharWuHun::CCharWuHun()
{
    CExtSystemBase::CExtSystemBase(this);
      this->_vptr_IExtSystem = (int (**)(...))&off_8CD350;
}

CCharWuHun::~CCharWuHun()
{
    this->_vptr_IExtSystem = (int (**)(...))&off_8CD350;
      CExtSystemBase::~CExtSystemBase(this);
}


void CCharWuHun::OnLoadFromDB(const PlayerDBData *const dbData)
{
    int32_t v2; // ebx
      CfgData *v3; // rax
      WuHunItem *pCfg; // [rsp+18h] [rbp-18h]

      memcpy(this->m_WuHun, dbData->m_WuHunShopDBData.m_WuHun, sizeof(this->m_WuHun));
      for ( i = 0; i <= 4; ++i )
      {
        for ( j = 0; j <= 15; ++j )
        {
          v2 = this->m_WuHun[i][j];
          v3 = Answer::Singleton<CfgData>::instance();
          pCfg = CfgData::GetWuHunItem(v3, v2);
          if ( pCfg )
          {
            if ( pCfg->nTalentId > 0 )
              CCharWuHun::OnAddTalent(this, pCfg->nTalentId, pCfg->nTalentLevel);
          }
        }
      }
}

void CCharWuHun::OnSaveToDB(PlayerDBData *const dbData)
{
    memcpy(dbData->m_WuHunShopDBData.m_WuHun, this->m_WuHun, sizeof(dbData->m_WuHunShopDBData.m_WuHun));
}

void CCharWuHun::GetInterestsProtocol(ProcIdList *const procList)
{
    unsigned __int16 __x; // [rsp+18h] [rbp-8h] BYREF
      unsigned __int16 v3; // [rsp+1Ah] [rbp-6h] BYREF
      unsigned __int16 v4; // [rsp+1Ch] [rbp-4h] BYREF
      unsigned __int16 v5; // [rsp+1Eh] [rbp-2h] BYREF

      __x = 653;
      std::list<unsigned short>::push_back(procList, &__x);
      v3 = 654;
      std::list<unsigned short>::push_back(procList, &v3);
      v4 = 655;
      std::list<unsigned short>::push_back(procList, &v4);
      v5 = 656;
      std::list<unsigned short>::push_back(procList, &v5);
}

int32_t CCharWuHun::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket *inPacket)
{
    if ( !inPacket )
        return 10002;
      if ( nProcId == 654 )
        return CCharWuHun::onDressWuHun(this, inPacket);
      if ( nProcId > 0x28Eu )
      {
        if ( nProcId == 655 )
          return CCharWuHun::onUnDressWuHun(this, inPacket);
        if ( nProcId == 656 )
          return CCharWuHun::onCreateWuHun(this, inPacket);
      }
      else if ( nProcId == 653 )
      {
        return CCharWuHun::onAskWuHunInfo(this, inPacket);
      }
      return 0;
}

void CCharWuHun::AddCharAttr()
{
    int32_t v1; // ebx
      CfgData *v2; // rax
      int32_t m_nAddAttrValue; // ebx
      AddAttribute *v4; // rax
      WuHunItem *pCfg; // [rsp+38h] [rbp-18h]

      if ( this->m_pPlayer )
      {
        for ( i = 0; i <= 4; ++i )
        {
          for ( j = 0; j <= 15; ++j )
          {
            if ( this->m_WuHun[i][j] > 0 )
            {
              v1 = this->m_WuHun[i][j];
              v2 = Answer::Singleton<CfgData>::instance();
              pCfg = CfgData::GetWuHunItem(v2, v1);
              if ( pCfg )
              {
                for ( it._M_node = std::list<AddAttribute>::begin(&pCfg->lAttrList)._M_node;
                      ;
                      std::_List_iterator<AddAttribute>::operator++(&it, 0) )
                {
                  __x._M_node = std::list<AddAttribute>::end(&pCfg->lAttrList)._M_node;
                  if ( !it != __x )
                    break;
                  m_nAddAttrValue = it->m_nAddAttrValue;
                  v4 = it->;
                  Unit::AddAttrValue(this->m_pPlayer, (const CObjAttrs::Index_T)v4->m_nAddAttrType, m_nAddAttrValue);
                }
              }
            }
          }
        }
      }
}

int32_t CCharWuHun::onAskWuHunInfo(Answer::NetPacket *inPacket)
{
    if ( !inPacket || !this->m_pPlayer )
        return 10002;
      Answer::NetPacket::readInt32(inPacket);
      return 0;
}

int32_t CCharWuHun::onDressWuHun(Answer::NetPacket *inPacket)
{
    CExtCharBag *Bag; // rax
      CfgData *v4; // rax
      int32_t nDressLevel; // ebx
      CExtCharBag *v6; // rax
      CfgData *v7; // rax
      CExtCharBag *v8; // rax
      WuHunItem *pCfg; // [rsp+68h] [rbp-28h]
      WuHunItem *pOld; // [rsp+78h] [rbp-18h]

      if ( !inPacket || !this->m_pPlayer )
        return 10002;
      Level = Answer::NetPacket::readInt32(inPacket);
      BagSlot = Answer::NetPacket::readInt32(inPacket);
      WuHunSlot = Answer::NetPacket::readInt32(inPacket);
      if ( Level <= 0 || Level > 5 )
        return 10002;
      if ( (uint32_t)WuHunSlot >= 0x10 )
        return 10002;
      Bag = Player::GetBag(this->m_pPlayer);
      bagSlot = *CExtCharBag::GetSlotData(Bag, BagSlot);
      v4 = Answer::Singleton<CfgData>::instance();
      pCfg = CfgData::GetWuHunItem(v4, bagSlot.itemId);
      if ( !pCfg || bagSlot.itemClass != 8 )
        return 10002;
      nDressLevel = pCfg->nDressLevel;
      if ( nDressLevel > (*((int (__fastcall **)(Player *))this->m_pPlayer->_vptr_Entity + 9))(this->m_pPlayer) )
        return 10002;
      if ( pCfg->nNeedQuality > 0 && CCharWuHun::GetSuitLevel(this, Level) < pCfg->nNeedQuality )
        return 10002;
      if ( !CCharWuHun::checkWuHunPlace(this, pCfg->nType, WuHunSlot) )
        return 10002;
      OldId = this->m_WuHun[Level - 1][WuHunSlot];
      this->m_WuHun[Level - 1][WuHunSlot] = bagSlot.itemId;
      --bagSlot.itemCount;
      v6 = Player::GetBag(this->m_pPlayer);
      CExtCharBag::SetSlotData(v6, BagSlot, &bagSlot, ITEM_CHANGE_REASON::ICR_WU_HUN_DRESS, 0);
      if ( OldId > 0 )
      {
        v7 = Answer::Singleton<CfgData>::instance();
        pOld = CfgData::GetWuHunItem(v7, OldId);
        if ( pOld )
        {
          *(int32_t *)&stu.itemClass = 8;
          *(int64_t *)&stu.itemCount = 0x100000001LL;
          *(int64_t *)&stu.endTime = 0;
          stu.srcId = 0;
          stu.itemId = OldId;
          v8 = Player::GetBag(this->m_pPlayer);
          if ( !CExtCharBag::AddItem(v8, &stu, ITEM_CHANGE_REASON::ICR_WU_HUN_DRESS) )
            return 10002;
          if ( pOld->nTalentId > 0 )
            CCharWuHun::OnRemoveTalent(this, pOld->nTalentId, pOld->nTalentLevel);
        }
      }
      if ( pCfg->nTalentId > 0 )
        CCharWuHun::OnAddTalent(this, pCfg->nTalentId, pCfg->nTalentLevel);
      CCharWuHun::sendWuHunSlotInfo(this, Level, WuHunSlot);
      Player::RecalcAttr(this->m_pPlayer);
      return 0;
}

int32_t CCharWuHun::onUnDressWuHun(Answer::NetPacket *inPacket)
{
    int32_t v3; // ebx
      CfgData *v4; // rax
      CExtCharBag *Bag; // rax
      WuHunItem *pOld; // [rsp+38h] [rbp-18h]

      if ( !inPacket || !this->m_pPlayer )
        return 10002;
      Level = Answer::NetPacket::readInt32(inPacket);
      WuHunSlot = Answer::NetPacket::readInt32(inPacket);
      if ( Level <= 0 || Level > 5 )
        return 10002;
      if ( (uint32_t)WuHunSlot >= 0x10 )
        return 10002;
      if ( this->m_WuHun[Level - 1][WuHunSlot] <= 0 )
        return 10002;
      v3 = this->m_WuHun[Level - 1][WuHunSlot];
      v4 = Answer::Singleton<CfgData>::instance();
      pOld = CfgData::GetWuHunItem(v4, v3);
      if ( !pOld )
        return 10002;
      memset(&stu, 0, sizeof(stu));
      stu.itemId = pOld->nId;
      stu.itemClass = 8;
      stu.bind = 1;
      stu.itemCount = 1;
      Bag = Player::GetBag(this->m_pPlayer);
      if ( !CExtCharBag::AddItem(Bag, &stu, ITEM_CHANGE_REASON::ICR_WU_HUN_UNDRESS) )
        return 10002;
      if ( pOld->nTalentId > 0 )
        CCharWuHun::OnRemoveTalent(this, pOld->nTalentId, pOld->nTalentLevel);
      this->m_WuHun[Level - 1][WuHunSlot] = 0;
      CCharWuHun::sendWuHunSlotInfo(this, Level, WuHunSlot);
      Player::RecalcAttr(this->m_pPlayer);
      return 0;
}

void CCharWuHun::OnRemoveTalent(int32_t TalentId, int32_t TalentLevel)
{
    CExtCharSkill *CharSkill; // rax

      if ( this->m_pPlayer )
      {
        CharSkill = Player::GetCharSkill(this->m_pPlayer);
        CExtCharSkill::removeSkillByTalent(CharSkill, TalentId, TalentLevel);
      }
}

void CCharWuHun::OnAddTalent(int32_t TalentId, int32_t TalentLevel)
{
    CExtCharSkill *CharSkill; // rax

      if ( this->m_pPlayer )
      {
        CharSkill = Player::GetCharSkill(this->m_pPlayer);
        CExtCharSkill::AddOtherSkill(CharSkill, TalentId, TalentLevel);
      }
}

int32_t CCharWuHun::onCreateWuHun(Answer::NetPacket *inPacket)
{
    int32_t v2; // ebx
      CfgData *v3; // rax
      CExtCharBag *Bag; // rax
      Answer::Random *v5; // rax
      RateItem *v6; // rax
      RateItem *v7; // rax
      CExtCharBag *v8; // rax
      CExtCharBag *v9; // rax
      int32_t itemId; // ebx
      CfgData *v11; // rax
      GameService *v12; // rax
      CharId_t Cid; // rax
      uint32_t WOffset; // eax
      GameService *v15; // rax
      int32_t nType; // ebx
      ChrTask *Task; // rax
      int64_t v18; // r14
      uint16_t Proc; // r13
      int16_t GateIndex; // r12
      int8_t ConnId; // bl
      GameService *v22; // rax
      std::string val; // [rsp+70h] [rbp-50h] BYREF
      CreateWuHun *pCfg; // [rsp+80h] [rbp-40h]
      WuHunItem *pCfgItem; // [rsp+90h] [rbp-30h]
      Answer::NetPacket *packet; // [rsp+98h] [rbp-28h]

      if ( this->m_pPlayer && inPacket )
      {
        nId = Answer::NetPacket::readInt32(inPacket);
        CostSpecial = Answer::NetPacket::readInt8(inPacket);
        v3 = Answer::Singleton<CfgData>::instance();
        pCfg = CfgData::GetCreateWuHun(v3, nId);
        if ( !pCfg )
          return 10002;
        Bag = Player::GetBag(this->m_pPlayer);
        if ( CExtCharBag::GetFreeSlotCount(Bag) <= 0 )
          return 10002;
        GongGaoId = 0;
        memset(&stu, 0, sizeof(stu));
        std::list<RateItem>::list(&Items, &pCfg->GetItemRate);
        std::list<ItemData>::list(&CostItem, &pCfg->ConstItem);
        if ( CostSpecial == 1 )
        {
          if ( ItemData::IsEmpty(&pCfg->SpecialCost) )
          {
            v2 = 10002;
            goto LABEL_30;
          }
          Items = &pCfg->GetItemRate2;
          std::list<ItemData>::push_back(&CostItem, &pCfg->SpecialCost);
        }
        v5 = Answer::Singleton<Answer::Random>::instance();
        nRand = Answer::Random::generate(v5, 1, 10000);
        for ( it._M_node = Items.begin()._M_node; ; std::_List_iterator<RateItem>::operator++(&it, 0) )
        {
          __x._M_node = Items.end()._M_node;
          if ( !it != __x )
            break;
          v6 = it->;
          if ( v6->nRate >= nRand )
          {
            stu.itemId = it->nItemId;
            stu.itemClass = it->nItemClass;
            stu.itemCount = it->nItemCount;
            stu.bind = it->nBind;
            if ( it->nGongGaoId > 0 )
              GongGaoId = it->nGongGaoId;
            break;
          }
          v7 = it->;
          nRand -= v7->nRate;
        }
        if ( stu.itemId > 0 && stu.itemCount > 0 )
        {
          v8 = Player::GetBag(this->m_pPlayer);
          if ( CExtCharBag::RemoveItem(v8, &CostItem, ITEM_CHANGE_REASON::ICR_WU_HUN_CREATE) )
          {
            v9 = Player::GetBag(this->m_pPlayer);
            CExtCharBag::AddItem(v9, &stu, ITEM_CHANGE_REASON::ICR_WU_HUN_CREATE);
            itemId = stu.itemId;
            v11 = Answer::Singleton<CfgData>::instance();
            pCfgItem = CfgData::GetWuHunItem(v11, itemId);
            if ( pCfgItem )
            {
              if ( GongGaoId > 0 )
              {
                v12 = Answer::Singleton<GameService>::instance();
                packet = GameService::popNetpacket(v12, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
                if ( !packet )
                {
                  v2 = 10002;
                  goto LABEL_30;
                }
                Answer::NetPacket::writeInt32(packet, GongGaoId);
                Player::getName((const Player *const)&val);
                Answer::NetPacket::writeUTF8(packet, &val);
                std::string::~string(&val);
                Cid = Player::getCid(this->m_pPlayer);
                Answer::NetPacket::writeInt64(packet, Cid);
                Answer::NetPacket::writeInt32(packet, stu.itemId);
                WOffset = Answer::NetPacket::getWOffset(packet);
                Answer::NetPacket::setSize(packet, WOffset);
                v15 = Answer::Singleton<GameService>::instance();
                GameService::worldBroadcast(v15, 0, packet);
              }
              nType = pCfgItem->nType;
              Task = Player::GetTask(this->m_pPlayer);
              ChrTask::updateTaskCount(Task, 43, nType);
            }
            v18 = stu.itemId;
            Proc = Answer::NetPacket::getProc(inPacket);
            GateIndex = Player::getGateIndex(this->m_pPlayer);
            ConnId = Player::getConnId(this->m_pPlayer);
            v22 = Answer::Singleton<GameService>::instance();
            GameService::replySuccess(v22, ConnId, GateIndex, Proc, v18);
            v2 = 0;
            goto LABEL_30;
          }
          v2 = 10002;
        }
        else
        {
          v2 = 10002;
        }
    LABEL_30:
        // CostItem destroyed on scope exit
        // Items destroyed on scope exit
        return v2;
      }
      return 10002;
}

void CCharWuHun::sendWuHunInfo()
{
    int8_t ConnId; // bl
      GameService *v2; // rax
      uint32_t WOffset; // edx
      int16_t GateIndex; // r12
      int8_t v5; // bl
      GameService *v6; // rax
      Answer::NetPacket *packet; // [rsp+10h] [rbp-30h]

      if ( this->m_pPlayer )
      {
        ConnId = Player::getConnId(this->m_pPlayer);
        v2 = Answer::Singleton<GameService>::instance();
        packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2EA1u);
        if ( packet )
        {
          for ( i = 0; i <= 4; ++i )
          {
            Answer::NetPacket::writeInt32(packet, i + 1);
            Count = 0;
            oldoffest = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::writeInt32(packet, 0);
            for ( j = 0; j <= 15; ++j )
            {
              if ( this->m_WuHun[i][j] > 0 )
              {
                Answer::NetPacket::writeInt32(packet, j);
                Answer::NetPacket::writeInt32(packet, this->m_WuHun[i][j]);
                ++Count;
              }
            }
            newwoffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setWOffset(packet, oldoffest);
            Answer::NetPacket::writeInt8(packet, Count);
            Answer::NetPacket::setWOffset(packet, newwoffset);
          }
          WOffset = Answer::NetPacket::getWOffset(packet);
          Answer::NetPacket::setSize(packet, WOffset);
          GateIndex = Player::getGateIndex(this->m_pPlayer);
          v5 = Player::getConnId(this->m_pPlayer);
          v6 = Answer::Singleton<GameService>::instance();
          GameService::sendPacketTo(v6, v5, GateIndex, packet);
        }
      }
}

void CCharWuHun::sendWuHunSlotInfo(int32_t Level, int32_t Slot)
{
    int8_t ConnId; // bl
      GameService *v4; // rax
      uint32_t WOffset; // edx
      int16_t GateIndex; // r12
      int8_t v7; // bl
      GameService *v8; // rax
      Answer::NetPacket *packet; // [rsp+18h] [rbp-18h]

      if ( this->m_pPlayer && Level > 0 && Level <= 5 && (uint32_t)Slot < 0x10 )
      {
        ConnId = Player::getConnId(this->m_pPlayer);
        v4 = Answer::Singleton<GameService>::instance();
        packet = GameService::popNetpacket(v4, ConnId, Answer::PackType::PACK_DISPATCH, 0x2EA2u);
        if ( packet )
        {
          Answer::NetPacket::writeInt32(packet, Level);
          Answer::NetPacket::writeInt32(packet, Slot);
          Answer::NetPacket::writeInt32(packet, this->m_WuHun[Level - 1][Slot]);
          WOffset = Answer::NetPacket::getWOffset(packet);
          Answer::NetPacket::setSize(packet, WOffset);
          GateIndex = Player::getGateIndex(this->m_pPlayer);
          v7 = Player::getConnId(this->m_pPlayer);
          v8 = Answer::Singleton<GameService>::instance();
          GameService::sendPacketTo(v8, v7, GateIndex, packet);
        }
      }
}

bool CCharWuHun::checkWuHunPlace(int32_t Type, int32_t Slot)
{
    if ( Type == 2 )
      {
        if ( Slot > 4 && Slot <= 9 )
          return 1;
      }
      else if ( Type > 2 )
      {
        if ( Type == 3 )
        {
          if ( Slot > 9 && Slot <= 14 )
            return 1;
        }
        else if ( Type == 4 && Slot == 15 )
        {
          return 1;
        }
      }
      else if ( Type == 1 && (uint32_t)Slot <= 4 )
      {
        return 1;
      }
      return 0;
}

int32_t CCharWuHun::GetSuitLevel(int32_t nLevel)
{
    {
}
