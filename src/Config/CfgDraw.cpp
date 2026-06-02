#include "Config/CfgDraw.h"

#include <cstring>
#include <algorithm>

CfgDraw::CfgDraw(int8_t type)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // void __cdecl CfgDraw::CfgDraw(int8_t type)
    // {
    // this->m_nType = type;
    // this->m_nMaxRate = 0;
    // std::list<CfgDrawReward>::list(&this->m_rewards);
    // }
}

void CfgDraw::Add(const CfgDrawReward *const reward)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // void __cdecl CfgDraw::Add(const CfgDrawReward *const reward
    // {
    // std::list<CfgDrawReward>::push_back(&this->m_rewards, reward);
    // this->m_nMaxRate += reward->nRate;
    // }
}

CfgDraw::CfgDraw(const CfgDraw *const a2)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // void __cdecl CfgDraw::CfgDraw(const CfgDraw *const a2)
    // {
    // this->m_nType = a2->m_nType;
    // this->m_nMaxRate = a2->m_nMaxRate;
    // std::list<CfgDrawReward>::list(&this->m_rewards, &a2->m_rewards);
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
