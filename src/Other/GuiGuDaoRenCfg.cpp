#include "Other/GuiGuDaoRenCfg.h"
#include "Common/CommonTypes.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

GuiGuDaoRenCfg::GuiGuDaoRenCfg()
    : nNpcId(0), nMaxCount(0)
{
}

GuiGuDaoRenCfg::~GuiGuDaoRenCfg()
{
}

GuiGuDaoRenCfg::GuiGuDaoRenCfg(const GuiGuDaoRenCfg& other)
    : nNpcId(other.nNpcId)
    , nMaxCount(other.nMaxCount)
    , lRefreshMonster(other.lRefreshMonster)
    , vItemData(other.vItemData)
    , vItem(other.vItem)
    , vMapId(other.vMapId)
{
}
