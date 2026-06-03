#include "common.h"
#include "Game/CGuardPrivilege.h"
#include "Game/Player.h"
#include "Game/GameService.h"
#include "Game/DBService.h"
#include "Other/CfgData.h"
#include "Other/CfgGuardPrivilegeTable.h"
#include "Other/CExtOperateLimit.h"
#include "Other/CExtCharBag.h"
#include "Other/ItemChangeReason.h"
#include "Other/CExtSystemBase.h"
#include "Other/ShowIcon.h"
#include "Other/IconStateList.h"

#include <cstring>
#include <algorithm>
#include <string>

CGuardPrivilege::CGuardPrivilege()
    : m_pPlayer(nullptr)
    , m_nLastTick(0)
    , m_nState(0)
{
}

CGuardPrivilege::~CGuardPrivilege()
{
}

void CGuardPrivilege::OnUpdate(int64_t curTick)
{
    if (isOpen() && curTick - m_nLastTick > 999)
    {
        m_nLastTick = curTick;
        int8_t nState = getTimeState();
        if (m_nState != nState)
        {
            m_nState = nState;
            sendIconState();
        }
    }
}

void CGuardPrivilege::GetInterestsProtocol(ProcIdList *procList)
{
    procList->push_back(980);
    procList->push_back(981);
    procList->push_back(20259);
}

int32_t CGuardPrivilege::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 2;

    if (!isOpen())
        return 2;

    switch (nProcId)
    {
        case 0x3D5:
            return onGetReward(inPacket);
        case 0x4F23:
            return onDBGetReward(inPacket);
        case 0x3D4:
            return onRequestInfo(inPacket);
    }
    return 0;
}

int32_t CGuardPrivilege::onRequestInfo(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    sendInfo();
    return 0;
}

int32_t CGuardPrivilege::onGetReward(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int8_t nIndex = Answer::NetPacket::readInt8(inPacket);
    CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgGuardPrivilegeTable *GuardPrivilegeTable = CfgData::GetGuardPrivilegeTable(pCfgData);
    const CfgGuardPrivilege *pReward = CfgGuardPrivilegeTable::GetGuardPrivilege(GuardPrivilegeTable, nIndex);

    if (!pReward)
        return 10002;

    int32_t nRecord = 1109;
    if (pReward->nDaily == 1)
        nRecord = 2053;

    CExtOperateLimit *OperateLimit = Player::GetOperateLimit(m_pPlayer);
    int32_t nOldRecord = CExtOperateLimit::GetLimitCount(OperateLimit, nRecord);

    if (nOldRecord == (nOldRecord | (1 << nIndex)))
        return 10002;

    int32_t param = pReward->nParam;
    int8_t type = pReward->nType;
    int32_t Sid = Player::getSid(m_pPlayer);
    int64_t Uid = Player::getUid(m_pPlayer);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);

    DBService *pDBService = Answer::Singleton<DBService>::instance();
    DBService::CheckGuardPrivilege(pDBService, ConnId, GateIndex, Uid, Sid, type, param, nIndex);
    return 0;
}

int32_t CGuardPrivilege::onDBGetReward(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int8_t nIndex = Answer::NetPacket::readInt8(inPacket);
    if (!Answer::NetPacket::readInt8(inPacket))
        return 2;

    CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgGuardPrivilegeTable *GuardPrivilegeTable = CfgData::GetGuardPrivilegeTable(pCfgData);
    const CfgGuardPrivilege *pReward = CfgGuardPrivilegeTable::GetGuardPrivilege(GuardPrivilegeTable, nIndex);

    if (!pReward)
        return 10002;

    int32_t nRecord = 1109;
    if (pReward->nDaily == 1)
        nRecord = 2053;

    CExtOperateLimit *OperateLimit = Player::GetOperateLimit(m_pPlayer);
    int32_t nOldRecord = CExtOperateLimit::GetLimitCount(OperateLimit, nRecord);
    int32_t nNewRecord = nOldRecord | (1 << nIndex);

    if (nOldRecord == nNewRecord)
        return 10002;

    CExtCharBag *Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &pReward->vReward, ITEM_CHANGE_REASON::ICR_GUARD_PRIVILEGE_REWARD))
        return 10002;

    CExtOperateLimit *pOperateLimit = Player::GetOperateLimit(m_pPlayer);
    CExtOperateLimit::UpdateLimitCount(pOperateLimit, nRecord, nNewRecord);

    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGameService = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(pGameService, ConnId, GateIndex, 0x3D5, nIndex);
    return 0;
}

void CGuardPrivilege::sendInfo()
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGameService = Answer::Singleton<GameService>::instance();

    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, ConnId, Answer::PackType::PACK_DISPATCH, 0x2F2B);
    if (!packet)
        return;

    CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgGuardPrivilegeTable *GuardPrivilegeTable = CfgData::GetGuardPrivilegeTable(pCfgData);
    const std::map<int8_t, CfgGuardPrivilege> &mGuardPrivilege = CfgGuardPrivilegeTable::GetGuardPrivilegeMap(GuardPrivilegeTable);

    CExtOperateLimit *OperateLimit = Player::GetOperateLimit(m_pPlayer);
    int32_t nRewardRecord = CExtOperateLimit::GetLimitCount(OperateLimit, 1109);
    CExtOperateLimit *v6 = Player::GetOperateLimit(m_pPlayer);
    int32_t nDailyRecord = CExtOperateLimit::GetLimitCount(v6, 2053);

    Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(mGuardPrivilege.size()));

    CExtOperateLimit *v8 = Player::GetOperateLimit(m_pPlayer);
    int32_t nRechargeSumRecord = CExtOperateLimit::GetLimitCount(v8, 1104);
    (void)nRechargeSumRecord;

    for (auto iter = mGuardPrivilege.begin(); iter != mGuardPrivilege.end(); ++iter)
    {
        const CfgGuardPrivilege &guard = iter->second;
        Answer::NetPacket::writeInt8(packet, guard.nIndex);
        if (guard.nDaily == 1)
            Answer::NetPacket::writeInt8(packet, (nDailyRecord >> guard.nIndex) & 1);
        else
            Answer::NetPacket::writeInt8(packet, (nRewardRecord >> guard.nIndex) & 1);
    }

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnIdSend = Player::getConnId(m_pPlayer);
    GameService *v12 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v12, ConnIdSend, GateIndex, packet);
}

bool CGuardPrivilege::isOpen()
{
    if (!m_pPlayer)
        return false;

    std::string platform;
    Player::GetPlatform(m_pPlayer, &platform);
    return platform == "w360";
}

int8_t CGuardPrivilege::getTimeState()
{
    if (!m_pPlayer)
        return 0;

    int32_t nNowTime = Unit::getNow(m_pPlayer);
    CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgGuardPrivilegeTable *table = CfgData::GetGuardPrivilegeTable(pCfgData);

    if (CfgGuardPrivilegeTable::GetStartTime(table) > nNowTime)
        return 0;

    if (CfgGuardPrivilegeTable::GetEndTime(table) >= nNowTime)
        return 2;

    return 4;
}

void CGuardPrivilege::GetIconState(IconStateList *iconList)
{
    if (isOpen() && getTimeState() == 2)
    {
        ShowIcon icon;
        icon.nId = 62;
        icon.nState = 2;
        icon.nLeftTime = 0;
        icon.nIconRight = 0;
        iconList->push_back(icon);
    }
}

void CGuardPrivilege::getIconState(ShowIcon *retstr)
{
    retstr->nId = 62;
    retstr->nState = getTimeState();
    retstr->nLeftTime = 0;
    retstr->nIconRight = 0;
}

void CGuardPrivilege::sendIconState()
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGameService = Answer::Singleton<GameService>::instance();

    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, ConnId, Answer::PackType::PACK_DISPATCH, 0x2EB);
    if (!packet)
        return;

    ShowIcon stu;
    stu.nId = 62;
    stu.nState = m_nState;
    stu.nLeftTime = 0;
    stu.nIconRight = 0;

    Answer::NetPacket::writeInt32(packet, stu.nId);
    Answer::NetPacket::writeInt8(packet, stu.nState);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v5 = Player::getConnId(m_pPlayer);
    GameService *v6 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v6, v5, GateIndex, packet);
}
