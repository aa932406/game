#include "common.h"
#include "Other/COutLinkFestival.h"

#include "Game/GameService.h"
#include "Game/CTimer.h"
#include "Config/CfgData.h"
#include "Config/CfgOutLinkFestival.h"
#include "Utility/Answer.h"
#include "Utility/ShowIcon.h"

COutLinkFestival::COutLinkFestival()
{
}

COutLinkFestival::~COutLinkFestival()
{
}

void COutLinkFestival::OnNewMinute(int32_t nMinute)
{
    if (nMinute % 60)
        return;

    if (GameService::instance()->getLine() == 1)
    {
        checkIconState();
    }
    else
    {
        CTimer::instance()->GetNow();
        int32_t OutLinkFestivalTable = CfgData::instance()->GetOutLinkFestivalTable();
        CfgOutLinkFestivalTable v6(OutLinkFestivalTable);
        m_lOutLinkIcons = v6.m_lFestival;
    }
}

void COutLinkFestival::Init(int32_t line)
{
    Answer::DayTime::now();
    int32_t OutLinkFestivalTable = CfgData::instance()->GetOutLinkFestivalTable();
    CfgOutLinkFestivalTable v4(OutLinkFestivalTable);
    m_lOutLinkIcons = v4.m_lFestival;
}

void COutLinkFestival::GetIconState(IconStateList *IconList)
{
    int32_t nNowTime = CTimer::instance()->GetNow();
    Answer::MutexGuard lock(&m_lock);

    int32_t OutLinkFestivalTable = CfgData::instance()->GetOutLinkFestivalTable();
    CfgOutLinkFestivalTable v9(OutLinkFestivalTable);
    m_lOutLinkIcons = v9.m_lFestival;

    if (!m_lOutLinkIcons.empty())
    {
        for (auto iter = m_lOutLinkIcons.begin(); iter != m_lOutLinkIcons.end(); ++iter)
        {
            ShowIcon stu;
            memset(&stu, 0, sizeof(stu));
            stu.nId = iter->nIcon;
            stu.nState = 2;
            stu.nLeftTime = iter->nEndTime - nNowTime;
            IconList->push_back(stu);
        }
    }
}

void COutLinkFestival::checkIconState()
{
    int32_t nNowTime = CTimer::instance()->GetNow();
    int32_t OutLinkFestivalTable = CfgData::instance()->GetOutLinkFestivalTable();
    CfgOutLinkFestivalTable lShowIcons(OutLinkFestivalTable);
    CfgOutLinkFestivalList lAdd;
    CfgOutLinkFestivalList lHide;

    Answer::MutexGuard lock(&m_lock);

    for (auto iter = lShowIcons.m_lFestival.begin(); iter != lShowIcons.m_lFestival.end(); ++iter)
    {
        int32_t nIndex = iter->nIndex;
        if (!isInIconList(&m_lOutLinkIcons, nIndex))
        {
            lAdd.push_back(*iter);
        }
    }

    for (auto iter_0 = m_lOutLinkIcons.begin(); iter_0 != m_lOutLinkIcons.end(); ++iter_0)
    {
        int32_t nIndex = iter_0->nIndex;
        if (!isInIconList(&lShowIcons.m_lFestival, nIndex))
        {
            lHide.push_back(*iter_0);
        }
    }

    m_lOutLinkIcons = lShowIcons.m_lFestival;
    lock.~MutexGuard();

    if (!lAdd.empty())
    {
        for (auto iter_1 = lAdd.begin(); iter_1 != lAdd.end(); ++iter_1)
        {
            ShowIcon icon;
            memset(&icon, 0, sizeof(icon));
            icon.nId = iter_1->nIcon;
            icon.nState = 2;
            icon.nLeftTime = iter_1->nEndTime - nNowTime;

            Answer::NetPacket *packet = GameService::instance()->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2CC3);
            if (packet)
            {
                ShowIcon::AppendInfo(&icon, packet);
                uint32_t WOffset = packet->getWOffset();
                packet->setSize(WOffset);
                GameService::instance()->worldBroadcast(packet);
            }
        }
    }

    if (!lHide.empty())
    {
        for (auto iter_2 = lHide.begin(); iter_2 != lHide.end(); ++iter_2)
        {
            ShowIcon icon_0;
            memset(&icon_0, 0, sizeof(icon_0));
            icon_0.nId = iter_2->nIcon;
            icon_0.nState = 4;

            Answer::NetPacket *packet_0 = GameService::instance()->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2CC3);
            if (packet_0)
            {
                ShowIcon::AppendInfo(&icon_0, packet_0);
                uint32_t WOffset = packet_0->getWOffset();
                packet_0->setSize(WOffset);
                GameService::instance()->worldBroadcast(packet_0);
            }
        }
    }
}

bool COutLinkFestival::isInIconList(const CfgOutLinkFestivalList *lst, int32_t nIndex)
{
    for (auto iter = lst->begin(); iter != lst->end(); ++iter)
    {
        if (iter->nIndex == nIndex)
            return true;
    }
    return false;
}
