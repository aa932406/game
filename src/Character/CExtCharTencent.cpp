// CExtCharTencent.cpp
#include "CExtCharTencent.h"
#include <cstring>

CExtCharTencent::CExtCharTencent()
{
    OnCleanUp();
}

CExtCharTencent::~CExtCharTencent()
{
}

void CExtCharTencent::OnCleanUp()
{
    memset(&m_info, 0, sizeof(m_info));
}

void CExtCharTencent::OnDaySwitch(int32_t nDiffDays)
{
    resetQZoneDailyReward();
    resetQQGameDailyReward();
    SendYellowStoneIcon();
    SendQZoneIcon();
    SendQQGameIcon();
    SendBlueStoneIcon();
    SendSevenDayLoginIcon();
}

void CExtCharTencent::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t procs[] = {216, 218, 220, 219, 221, 223, 225, 224, 226, 227, 
                        228, 235, 236, 240, 237, 238, 239};
    for (int i = 0; i < 17; ++i)
    {
        procList->push_back(procs[i]);
    }
}

int32_t CExtCharTencent::onRequestYellowStoneInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetYellowNewerAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetYellowDailyAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetYellowYearAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetYellowLevelAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetQZoneGift(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetQQGameGift(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onRequestBlueStoneInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetBlueAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetBlueDailyAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetBlueYearAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetBlueHighAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetBlueLevelAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onReloadTencentInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onRequestSevenDayLoginInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetSevenDayLoginReward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onRequestTGPInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetTGPAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetTGPDailyAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTencent::onGetTGPLevelAward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

void CExtCharTencent::GetYellowStoneIcon(void* IconList)
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::SendYellowStoneIcon()
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::GetQZoneIcon(void* IconList)
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::SendQZoneIcon()
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::GetQQGameIcon(void* IconList)
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::SendQQGameIcon()
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::GetYellowNewerIcon(void* IconList)
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::SendYellowNewerIcon()
{
    if (!m_pPlayer) return;
}

int32_t CExtCharTencent::canGetYellowReward()
{
    return 0;
}

void CExtCharTencent::GetBlueStoneIcon(void* IconList)
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::SendBlueStoneIcon()
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::GetBluePayIcon(void* IconList)
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::SendBluePayIcon()
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::GetSevenDayLoginIcon(void* IconList)
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::SendSevenDayLoginIcon()
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::GetTGPIcon(void* IconList)
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::SendTGPIcon()
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::GetFriendIcon(void* IconList)
{
    if (!m_pPlayer) return;
}

void CExtCharTencent::AppendInfo(Answer::NetPacket* packet)
{
    if (!packet) return;
}

void CExtCharTencent::InitTencentInfo(const TencentInfo* info)
{
    if (info) m_info = *info;
}

void CExtCharTencent::UpdateTencentInfo(const TencentInfo* info)
{
    if (!m_pPlayer) return;
    if (info) m_info = *info;
    SendBlueStoneIcon();
    SendBluePayIcon();
}

int32_t CExtCharTencent::canGetBlueReward()
{
    return 0;
}

bool CExtCharTencent::needShowBluePayIcon()
{
    return false;
}

bool CExtCharTencent::isInSevenDay()
{
    return false;
}

bool CExtCharTencent::needShowSevenDayIcon()
{
    return false;
}

int32_t CExtCharTencent::getLoginDay()
{
    return 0;
}

void CExtCharTencent::resetQZoneDailyReward()
{
}

void CExtCharTencent::resetQQGameDailyReward()
{
}

bool CExtCharTencent::checkQQGiftCondition(int8_t nType, int32_t nCondition)
{
    return false;
}

bool CExtCharTencent::IsYellowVip()
{
    return m_info.m_bYellowVip != 0;
}

int32_t CExtCharTencent::GetYellowVipLevel()
{
    return m_info.m_nYellowVipLevel;
}

bool CExtCharTencent::IsYellowYearVip()
{
    return m_info.m_bYellowYearVip != 0;
}

bool CExtCharTencent::IsBlueVip()
{
    return m_info.m_bBlueVip != 0;
}

int32_t CExtCharTencent::GetBlueVipLevel()
{
    return m_info.m_nBlueVipLevel;
}

bool CExtCharTencent::IsBlueYearVip()
{
    return m_info.m_bBlueYearVip != 0;
}

bool CExtCharTencent::IsBlueHighVip()
{
    return m_info.m_bBlueHighVip != 0;
}

bool CExtCharTencent::isYellowEnter()
{
    return true;
}

bool CExtCharTencent::isBlueEnter()
{
    return true;
}

bool CExtCharTencent::isTGPEnter()
{
    return true;
}

void CExtCharTencent::OnUpdate(int64_t curTick)
{
}

void CExtCharTencent::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CExtCharTencent::OnSaveToDB(PlayerDBData* dbData)
{
}