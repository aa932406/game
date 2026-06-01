#include "Other/CEquipBack.h"

#include <cstring>
#include <algorithm>
#include <sstream>
#include "Game/GameService.h"
#include "Game/DBService.h"
#include "Game/CTimer.h"
#include "Character/Player.h"
#include "Character/CExtCharBag.h"
#include "Character/ChrTask.h"
#include "Config/CfgData.h"
#include "Config/CfgEquipTable.h"
#include "Database/DBService.h"
#include "Network/NetPacket.h"
#include "Utility/Answer.h"

CEquipBack::CEquipBack()
{
}

CEquipBack::~CEquipBack()
{
}

void CEquipBack::Init(int32_t line)
{
    if (line == 9)
        return;

    Answer::DBPool *v2 = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(v2);

    char szSQL[4096] = {0};
    snprintf(szSQL, sizeof(szSQL), "SELECT * FROM `mem_equip_back_record` order by nTime desc Limit 0, %d", 50);
    Answer::MySqlQuery result(db.query(szSQL));
    while (!result.eof())
    {
        BackRecord stu;
        memset(&stu, 0, sizeof(stu));
        stu.nId = result.getIntValue("nId", 0);
        stu.nType = result.getIntValue("nType", 0);
        stu.nTime = result.getIntValue("nTime", 0);
        const char *StringValue = result.getStringValue("name", "");
        stu.name = StringValue;
        m_BackRecord.push_back(stu);
        result.nextRow();
    }

    char szSQL_1[4096] = {0};
    snprintf(szSQL_1, sizeof(szSQL_1), "SELECT * FROM `mem_equip_back_depot`");
    Answer::MySqlQuery result_1(db.query(szSQL_1));
    while (!result_1.eof())
    {
        int8_t Type = result_1.getIntValue("nType", 0);
        if (Type == 1)
        {
            BackDepot stu_0;
            memset(&stu_0, 0, sizeof(stu_0));
            stu_0.Count = result_1.getIntValue("nCount", 0);
            stu_0.nId = result_1.getIntValue("nId", 0);
            m_BackDepotMap[stu_0.nId] = stu_0;
        }
        else if (Type == 2)
        {
            BackLimitInfo stu_1;
            memset(&stu_1, 0, sizeof(stu_1));
            stu_1.nId = result_1.getIntValue("nId", 0);
            stu_1.nCount = result_1.getIntValue("nCount", 0);
            const char *v7 = result_1.getStringValue("name", "");
            stu_1.sLastName = v7;
            m_BackLimitInfoMap[stu_1.nId] = stu_1;
        }
        result_1.nextRow();
    }
}

int32_t CEquipBack::OnGoldBack(Player *pPlayer, Answer::NetPacket *packet)
{
    if (!pPlayer || !packet)
        return 10002;

    ChrTask *Task = pPlayer->GetTask();
    Task->updateTaskCount(42, 1);

    int32_t nId = packet->readInt32();
    const CfgEquipTable *EquipTable = CfgData::instance()->GetEquipTable();
    const EquipBack *pCfg = EquipTable->GetEquipBackCfg(nId);
    if (!pCfg)
        return 10002;

    Int32Vector vSlot;
    pPlayer->queryBagInfo(packet, &vSlot);

    ItemDataList ItemList;
    for (auto EquipIt = pCfg->nEquipList.begin(); EquipIt != pCfg->nEquipList.end(); ++EquipIt)
    {
        ItemData stu_0;
        stu_0.m_nId = *EquipIt;
        stu_0.m_nClass = 2;
        stu_0.m_nCount = 1;
        ItemList.push_back(stu_0);
    }

    if (ItemList.empty())
        return 10002;

    int32_t v3 = 10002;

    if (pCfg->nType == 1)
    {
        if (pCfg->nLimitNum <= 0)
            return 10002;

        if (pCfg->nDisplayDay > 0)
        {
            int32_t DayDiff = CfgData::instance()->getServerDiffDay(SERVER_TYPE::SVT_NORMAL) + 1;
            if (pCfg->nDisplayDay > DayDiff)
                return 10002;
        }

        auto it = m_BackLimitInfoMap.find(nId);
        if (it != m_BackLimitInfoMap.end())
        {
            if (it->second.nCount >= pCfg->nLimitNum)
                return 10002;
        }

        if (pCfg->nRecovType < 0 || pCfg->nRecovValues <= 0)
            return 10002;

        CExtCharBag *Bag = pPlayer->GetBag();
        if (!Bag->RemoveItem(&vSlot, &ItemList, ITEM_CHANGE_REASON::ICR_EQUIP_GOLD_BACK))
            return 10002;

        int32_t RecovValue = pCfg->nRecovValues;
        pPlayer->AddCurrency((const CURRENCY_TYPE)pCfg->nRecovType, RecovValue, CURRENCY_CHANGE_REASON::GCR_EQUIP_GOLD_BACK, 0);

        BackLimitInfo stu_1;
        stu_1.nCount = 1;
        stu_1.nId = nId;
        pPlayer->getName(stu_1.sLastName);
        AddLimitCount(&stu_1);
        SendEquipBackLimitChange(pPlayer, nId);

        Answer::NetPacket *packet_0 = GameService::instance()->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet_0)
        {
            packet_0->writeInt32(451);
            std::string val;
            pPlayer->getName(val);
            packet_0->writeUTF8(val);
            packet_0->writeInt64(pPlayer->getCid());
            packet_0->writeInt32(ItemList.front().m_nId);
            packet_0->writeInt32(RecovValue);
            uint32_t WOffset = packet_0->getWOffset();
            packet_0->setSize(WOffset);
            GameService::instance()->worldBroadcast(packet_0);
        }
        else
        {
            return 10002;
        }
    }
    else if (pCfg->nType == 2)
    {
        int32_t DayDiff_0 = CfgData::instance()->getServerDiffDay(SERVER_TYPE::SVT_NORMAL);
        if (DayDiff_0 + 1 > pCfg->nOpenDay)
            return 10002;

        if (pCfg->nLimitNum <= 0)
            return 10002;

        auto it_0 = m_BackLimitInfoMap.find(nId);
        if (it_0 != m_BackLimitInfoMap.end())
        {
            if (it_0->second.nCount >= pCfg->nLimitNum)
                return 10002;
        }

        if (pCfg->nRecovType < 0 || pCfg->nRecovValues <= 0)
            return 10002;

        CExtCharBag *v18 = pPlayer->GetBag();
        if (!v18->RemoveItem(&vSlot, &ItemList, ITEM_CHANGE_REASON::ICR_EQUIP_GOLD_BACK))
            return 10002;

        int32_t RecovValue_0 = pCfg->nRecovValues;
        pPlayer->AddCurrency((const CURRENCY_TYPE)pCfg->nRecovType, RecovValue_0, CURRENCY_CHANGE_REASON::GCR_EQUIP_GOLD_BACK, 0);

        BackLimitInfo stu_2;
        stu_2.nCount = 1;
        stu_2.nId = nId;
        pPlayer->getName(stu_2.sLastName);
        AddLimitCount(&stu_2);
        SendEquipBackLimitChange(pPlayer, nId);

        Answer::NetPacket *packet_1 = GameService::instance()->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet_1)
        {
            packet_1->writeInt32(443);
            packet_1->writeInt64(pPlayer->getCid());
            std::string v45;
            pPlayer->getName(v45);
            packet_1->writeUTF8(v45);
            packet_1->writeInt32(nId);
            packet_1->writeInt8(pCfg->nRecovType);
            packet_1->writeInt32(RecovValue_0);
            uint32_t WOffset = packet_1->getWOffset();
            packet_1->setSize(WOffset);
            GameService::instance()->worldBroadcast(packet_1);
        }
        else
        {
            return 10002;
        }
    }
    else
    {
        return 10002;
    }

    BackRecord stu;
    memset(&stu, 0, sizeof(stu));
    stu.nId = nId;
    stu.nType = 1;
    stu.nTime = Answer::DayTime::now();
    pPlayer->getName(stu.name);
    SendEquipBackOnRecord(pPlayer, &stu);
    AddRecord(&stu);

    return 0;
}

void CEquipBack::AddLimitCount(BackLimitInfo *p_stu)
{
    Answer::MutexGuard lock(&m_DepotLock);

    auto it = m_BackLimitInfoMap.find(p_stu->nId);
    if (it != m_BackLimitInfoMap.end())
    {
        it->second.nCount += p_stu->nCount;
        it->second.sLastName = p_stu->sLastName;
        it->second.nId = p_stu->nId;
    }
    else
    {
        m_BackLimitInfoMap[p_stu->nId] = *p_stu;
    }

    auto itTmp = m_BackLimitInfoMap.find(p_stu->nId);
    if (itTmp != m_BackLimitInfoMap.end())
    {
        int32_t nCount = itTmp->second.nCount;
        int32_t first = itTmp->first;
        DBService::instance()->SaveEquipBackCount(first, 2, nCount, &itTmp->second.sLastName);
    }
}

void CEquipBack::ChangeDeptCount(BackDepot stu)
{
    Answer::MutexGuard lock(&m_DepotLock);

    auto it = m_BackDepotMap.find(stu.nId);
    if (it != m_BackDepotMap.end())
    {
        it->second.Count += stu.Count;
        it->second.nId = stu.nId;
    }
    else
    {
        m_BackDepotMap[stu.nId] = stu;
    }

    auto itTmp = m_BackDepotMap.find(stu.nId);
    if (itTmp != m_BackDepotMap.end())
    {
        std::string p_name;
        int32_t Count = itTmp->second.Count;
        int32_t first = itTmp->first;
        DBService::instance()->SaveEquipBackCount(first, 1, Count, &p_name);
    }
}

void CEquipBack::AddRecord(BackRecord *p_stu)
{
    Answer::MutexGuard lock(&m_RecordLock);

    if (m_BackRecord.size() > 50)
        m_BackRecord.pop_front();

    std::string p_name = p_stu->name;
    int32_t nTime = p_stu->nTime;
    int8_t nType = p_stu->nType;
    int32_t nId = p_stu->nId;
    DBService::instance()->SaveEquipBackRecord(nId, nType, nTime, &p_name);
    m_BackRecord.push_back(*p_stu);
}

int32_t CEquipBack::OnBuyBack(Player *pPlayer, Answer::NetPacket *packet)
{
    if (!pPlayer || !packet)
        return 10002;

    int32_t nId = packet->readInt32();
    const CfgEquipTable *EquipTable = CfgData::instance()->GetEquipTable();
    const EquipBack *pCfg = EquipTable->GetEquipBackCfg(nId);
    if (!pCfg)
        return 10002;
    if (pCfg->nType != 1)
        return 10002;
    if (pCfg->nRecovType < 0 || pCfg->nRecovValues <= 0)
        return 10002;

    auto it = m_BackDepotMap.find(nId);
    if (it == m_BackDepotMap.end())
        return 10002;
    if (it->second.Count <= 0)
        return 10002;

    std::vector<MemChrBag> ItemVt;
    for (auto EquipIt = pCfg->nEquipList.begin(); EquipIt != pCfg->nEquipList.end(); ++EquipIt)
    {
        MemChrBag stu_0;
        stu_0.itemId = *EquipIt;
        stu_0.itemClass = 2;
        stu_0.itemCount = 1;
        stu_0.endTime = 0;
        stu_0.srcId = 0;
        ItemVt.push_back(stu_0);
    }

    CExtCharBag *Bag = pPlayer->GetBag();
    int32_t FreeSlotCount = Bag->GetFreeSlotCount();
    if (FreeSlotCount < (int)ItemVt.size())
        return 10002;

    if (!pPlayer->DecCurrency((const CURRENCY_TYPE)pCfg->nBuyBackType, pCfg->nBuyBackValue, CURRENCY_CHANGE_REASON::GCR_EQUIP_BUY_BACK, 0))
        return 10002;

    BackDepot Deptstu;
    Deptstu.nId = nId;
    Deptstu.Count = -1;
    ChangeDeptCount(Deptstu);

    CExtCharBag *v8 = pPlayer->GetBag();
    v8->AddItem(&ItemVt, ITEM_CHANGE_REASON::ICR_EQUIP_BUY_BACK);

    BackRecord stu;
    memset(&stu, 0, sizeof(stu));
    stu.nId = nId;
    stu.nType = 2;
    stu.nTime = Answer::DayTime::now();
    pPlayer->getName(stu.name);
    SendEquipBackOnRecord(pPlayer, &stu);
    SendEquipBackCountChange(pPlayer, nId);
    AddRecord(&stu);

    Answer::NetPacket *SendPacket = GameService::instance()->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (SendPacket)
    {
        SendPacket->writeInt32(452);
        std::string val;
        pPlayer->getName(val);
        SendPacket->writeUTF8(val);
        SendPacket->writeInt64(pPlayer->getCid());
        SendPacket->writeInt32(ItemVt.begin()->itemId);
        SendPacket->writeInt32(pCfg->nBuyBackValue);
        uint32_t WOffset = SendPacket->getWOffset();
        SendPacket->setSize(WOffset);
        GameService::instance()->worldBroadcast(SendPacket);
        return 0;
    }

    return 10002;
}

void CEquipBack::SendEquipBackInfo(Player *pPlayer)
{
    if (!pPlayer)
        return;

    Answer::NetPacket *packet = GameService::instance()->popNetpacket(pPlayer->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2F4B);
    if (!packet)
        return;

    packet->writeInt32(m_BackRecord.size());
    for (auto it = m_BackRecord.begin(); it != m_BackRecord.end(); ++it)
    {
        packet->writeInt32(it->nId);
        packet->writeInt8(it->nType);
        packet->writeUTF8(it->name);
        packet->writeInt32(it->nTime);
    }

    packet->writeInt32(m_BackDepotMap.size());
    for (auto DepotIt = m_BackDepotMap.begin(); DepotIt != m_BackDepotMap.end(); ++DepotIt)
    {
        packet->writeInt32(DepotIt->first);
        packet->writeInt32(DepotIt->second.Count);
    }

    packet->writeInt32(m_BackLimitInfoMap.size());
    for (auto LimitIt = m_BackLimitInfoMap.begin(); LimitIt != m_BackLimitInfoMap.end(); ++LimitIt)
    {
        packet->writeInt32(LimitIt->first);
        packet->writeInt32(LimitIt->second.nCount);
        packet->writeUTF8(LimitIt->second.sLastName);
    }

    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    GameService::instance()->sendPacketTo(pPlayer->getConnId(), pPlayer->getGateIndex(), packet);
}

void CEquipBack::SendEquipBackOnRecord(Player *pPlayer, BackRecord *p_stu)
{
    if (!pPlayer)
        return;

    Answer::NetPacket *packet = GameService::instance()->popNetpacket(pPlayer->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2F4C);
    if (!packet)
        return;

    packet->writeInt32(p_stu->nId);
    packet->writeInt8(p_stu->nType);
    packet->writeUTF8(p_stu->name);
    packet->writeInt32(p_stu->nTime);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    GameService::instance()->sendPacketTo(pPlayer->getConnId(), pPlayer->getGateIndex(), packet);
}

void CEquipBack::SendEquipBackCountChange(Player *pPlayer, int32_t nId)
{
    if (!pPlayer)
        return;

    auto it = m_BackDepotMap.find(nId);
    if (it == m_BackDepotMap.end())
        return;

    Answer::NetPacket *packet = GameService::instance()->popNetpacket(pPlayer->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2F4D);
    if (!packet)
        return;

    packet->writeInt32(it->first);
    packet->writeInt32(it->second.Count);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    GameService::instance()->sendPacketTo(pPlayer->getConnId(), pPlayer->getGateIndex(), packet);
}

void CEquipBack::SendEquipBackLimitChange(Player *pPlayer, int32_t nId)
{
    if (!pPlayer)
        return;

    auto it = m_BackLimitInfoMap.find(nId);
    if (it == m_BackLimitInfoMap.end())
        return;

    Answer::NetPacket *packet = GameService::instance()->popNetpacket(pPlayer->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2F4E);
    if (!packet)
        return;

    packet->writeInt32(it->first);
    packet->writeInt32(it->second.nCount);
    packet->writeUTF8(it->second.sLastName);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    GameService::instance()->sendPacketTo(pPlayer->getConnId(), pPlayer->getGateIndex(), packet);
}
