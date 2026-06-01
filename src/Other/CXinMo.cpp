#include "Other/CXinMo.h"

CXinMo::CXinMo()
    : CExtSystemBase()
    , m_Exp(0)
    , m_XinMoLevel(0)
    , m_XinMoActiveLevel(0)
    , m_QiQingInfo(0)
{
    memset(m_ItemList, 0, sizeof(m_ItemList));
}

CXinMo::~CXinMo()
{
}

void CXinMo::OnCleanUp()
{
    m_Exp = 0;
    m_XinMoLevel = 0;
    m_XinMoActiveLevel = 0;
    m_QiQingLevel.clear();
    m_QiQingInfo = 0;
    memset(m_ItemList, 0, sizeof(m_ItemList));
}

void CXinMo::OnUpdate(int64_t curTick)
{
}

void CXinMo::OnDaySwitch(int32_t nDiffDays)
{
}

void CXinMo::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_Exp = dbData->m_XinMoDBData.m_Exp;
    m_XinMoLevel = dbData->m_XinMoDBData.m_XinMoLevel;
    m_XinMoActiveLevel = dbData->m_XinMoDBData.m_XinMoActiveLevel;
    m_QiQingLevel = dbData->m_XinMoDBData.m_QiQingLevel;
    m_QiQingInfo = dbData->m_XinMoDBData.m_QiQingInfo;
    memcpy(m_ItemList, dbData->m_XinMoDBData.m_ItemList, sizeof(m_ItemList));
}

void CXinMo::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_XinMoDBData.m_Exp = m_Exp;
    dbData->m_XinMoDBData.m_XinMoLevel = m_XinMoLevel;
    dbData->m_XinMoDBData.m_XinMoActiveLevel = m_XinMoActiveLevel;
    dbData->m_XinMoDBData.m_QiQingLevel = m_QiQingLevel;
    dbData->m_XinMoDBData.m_QiQingInfo = m_QiQingInfo;
    memcpy(dbData->m_XinMoDBData.m_ItemList, m_ItemList, sizeof(m_ItemList));
}

void CXinMo::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(302);
    procList->push_back(303);
    procList->push_back(304);
    procList->push_back(306);
    procList->push_back(307);
    procList->push_back(308);
    procList->push_back(309);
    procList->push_back(310);
}

int32_t CXinMo::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 10002;
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    if (CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL) + 1 <= 29)
        return 10002;
    switch (nProcId)
    {
        case 0x12E: return OnXinMoLevel(inPacket);
        case 0x12F: return OnActiveXinMo(inPacket);
        case 0x130: return OnJinHua(inPacket);
        case 0x132: return OnGetItem(inPacket);
        case 0x133: return OnTidy(inPacket);
        case 0x134: return OnDec(inPacket);
        case 0x135: return OnQiQingLevelUp(inPacket);
        case 0x136: return OnXinQing(inPacket);
    }
    return 10002;
}

void CXinMo::AddCharAttr()
{
    if (!m_pPlayer)
        return;
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    const XinMoTable* pTable = CfgData::GetXinMoTable(pCfg);

    if (m_XinMoLevel > 0)
    {
        const XinMoLevelCfg* pCfg = CfgData::GetXinMoLevelCfg(pTable, m_XinMoLevel);
        if (pCfg)
        {
            for (auto& attr : pCfg->AddAttr)
            {
                Unit::AddAttrValue(m_pPlayer, (CObjAttrs::Index_T)attr.m_nAddAttrType, attr.m_nAddAttrValue);
            }
        }
    }

    if (m_XinMoActiveLevel > 0)
    {
        const XinMoActiveCfg* pCfg = CfgData::GetXinMoActiveCfg(pTable, m_XinMoActiveLevel);
        if (pCfg)
        {
            for (auto& attr : pCfg->AddAttr)
            {
                Unit::AddAttrValue(m_pPlayer, (CObjAttrs::Index_T)attr.m_nAddAttrType, attr.m_nAddAttrValue);
            }
        }
    }

    for (auto& kv : m_QiQingLevel)
    {
        const QiQingLevelCfg* pCfg = CfgData::GetQiQingLevelCfg(pTable, kv.first, kv.second);
        if (pCfg)
        {
            for (auto& attr : pCfg->AddAttr)
            {
                Unit::AddAttrValue(m_pPlayer, (CObjAttrs::Index_T)attr.m_nAddAttrType, attr.m_nAddAttrValue);
            }
        }
    }
}

int32_t CXinMo::OnXinMoLevel(Answer::NetPacket* inPacket)
{
    return 10002;
}

int32_t CXinMo::OnQiQingLevelUp(Answer::NetPacket* inPacket)
{
    return 10002;
}

int32_t CXinMo::OnXinQing(Answer::NetPacket* inPacket)
{
    return 10002;
}

void CXinMo::SetYaoYiYaoFlag(int32_t nCount)
{
}

int32_t CXinMo::GetFlagCount()
{
    return 0;
}

void CXinMo::SetFlag(int32_t nId)
{
}

void CXinMo::GetNoFlagIdList(Int32Vector* retstr)
{
}

void CXinMo::AddExp(int32_t nValue)
{
    if (m_pPlayer)
    {
        m_Exp += nValue;
    }
}

int32_t CXinMo::OnActiveXinMo(Answer::NetPacket* inPacket)
{
    return 10002;
}

int32_t CXinMo::OnJinHua(Answer::NetPacket* inPacket)
{
    return 10002;
}

void CXinMo::SendXiMoInfo()
{
    if (m_pPlayer)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* pGame = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D3E);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, m_Exp);
            Answer::NetPacket::writeInt32(packet, m_XinMoLevel);
            Answer::NetPacket::writeInt32(packet, m_XinMoActiveLevel);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
        }
    }
}

void CXinMo::SendXinQingInfo()
{
    if (m_pPlayer)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* pGame = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D43);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, GetLeftYaoYiYaoTimes());
            Answer::NetPacket::writeInt32(packet, GetChangeXinQingTimes());
            Answer::NetPacket::writeInt32(packet, m_QiQingInfo);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
        }
    }
}

void CXinMo::SendXinMoQiQingLevel(int32_t nType)
{
    if (m_pPlayer)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* pGame = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D42);
        if (packet)
        {
            if (nType)
            {
                Answer::NetPacket::writeInt32(packet, 1);
                Answer::NetPacket::writeInt32(packet, nType);
                Answer::NetPacket::writeInt32(packet, m_QiQingLevel[nType]);
            }
            else
            {
                Answer::NetPacket::writeInt32(packet, (int32_t)m_QiQingLevel.size());
                for (auto& kv : m_QiQingLevel)
                {
                    Answer::NetPacket::writeInt32(packet, kv.first);
                    Answer::NetPacket::writeInt32(packet, kv.second);
                }
            }
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
        }
    }
}

int32_t CXinMo::OnGetItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    const Map* pMap = StaticObj::getMap(m_pPlayer);
    if (!pMap || !Map::IsXinMoCun(pMap))
        return 2;
    int32_t Slot = Answer::NetPacket::readInt32(inPacket);
    if (Slot < -1 || Slot > 55)
        return 2;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (CExtCharBag::GetFreeSlotCount(Bag) <= 0)
        return 2;
    if (Slot == -1)
    {
        for (int i = 0; i <= 55; i++)
        {
            if (m_ItemList[i].itemCount > 0 && m_ItemList[i].itemId > 0)
            {
                Bag = Player::GetBag(m_pPlayer);
                if (CExtCharBag::GetFreeSlotCount(Bag) <= 0)
                {
                    Player::TiShiInfo(m_pPlayer, 2048, 0);
                    return 2;
                }
                GetItem(i);
            }
        }
    }
    else
    {
        GetItem(Slot);
    }
    return 0;
}

int32_t CXinMo::OnDec(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t Slot = Answer::NetPacket::readInt32(inPacket);
    if (Slot >= 0x38)
        return 2;
    if (m_ItemList[Slot].itemCount <= 0 || m_ItemList[Slot].itemId <= 0)
        return 2;
    ResetBagSlot(Slot, 1);
    std::list<int> TmpList;
    TmpList.push_back(Slot);
    SendChangeItem(&TmpList);
    return 0;
}

int32_t CXinMo::OnTidy(Answer::NetPacket* inPacket)
{
    return 0;
}

bool CXinMo::AddItem(MemChrBagVector* p_Items)
{
    return false;
}

void CXinMo::GetItem(int32_t Slot)
{
}

void CXinMo::SendAllItem()
{
}

void CXinMo::SendChangeItem(std::list<int>* ItemList)
{
}

void CXinMo::CleanXinMoBag()
{
}

void CXinMo::DieDropFromXinMoBag(MemChrBagVector* TmpVt)
{
}

int32_t CXinMo::GetFreeSlotCount()
{
    return 0;
}

int32_t CXinMo::ResetBagSlot(int32_t nSlot, int32_t Opway)
{
    return 0;
}

void CXinMo::AddXinMoBagLog(MemChrBag item, int32_t falg, int32_t Opway)
{
}

int32_t CXinMo::GetQiQingLevel(int32_t nType)
{
    auto it = m_QiQingLevel.find(nType);
    if (it != m_QiQingLevel.end())
        return it->second;
    return 0;
}

int32_t CXinMo::GetLeftYaoYiYaoTimes()
{
    return 0;
}

int32_t CXinMo::GetChangeXinQingTimes()
{
    return 0;
}

void CXinMo::AddYaoYiYaoTimes()
{
}

void CXinMo::AddChangeXinQingTimes()
{
}
