#include "Other/FaBaoTable.h"
#include "Config/CfgFaBao.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

FaBaoTable::FaBaoTable()
{
}

FaBaoTable::~FaBaoTable()
{
}

void FaBaoTable::AddFaBao(int8_t Type, CfgFaBao * Stu)
{
    if (Type >= 0 && Type < 4)
    {
        m_CfgFaBao[Type][Stu->FaBaoLevel] = *Stu;
    }
}

const CfgFaBao* FaBaoTable::GetFaBaoCfg(int8_t Type, int32_t Level)
{
    if (Type < 0 || Type >= 4)
    {
        return nullptr;
    }
    auto it = m_CfgFaBao[Type].find(Level);
    if (it != m_CfgFaBao[Type].end())
    {
        return &it->second;
    }
    return nullptr;
}
