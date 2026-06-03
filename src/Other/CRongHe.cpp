#include "common.h"
#include "Other/CRongHe.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"

CRongHe::CRongHe()
    : CExtSystemBase()
{
    m_BagLvl = 0;
    m_SlotCount_ = 0;
}

CRongHe::~CRongHe()
{
}

void CRongHe::OnCleanUp()
{
}

void CRongHe::OnUpdate(int64_t curTick)
{
}

void CRongHe::OnDaySwitch(int32_t nDiffDays)
{
}

void CRongHe::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_BagLvl = dbData->m_RongHeDBData.m_BagLvl;
    m_SlotCount_ = dbData->m_RongHeDBData.m_SlotCount_;
}

void CRongHe::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_RongHeDBData.m_BagLvl = m_BagLvl;
    dbData->m_RongHeDBData.m_SlotCount_ = m_SlotCount_;
}

void CRongHe::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(2216);
}

int32_t CRongHe::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    if (nProcId == 2216)
        return OnC2SBuyRongHeBagLvl(inPacket);
    return 2;
}

int32_t CRongHe::OnC2SBuyRongHeBagLvl(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    CfgData* v2 = Answer::Singleton<CfgData>::instance();
    const CfgRongHeTable* pTable = CfgData::GetRongHeTable(v2);
    const CfgRongHe* pCfg = CfgRongHeTable::GetRow(pTable, m_BagLvl + 1);
    if (!pCfg)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_BagPrice, CURRENCY_CHANGE_REASON::GCR_RONGHE_BUYBAG, 0))
        return 2;
    ++m_BagLvl;
    int32_t nAddSlot = pCfg->m_MaterialSlot + m_SlotCount_;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (nAddSlot > 0)
    {
        if (pCfg->m_EquipSlot > 0)
        {
            CExtCharBag::AddSlot(pBag, CExtCharBag::BAG_TYPE::BAG_RONGHE_EQUIP, pCfg->m_EquipSlot);
        }
        if (pCfg->m_MaterialSlot > 0)
        {
            m_SlotCount_ = pCfg->m_MaterialSlot;
            CExtCharBag::AddSlot(pBag, CExtCharBag::BAG_TYPE::BAG_RONGHE_MATERIAL, pCfg->m_MaterialSlot);
        }
    }
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v7 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v7, ConnId, GateIndex, Proc, 0);
    return 0;
}

CRongHe::SendOneRongHeInfo()
{
      int8_t ConnId;
}


CRongHe::GetRongHeEquipString()
{
      __int64 v1;
}


CRongHe::ParesRongHeEquipString()
{
      std::string *v2;
}


CRongHe::OnEquipRongLian()
{
      int32_t m_nId;
}


CRongHe::SendAllRongHeInfo()
{
      int8_t ConnId;
}


CRongHe::GetAttrList()
{
      int32_t v2;
}


CRongHe::SendRongHeResult()
{
      int8_t ConnId;
}


CRongHe::OnRongLian()
{
      CfgData *v3;
}


CRongHe::OnItemRecovery()
{
      int32_t itemId;
}


CRongHe::OnOneKeyItemRecovery()
{
      int32_t v2;
}


CRongHe::OnDismantlingEquip()
{
      CExtCharBag *Bag;
}
