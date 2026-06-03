#include "common.h"
#include "Other/CDaTiHD.h"
#include "Answer.h"
#include "GameService.h"
#include "CTimer.h"
#include "CfgData.h"
#include "CFestivalDoubleEleven.h"

CDaTiHD::CDaTiHD(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_DaTiIndex = 0;
    m_LastSendQuestions = 0;
    m_RandAnswerDaTiIndex = 0;
    m_IsSendReadyStartToCs = 0;
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    QuestionsVector questions;
    CfgData::GetAllQuestions(&questions, cfgData, 0);
    m_QuestionsVct = questions;
}

CDaTiHD::~CDaTiHD()
{
}

void CDaTiHD::reset()
{
    CActivity::reset();
    m_DaTiIndex = 0;
    m_LastSendQuestions = 0;
    m_RandAnswerDaTiIndex = 0;
}

void CDaTiHD::CheckActivity()
{
    if (!IsRightLine())
    {
        m_nState = ACTIVITY_STATE::AS_TIME_OUT;
    }
    else if (!CheckData())
    {
        m_nState = ACTIVITY_STATE::AS_NOT_START;
    }
    else
    {
        CTimer* timer = Answer::Singleton<CTimer>::instance();
        tm localnow;
        CTimer::GetLocalNow(&localnow, timer);
        bool bRightLine = IsRightLine();
        ACTIVITY_STATE nState = GetActivityStateByTime(&localnow);

        if (nState == ACTIVITY_STATE::AS_READY)
        {
            if (m_nState == ACTIVITY_STATE::AS_END || m_nState == ACTIVITY_STATE::AS_NOT_START)
            {
                m_nState = ACTIVITY_STATE::AS_READY;
                if (bRightLine)
                    BroadcastActivityState();
            }
            if (m_nState == ACTIVITY_STATE::AS_READY && bRightLine)
            {
                timer = Answer::Singleton<CTimer>::instance();
                tm localnow0;
                CTimer::GetLocalNow(&localnow0, timer);
                int32_t nowSeconds = 60 * localnow0.tm_min + 3600 * localnow0.tm_hour + localnow0.tm_sec;
                int32_t startSeconds = 60 * m_cfgActivity->start_hour[0];
                if (startSeconds - nowSeconds <= 100 && !m_IsSendReadyStartToCs)
                {
                    OnActivityReady();
                    SendReadyStartDaTiHD();
                    m_IsSendReadyStartToCs = 1;
                }
            }
        }
        else if (nState > ACTIVITY_STATE::AS_READY)
        {
            if (nState == ACTIVITY_STATE::AS_RUNNING)
            {
                if (m_nState == ACTIVITY_STATE::AS_READY)
                {
                    m_nState = ACTIVITY_STATE::AS_RUNNING;
                    if (bRightLine)
                    {
                        RandomQuestions();
                        SendStartDaTiHD();
                        BroadcastActivityState();
                        OnActivityStart();
                    }
                }
                else if (m_nState == ACTIVITY_STATE::AS_RUNNING && bRightLine)
                {
                    timer = Answer::Singleton<CTimer>::instance();
                    tm localnow0;
                    CTimer::GetLocalNow(&localnow0, timer);
                    int32_t nowSeconds0 = 60 * localnow0.tm_min + 3600 * localnow0.tm_hour + localnow0.tm_sec;
                    if (m_LastSendQuestions + 20 <= nowSeconds0)
                    {
                        ++m_DaTiIndex;
                        SendQuestionsInfo(nowSeconds0);
                        m_LastSendQuestions = nowSeconds0;
                    }
                }
            }
            else if (nState == ACTIVITY_STATE::AS_END)
            {
                if (m_nState == ACTIVITY_STATE::AS_RUNNING)
                {
                    m_nState = ACTIVITY_STATE::AS_END;
                    if (bRightLine)
                    {
                        SendEndDaTiHD();
                        BroadcastActivityState();
                    }
                    OnTimeEnd();
                }
                m_nState = ACTIVITY_STATE::AS_END;
            }
        }
        else if (nState == ACTIVITY_STATE::AS_NOT_START)
        {
            if (m_nState == ACTIVITY_STATE::AS_END)
            {
                m_nState = ACTIVITY_STATE::AS_NOT_START;
            }
            else if (m_nState == ACTIVITY_STATE::AS_RUNNING)
            {
                m_nState = ACTIVITY_STATE::AS_NOT_START;
                if (bRightLine)
                {
                    SendEndDaTiHD();
                    BroadcastActivityState();
                }
                OnTimeEnd();
            }
        }
    }
}

void CDaTiHD::RandAnswer()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_PROC, 0x4E47u);
    if (packet)
    {
        packet->writeInt8(0);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::sendPacket(gs2, 0, packet);
    }
}

void CDaTiHD::RandomQuestions()
{
    CFestivalDoubleEleven* festival = Answer::Singleton<CFestivalDoubleEleven>::instance();
    QuestionsVector questions;
    if (CFestivalDoubleEleven::IsInTime(festival, FESTIVAL_ACTIVITY_TYPE::FAT_DA_TI))
    {
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        CfgData::GetAllQuestions(&questions, cfgData, 1);
        m_QuestionsVct = questions;
    }
    else
    {
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        CfgData::GetAllQuestions(&questions, cfgData, 0);
        m_QuestionsVct = questions;
    }
    srand((unsigned int)time(0));
    std::random_shuffle(m_QuestionsVct.begin(), m_QuestionsVct.end());
}

void CDaTiHD::SendReadyStartDaTiHD()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_PROC, 0x4E43u);
    if (packet)
    {
        CTimer* timer = Answer::Singleton<CTimer>::instance();
        tm localnow;
        CTimer::GetLocalNow(&localnow, timer);
        int32_t nowSeconds = 60 * localnow.tm_min + 3600 * localnow.tm_hour + localnow.tm_sec;
        int32_t startSeconds = 60 * m_cfgActivity->start_hour[0];
        packet->writeInt32(nowSeconds);
        packet->writeInt32(startSeconds);
        packet->writeInt32(GetId());
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::sendPacket(gs2, 0, packet);
    }
}

void CDaTiHD::SendEndDaTiHD()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_PROC, 0x4E45u);
    if (packet)
    {
        packet->writeInt8(0);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::sendPacket(gs2, 0, packet);
    }
}

void CDaTiHD::SendStartDaTiHD()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_PROC, 0x4E44u);
    if (packet)
    {
        packet->writeInt8(0);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::sendPacket(gs2, 0, packet);
    }
}

void CDaTiHD::SendQuestionsInfo(int32_t nowSeconds)
{
    if (m_DaTiIndex <= 30 && (size_t)m_DaTiIndex <= m_QuestionsVct.size())
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_PROC, 0x4E46u);
        if (packet)
        {
            packet->writeInt8(m_DaTiIndex);
            packet->writeInt32(m_QuestionsVct[m_DaTiIndex - 1].QuestionId);
            packet->writeInt8(m_QuestionsVct[m_DaTiIndex - 1].AnswerId);
            packet->writeInt32(nowSeconds);
            packet->setSize(packet->getWOffset());
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacket(gs2, 0, packet);
        }
    }
}

void CDaTiHD::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(132);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CDaTiHD::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(133);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}
