#include "Other/CGMBackstate.h"

#include "Game/GameService.h"
#include "Game/CTimer.h"
#include "Config/CfgData.h"
#include "Database/DBService.h"
#include "Network/NetPacket.h"
#include "Utility/Answer.h"

CGMBackstate::CGMBackstate()
    : m_nLastTick(0)
{
}

CGMBackstate::~CGMBackstate()
{
}

void CGMBackstate::Init(int32_t line)
{
    if (line == 9)
        return;

    Answer::DBPool *pool = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(pool);
    char szSQL[4096] = {0};
    int32_t v3 = Answer::DayTime::now();
    snprintf(szSQL, sizeof(szSQL), "SELECT * FROM `gmt_ban_chat` WHERE `expire_time`>%d", v3);
    Answer::MySqlQuery result(db.query(szSQL));
    while (!result.eof())
    {
        GMBanChat chat;
        chat.nCharId = result.getInt64Value("cid", 0);
        chat.nExpireTime = result.getIntValue("expire_time", 0);
        m_mBanChat[chat.nCharId] = chat;
        result.nextRow();
    }
}

void CGMBackstate::OnUpdate()
{
    CTimer *v1 = CTimer::instance();
    int64_t curTick = v1->GetTick();
    if (curTick - m_nLastTick > 999)
    {
        m_nLastTick = curTick;
        checkBroadcast();
    }
}

void CGMBackstate::checkBroadcast()
{
    if (GameService::instance()->getLine() != 1)
        return;

    int32_t nowTime = CTimer::instance()->GetNow();
    for (auto iter = m_mBroadcasts.begin(); iter != m_mBroadcasts.end(); ++iter)
    {
        GMBroadcast *broad = &iter->second;
        if (iter->second.nStartTime <= nowTime
            && broad->nCount < broad->nTimes
            && nowTime - broad->nLastTime >= broad->nInterval)
        {
            sendGMBroadcast(&broad->strText, broad->nType);
            ++broad->nCount;
            broad->nLastTime = nowTime;
        }
    }
}

void CGMBackstate::sendGMBroadcast(std::string *text, int8_t type)
{
    Answer::NetPacket *packet = GameService::instance()->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2CD5);
    if (packet)
    {
        packet->writeUTF8(*text);
        packet->writeInt8(type);
        uint32_t WOffset = packet->getWOffset();
        packet->setSize(WOffset);
        GameService::instance()->worldBroadcast(packet);
    }
}

int32_t CGMBackstate::GetBanChatLeftTime(CharId_t cid)
{
    Answer::MutexGuard lock(&m_mutex);
    auto iter = m_mBanChat.find(cid);
    if (iter == m_mBanChat.end())
        return -1;

    int32_t nowTime = CTimer::instance()->GetNow();
    if (iter->second.nExpireTime <= nowTime)
        return -1;

    return iter->second.nExpireTime - nowTime;
}

int32_t CGMBackstate::GetSealLeftTime(CharId_t cid)
{
    Answer::MutexGuard lock(&m_mutex);
    auto iter = m_mSeal.find(cid);
    if (iter == m_mSeal.end())
        return -1;

    int32_t nowTime = CTimer::instance()->GetNow();
    if (iter->second.nExpireTime <= nowTime)
        return -1;

    return iter->second.nExpireTime - nowTime;
}

void CGMBackstate::onUpdateGMBroadcast(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return;

    Answer::MutexGuard lock(&m_mutex);
    int32_t count = inPacket->readInt32();
    for (int32_t i = 0; i < count; ++i)
    {
        int32_t nId = inPacket->readInt32();
        auto iter = m_mBroadcasts.find(nId);
        if (iter != m_mBroadcasts.end())
        {
            std::string strText;
            inPacket->readUTF8(strText);
            iter->second.strText = strText;
            iter->second.nType = inPacket->readInt8();
            iter->second.nStartTime = inPacket->readInt32();
            iter->second.nInterval = inPacket->readInt32();
            iter->second.nTimes = inPacket->readInt32();
        }
        else
        {
            std::string strText;
            inPacket->readUTF8(strText);
            int8_t nType = inPacket->readInt8();
            int32_t nStartTime = inPacket->readInt32();
            int32_t nInterval = inPacket->readInt32();
            int32_t nTimes = inPacket->readInt32();
            GMBroadcast broad(nId, strText, nType, nStartTime, nInterval, nTimes);
            m_mBroadcasts[nId] = broad;
        }
    }
}

void CGMBackstate::onUpdateGMBanChat(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return;

    Answer::MutexGuard lock(&m_mutex);
    int32_t isize = inPacket->readInt32();
    for (int32_t i = 0; i < isize; ++i)
    {
        CharId_t nCharId = inPacket->readInt64();
        auto iter = m_mBanChat.find(nCharId);
        if (iter != m_mBanChat.end())
        {
            iter->second.nExpireTime = inPacket->readInt32();
        }
        else
        {
            GMBanChat gmbanchat;
            gmbanchat.nCharId = nCharId;
            gmbanchat.nExpireTime = inPacket->readInt32();
            m_mBanChat[nCharId] = gmbanchat;
        }
    }
}

void CGMBackstate::onUpdateGMSeal(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return;

    Answer::MutexGuard lock(&m_mutex);
    int32_t isize = inPacket->readInt32();
    for (int32_t i = 0; i < isize; ++i)
    {
        CharId_t nCharId = inPacket->readInt64();
        int32_t nExpireTime = inPacket->readInt32();
        auto iter = m_mSeal.find(nCharId);
        if (iter != m_mSeal.end())
        {
            iter->second.nExpireTime = nExpireTime;
        }
        else
        {
            GMSeal gmseal;
            gmseal.nCharId = nCharId;
            gmseal.nExpireTime = nExpireTime;
            m_mSeal[nCharId] = gmseal;
        }

        int32_t Now = CTimer::instance()->GetNow();
        if (Now < nExpireTime)
        {
            GameService::instance()->KickUser(nCharId, 4);
        }
    }
}
