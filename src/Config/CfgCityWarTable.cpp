#include "Config/CfgCityWarTable.h"

#include <cstring>
#include <algorithm>

void CfgCityWarTable::AddContReward(CfgCityWarTable *const this, const CfgCityWarContReward *const stu)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // void __cdecl CfgCityWarTable::AddContReward(CfgCityWarTable *const this, const C
    // {
    // std::list<CfgCityWarContReward>::push_back(&this->m_lContReward, stu);
    // }
}

void CfgCityWarTable::GetContReward(const CfgCityWarTable *const this, int32_t id)
{
    // TODO: 从反编译代码还原实现
    // 反编译代码片段:
    // const CfgCityWarContReward *__cdecl CfgCityWarTable::GetContReward(const CfgCity
    // {
    // std::_List_const_iterator<CfgCityWarContReward> iter; // [rsp+10h] [rbp-20h] BYR
    // std::_List_const_iterator<CfgCityWarContReward> __x; // [rsp+20h] [rbp-10h] BYRE
    // for ( iter._M_node = std::list<CfgCityWarContReward>::begin(&this->m_lContReward
    // ;
    // std::_List_const_iterator<CfgCityWarContReward>::operator++(&iter) )
    // {
    // __x._M_node = std::list<CfgCityWarContReward>::end(&this->m_lContReward)._M_node
}
