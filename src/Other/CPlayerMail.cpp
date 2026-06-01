#include "Other/CPlayerMail.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CEquipManager.h"
#include "DBService.h"

CPlayerMail::CPlayerMail()
    : CExtSystemBase()
{
    m_MailInfo.clear();
}

CPlayerMail::~CPlayerMail()
{
}

void CPlayerMail::OnCleanUp()
{
}

void CPlayerMail::OnUpdate(int64_t curTick)
{
}

void CPlayerMail::OnDaySwitch(int32_t nDiffDays)
{
}

void CPlayerMail::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_MailInfo = dbData->m_MailDBData.m_MailInfo;
}

void CPlayerMail::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_MailDBData.m_MailInfo = m_MailInfo;
}

void CPlayerMail::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(429);
    procList->push_back(430);
    procList->push_back(431);
    procList->push_back(432);
    procList->push_back(20206);
}

int32_t CPlayerMail::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    if (nProcId == 431)
        return OnGetFuJian(inPacket);
    if (nProcId > 0x1AF)
    {
        if (nProcId == 432)
            return OnDeleteMail(inPacket);
        if (nProcId == 20206)
            return OnReciveNewMail(inPacket);
    }
    else
    {
        if (nProcId == 429)
            return OnReadMail(inPacket);
        if (nProcId == 430)
            return OnRequestMailList(inPacket);
    }
    return 2;
}

int32_t CPlayerMail::OnReadMail(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 2;
    int32_t MailId = inPacket->readInt32();
    auto it = m_MailInfo.find(MailId);
    if (it == m_MailInfo.end())
        return 2;
    it->second.HasRead = 1;
    SendMailContent(MailId);
    return 0;
}

int32_t CPlayerMail::OnRequestMailList(Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    SendMailList();
    return 0;
}

int32_t CPlayerMail::OnGetFuJian(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t MailId = inPacket->readInt32();
    auto it = m_MailInfo.find(MailId);
    if (it == m_MailInfo.end())
        return 2;
    MailInfo* mail = &it->second;
    if (mail->Extract != 1)
        return 2;
    int32_t FuJianCount = 6;
    if (mail->SysMailId == 2000)
    {
        FuJianCount = 1;
        if (mail->Item[0].itemClass != 2)
            return 2;
        int32_t itemId = mail->Item[0].itemId;
        CfgData* v5 = Answer::Singleton<CfgData>::instance();
        const CfgEquipTable* EquipTable = CfgData::GetEquipTable(v5);
        const CfgEquip* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, itemId);
        if (!pCfgEquip || pCfgEquip->m_BackGold <= 0)
            return 2;
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfgEquip->m_BackGold, CURRENCY_CHANGE_REASON::GCR_RANSOM_ITEM, 0))
            return 2;
    }
    MemChrBagVector ItemFuJian;
    ItemFuJian.reserve(FuJianCount);
    for (int32_t i = 0; i < FuJianCount; ++i)
    {
        if (mail->Item[i].itemId > 0 && mail->Item[i].itemCount > 0)
            ItemFuJian.push_back(mail->Item[i]);
    }
    int32_t nReason = (mail->nReason <= 0) ? 2 : mail->nReason;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItemsAndMingGe(Bag, &ItemFuJian, (ITEM_CHANGE_REASON)nReason))
        return 2;
    it->second.HasRead = 1;
    it->second.Extract = 2;
    SendMailContent(MailId);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v12 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v12, ConnId, GateIndex, Proc, 0);
    return 0;
}

int32_t CPlayerMail::OnDeleteMail(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t MailId = inPacket->readInt32();
    auto it = m_MailInfo.find(MailId);
    if (it == m_MailInfo.end())
        return 2;
    m_MailInfo.erase(it);
    SendMailList();
    int8_t ConnId = Player::getConnId(m_pPlayer);
    DBService* v5 = Answer::Singleton<DBService>::instance();
    DBService::OnDeleteMail(v5, ConnId, MailId);
    return 0;
}

int32_t CPlayerMail::OnReciveNewMail(Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    MemEquipVector vEquip;
    vEquip.reserve(6);
    MailInfo mail;
    memset(&mail, 0, sizeof(mail));
    mail.MailId = inPacket->readInt32();
    mail.SysMailId = inPacket->readInt32();
    mail.SenderId = inPacket->readInt64();
    inPacket->readUTF8(&mail.SenderName);
    mail.ReceiveId = inPacket->readInt64();
    inPacket->readUTF8(&mail.ReceiveName);
    mail.SendTime = inPacket->readInt32();
    mail.HasRead = inPacket->readInt8();
    mail.Extract = inPacket->readInt8();
    inPacket->readUTF8(&mail.MailTitle);
    inPacket->readUTF8(&mail.MailContent);
    mail.nReason = inPacket->readInt32();
    inPacket->readUTF8(&mail.Param);
    if (mail.Extract == 1)
    {
        int8_t Count = inPacket->readInt8();
        if (Count > 5 || Count <= 0)
            return 2;
        for (int32_t i = 0; i < Count; ++i)
        {
            int8_t nIndex = inPacket->readInt8();
            if ((uint8_t)nIndex <= 5)
            {
                MemChrBag item;
                memset(&item, 0, sizeof(item));
                item.itemId = inPacket->readInt32();
                item.itemClass = inPacket->readInt8();
                item.itemCount = inPacket->readInt32();
                item.bind = inPacket->readInt8();
                item.endTime = inPacket->readInt32();
                item.srcId = inPacket->readInt64();
                mail.Item[nIndex] = item;
                if (item.itemClass == 2 && item.srcId > 0)
                {
                    CEquipManager* v4 = Answer::Singleton<CEquipManager>::instance();
                    MemEquip equip;
                    CEquipManager::GetMemEquip(&equip, v4, item.srcId);
                    if (equip.base == item.itemId)
                        vEquip.push_back(equip);
                }
            }
        }
    }
    MailInfo* v5 = &m_MailInfo[mail.MailId];
    *v5 = mail;
    if (!vEquip.empty())
    {
        Player* m_pPlayer = this->m_pPlayer;
        CEquipManager* v7 = Answer::Singleton<CEquipManager>::instance();
        CEquipManager::SendPlayerEquipInfo(v7, m_pPlayer, &vEquip);
    }
    SendNewMail();
    return 0;
}

void CPlayerMail::SendMailList()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CAC);
    if (!packet)
        return;
    int32_t CurTime = Unit::getNow(m_pPlayer);
    int32_t nSize = 0;
    packet->writeInt32(0);
    for (auto it = m_MailInfo.begin(); it != m_MailInfo.end(); ++it)
    {
        if (it->second.SendTime + 1209600 >= CurTime)
        {
            packet->writeInt32(it->second.MailId);
            packet->writeInt32(it->second.SysMailId);
            packet->writeInt64(it->second.SenderId);
            packet->writeUTF8(&it->second.SenderName);
            packet->writeInt64(it->second.ReceiveId);
            packet->writeUTF8(&it->second.ReceiveName);
            packet->writeInt32(it->second.SendTime);
            packet->writeInt8(it->second.HasRead);
            packet->writeInt8(it->second.Extract);
            packet->writeUTF8(&it->second.MailTitle);
            if (it->second.Extract == 1)
            {
                packet->writeInt32(it->second.Item[0].itemId);
                packet->writeInt8(it->second.Item[0].itemClass);
            }
            else
            {
                packet->writeInt32(0);
                packet->writeInt8(0);
            }
            ++nSize;
        }
    }
    char* Buffer = Answer::NetPacket::getBuffer(packet);
    *(int32_t*)Buffer = nSize;
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v19 = Player::getConnId(m_pPlayer);
    GameService* v20 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v20, v19, GateIndex, packet);
}

void CPlayerMail::SendMailContent(int32_t MailId)
{
    if (!m_pPlayer)
        return;
    auto it = m_MailInfo.find(MailId);
    if (it == m_MailInfo.end())
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v3 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v3, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CAD);
    if (!packet)
        return;
    packet->writeInt32(it->second.MailId);
    packet->writeUTF8(&it->second.MailContent);
    packet->writeInt8(it->second.Extract);
    if (it->second.Extract == 1 || it->second.Extract == 2)
    {
        int8_t nCount = 0;
        uint32_t nOldOffset = Answer::NetPacket::getWOffset(packet);
        packet->writeInt8(nCount);
        for (int8_t i = 0; i <= 5; ++i)
        {
            if (it->second.Item[i].itemId > 0 && it->second.Item[i].itemCount > 0)
            {
                packet->writeInt8(i);
                packet->writeInt32(it->second.Item[i].itemId);
                packet->writeInt8(it->second.Item[i].itemClass);
                packet->writeInt32(it->second.Item[i].itemCount);
                packet->writeInt8(it->second.Item[i].bind);
                packet->writeInt32(it->second.Item[i].endTime);
                packet->writeInt64(it->second.Item[i].srcId);
                ++nCount;
            }
        }
        uint32_t nNewOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setWOffset(packet, nOldOffset);
        packet->writeInt8(nCount);
        Answer::NetPacket::setWOffset(packet, nNewOffset);
    }
    std::string p_Param = it->second.Param;
    PackParam(&p_Param, packet);
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v20 = Player::getConnId(m_pPlayer);
    GameService* v21 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v21, v20, GateIndex, packet);
}

void CPlayerMail::PackParam(std::string* p_Param, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return;
    if (p_Param->empty())
    {
        inPacket->writeInt32(0);
    }
    else
    {
        StringVector vStr;
        Answer::StringUtility::split(&vStr, p_Param, "|", 0);
        int32_t nSize = vStr.size();
        inPacket->writeInt32(nSize);
        for (auto it = vStr.begin(); it != vStr.end(); ++it)
            inPacket->writeUTF8(&(*it));
    }
}

void CPlayerMail::SendNewMail()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CAE);
    if (packet)
    {
        packet->writeInt8(0);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        int16_t GateIndex = Player::getGateIndex(m_pPlayer);
        int8_t v5 = Player::getConnId(m_pPlayer);
        GameService* v6 = Answer::Singleton<GameService>::instance();
        GameService::sendPacketTo(v6, v5, GateIndex, packet);
    }
}
