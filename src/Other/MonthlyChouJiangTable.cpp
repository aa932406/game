#include "Other/MonthlyChouJiangTable.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

MonthlyChouJiangTable::MonthlyChouJiangTable()
{
}

MonthlyChouJiangTable::~MonthlyChouJiangTable()
{
}

void MonthlyChouJiangTable::AddMonthlyChouJiangCfg(MonthlyChouJiangCfg stu)
{
    m_MonthlyChouJiangCfgMap[stu.nIndx] = stu;
}
