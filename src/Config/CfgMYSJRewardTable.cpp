#include "Config/CfgMYSJRewardTable.h"

#include <cstring>
#include <algorithm>

CfgMYSJRewardTable::CfgMYSJRewardTable(CfgMYSJRewardTable *const this)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // void __cdecl CfgMYSJRewardTable::CfgMYSJRewardTable(CfgMYSJRewardTable *const th
    // {
    // std::map<int,std::list<CfgMYSJReward>>::map(&this->m_mReward);
    // std::map<int,std::list<CfgMYSJReward>>::map(&this->m_mGroupReward);
    // std::map<int,int>::map(&this->m_mDungeonGroup);
    // }
}

bool CfgMYSJRewardTable::Add(CfgMYSJRewardTable *const this, int32_t nId, const CfgMYSJReward *const reward)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // bool __cdecl CfgMYSJRewardTable::Add(CfgMYSJRewardTable *const this, int32_t nId
    // {
    // std::list<CfgMYSJReward> *v3; // rax
    // std::list<CfgMYSJReward> *v4; // rax
    // int32_t nIda; // [rsp+14h] [rbp-Ch] BYREF
    // CfgMYSJRewardTable *thisa; // [rsp+18h] [rbp-8h]
    // thisa = this;
    // nIda = nId;
    // v3 = std::map<int,std::list<CfgMYSJReward>>::operator[](&this->m_mReward, &nIda)
}

CfgMYSJRewardTable::~CfgMYSJRewardTable()
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // void __cdecl CfgMYSJRewardTable::~CfgMYSJRewardTable(CfgMYSJRewardTable *const t
    // {
    // std::map<int,int>::~map(&this->m_mDungeonGroup);
    // std::map<int,std::list<CfgMYSJReward>>::~map(&this->m_mGroupReward);
    // std::map<int,std::list<CfgMYSJReward>>::~map(&this->m_mReward);
    // }
}

int32_t CfgMYSJRewardTable::GetGroupId(const CfgMYSJRewardTable *const this, int32_t nDungeonId)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // int32_t __cdecl CfgMYSJRewardTable::GetGroupId(const CfgMYSJRewardTable *const t
    // {
    // int32_t nDungeonIda; // [rsp+4h] [rbp-2Ch] BYREF
    // const CfgMYSJRewardTable *thisa; // [rsp+8h] [rbp-28h]
    // std::_Rb_tree_const_iterator<std::pair<const int,int> > findIter; // [rsp+10h] [
    // std::_Rb_tree_const_iterator<std::pair<const int,int> > __x; // [rsp+20h] [rbp-1
    // thisa = this;
    // nDungeonIda = nDungeonId;
    // findIter._M_node = std::map<int,int>::find(&this->m_mDungeonGroup, &nDungeonIda)
}
