#include "Other/CTreasureMap.h"

CTreasureMap::CTreasureMap()
    : CExtSystemBase()
{
}

CTreasureMap::~CTreasureMap()
{
}

void CTreasureMap::OnCleanUp()
{
}

void CTreasureMap::OnUpdate(int64_t curTick)
{
}

void CTreasureMap::OnDaySwitch(int32_t nDiffDays)
{
}

void CTreasureMap::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CTreasureMap::OnSaveToDB(PlayerDBData* dbData)
{
}

void CTreasureMap::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(299);
}

int32_t CTreasureMap::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 10002;
    if (nProcId == 299)
        return OnTreasureMapAction(inPacket);
    return 10002;
}

int32_t CTreasureMap::OnTreasureMapAction(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    uint8_t nActionSub;
    Answer::NetPacket::readUInt8(inPacket, &nActionSub);

    if (nActionSub == 1)
        return BuryMap(inPacket);
    else if (nActionSub == 2)
        return DigMap(inPacket);
    else if (nActionSub == 3)
        return FindTreasureTarget(inPacket);
    else if (nActionSub == 4)
        return FightResult(inPacket);
    else if (nActionSub == 5)
        return OpenBox(inPacket);

    return 10002;
}

int32_t CTreasureMap::BuryMap(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    uint16_t nMap;
    Answer::NetPacket::readUInt16(inPacket, &nMap);
    int32_t x;
    Answer::NetPacket::readInt32(inPacket, &x);
    int32_t y;
    Answer::NetPacket::readInt32(inPacket, &y);
    int32_t z;
    Answer::NetPacket::readInt32(inPacket, &z);

    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    CfgTreasureMap* pCfgBase = CfgData::GetCFGTreasureMap(pCfgData, nMap);
    if (!pCfgBase)
        return 10002;

    int32_t nRecord = Player::getRecord(m_pPlayer, 1156);
    if (nRecord > 0)
        return 10002;

    int32_t nBuryMapCfg = CfgData::GetCFGTreasureMapBury(pCfgData, nMap);
    Player::updateRecord(m_pPlayer, 1156, 1);
    Player::updateRecord(m_pPlayer, 1157, nMap);
    Player::updateRecord(m_pPlayer, 1158, x);
    Player::updateRecord(m_pPlayer, 1159, y);
    Player::updateRecord(m_pPlayer, 1160, z);

    if (nBuryMapCfg > 0)
    {
        CExtCharBag* pBag = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::RemoveItemByCfgId(pBag, nBuryMapCfg, 1, ITEM_CHANGE_REASON::ICR_TREASURE_BURY))
            return 10002;
    }

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, 1);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        GameService::sendPacket(pGame, packet);
    }

    return 0;
}

int32_t CTreasureMap::DigMap(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    uint16_t nMap;
    Answer::NetPacket::readUInt16(inPacket, &nMap);
    int32_t x;
    Answer::NetPacket::readInt32(inPacket, &x);
    int32_t y;
    Answer::NetPacket::readInt32(inPacket, &y);
    int32_t z;
    Answer::NetPacket::readInt32(inPacket, &z);

    int32_t nBuryMapId = Player::getRecord(m_pPlayer, 1157);
    int32_t nBuryX = Player::getRecord(m_pPlayer, 1158);
    int32_t nBuryY = Player::getRecord(m_pPlayer, 1159);
    int32_t nBuryZ = Player::getRecord(m_pPlayer, 1160);

    int32_t nMapCfg = Player::getRecord(m_pPlayer, 1156);
    if (nMapCfg == 1 && nBuryMapId == nMap)
    {
        int32_t distance = abs(x - nBuryX) + abs(y - nBuryY) + abs(z - nBuryZ);
        if (distance <= 2)
        {
            CfgData* pCfg = Answer::Singleton<CfgData>::instance();
            CfgTreasureMap* pCfgMap = CfgData::GetCFGTreasureMap(pCfg, nMap);
            if (!pCfgMap)
                return 10002;

            Player::updateRecord(m_pPlayer, 1156, 2);
            Player::updateRecord(m_pPlayer, 1157, 0);
            Player::updateRecord(m_pPlayer, 1158, 0);
            Player::updateRecord(m_pPlayer, 1159, 0);
            Player::updateRecord(m_pPlayer, 1160, 0);

            int8_t ConnId = Player::getConnId(m_pPlayer);
            GameService* pGame = Answer::Singleton<GameService>::instance();
            Answer::NetPacket* pPacket = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
            if (pPacket)
            {
                Answer::NetPacket::writeInt32(pPacket, 2);
                Answer::NetPacket::writeUInt16(pPacket, pCfgMap->Monster);
                Answer::NetPacket::writeUInt16(pPacket, pCfgMap->OpenCost);
                uint32_t WOffset = Answer::NetPacket::getWOffset(pPacket);
                Answer::NetPacket::setSize(pPacket, WOffset);
                GameService::sendPacket(pGame, pPacket);
            }
            return 0;
        }
    }
    return 10002;
}

int32_t CTreasureMap::FindTreasureTarget(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    int32_t nMapCfg = Player::getRecord(m_pPlayer, 1156);
    if (nMapCfg != 2)
        return 10002;

    uint16_t nMap;
    Answer::NetPacket::readUInt16(inPacket, &nMap);

    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    CfgTreasureMap* pCfgMap = CfgData::GetCFGTreasureMap(pCfg, nMap);
    if (!pCfgMap)
        return 10002;

    Answer::Random* pRand = Answer::Singleton<Answer::Random>::instance();
    int32_t nRate = Answer::Random::generate(pRand, 0, pCfgMap->BossListSize - 1);
    int32_t BossId = (int32_t)(*std::next(pCfgMap->BossList.begin(), nRate));

    Player::updateRecord(m_pPlayer, 1161, BossId);
    Player::updateRecord(m_pPlayer, 1156, 3);

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, 3);
        Answer::NetPacket::writeInt32(packet, BossId);
        Answer::NetPacket::writeUInt16(packet, pCfgMap->Monster);
        Answer::NetPacket::writeUInt16(packet, pCfgMap->OpenCost);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        GameService::sendPacket(pGame, packet);
    }
    return 0;
}

int32_t CTreasureMap::FightResult(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    int32_t nMapCfg = Player::getRecord(m_pPlayer, 1156);
    if (nMapCfg != 3)
        return 10002;

    uint8_t result;
    Answer::NetPacket::readUInt8(inPacket, &result);

    int32_t BossId = Player::getRecord(m_pPlayer, 1161);

    if (result == 1)
    {
        Player::updateRecord(m_pPlayer, 1156, 4);

        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* pGame = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, 4);
            Answer::NetPacket::writeInt32(packet, BossId);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            GameService::sendPacket(pGame, packet);
        }
        return 0;
    }

    Player::updateRecord(m_pPlayer, 1156, 0);
    Player::updateRecord(m_pPlayer, 1161, 0);
    return 10002;
}

int32_t CTreasureMap::OpenBox(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    uint16_t nMap;
    Answer::NetPacket::readUInt16(inPacket, &nMap);

    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    CfgTreasureMap* pCfgMap = CfgData::GetCFGTreasureMap(pCfgData, nMap);
    if (!pCfgMap)
        return 10002;

    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (pCfgMap->OpenCost > 0)
    {
        if (!CExtCharBag::RemoveItemByCfgId(pBag, pCfgMap->OpenCost, 1, ITEM_CHANGE_REASON::ICR_TREASURE_OPEN))
            return 10002;
    }

    CExtCurrency* pCurrency = Player::GetCurrency(m_pPlayer);
    if (pCfgMap->MoneyCost > 0)
    {
        if (!CExtCurrency::DecMoney(pCurrency, pCfgMap->MoneyCost, CURRENCY_CHANGE_REASON::MCR_TREASURE_OPEN, 0))
            return 10002;
    }

    Answer::Random* pRand = Answer::Singleton<Answer::Random>::instance();
    int32_t nRandom = Answer::Random::generate(pRand, 1, 10000);
    int32_t Cumulate = 0;
    int32_t RewardCfgId = 0;

    for (auto it = pCfgMap->RewardList.begin(); it != pCfgMap->RewardList.end(); ++it)
    {
        Cumulate += it->Probability;
        if (nRandom <= Cumulate)
        {
            RewardCfgId = it->RewardId;
            break;
        }
    }

    if (RewardCfgId <= 0)
        return 10002;

    CfgData* pRewardCfg = Answer::Singleton<CfgData>::instance();
    CfgReward* pReward = CfgData::GetReward(pRewardCfg, RewardCfgId);
    if (!pReward)
        return 10002;

    MailReward mailReward;
    mailReward.reason = MAIL_CHANGE_REASON::MCR_TREASURE_REWARD;
    Player::MailSendReward(m_pPlayer, &pReward->MailTitle, &pReward->MailContent, &pReward->Items, &mailReward);

    Player::updateRecord(m_pPlayer, 1156, 0);
    Player::updateRecord(m_pPlayer, 1161, 0);

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, 5);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        GameService::sendPacket(pGame, packet);
    }
    return 0;
}

CTreasureMap::GetDidff()
{
      return abs32(m_SpecialX - X) + abs32(m_SpecialY - Y);
}


CTreasureMap::OnGetTreasureMapReward()
{
      int32_t v2;
}


CTreasureMap::SendGetItemInfo()
{
      int8_t ConnId;
}


CTreasureMap::GetUseMaxTime()
{
      CVip *PlayerVip;
}


CTreasureMap::SendEndInfo()
{
      int8_t ConnId;
}


CTreasureMap::OnAskSpecialTreasureMapInfo()
{
      int32_t v2;
}


CTreasureMap::OnGetSpecialTreasureMapReward()
{
      CExtCharBag *Bag;
}


CTreasureMap::SendSpecialTreasureInfo()
{
      int8_t ConnId;
}


CTreasureMap::OnAskTreasureMapInfo()
{
      CExtCharBag *Bag;
}


CTreasureMap::SendTreasureInfo()
{
      int8_t ConnId;
}
