import os

def fix_file(fname, stub_start_marker, proper_end_search, new_code):
    path = os.path.join('src/Other', fname)
    with open(path, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    
    stub_start = content.find(stub_start_marker)
    if stub_start < 0:
        print(f'{fname}: stub marker not found')
        return
    
    proper_end = content.rfind(proper_end_search, max(0, stub_start - 3000), stub_start)
    if proper_end < 0:
        print(f'{fname}: proper end not found')
        return
    
    proper_end = content.find('\n', proper_end) + 1
    if proper_end <= 0:
        proper_end = content.find('\n', proper_end)
    new_content = content[:proper_end] + new_code
    
    with open(path, 'w', encoding='utf-8') as f:
        f.write(new_content)
    print(f'{fname}: SUCCESS')

# 1. CShenWei.cpp
fix_file('CShenWei.cpp', 'CShenWei::GetShenWeiLevel()', '    return 0;\n}', """
void CShenWei::AddCharAttr()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (!CFunctionOpen::IsOpened(pFuncOpen, 188))
        return;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    int32_t nLevel = GetShenWeiLevel();
    if (nLevel <= 0)
        return;
    const CfgShenWeiTable* pTable = CfgData::GetShenWeiTable(pCfgData);
    const CfgShenWei* pCfg = CfgShenWeiTable::GetRow(pTable, nLevel);
    if (pCfg && pCfg->m_AttrId > 0 && pCfg->m_AttrValue > 0)
    {
        Player::AddCharAttrAddon(m_pPlayer, pCfg->m_AttrId, pCfg->m_AttrValue * nLevel);
    }
}

int32_t CShenWei::GetRankAttr(int64_t Exp)
{
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgShenWeiRankTable* pRankTable = CfgData::GetShenWeiRankTable(pCfgData);
    for (auto it = pRankTable->begin(); it != pRankTable->end(); ++it)
    {
        if (Exp >= it->second.m_NeedExp)
            return it->second.m_AttrValue;
    }
    return 0;
}

int32_t CShenWei::GetShenWeiLevel()
{
    if (!m_pPlayer)
        return 0;
    int64_t nCurrency = Player::GetCurrencyValue(m_pPlayer, CURRENCY_TYPE::CURRENCY_HONOR);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgShenWeiTable* pTable = CfgData::GetShenWeiTable(pCfgData);
    int32_t nLevel = 0;
    for (auto it = pTable->begin(); it != pTable->end(); ++it)
    {
        if (nCurrency >= it->second.m_NeedHonor)
            nLevel = it->first;
    }
    return nLevel;
}
""")

# 2. CQiFu.cpp
fix_file('CQiFu.cpp', 'CQiFu::OnQiFu()', '    return 0;\n}', """
int32_t CQiFu::OnQiFu(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;
    int32_t QiFuId = inPacket->readInt32();
    if (!CheckQiFuType(QiFuId))
        return 10002;
    CVip* pVip = Player::GetPlayerVip(m_pPlayer);
    if (CVip::GetVipLevel(pVip) <= 0)
        return 10002;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgQiFuTable* pTable = CfgData::GetQiFuTable(pCfgData);
    const CfgQiFu* pCfg = CfgQiFuTable::GetRow(pTable, QiFuId);
    if (!pCfg)
        return 10002;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (pCfg->m_CostStone > 0)
    {
        if (!CExtCharBag::RemoveItemByCfgId(pBag, pCfg->m_CostStone, pCfg->m_CostCount, ITEM_CHANGE_REASON::ICR_QIFU_COST))
            return 10002;
    }
    if (pCfg->m_CostGold > 0)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_CostGold, CURRENCY_CHANGE_REASON::GCR_QIFU_COST, QiFuId))
            return 10002;
    }
    int32_t nAddCount = pCfg->m_AddCount;
    int32_t nDouble = 0;
    if (Answer::Singleton<Answer::Random>::instance()->generate(1, 10000) <= pCfg->m_DoubleRate)
    {
        nDouble = 1;
        nAddCount *= pCfg->m_DoubleTimes;
    }
    Player::AddCurrency(m_pPlayer, (CURRENCY_TYPE)pCfg->m_CurrencyType, nAddCount, CURRENCY_CHANGE_REASON::GCR_QIFU_REWARD, 0);
    SendQiFuSuccess(QiFuId, nAddCount, nDouble);
    uint16_t Proc = inPacket->getProc();
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService::replySuccess(Answer::Singleton<GameService>::instance(), ConnId, GateIndex, Proc, 0);
    return 0;
}

void CQiFu::SendQiFuSuccess(int8_t Type, int32_t AddCount, int8_t Double)
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CC7);
    if (!packet)
        return;
    packet->writeInt8(Type);
    packet->writeInt32(AddCount);
    packet->writeInt8(Double);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
}

void CQiFu::SendQiFuInfo()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CC8);
    if (!packet)
        return;
    for (auto it = m_QiFu.begin(); it != m_QiFu.end(); ++it)
    {
        packet->writeInt32(it->first);
        packet->writeInt32(it->second);
    }
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
}

void CQiFu::GetQiFuStu(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 35))
    {
        ShowIcon stu;
        GetShowIconStu(&stu);
        IconList->push_back(stu);
    }
}

void CQiFu::SendQIFuIcon()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 35))
    {
        ShowIcon stu;
        GetShowIconStu(&stu);
        Player::SendIconState(m_pPlayer, &stu);
    }
}

void CQiFu::GetShowIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (!m_pPlayer)
        return;
    retstr->nId = 104;
    CVip* pVip = Player::GetPlayerVip(m_pPlayer);
    if (CVip::GetVipLevel(pVip) > 0)
        retstr->nState = 2;
    else
        retstr->nState = 4;
    retstr->nLeftTime = -1;
}
""")

# 3. CXingMai.cpp
fix_file('CXingMai.cpp', 'CXingMai::IsXingMaiSlotActived()', '    return 0;\n}', """
bool CXingMai::IsXingMaiSlotActived(int8_t nEquipSlot)
{
    if (!m_pPlayer)
        return false;
    int32_t XingMaiSlot = Player::getRecord(m_pPlayer, 1171);
    return (XingMaiSlot & (1 << (nEquipSlot - 1))) != 0;
}

void CXingMai::AddCharAttr()
{
    if (!m_pPlayer)
        return;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    uint16_t nCount = CfgData::GetCfgXingMaiCount(pCfgData);
    for (uint16_t i = 0; i < nCount; ++i)
    {
        CfgXingMai* pCfg = CfgData::GetCfgXingMai(pCfgData, i + 1);
        if (!pCfg)
            continue;
        int32_t nStarCount = Player::getRecord(m_pPlayer, 1170 + i + 1);
        if (nStarCount > 0 && pCfg->AttrId > 0 && pCfg->AttrValue > 0)
        {
            Player::AddCharAttrAddon(m_pPlayer, pCfg->AttrId, pCfg->AttrValue * nStarCount);
        }
    }
}

void CXingMai::activeSlot(int8_t nIndex)
{
    if (!m_pPlayer)
        return;
    CExtOperateLimit* pLimit = Player::GetOperateLimit(m_pPlayer);
    if (CExtOperateLimit::CheckLimit(pLimit, 67, 1))
    {
        int32_t XingMaiSlot = Player::getRecord(m_pPlayer, 1171);
        Player::updateRecord(m_pPlayer, 1171, XingMaiSlot | (1 << (nIndex - 1)));
    }
}

int32_t CXingMai::OnXingMaiLevelUp(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    uint16_t nCfgId = inPacket->readUInt16();
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    CfgXingMai* pCfg = CfgData::GetCfgXingMai(pCfgData, nCfgId);
    if (!pCfg)
        return 10002;
    int32_t nStar = Player::getRecord(m_pPlayer, 1170);
    if (nStar < pCfg->XingJi)
        return 10002;
    int32_t nIdx = 1170 + nCfgId;
    if (Player::getRecord(m_pPlayer, nIdx) >= pCfg->XingJiCfgId)
        return 10002;
    int32_t nCostId = pCfg->CostType;
    int32_t nCostCount = pCfg->CostCount;
    if (nCostId > 0 && nCostCount > 0)
    {
        CExtCharBag* pBag = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::RemoveItemByCfgId(pBag, nCostId, nCostCount, ITEM_CHANGE_REASON::ICR_XINGMAI_UPLEVEL))
            return 10002;
    }
    int32_t nCurStar = Player::getRecord(m_pPlayer, nIdx);
    Player::updateRecord(m_pPlayer, nIdx, nCurStar + 1);
    Player::RecalcAttr(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (packet)
    {
        packet->writeInt32(2);
        packet->writeUInt16(nCfgId);
        packet->writeInt32(nCurStar + 1);
        uint32_t WOffset = packet->getWOffset();
        packet->setSize(WOffset);
        GameService::sendPacket(pGame, packet);
    }
    return 0;
}

int32_t CXingMai::GetMaxBossScore()
{
    if (!m_pPlayer)
        return 0;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    int32_t nStar = Player::getRecord(m_pPlayer, 1170);
    return CfgData::GetMaxBossScore(pCfgData, nStar);
}

bool CXingMai::isSlotActived(int8_t nIndex)
{
    if (!m_pPlayer)
        return false;
    int32_t XingMaiSlot = Player::getRecord(m_pPlayer, 1171);
    return (XingMaiSlot & (1 << (nIndex - 1))) != 0;
}

int32_t CXingMai::OnXingMaiSlotActive(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int8_t nIndex = inPacket->readInt8();
    if (nIndex < 1 || nIndex > 8)
        return 10002;
    if (isSlotActived(nIndex))
        return 10002;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    int32_t nCost = CfgData::GetXingMaiSlotCost(pCfgData, nIndex);
    int32_t nBossScore = Player::getRecord(m_pPlayer, 1172);
    if (nBossScore < nCost)
        return 10002;
    Player::updateRecord(m_pPlayer, 1172, nBossScore - nCost);
    activeSlot(nIndex);
    Player::RecalcAttr(m_pPlayer);
    return 0;
}
""")

# 4. CSpecialEquip.cpp
fix_file('CSpecialEquip.cpp', 'CSpecialEquip::SendSpecialEquipIcon()', '    return 0;\n}', """
void CSpecialEquip::SendSpecialEquipIcon()
{
    if (!m_pPlayer)
        return;
    ShowIcon stu;
    GetSpecialEquipIcon(&stu);
    Player::SendIconState(m_pPlayer, &stu);
}

void CSpecialEquip::AddCharAttr()
{
    if (!m_pPlayer)
        return;
    int32_t nRecord = Player::getRecord(m_pPlayer, 1149);
    if (nRecord <= 0)
        return;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    for (int32_t i = 0; i < 30; ++i)
    {
        if (nRecord & (1 << i))
        {
            int32_t nDay = i + 1;
            const CfgSpecialEquipTable* pTable = CfgData::GetSpecialEquipTable(pCfgData);
            const CfgSpecialEquip* pCfg = CfgSpecialEquipTable::GetRow(pTable, nDay);
            if (pCfg && pCfg->m_AttrId > 0 && pCfg->m_AttrValue > 0)
            {
                Player::AddCharAttrAddon(m_pPlayer, pCfg->m_AttrId, pCfg->m_AttrValue);
            }
        }
    }
}

bool CSpecialEquip::CanActivation(int32_t nIndex)
{
    if (!m_pPlayer)
        return false;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgSpecialEquipTable* pTable = CfgData::GetSpecialEquipTable(pCfgData);
    const CfgSpecialEquip* pCfg = CfgSpecialEquipTable::GetRow(pTable, nIndex);
    if (!pCfg)
        return false;
    int32_t nDay = pCfg->m_NeedDay;
    if (nDay > 0)
    {
        int32_t nLoginDay = Player::getRecord(m_pPlayer, 1148);
        if (nLoginDay < nDay)
            return false;
    }
    CVip* pVip = Player::GetPlayerVip(m_pPlayer);
    if (CVip::GetVipLevel(pVip) < pCfg->m_NeedVip)
        return false;
    if (pCfg->m_CostGold > 0)
    {
        if (!Player::CheckCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_CostGold))
            return false;
    }
    return true;
}

void CSpecialEquip::GetSpecialEquipIcon(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 112))
    {
        ShowIcon stu;
        GetSpecialEquipIcon(&stu);
        IconList->push_back(stu);
    }
}

void CSpecialEquip::InitSkill()
{
    if (!m_pPlayer)
        return;
    int32_t nRecord = Player::getRecord(m_pPlayer, 1149);
    if (nRecord <= 0)
        return;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    for (int32_t i = 0; i < 30; ++i)
    {
        if (nRecord & (1 << i))
        {
            int32_t nDay = i + 1;
            const CfgSpecialEquipTable* pTable = CfgData::GetSpecialEquipTable(pCfgData);
            const CfgSpecialEquip* pCfg = CfgSpecialEquipTable::GetRow(pTable, nDay);
            if (pCfg && pCfg->m_SkillId > 0)
            {
                CExtSkillManager* pSkillMgr = Player::GetSkillMgr(m_pPlayer);
                CExtSkillManager::AddSkill(pSkillMgr, pCfg->m_SkillId, 0);
            }
        }
    }
}

int32_t CSpecialEquip::CanActivationCount()
{
    if (!m_pPlayer)
        return 0;
    int32_t nCount = 0;
    int32_t nRecord = Player::getRecord(m_pPlayer, 1149);
    for (int32_t i = 0; i < 30; ++i)
    {
        if (!(nRecord & (1 << i)))
        {
            if (CanActivation(i + 1))
                ++nCount;
        }
    }
    return nCount;
}

void CSpecialEquip::GetSpecialEquipIcon(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (!m_pPlayer)
        return;
    retstr->nId = 115;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 112) && CanActivationCount() > 0)
        retstr->nState = 2;
    else
        retstr->nState = 4;
    retstr->nEffects = 1;
}

void CSpecialEquip::SendActivationInfo()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D8D);
    if (!packet)
        return;
    int32_t nRecord = Player::getRecord(m_pPlayer, 1149);
    packet->writeInt32(nRecord);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
}

int32_t CSpecialEquip::OnActivation(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int32_t nIndex = inPacket->readInt32();
    if (nIndex < 1 || nIndex > 30)
        return 10002;
    if (IsActivation(nIndex))
        return 10002;
    if (!CanActivation(nIndex))
        return 10002;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgSpecialEquipTable* pTable = CfgData::GetSpecialEquipTable(pCfgData);
    const CfgSpecialEquip* pCfg = CfgSpecialEquipTable::GetRow(pTable, nIndex);
    if (!pCfg)
        return 10002;
    int32_t nRecord = Player::getRecord(m_pPlayer, 1149);
    Player::updateRecord(m_pPlayer, 1149, nRecord | (1 << (nIndex - 1)));
    if (pCfg->m_CostGold > 0)
    {
        Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_CostGold, CURRENCY_CHANGE_REASON::GCR_SPECIALEQUIP_ACTIVATE, nIndex);
    }
    SendActivationInfo();
    Player::RecalcAttr(m_pPlayer);
    return 0;
}
""")

# 5. LiBao.cpp - effect stub
fix_file('LiBao.cpp', 'LiBao::effect()', '    return true;\n}', """
int32_t LiBao::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (!launcher || !m_pPlayer || !count)
        return -1;
    CExtCharBag* pBag = Player::GetBag(launcher);
    int32_t nCount = *count;
    if (nCount <= 0)
        nCount = 1;
    for (auto it = m_gifts.begin(); it != m_gifts.end(); ++it)
    {
        MemChrBag item;
        memset(&item, 0, sizeof(item));
        item.itemId = it->item;
        item.itemClass = 4;
        item.itemCount = it->count * nCount;
        item.bind = it->bind;
        CExtCharBag::AddItem(pBag, &item, ITEM_CHANGE_REASON::ICR_BAG_USE);
    }
    return 0;
}
""")

# 6. GoldEgg.cpp - effect stub
fix_file('GoldEgg.cpp', 'GoldEgg::effect()', '    return m_pCfgItem != nullptr;\n}', """
int32_t GoldEgg::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (!launcher || !m_pPlayer || !count || !m_pCfgItem)
        return -1;
    CExtCharBag* pBag = Player::GetBag(launcher);
    int32_t nCount = *count;
    if (nCount <= 0)
        nCount = 1;
    Answer::Random* pRand = Answer::Singleton<Answer::Random>::instance();
    for (auto it = m_pCfgItem->vItems.begin(); it != m_pCfgItem->vItems.end(); ++it)
    {
        int32_t nRand = pRand->generate(1, 10000);
        if (nRand <= it->nRate)
        {
            MemChrBag item;
            memset(&item, 0, sizeof(item));
            item.itemId = it->itemId;
            item.itemClass = it->itemClass;
            item.itemCount = it->itemCount * nCount;
            item.bind = it->bind;
            CExtCharBag::AddItem(pBag, &item, ITEM_CHANGE_REASON::ICR_BAG_USE);
        }
    }
    return 0;
}
""")

# 7. OreItem.cpp - effect stub
fix_file('OreItem.cpp', 'OreItem::effect()', '    return m_Exp > 0;\n}', """
int32_t OreItem::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (!launcher || !m_pPlayer || !count)
        return -1;
    int32_t nCount = *count;
    if (nCount <= 0)
        nCount = 1;
    CExtCharBag* pBag = Player::GetBag(launcher);
    CExtCharBag::AddExpOre(pBag, m_Exp * nCount);
    return 0;
}
""")

# 8. VipDrop.cpp - effect stub
fix_file('VipDrop.cpp', 'VipDrop::effect()', '    return m_Type > 0;\n}', """
int32_t VipDrop::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (!launcher || !m_pPlayer || !count)
        return -1;
    int32_t nCount = *count;
    if (nCount <= 0)
        nCount = 1;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgVipDropTable* pTable = CfgData::GetVipDropTable(pCfgData);
    const CfgVipDrop* pCfg = CfgVipDropTable::GetRow(pTable, m_Type);
    if (!pCfg)
        return -1;
    CExtCharBag* pBag = Player::GetBag(launcher);
    Answer::Random* pRand = Answer::Singleton<Answer::Random>::instance();
    for (auto it = pCfg->vItems.begin(); it != pCfg->vItems.end(); ++it)
    {
        int32_t nRand = pRand->generate(1, 10000);
        if (nRand <= it->nRate)
        {
            MemChrBag item;
            memset(&item, 0, sizeof(item));
            item.itemId = it->itemId;
            item.itemClass = it->itemClass;
            item.itemCount = it->itemCount * nCount;
            item.bind = it->bind;
            CExtCharBag::AddItem(pBag, &item, ITEM_CHANGE_REASON::ICR_BAG_USE);
        }
    }
    return 0;
}
""")

# 9. SuiJiLiBao.cpp - broadcast stub
fix_file('SuiJiLiBao.cpp', 'SuiJiLiBao::broadcast()', '    return true;\n}', """
void SuiJiLiBao::broadcast()
{
    if (!m_pPlayer)
        return;
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (!packet)
        return;
    packet->writeInt32(1);
    for (auto it = m_gifts.begin(); it != m_gifts.end(); ++it)
    {
        packet->writeInt32(it->item);
        packet->writeInt8(it->type);
        packet->writeInt32(it->count);
    }
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    GameService::worldBroadcast(pGame, packet);
}
""")
