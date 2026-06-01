#include "Other/CXingMai.h"

CXingMai::CXingMai()
    : CExtSystemBase()
{
}

CXingMai::~CXingMai()
{
}

void CXingMai::OnCleanUp()
{
}

void CXingMai::OnUpdate(int64_t curTick)
{
}

void CXingMai::OnDaySwitch(int32_t nDiffDays)
{
}

void CXingMai::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CXingMai::OnSaveToDB(PlayerDBData* dbData)
{
}

void CXingMai::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(300);
    procList->push_back(301);
}

int32_t CXingMai::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 10002;
    if (nProcId == 300)
        return OnCxingMaiInfo(inPacket);
    if (nProcId == 301)
        return OnCxingMaiAct(inPacket);
    return 10002;
}

int32_t CXingMai::OnCxingMaiInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    uint16_t nTotalCount = CfgData::GetCfgXingMaiCount(pCfgData);

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, 1);
        Answer::NetPacket::writeUInt16(packet, nTotalCount);

        int32_t nRewardBit = Player::getRecord(m_pPlayer, 1162);
        int32_t nStar = Player::getRecord(m_pPlayer, 1170);

        Answer::NetPacket::writeInt32(packet, nRewardBit);
        Answer::NetPacket::writeInt32(packet, nStar);

        for (uint16_t i = 0; i < nTotalCount; ++i)
        {
            CfgXingMai* pCfg = CfgData::GetCfgXingMai(pCfgData, i + 1);
            if (!pCfg)
                continue;

            Answer::NetPacket::writeUInt16(packet, pCfg->Id);
            int32_t nStarCount = Player::getRecord(m_pPlayer, 1170 + i + 1);
            Answer::NetPacket::writeInt32(packet, nStarCount);
            Answer::NetPacket::writeUInt16(packet, pCfg->XingJi);
            Answer::NetPacket::writeUInt16(packet, pCfg->XingJiCfgId);
            Answer::NetPacket::writeInt32(packet, pCfg->CostType);
            Answer::NetPacket::writeInt32(packet, pCfg->CostCount);
            Answer::NetPacket::writeInt32(packet, pCfg->AttrId);
            Answer::NetPacket::writeInt32(packet, pCfg->AttrValue);
        }

        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        GameService::sendPacket(pGame, packet);
    }
    return 0;
}

int32_t CXingMai::OnCxingMaiAct(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    uint8_t nAct;
    Answer::NetPacket::readUInt8(inPacket, &nAct);

    if (nAct == 1)
        return XingMaiUpLevel(inPacket);
    if (nAct == 2)
        return XingMaiReward(inPacket);

    return 10002;
}

int32_t CXingMai::XingMaiUpLevel(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    uint16_t nCfgId;
    Answer::NetPacket::readUInt16(inPacket, &nCfgId);

    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    CfgXingMai* pCfg = CfgData::GetCfgXingMai(pCfgData, nCfgId);
    if (!pCfg)
        return 10002;

    int32_t nStar = Player::getRecord(m_pPlayer, 1170);
    if (nStar < pCfg->XingJi)
        return 10002;

    int32_t nIdx = 1170 + nCfgId;
    if (Player::getRecord(m_pPlayer, nIdx) >= pCfg->XingJiCfgId)
        return 10002;

    int32_t nCostId = pCfg->CostType;
    int32_t nCostCount = pCfg->CostCount;

    if (nCostId > 0 && nCostCount > 0)
    {
        CExtCharBag* pBag = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::RemoveItemByCfgId(pBag, nCostId, nCostCount, ITEM_CHANGE_REASON::ICR_XINGMAI_UPLEVEL))
            return 10002;
    }

    int32_t nCurStar = Player::getRecord(m_pPlayer, nIdx);
    Player::updateRecord(m_pPlayer, nIdx, nCurStar + 1);
    Player::RecalcAttr(m_pPlayer);

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, 2);
        Answer::NetPacket::writeUInt16(packet, nCfgId);
        Answer::NetPacket::writeInt32(packet, nCurStar + 1);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        GameService::sendPacket(pGame, packet);
    }
    return 0;
}

int32_t CXingMai::XingMaiReward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    uint16_t nCfgId;
    Answer::NetPacket::readUInt16(inPacket, &nCfgId);

    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    CfgXingMai* pCfg = CfgData::GetCfgXingMai(pCfgData, nCfgId);
    if (!pCfg)
        return 10002;

    int32_t nRewardBit = Player::getRecord(m_pPlayer, 1162);
    if (nRewardBit & (1 << (nCfgId - 1)))
        return 10002;

    int32_t nNeedStar = pCfg->XingJi;
    int32_t nStar = Player::getRecord(m_pPlayer, 1170);
    if (nStar < nNeedStar)
        return 10002;

    nRewardBit |= (1 << (nCfgId - 1));
    Player::updateRecord(m_pPlayer, 1162, nRewardBit);

    if (pCfg->RewardId > 0)
    {
        CfgData* pRewardCfg = Answer::Singleton<CfgData>::instance();
        CfgReward* pReward = CfgData::GetReward(pRewardCfg, pCfg->RewardId);
        if (pReward)
        {
            MailReward stu;
            stu.reason = MAIL_CHANGE_REASON::MCR_XINGMAI_REWARD;
            Player::MailSendReward(m_pPlayer, &pReward->MailTitle, &pReward->MailContent, &pReward->Items, &stu);
        }
    }

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, 3);
        Answer::NetPacket::writeUInt16(packet, nCfgId);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        GameService::sendPacket(pGame, packet);
    }
    return 0;

bool CXingMai::IsXingMaiSlotActived(int8_t nEquipSlot)
{
    if (!m_pPlayer)
        return false;
    int32_t XingMaiSlot = Player::getRecord(m_pPlayer, 1171);
    return (XingMaiSlot & (1 << (nEquipSlot - 1))) != 0;
}

void CXingMai::AddCharAttr()
{
    if (!m_pPlayer)
        return;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    uint16_t nCount = CfgData::GetCfgXingMaiCount(pCfgData);
    for (uint16_t i = 0; i < nCount; ++i)
    {
        CfgXingMai* pCfg = CfgData::GetCfgXingMai(pCfgData, i + 1);
        if (!pCfg)
            continue;
        int32_t nStarCount = Player::getRecord(m_pPlayer, 1170 + i + 1);
        if (nStarCount > 0 && pCfg->AttrId > 0 && pCfg->AttrValue > 0)
        {
            Player::AddCharAttrAddon(m_pPlayer, pCfg->AttrId, pCfg->AttrValue * nStarCount);
        }
    }
}

void CXingMai::activeSlot(int8_t nIndex)
{
    if (!m_pPlayer)
        return;
    CExtOperateLimit* pLimit = Player::GetOperateLimit(m_pPlayer);
    if (CExtOperateLimit::CheckLimit(pLimit, 67, 1))
    {
        int32_t XingMaiSlot = Player::getRecord(m_pPlayer, 1171);
        Player::updateRecord(m_pPlayer, 1171, XingMaiSlot | (1 << (nIndex - 1)));
    }
}

int32_t CXingMai::OnXingMaiLevelUp(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    uint16_t nCfgId = inPacket->readUInt16();
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    CfgXingMai* pCfg = CfgData::GetCfgXingMai(pCfgData, nCfgId);
    if (!pCfg)
        return 10002;
    int32_t nStar = Player::getRecord(m_pPlayer, 1170);
    if (nStar < pCfg->XingJi)
        return 10002;
    int32_t nIdx = 1170 + nCfgId;
    if (Player::getRecord(m_pPlayer, nIdx) >= pCfg->XingJiCfgId)
        return 10002;
    int32_t nCostId = pCfg->CostType;
    int32_t nCostCount = pCfg->CostCount;
    if (nCostId > 0 && nCostCount > 0)
    {
        CExtCharBag* pBag = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::RemoveItemByCfgId(pBag, nCostId, nCostCount, ITEM_CHANGE_REASON::ICR_XINGMAI_UPLEVEL))
            return 10002;
    }
    int32_t nCurStar = Player::getRecord(m_pPlayer, nIdx);
    Player::updateRecord(m_pPlayer, nIdx, nCurStar + 1);
    Player::RecalcAttr(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (packet)
    {
        packet->writeInt32(2);
        packet->writeUInt16(nCfgId);
        packet->writeInt32(nCurStar + 1);
        uint32_t WOffset = packet->getWOffset();
        packet->setSize(WOffset);
        GameService::sendPacket(pGame, packet);
    }
    return 0;
}

int32_t CXingMai::GetMaxBossScore()
{
    if (!m_pPlayer)
        return 0;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    int32_t nStar = Player::getRecord(m_pPlayer, 1170);
    return CfgData::GetMaxBossScore(pCfgData, nStar);
}

bool CXingMai::isSlotActived(int8_t nIndex)
{
    if (!m_pPlayer)
        return false;
    int32_t XingMaiSlot = Player::getRecord(m_pPlayer, 1171);
    return (XingMaiSlot & (1 << (nIndex - 1))) != 0;
}

int32_t CXingMai::OnXingMaiSlotActive(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int8_t nIndex = inPacket->readInt8();
    if (nIndex < 1 || nIndex > 8)
        return 10002;
    if (isSlotActived(nIndex))
        return 10002;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    int32_t nCost = CfgData::GetXingMaiSlotCost(pCfgData, nIndex);
    int32_t nBossScore = Player::getRecord(m_pPlayer, 1172);
    if (nBossScore < nCost)
        return 10002;
    Player::updateRecord(m_pPlayer, 1172, nBossScore - nCost);
    activeSlot(nIndex);
    Player::RecalcAttr(m_pPlayer);
    return 0;
}
