#include "Config/CfgDungeonDropGroup.h"

#include <cstring>
#include <algorithm>

CfgDungeonDropGroup::CfgDungeonDropGroup(CfgDungeonDropGroup *const this)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // void __cdecl CfgDungeonDropGroup::CfgDungeonDropGroup(CfgDungeonDropGroup *const
    // {
    // std::list<CfgDungeonDrop>::list(&this->m_lstDropItems);
    // CfgDungeonDropGroup::CleanUp(this);
    // }
}

void CfgDungeonDropGroup::CleanUp(CfgDungeonDropGroup *const this)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // void __cdecl CfgDungeonDropGroup::CleanUp(CfgDungeonDropGroup *const this)
    // {
    // this->m_nMaxWight = 0;
    // std::list<CfgDungeonDrop>::clear(&this->m_lstDropItems);
    // }
}

void CfgDungeonDropGroup::Add(CfgDungeonDropGroup *const this, CfgDungeonDrop *const drop)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // void __cdecl CfgDungeonDropGroup::Add(CfgDungeonDropGroup *const this, CfgDungeo
    // {
    // this->m_nMaxWight += drop->weight;
    // drop->weight = this->m_nMaxWight;
    // std::list<CfgDungeonDrop>::push_back(&this->m_lstDropItems, drop);
    // }
}

void CfgDungeonDropGroup::RandDrop(const CfgDungeonDropGroup *const this)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // const CfgDungeonDrop *__cdecl CfgDungeonDropGroup::RandDrop(const CfgDungeonDrop
    // {
    // int32_t m_nMaxWight; // ebx
    // Answer::Random *v2; // rax
    // const CfgDungeonDrop *v3; // rax
    // Answer::Random *v4; // rax
    // const CfgDungeonDrop *v5; // rax
    // std::_List_const_iterator<CfgDungeonDrop> iter; // [rsp+10h] [rbp-30h] BYREF
    // std::_List_const_iterator<CfgDungeonDrop> __x; // [rsp+20h] [rbp-20h] BYREF
    // int32_t nRand; // [rsp+28h] [rbp-18h]
}
