#include "Game/CCharWuHun.h"
#include "Game/Player.h"
#include "Game/GameService.h"
#include "Config/CfgData.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtCharSkill.h"
#include "Other/ChrTask.h"
#include "Answer/Singleton.h"
#include "Common/Random.h"

#include <cstring>
#include <algorithm>

CCharWuHun::CCharWuHun()
    : m_pPlayer(nullptr)
{
    memset(m_WuHun, 0, sizeof(m_WuHun));
}

CCharWuHun::~CCharWuHun()
{
}

void CCharWuHun::Init(Player* pPlayer)
{
    m_pPlayer = pPlayer;
}

void CCharWuHun::OnCleanUp()
{
    m_pPlayer = nullptr;
    memset(m_WuHun, 0, sizeof(m_WuHun));
}

void CCharWuHun::OnUpdate(int64_t curTick)
{
}

void CCharWuHun::OnDaySwitch(int32_t nDiffDays)
{
}

void CCharWuHun::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (!dbData) return;

    memcpy(m_WuHun, dbData->m_WuHunShopDBData.m_WuHun, sizeof(m_WuHun));

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            int32_t itemId = m_WuHun[i][j];
            if (itemId > 0)
            {
                CfgData* cfg = Answer::Singleton<CfgData>::instance();
                WuHunItem* pCfg = cfg->GetWuHunItem(itemId);
                if (pCfg && pCfg->nTalentId > 0)
                {
                    OnAddTalent(pCfg->nTalentId, pCfg->nLevel);
                }
            }
        }
    }
}

void CCharWuHun::OnSaveToDB(PlayerDBData* dbData)
{
    if (!dbData) return;
    memcpy(dbData->m_WuHunShopDBData.m_WuHun, m_WuHun, sizeof(m_WuHun));
}

void CCharWuHun::GetInterestsProtocol(ProcIdList* procList)
{
    if (!procList) return;
    procList->push_back(653);
    procList->push_back(654);
    procList->push_back(655);
    procList->push_back(656);
}

int32_t CCharWuHun::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket) return 10002;

    if (nProcId == 654) return onDressWuHun(inPacket);
    if (nProcId > 0x28E)
    {
        if (nProcId == 655) return onUnDressWuHun(inPacket);
        if (nProcId == 656) return onCreateWuHun(inPacket);
    }
    else if (nProcId == 653)
    {
        return onAskWuHunInfo(inPacket);
    }
    return 0;
}

void CCharWuHun::AddCharAttr()
{
    if (!m_pPlayer) return;

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            int32_t itemId = m_WuHun[i][j];
            if (itemId > 0)
            {
                CfgData* cfg = Answer::Singleton<CfgData>::instance();
                WuHunItem* pCfg = cfg->GetWuHunItem(itemId);
                if (pCfg)
                {
                    for (const auto& attr : pCfg->lAttrList)
                    {
                        m_pPlayer->AddAttrValue( (CObjAttrs::Index_T)attr.m_nAddAttrType, attr.m_nAddAttrValue);
                    }
                }
            }
        }
    }
}

int32_t CCharWuHun::onAskWuHunInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    Answer::NetPacket::readInt32(inPacket);
    return 0;
}

int32_t CCharWuHun::onDressWuHun(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;

    int32_t Level = Answer::NetPacket::readInt32(inPacket);
    int32_t BagSlot = Answer::NetPacket::readInt32(inPacket);
    int32_t WuHunSlot = Answer::NetPacket::readInt32(inPacket);

    if (Level <= 0 || Level > 5) return 10002;
    if ((uint32_t)WuHunSlot >= 16) return 10002;

    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    MemChrBag bagSlot = *CExtCharBag::GetSlotData(Bag, BagSlot);

    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    WuHunItem* pCfg = cfg->GetWuHunItem(bagSlot.itemId);
    if (!pCfg || bagSlot.itemClass != 8) return 10002;

    int32_t nDressLevel = pCfg->nLevel;
    if (nDressLevel > Player::getLevel(m_pPlayer)) return 10002;
    if (pCfg->nNeedQuality > 0 && GetSuitLevel(Level) < pCfg->nNeedQuality) return 10002;
    if (!checkWuHunPlace(pCfg->nType, WuHunSlot)) return 10002;

    int32_t OldId = m_WuHun[Level - 1][WuHunSlot];
    m_WuHun[Level - 1][WuHunSlot] = bagSlot.itemId;
    bagSlot.itemCount--;
    CExtCharBag::SetSlotData(Bag, BagSlot, &bagSlot, static_cast<int32_t>(ITEM_CHANGE_REASON::ICR_WU_HUN_DRESS), 0);

    if (OldId > 0)
    {
        WuHunItem* pOld = cfg->GetWuHunItem(OldId);
        if (pOld)
        {
            MemChrBag stu;
            memset(&stu, 0, sizeof(stu));
            stu.itemId = OldId;
            stu.itemClass = 8;
            stu.itemCount = 1;

            CExtCharBag* v8 = Player::GetBag(m_pPlayer);
            if (!CExtCharBag::AddItem(v8, &stu, static_cast<int32_t>(ITEM_CHANGE_REASON::ICR_WU_HUN_DRESS)))
                return 10002;

            if (pOld->nTalentId > 0)
                OnRemoveTalent(pOld->nTalentId, pOld->nLevel);
        }
    }

    if (pCfg->nTalentId > 0)
        OnAddTalent(pCfg->nTalentId, pCfg->nLevel);

    sendWuHunSlotInfo(Level, WuHunSlot);
    Player::RecalcAttr(m_pPlayer);
    return 0;
}

int32_t CCharWuHun::onUnDressWuHun(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;

    int32_t Level = Answer::NetPacket::readInt32(inPacket);
    int32_t WuHunSlot = Answer::NetPacket::readInt32(inPacket);

    if (Level <= 0 || Level > 5) return 10002;
    if ((uint32_t)WuHunSlot >= 16) return 10002;
    if (m_WuHun[Level - 1][WuHunSlot] <= 0) return 10002;

    int32_t itemId = m_WuHun[Level - 1][WuHunSlot];
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    WuHunItem* pOld = cfg->GetWuHunItem(itemId);
    if (!pOld) return 10002;

    MemChrBag stu;
    memset(&stu, 0, sizeof(stu));
    stu.itemId = pOld->nId;
    stu.itemClass = 8;
    stu.bind = 1;
    stu.itemCount = 1;

    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &stu, static_cast<int32_t>(ITEM_CHANGE_REASON::ICR_WU_HUN_UNDRESS)))
        return 10002;

    if (pOld->nTalentId > 0)
        OnRemoveTalent(pOld->nTalentId, pOld->nLevel);

    m_WuHun[Level - 1][WuHunSlot] = 0;
    sendWuHunSlotInfo(Level, WuHunSlot);
    Player::RecalcAttr(m_pPlayer);
    return 0;
}

void CCharWuHun::OnRemoveTalent(int32_t TalentId, int32_t TalentLevel)
{
    if (m_pPlayer)
    {
        CExtCharSkill* CharSkill = m_pPlayer->GetCharSkill();
        if (CharSkill)
            CharSkill->removeSkillByTalent(TalentId, TalentLevel);
    }
}

void CCharWuHun::OnAddTalent(int32_t TalentId, int32_t TalentLevel)
{
    if (m_pPlayer)
    {
        CExtCharSkill* CharSkill = m_pPlayer->GetCharSkill();
        if (CharSkill)
            CharSkill->AddOtherSkill(TalentId, TalentLevel);
    }
}

int32_t CCharWuHun::onCreateWuHun(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;

    int32_t nId = Answer::NetPacket::readInt32(inPacket);
    int8_t CostSpecial = Answer::NetPacket::readInt8(inPacket);

    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    CreateWuHun* pCfg = cfg->GetCreateWuHun(nId);
    if (!pCfg) return 10002;

    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (CExtCharBag::GetFreeSlotCount(Bag) <= 0) return 10002;

    int32_t GongGaoId = 0;
    MemChrBag stu;
    memset(&stu, 0, sizeof(stu));

    // Determine which item rate table and cost items to use
    std::list<RateItem> Items = pCfg->GetItemRate;
    std::list<ItemData> CostItem = pCfg->ConstItem;

    if (CostSpecial == 1)
    {
        if (pCfg->SpecialCost.m_nId == 0)
            return 10002;
        Items = pCfg->GetItemRate2;
        CostItem.push_back(pCfg->SpecialCost);
    }

    // Random selection
    int32_t nRand = Answer::Singleton<Answer::Random>::instance()->generate(1, 10000);
    for (const auto& it : Items)
    {
        if (it.nRate >= nRand)
        {
            stu.itemId = it.nItemId;
            stu.itemClass = it.nItemClass;
            stu.itemCount = it.nItemCount;
            stu.bind = it.nBind;
            if (it.nGongGaoId > 0)
                GongGaoId = it.nGongGaoId;
            break;
        }
        nRand -= it.nRate;
    }

    if (stu.itemId > 0 && stu.itemCount > 0)
    {
        if (Bag->RemoveItem(&CostItem, ITEM_CHANGE_REASON::ICR_WU_HUN_CREATE))
        {
            CExtCharBag::AddItem(Bag, &stu, static_cast<int32_t>(ITEM_CHANGE_REASON::ICR_WU_HUN_CREATE));

            WuHunItem* pCfgItem = cfg->GetWuHunItem(stu.itemId);
            if (pCfgItem && GongGaoId > 0)
            {
                GameService* gs = Answer::Singleton<GameService>::instance();
                Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6);
                if (packet)
                {
                    Answer::NetPacket::writeInt32(packet, GongGaoId);
                    std::string val;
                    Player::getName(m_pPlayer, &val);
                    Answer::NetPacket::writeUTF8(packet, &val);
                    Answer::NetPacket::writeInt64(packet, Player::getCid(m_pPlayer));
                    Answer::NetPacket::writeInt32(packet, stu.itemId);
                    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                    Answer::NetPacket::setSize(packet, WOffset);
                    GameService::worldBroadcast(gs, 0, packet);
                }
            }

            if (pCfgItem)
            {
                int32_t nType = pCfgItem->nType;
                ChrTask* Task = Player::GetTask(m_pPlayer);
                if (Task)
                    Task->updateTaskCount(43, nType);
            }

            uint16_t Proc = Answer::NetPacket::getProc(inPacket);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            int8_t ConnId = Player::getConnId(m_pPlayer);
            GameService::replySuccess(
                Answer::Singleton<GameService>::instance(),
                ConnId, GateIndex, Proc, stu.itemId);
            return 0;
        }
    }
    return 10002;
}

void CCharWuHun::sendWuHunInfo()
{
    if (!m_pPlayer) return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2EA1);
    if (!packet) return;

    for (int i = 0; i < 5; ++i)
    {
        Answer::NetPacket::writeInt32(packet, i + 1);
        int32_t Count = 0;
        int32_t oldoffest = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::writeInt32(packet, 0);

        for (int j = 0; j < 16; ++j)
        {
            if (m_WuHun[i][j] > 0)
            {
                Answer::NetPacket::writeInt32(packet, j);
                Answer::NetPacket::writeInt32(packet, m_WuHun[i][j]);
                ++Count;
            }
        }

        int32_t newwoffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setWOffset(packet, oldoffest);
        Answer::NetPacket::writeInt8(packet, Count);
        Answer::NetPacket::setWOffset(packet, newwoffset);
    }

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v5 = Player::getConnId(m_pPlayer);
    GameService::sendPacketTo(Answer::Singleton<GameService>::instance(), v5, GateIndex, packet);
}

void CCharWuHun::sendWuHunSlotInfo(int32_t Level, int32_t Slot)
{
    if (!m_pPlayer || Level <= 0 || Level > 5 || (uint32_t)Slot >= 16) return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2EA2);
    if (!packet) return;

    Answer::NetPacket::writeInt32(packet, Level);
    Answer::NetPacket::writeInt32(packet, Slot);
    Answer::NetPacket::writeInt32(packet, m_WuHun[Level - 1][Slot]);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v7 = Player::getConnId(m_pPlayer);
    GameService::sendPacketTo(Answer::Singleton<GameService>::instance(), v7, GateIndex, packet);
}

bool CCharWuHun::checkWuHunPlace(int32_t Type, int32_t Slot)
{
    if (Type == 2)
    {
        if (Slot > 4 && Slot <= 9)
            return true;
    }
    else if (Type > 2)
    {
        if (Type == 3)
        {
            if (Slot > 9 && Slot <= 14)
                return true;
        }
        else if (Type == 4 && Slot == 15)
        {
            return true;
        }
    }
    else if (Type == 1 && (uint32_t)Slot <= 4)
    {
        return true;
    }
    return false;
}

int32_t CCharWuHun::GetSuitLevel(int32_t nLevel)
{
    return 0;
}
