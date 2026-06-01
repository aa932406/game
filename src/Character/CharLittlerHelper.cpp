// CharLittlerHelper.cpp
#include "CharLittlerHelper.h"
#include "Game/Player.h"
#include "Game/GameService.h"
#include "Game/CLittleHelper.h"
#include "Game/CPoolManager.h"
#include "Game/Map.h"
#include "Character/CExtCharBag.h"
#include "Config/CfgData.h"
#include "Config/CfgEquip.h"
#include "Other/LittleHelperCfg.h"
#include "Other/StaticObj.h"
#include "Utility/StringUtility.h"
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>

CharLittlerHelper::CharLittlerHelper()
{
    m_ActMap.clear();
    m_EquipFilter.clear();
    m_ItemFilterMap.clear();
    m_CLittleHelper = nullptr;
    m_LittleHelperId = 0;
    memset(m_PickSetUp, 0, sizeof(m_PickSetUp));
    m_NeedPick = true;
}

CharLittlerHelper::~CharLittlerHelper()
{
}

void CharLittlerHelper::OnCleanUp()
{
    if (m_CLittleHelper)
    {
        m_CLittleHelper = nullptr;
    }
    m_ActMap.clear();
    m_LittleHelperId = 0;
}

void CharLittlerHelper::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (!dbData) return;

    m_LittleHelperId = dbData->m_CLittleHelper.m_LittleHelperId;
    m_ActMap = dbData->m_CLittleHelper.m_ActLittleHelperInfoMap;
}

void CharLittlerHelper::OnSaveToDB(PlayerDBData* dbData)
{
    if (!dbData) return;

    dbData->m_CLittleHelper.m_ActLittleHelperInfoMap = m_ActMap;
    dbData->m_CLittleHelper.m_LittleHelperId = m_LittleHelperId;
}

void CharLittlerHelper::GetInterestsProtocol(ProcIdList* procList)
{
    if (!procList) return;
    procList->push_back(507);
    procList->push_back(508);
    procList->push_back(509);
}

void CharLittlerHelper::OnUpdate(int64_t curTick)
{
    (void)curTick;
    if (!m_CLittleHelper) return;
}

void CharLittlerHelper::OnDaySwitch(int32_t nDiffDays)
{
    (void)nDiffDays;
}

// ===== OnAutoWork =====

int32_t CharLittlerHelper::OnAutoWork()
{
    return OnWork(m_LittleHelperId);
}

// ===== OnWork (int32_t nId) =====

int32_t CharLittlerHelper::OnWork(int32_t nId)
{
    if (!m_pPlayer)
        return 2;

    ActLittleHelperInfo* pInfo = GetActLittleHelperInfo(nId);
    if (!pInfo)
        return 2;

    int32_t Now = m_pPlayer->getNow();
    if (pInfo->nTimes < Now)
        return 2;

    if (m_CLittleHelper)
    {
        if (m_CLittleHelper->OnGetLittleHelperId() == nId && m_CLittleHelper->IsWork())
            return 2;

        if (m_CLittleHelper->OnGetLittleHelperId() != nId)
            m_CLittleHelper->ChangeLittleHelperId(nId, pInfo->nSkin);
    }
    else
    {
        CPoolManager* pool = Answer::Singleton<CPoolManager>::instance();
        m_CLittleHelper = pool->pop<CLittleHelper>();
        if (!m_CLittleHelper)
            return 2;

        m_CLittleHelper->init(m_pPlayer, nId, pInfo->nSkin);
    }

    m_CLittleHelper->EnterMap();
    m_CLittleHelper->OnWork();
    m_LittleHelperId = nId;
    SendCharLittlerhelper();
    return 0;
}

// ===== OnWork (NetPacket) =====

int32_t CharLittlerHelper::OnWork(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    GameService* gs = Answer::Singleton<GameService>::instance();
    if (gs->getLine() == 9)
        return 2;

    int32_t nId = inPacket->readInt32();

    Map* pMap = m_pPlayer->getMap();
    if (!pMap)
        return 2;
    if (!pMap->CanUsePet())
        return 2;

    if (!OnWork(nId))
    {
        uint16_t Proc = inPacket->getProc();
        int16_t GateIndex = m_pPlayer->getGateIndex();
        int8_t ConnId = m_pPlayer->getConnId();
        gs->replySuccess(ConnId, GateIndex, Proc, nId);
    }
    return 0;
}

// ===== SetTargetTile =====

void CharLittlerHelper::SetTargetTile(int32_t tx, int32_t ty, Direction dir)
{
    if (m_CLittleHelper && m_CLittleHelper->IsWork())
        m_CLittleHelper->ResetTargetTile(tx, ty, dir);
}

// ===== AddCharAttr =====

void CharLittlerHelper::AddCharAttr()
{
    if (!m_pPlayer)
        return;

    int32_t Now = m_pPlayer->getNow();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();

    for (auto& kv : m_ActMap)
    {
        if (kv.second.nTimes > Now)
        {
            const LittleHelperCfg* pLittleHelper = cfg->GetLittleHelperCfg(kv.first);
            if (pLittleHelper)
            {
                for (auto& attr : pLittleHelper->lAttr)
                {
                    m_pPlayer->AddAttrValue(
                        static_cast<CObjAttrs::Index_T>(attr.m_nAddAttrType),
                        attr.m_nAddAttrValue);
                }
            }
        }
    }
}

// ===== OnRest (NetPacket) =====

int32_t CharLittlerHelper::OnRest(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer || !m_CLittleHelper)
        return 2;

    if (!m_CLittleHelper->IsWork())
        return 10002;

    m_CLittleHelper->LeaveMap();
    m_CLittleHelper->OnRest();
    m_LittleHelperId = 0;

    uint16_t Proc = inPacket->getProc();
    int16_t GateIndex = m_pPlayer->getGateIndex();
    int8_t ConnId = m_pPlayer->getConnId();
    GameService* gs = Answer::Singleton<GameService>::instance();
    gs->replySuccess(ConnId, GateIndex, Proc, 0);
    return 0;
}

// ===== OnRest () =====

int32_t CharLittlerHelper::OnRest()
{
    if (!m_pPlayer || !m_CLittleHelper)
        return 2;

    if (m_CLittleHelper->IsWork())
    {
        m_CLittleHelper->LeaveMap();
        m_CLittleHelper->OnRest();
        SendCharLittlerhelper();
    }
    return 0;
}

// ===== AddLittlerHelper =====

int32_t CharLittlerHelper::AddLittlerHelper(int32_t nId, int32_t nAddTime)
{
    if (!m_pPlayer)
        return 2;

    GameService* gs = Answer::Singleton<GameService>::instance();
    if (gs->getLine() == 9)
        return 2;

    Map* pMap = m_pPlayer->getMap();
    if (!pMap)
        return 2;

    ActLittleHelperInfo* pInfo = GetActLittleHelperInfo(nId);
    int32_t nTime = m_pPlayer->getNow();

    if (pInfo)
    {
        int32_t DaoQiTime;
        if (pInfo->nTimes < nTime)
            DaoQiTime = nTime + nAddTime;
        else
            DaoQiTime = pInfo->nTimes + nAddTime;
        pInfo->nTimes = DaoQiTime;
    }
    else
    {
        ActLittleHelperInfo stu;
        stu.nId = nId;
        stu.nTimes = nTime + nAddTime;
        stu.nSkin = 0;
        m_ActMap[nId] = stu;
    }

    if (pMap)
        OnWork(nId);

    SendCharLittlerhelper();
    m_pPlayer->RecalcAttr();

    // broadcast announcement
    Answer::NetPacket* packet = gs->popNetpacket(
        m_pPlayer->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (packet)
    {
        packet->writeInt32(3011);
        std::string name = m_pPlayer->getName();
        packet->writeUTF8(name);
        CharId_t cid = m_pPlayer->getCid();
        packet->writeInt64(cid);
        uint32_t WOffset = packet->getWOffset();
        packet->setSize(WOffset);
        int8_t connId = m_pPlayer->getConnId();
        gs->worldBroadcast(connId, packet);
    }

    return 0;
}

// ===== OnBuyLittlerHelper =====

int32_t CharLittlerHelper::OnBuyLittlerHelper(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    GameService* gs = Answer::Singleton<GameService>::instance();
    if (gs->getLine() == 9)
        return 2;

    Map* pMap = m_pPlayer->getMap();
    if (!pMap)
        return 2;
    if (!pMap->CanUsePet())
        return 2;

    int32_t nId = inPacket->readInt32();

    ActLittleHelperInfo* pInfo = GetActLittleHelperInfo(nId);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const LittleHelperCfg* pLittleHelper = cfg->GetLittleHelperCfg(nId);
    if (!pLittleHelper)
        return 2;
    if (pLittleHelper->nGold <= 0 || pLittleHelper->nReGold <= 0 || pLittleHelper->nTime <= 0)
        return 2;

    int32_t nTime = m_pPlayer->getNow();
    int32_t NeedGold = 0;
    int32_t DaoQiTime = 0;

    if (pInfo)
    {
        if (pInfo->nTimes < nTime)
        {
            NeedGold = pLittleHelper->nGold;
            DaoQiTime = nTime + pLittleHelper->nTime;
        }
        else
        {
            NeedGold = pLittleHelper->nReGold;
            DaoQiTime = pInfo->nTimes + pLittleHelper->nTime;
        }

        if (!m_pPlayer->DecCurrency(CURRENCY_TYPE::CURRENCY_GOLD, NeedGold,
                static_cast<CURRENCY_CHANGE_REASON>(GCR_BUY_LITTLE_HELPER), 0))
            return 2;

        pInfo->nTimes = DaoQiTime;
    }
    else
    {
        if (!m_pPlayer->DecCurrency(CURRENCY_TYPE::CURRENCY_GOLD, pLittleHelper->nGold,
                static_cast<CURRENCY_CHANGE_REASON>(GCR_BUY_LITTLE_HELPER), 0))
            return 2;

        ActLittleHelperInfo stu;
        stu.nId = nId;
        stu.nTimes = nTime + pLittleHelper->nTime;
        stu.nSkin = 0;
        m_ActMap[nId] = stu;
    }

    OnWork(nId);
    SendCharLittlerhelper();
    m_pPlayer->RecalcAttr();

    // reply success to client
    uint16_t Proc = inPacket->getProc();
    int16_t GateIndex = m_pPlayer->getGateIndex();
    int8_t ConnId = m_pPlayer->getConnId();
    gs->replySuccess(ConnId, GateIndex, Proc, static_cast<int64_t>(nId));

    // world broadcast announcement
    if (pLittleHelper->nGongGaoId > 0)
    {
        Answer::NetPacket* packet = gs->popNetpacket(
            m_pPlayer->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet)
        {
            packet->writeInt32(pLittleHelper->nGongGaoId);
            std::string val = m_pPlayer->getName();
            packet->writeUTF8(val);
            CharId_t cid = m_pPlayer->getCid();
            packet->writeInt64(cid);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            int8_t connId2 = m_pPlayer->getConnId();
            gs->worldBroadcast(connId2, packet);
        }
    }

    return 0;
}

// ===== SendCharLittlerhelper =====

void CharLittlerHelper::SendCharLittlerhelper()
{
    if (!m_pPlayer)
        return;

    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = gs->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2DEA);
    if (!packet)
        return;

    if (m_CLittleHelper && m_CLittleHelper->IsWork())
    {
        int32_t LittleHelperId = m_CLittleHelper->OnGetLittleHelperId();
        packet->writeInt32(LittleHelperId);
    }
    else
    {
        packet->writeInt32(0);
    }

    int32_t nSize = static_cast<int32_t>(m_ActMap.size());
    packet->writeInt32(nSize);

    for (auto& kv : m_ActMap)
    {
        packet->writeInt32(kv.second.nId);
        packet->writeInt32(kv.second.nTimes);
    }

    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = m_pPlayer->getGateIndex();
    int8_t ConnId = m_pPlayer->getConnId();
    gs->sendPacketTo(ConnId, GateIndex, packet);
}

// ===== CanPick =====

bool CharLittlerHelper::CanPick(int8_t nClass, int32_t nId)
{
    if (!m_pPlayer)
        return false;

    if (IsFilterItem(nClass, nId))
        return false;

    if (nClass == 2) // equipment
    {
        CExtCharBag* Bag = m_pPlayer->GetCharBag();
        if (Bag->GetFreeSlotCount() <= 0)
            return false;

        CfgData* cfg = Answer::Singleton<CfgData>::instance();
        const CfgEquip* pCfgEquip = cfg->getEquip(nId);
        if (!pCfgEquip)
            return false;

        if (pCfgEquip->m_nType > 16 && pCfgEquip->m_nType <= 28)
            return IsPickShouHu();

        if (pCfgEquip->m_nType > 28 && pCfgEquip->m_nType <= 34)
            return IsPickSpecialEquip();

        if (pCfgEquip->m_nType >= 8)
            return IsPickEquip();

        return IsPickEquip() && !IsFilterEquipGrade(pCfgEquip->m_nGrade);
    }
    else if (nClass == 1) // item
    {
        CExtCharBag* Bag = m_pPlayer->GetCharBag();
        CfgData* cfg = Answer::Singleton<CfgData>::instance();
        const struct CfgItem* pItem = cfg->getItem(nId);
        if (!pItem)
            return false;

        // currency items (type==1) do not take bag space
        bool hasSpace = (pItem->type == 1) || (Bag->GetFreeSlotCount() > 0);
        if (!hasSpace)
            return false;

        if (pItem->type == 1 || pItem->type == 82)
        {
            // parse effect string to determine pick strategy
            std::vector<std::string> params;
            std::string delims = ":";
            StringUtility::split(params, pItem->effect, delims);

            int32_t CurrType = 0;
            if (params.size() == 2)
            {
                CurrType = std::atoi(params[0].c_str());
            }

            if (CurrType == 0 || CurrType == 6)
                return IsPickMoney();
            else if (CurrType == 3)
                return IsPickSYB();
            else
                return true;
        }

        return IsPickImte();
    }
    else // other types
    {
        CExtCharBag* Bag = m_pPlayer->GetCharBag();
        return Bag->GetFreeSlotCount() > 0 && !IsFilterItem(nClass, nId);
    }
}

// ===== IsFilterEquipGrade =====

bool CharLittlerHelper::IsFilterEquipGrade(int32_t Grade)
{
    return m_EquipFilter.find(Grade) != m_EquipFilter.end();
}

// ===== IsFilterItem =====

bool CharLittlerHelper::IsFilterItem(int8_t nclass, int32_t nId)
{
    std::pair<int8_t, int32_t> key(nclass, nId);
    return m_ItemFilterMap.find(key) != m_ItemFilterMap.end();
}

// ===== parsePickString =====

void CharLittlerHelper::parsePickString(std::string* p_PickString)
{
    if (!p_PickString || p_PickString->empty())
        return;

    m_EquipFilter.clear();
    m_ItemFilterMap.clear();
    m_NeedPick = true;
    memset(m_PickSetUp, 0, sizeof(m_PickSetUp));

    // format: equipGrades|itemFilters|pickSetup0|pickSetup4|pickSetup2|pickSetup1|pickSetup5
    std::vector<std::string> vStr;
    std::string sep = "|";
    StringUtility::split(vStr, *p_PickString, sep);

    // parse equipment grade filter (index 0)
    if (vStr.size() > 0 && !vStr[0].empty())
    {
        std::vector<std::string> equipGrades;
        std::string comma = ",";
        StringUtility::split(equipGrades, vStr[0], comma);
        for (auto& g : equipGrades)
        {
            int32_t grade = std::atoi(g.c_str());
            if (grade > 0)
                m_EquipFilter.insert(grade);
        }
    }

    // parse item filter (index 1)  format: class:id,class:id,...
    if (vStr.size() > 1 && !vStr[1].empty())
    {
        std::vector<std::string> items;
        std::string comma = ",";
        StringUtility::split(items, vStr[1], comma);
        for (auto& item : items)
        {
            std::vector<std::string> parts;
            std::string colon = ":";
            StringUtility::split(parts, item, colon);
            if (parts.size() >= 2)
            {
                int8_t nClass = static_cast<int8_t>(std::atoi(parts[0].c_str()));
                int32_t filterId = std::atoi(parts[1].c_str());
                std::pair<int8_t, int32_t> key(nClass, filterId);
                m_ItemFilterMap[key] = 1;
            }
        }
    }

    // parse pick setup switches (indices 2-6)
    if (vStr.size() > 2 && !vStr[2].empty())
        m_PickSetUp[0] = static_cast<int8_t>(std::atoi(vStr[2].c_str()));
    if (vStr.size() > 3 && !vStr[3].empty())
        m_PickSetUp[4] = static_cast<int8_t>(std::atoi(vStr[3].c_str()));
    if (vStr.size() > 4 && !vStr[4].empty())
        m_PickSetUp[2] = static_cast<int8_t>(std::atoi(vStr[4].c_str()));
    if (vStr.size() > 5 && !vStr[5].empty())
        m_PickSetUp[1] = static_cast<int8_t>(std::atoi(vStr[5].c_str()));
    if (vStr.size() > 6 && !vStr[6].empty())
        m_PickSetUp[5] = static_cast<int8_t>(std::atoi(vStr[6].c_str()));
}

// ===== IsPick helpers =====

bool CharLittlerHelper::IsPickMoney()
{
    return m_PickSetUp[0] == 0;
}

bool CharLittlerHelper::IsPickSYB()
{
    return m_PickSetUp[4] == 0;
}

bool CharLittlerHelper::IsPickImte()
{
    return m_PickSetUp[2] == 0;
}

bool CharLittlerHelper::IsPickEquip()
{
    return m_PickSetUp[1] == 0;
}

bool CharLittlerHelper::IsPickSpecialEquip()
{
    return m_PickSetUp[5] == 0;
}

bool CharLittlerHelper::IsPickShouHu()
{
    return m_PickSetUp[3] == 0;
}

// ===== GetActLittleHelperInfo =====

ActLittleHelperInfo* CharLittlerHelper::GetActLittleHelperInfo(int32_t nId)
{
    auto it = m_ActMap.find(nId);
    if (it != m_ActMap.end())
    {
        return &it->second;
    }
    return nullptr;
}
