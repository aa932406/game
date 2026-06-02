#include "Config/CfgDraw.h"

#include <cstring>
#include <algorithm>

void CfgDraw::Add(CfgDrawReward * reward)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // void __cdecl CfgDraw::Add(CfgDrawReward * const reward
    // {
    // std::list<CfgDrawReward>::push_back(&this->m_rewards, reward);
    // this->m_nMaxRate += reward->nRate;
    // }
}


CfgDraw::CfgDraw(const CfgDraw& a2)
{
    m_nType = a2.m_nType;
    m_nMaxRate = a2.m_nMaxRate;
    m_rewards = a2.m_rewards;
}

CfgDraw& CfgDraw::operator=(const CfgDraw& a2)
{
    if (this != &a2)
    {
        m_nType = a2.m_nType;
        m_nMaxRate = a2.m_nMaxRate;
        m_rewards = a2.m_rewards;
    }
    return *this;
}
