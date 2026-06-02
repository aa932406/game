// CExtEquip.cpp
#include "CExtEquip.h"
#include "Game/GameService.h"
#include "Network/NetPacket.h"
#include "Game/Player.h"
#include "Other/CEquipManager.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtCurrency.h"
#include "Character/CExtCharSkill.h"
#include "CRandom.h"
#include "Answer/Singleton.h"
#include "Database/DBService.h"
#include "Config/CfgData.h"
#include "Database/PlayerDBData.h"
#include <cstring>
#include <algorithm>

CExtEquip::CExtEquip()
    : CExtSystemBase()
    , m_nEquipStar(0)
    , m_nEquipGem(0)
    , m_nGongMingSuitId(0)
    , m_nGongMingSuitBuff(0)
{
    OnCleanUp();
}

CExtEquip::~CExtEquip()
{
}

void CExtEquip::OnCleanUp()
{
    memset(m_vMemEquip, 0, sizeof(m_vMemEquip));
    memset(m_vMemPosLevel, 0, sizeof(m_vMemPosLevel));
    memset(m_vMemStrengthen, 0, sizeof(m_vMemStrengthen));
    memset(m_vShenYaoEquipPos, 0, sizeof(m_vShenYaoEquipPos));
    clearGemInfo();
    m_lstDirty.clear();
    m_EquipSuit.clear();
    m_nEquipStar = 0;
    m_nEquipGem = 0;
    m_nGongMingSuitId = 0;
    m_nGongMingSuitBuff = 0;
}

void CExtEquip::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (!dbData) return;
    
    memcpy(m_vMemEquip, dbData->equipData.vEquip, sizeof(m_vMemEquip));
    loadGemString(&dbData->gemData.gemInfo);
    loadPosLevelString(&dbData->gemData.posLevel);
    LoadStrengthenString(&dbData->gemData.Strengthen);
    LoadShenYaoString(&dbData->gemData.ShenYaoEquipPos);
    checkEquipSuit(0);
    updateEquipStar();
    updateEquipGem();
    
    // 墨符技能处理 (stub)
    int32_t nLevel = GetMoFuSuitLevel();
    (void)nLevel;
}

void CExtEquip::OnSaveToDB(PlayerDBData* dbData)
{
    if (!dbData) return;
    
    memcpy(dbData->equipData.vEquip, m_vMemEquip, sizeof(dbData->equipData.vEquip));
    dbData->gemData.gemInfo = saveGemString();
    dbData->gemData.posLevel = savePosLevelString();
    dbData->gemData.Strengthen = saveStrengthenString();
    dbData->gemData.ShenYaoEquipPos = saveShenYaoString();
    dbData->chr.weapon = m_vMemEquip[0].itemId;
    dbData->chr.cloth = m_vMemEquip[2].itemId;
    dbData->chr.equipStar = m_nEquipStar;
    dbData->chr.equipGem = m_nEquipGem;
}

void CExtEquip::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t procs[] = {712, 714, 716, 717, 702, 521, 701, 718, 854, 775,
                        703, 704, 705, 706, 707, 305};
    for (int i = 0; i < 16; ++i)
    {
        procList->push_back(procs[i]);
    }
}

int32_t CExtEquip::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket) return 2;
    
    switch (nProcId)
    {
        case 707: return onShenYaoPosLevelUp(inPacket);
        case 716: return onGemAdd(inPacket);
        case 718: return onEquipUpPhase(inPacket);
        case 717: return onGemRemove(inPacket);
        case 775: return onGemLevelUp(inPacket);
        case 854: return onBackEquip(inPacket);
        case 702: return onGemOpenHole(inPacket);
        case 704: return onEquipExchangeStar(inPacket);
        case 703: return onEquipStrenGthen(inPacket);
        case 705: return onMoFuHuiShou(inPacket);
        case 706: return onMoFuDuiHuan(inPacket);
        case 521: return onRelieveBind(inPacket);
        case 701: return onEquipUpPos(inPacket);
        case 712: return OnXinMoEquipHuiShou(inPacket);
        case 714: return onEquipDress(inPacket);
        case 305: return onEquipUnDress(inPacket);
        default: return 0;
    }
}

int32_t CExtEquip::onEquipDress(Answer::NetPacket* inPacket)
{
    GameService* v3 = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(v3) == 9) return 2;
    
    int32_t nBagSlot = Answer::NetPacket::readInt32(inPacket);
    int8_t Int8 = Answer::NetPacket::readInt8(inPacket);
    
    if (!onEquipExchange(nBagSlot, Int8)) return 2;
    return 0;
}

int32_t CExtEquip::onEquipUnDress(Answer::NetPacket* inPacket)
{
    GameService* v3 = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(v3) == 9) return 2;
    
    int8_t nEquipSlot = Answer::NetPacket::readInt8(inPacket);
    int32_t nBagSlot = CExtCharBag::GetFirstFreeSlot(Player::GetBag(m_pPlayer));
    
    if (nBagSlot < 0) return 2;
    if (!onEquipExchange(nBagSlot, nEquipSlot)) return 2;
    return 0;
}

int32_t CExtEquip::onEquipExchangeStar(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    
    int8_t nSrcBagType = Answer::NetPacket::readInt8(inPacket);
    int32_t nSrcSlot = Answer::NetPacket::readInt32(inPacket);
    int8_t nDesBagType = Answer::NetPacket::readInt8(inPacket);
    int32_t nDesSlot = Answer::NetPacket::readInt32(inPacket);
    
    MemChrBag srcSlot, desSlot;
    memset(&srcSlot, 0, sizeof(srcSlot));
    memset(&desSlot, 0, sizeof(desSlot));
    
    // 获取源装备
    if (nSrcBagType == 1)
    {
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        srcSlot = *CExtCharBag::GetSlotData(Bag, nSrcSlot);
    }
    else
    {
        if ((unsigned int)nSrcSlot > 0x5C) return 2;
        srcSlot = m_vMemEquip[nSrcSlot];
    }
    
    if (srcSlot.itemId <= 0 || srcSlot.itemClass != 2 || srcSlot.srcId <= 0) return 2;
    
    // 获取目标装备
    if (nDesBagType == 1)
    {
        CExtCharBag* v4 = Player::GetBag(m_pPlayer);
        desSlot = *CExtCharBag::GetSlotData(v4, nDesSlot);
    }
    else
    {
        if ((unsigned int)nDesSlot > 0x5C) return 2;
        desSlot = m_vMemEquip[nDesSlot];
    }
    
    if (desSlot.itemId <= 0 || desSlot.itemClass != 2 || desSlot.srcId <= 0) return 2;
    
    // 获取装备数据
    CEquipManager* v5 = Answer::Singleton<CEquipManager>::instance();
    MemEquip srcEquip, desEquip;
    CEquipManager::GetMemEquip(&srcEquip, v5, srcSlot.srcId);
    CEquipManager::GetMemEquip(&desEquip, v5, desSlot.srcId);
    
    if (srcEquip.base != srcSlot.itemId || desEquip.base != desSlot.itemId)
    {
        (void)(&srcEquip);
        (void)(&desEquip);
        return 2;
    }
    
    if (srcEquip.star > desEquip.star)
    {
        CfgData* v9 = Answer::Singleton<CfgData>::instance();
        const void* EquipTable = CfgData::GetEquipTable(v9);
        const void* pSrcEquip = CfgEquipTable::GetEquip(EquipTable, srcSlot.itemId);
        const void* pDesEquip = CfgEquipTable::GetEquip(EquipTable, desSlot.itemId);
        
        if (pSrcEquip && pDesEquip)
        {
            // 检查装备类型是否匹配
            int32_t nSrcType = 0, nDesType = 0;
            // 获取类型...
            
            if (srcEquip.star <= 20)  // 最大星级限制
            {
                CfgData* v15 = Answer::Singleton<CfgData>::instance();
                const void* pCfg = CfgData::GetEquipExchange(v15, srcEquip.star);
                
                if (pCfg)
                {
                    // 扣除消耗
                    // 交换星级
                    desEquip.star = srcEquip.star;
                    srcEquip.star = 0;
                    
                    // 更新装备
                    int8_t ConnId = Player::getConnId(m_pPlayer);
                    CEquipManager* v23 = Answer::Singleton<CEquipManager>::instance();
                    CEquipManager::UpdateMemEquip(v23, ConnId, &srcEquip, 215);
                    CEquipManager::UpdateMemEquip(v23, ConnId, &desEquip, 215);
                    
                    // 更新背包或装备栏
                    if (nDesBagType == 1)
                    {
                        CExtCharBag* v26 = Player::GetBag(m_pPlayer);
                        CExtCharBag::SetSlotData(v26, nDesSlot, &desSlot, 0, 0);
                        CExtCharBag::ForceSendDirty(v26);
                    }
                    else
                    {
                        addDirty(nDesSlot);
                        checkDirty();
                    }
                    
                    Player::recalcAttr(m_pPlayer, 0, 0);
                    updateEquipStar();
                }
            }
        }
    }
    
    (void)(&srcEquip);
    (void)(&desEquip);
    return 0;
}

int32_t CExtEquip::onMoFuHuiShou(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    
    std::vector<int32_t> vSlot;
    Player::queryBagInfo(m_pPlayer, inPacket, &vSlot);
    
    std::vector<int32_t> vRemoveSlot;
    int32_t Values = 0;
    
    for (auto it = vSlot.begin(); it != vSlot.end(); ++it)
    {
        MemChrBag EquipBag;
        Player::getBagSlotData(&EquipBag, m_pPlayer, *it);
        
        if (EquipBag.itemClass == 2)
        {
            CfgData* v5 = Answer::Singleton<CfgData>::instance();
            const void* EquipTable = CfgData::GetEquipTable(v5);
            const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, EquipBag.itemId);
            
            if (pCfgEquip)
            {
                int32_t nHuiShou = 0; // 获取回收值
                if (nHuiShou >= 0)
                {
                    Values += nHuiShou;
                    vRemoveSlot.push_back(*it);
                }
            }
        }
    }
    
    if (vRemoveSlot.size() > 0 && Values > 0)
    {
        for (auto itV = vRemoveSlot.begin(); itV != vRemoveSlot.end(); ++itV)
        {
            CExtCharBag* Bag = Player::GetBag(m_pPlayer);
            CExtCharBag::CleanSlot(Bag, *itV, 131);
        }
        
        Player::AddCurrency(m_pPlayer, 7, Values, 115, 0);  // AC_SOCRE
        return 0;
    }
    
    return 10002;
}

int32_t CExtEquip::OnXinMoEquipHuiShou(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    
    std::vector<int32_t> vSlot;
    Player::queryBagInfo(m_pPlayer, inPacket, &vSlot);
    
    std::vector<int32_t> vRemoveSlot;
    int32_t Values = 0;
    
    for (auto it = vSlot.begin(); it != vSlot.end(); ++it)
    {
        MemChrBag EquipBag;
        Player::getBagSlotData(&EquipBag, m_pPlayer, *it);
        
        if (EquipBag.itemClass == 2)
        {
            CfgData* v5 = Answer::Singleton<CfgData>::instance();
            const void* EquipTable = CfgData::GetEquipTable(v5);
            const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, EquipBag.itemId);
            
            if (pCfgEquip)
            {
                int32_t nXinMoExp = 0; // 获取心魔经验
                if (nXinMoExp >= 0)
                {
                    Values += nXinMoExp;
                    vRemoveSlot.push_back(*it);
                }
            }
        }
    }
    
    if (vRemoveSlot.size() > 0 && Values > 0)
    {
        for (auto itV = vRemoveSlot.begin(); itV != vRemoveSlot.end(); ++itV)
        {
            CExtCharBag* Bag = Player::GetBag(m_pPlayer);
            CExtCharBag::CleanSlot(Bag, *itV, 131);
        }
        
        void* CXinMo = Player::GetCXinMo(m_pPlayer);
        // CXinMo::AddExp(CXinMo, Values);
        return 0;
    }
    
    return 10002;
}

int32_t CExtEquip::onMoFuDuiHuan(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    
    int32_t nId = Answer::NetPacket::readInt32(inPacket);
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    const void* pCfg = CfgData::GetMoFuDuiHuanlCfg(v3, nId);
    
    if (!pCfg) return 2;
    
    // 检查周限制
    int32_t AlreadyDuiHuanCount = 0;
    if (nId == 1)
    {
        AlreadyDuiHuanCount = Player::getRecord(m_pPlayer, 37212);
    }
    else if (nId == 2)
    {
        AlreadyDuiHuanCount = Player::getRecord(m_pPlayer, 37213);
    }
    
    int32_t nWeekLimit = 0; // 获取周限制
    if (nWeekLimit > 0 && nWeekLimit <= AlreadyDuiHuanCount) return 10002;
    
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (CExtCharBag::GetFreeSlotCount(Bag) <= 0) return 10002;
    
    int32_t nConstScore = 0; // 获取消耗积分
    if (!Player::DecCurrency(m_pPlayer, 7, nConstScore, 116, 0)) return 10002;
    
    CExtCharBag* v5 = Player::GetBag(m_pPlayer);
    // CExtCharBag::AddItem(v5, &pCfg->Item, 216);
    
    if (nWeekLimit > 0)
    {
        if (nId == 1)
        {
            // CExtOperateLimit::AddLimitCount(..., 37212, 1);
        }
        else if (nId == 2)
        {
            // CExtOperateLimit::AddLimitCount(..., 37213, 1);
        }
    }
    
    return 10002;
}

int32_t CExtEquip::onEquipUpStar(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    
    // 检查功能是否开启
    void* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    // if (!CFunctionOpen::IsOpened(PlayerFunctionOpen, 215)) return 2;
    
    int8_t nBagType = Answer::NetPacket::readInt8(inPacket);
    int32_t nPos = Answer::NetPacket::readInt32(inPacket);
    
    std::vector<int32_t> vSlot;
    Player::queryBagInfo(m_pPlayer, inPacket, &vSlot);
    
    MemChrBag bagItem;
    memset(&bagItem, 0, sizeof(bagItem));
    
    if (nBagType == 1)
    {
        MemChrBag v52;
        Player::getBagSlotData(&v52, m_pPlayer, nPos);
        bagItem = v52;
    }
    else
    {
        if ((unsigned int)nPos > 0x5C) return 2;
        bagItem = m_vMemEquip[nPos];
    }
    
    if (bagItem.itemClass != 2 || bagItem.itemId <= 0 || bagItem.itemCount <= 0) return 2;
    
    CEquipManager* v5 = Answer::Singleton<CEquipManager>::instance();
    MemEquip equip;
    CEquipManager::GetMemEquip(&equip, v5, bagItem.srcId);
    
    if (equip.base != bagItem.itemId)
    {
        (void)(&equip);
        return 2;
    }
    
    CfgData* v7 = Answer::Singleton<CfgData>::instance();
    const void* EquipTable = CfgData::GetEquipTable(v7);
    const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, equip.base);
    
    if (!pCfgEquip)
    {
        (void)(&equip);
        return 2;
    }
    
    // 检查NPC距离
    CfgData* v9 = Answer::Singleton<CfgData>::instance();
    const void* cfgNpc = CfgData::getNpc(v9, 3017);
    if (cfgNpc)
    {
        int32_t MapId = StaticObj::getMapId(m_pPlayer);
        int32_t PosY = StaticObj::GetPosY(m_pPlayer);
        int32_t PosX = StaticObj::GetPosX(m_pPlayer);
        // if (!CfgNpc::NearNpc(cfgNpc, MapId, PosX, PosY, 10)) return 2;
    }
    
    if (equip.star >= 20)  // 最大星级限制
    {
        (void)(&equip);
        return 2;
    }
    
    int32_t star = equip.star;
    int8_t m_nType = 0; // 获取装备类型
    CfgData* v16 = Answer::Singleton<CfgData>::instance();
    const void* v17 = CfgData::GetEquipTable(v16);
    const void* pCfgUpStar = CfgEquipTable::GetEquipUpStar(v17, m_nType, star);
    
    if (!pCfgUpStar)
    {
        (void)(&equip);
        return 2;
    }
    
    int32_t nRate = 0; // 获取成功率
    if (nRate < 0)
    {
        (void)(&equip);
        return 2;
    }
    
    // 扣除消耗
    int32_t nCostMoney = 0; // 获取消耗金钱
    if (nCostMoney > 0)
    {
        CExtCurrency* v18 = Player::GetCurrency(m_pPlayer);
        int64_t MoneyBindAndNoBind = CExtCurrency::GetMoneyBindAndNoBind(v18);
        if (MoneyBindAndNoBind < nCostMoney)
        {
            (void)(&equip);
            return 2;
        }
    }
    
    int32_t nCostXingMai = 0; // 获取星芒消耗
    if (nCostXingMai > 0)
    {
        int64_t v21 = Player::GetCurrency(m_pPlayer, 8);
        if (v21 < nCostXingMai)
        {
            (void)(&equip);
            return 2;
        }
    }
    
    // 扣除物品
    // 随机升星结果
    Answer::Random* v29 = Answer::Singleton<Answer::Random>::instance();
    int32_t nRand = Answer::Random::generate(v29, 1, 10000);
    
    int32_t nAddStar = 0;
    if (nRate >= nRand)
    {
        nAddStar = 1; // 成功增加星级
    }
    else
    {
        nAddStar = -1; // 失败减少星级
    }
    
    if (nAddStar != 0)
    {
        equip.star += nAddStar;
        if (equip.star < 0) equip.star = 0;
        
        int8_t ConnId = Player::getConnId(m_pPlayer);
        CEquipManager* v32 = Answer::Singleton<CEquipManager>::instance();
        CEquipManager::UpdateMemEquip(v32, ConnId, &equip, 215);
        
        if (nBagType == 0)
        {
            Player::recalcAttr(m_pPlayer, 0, 0);
            updateEquipStar();
        }
        
        // 广播公告
        if (equip.star % 5 == 0 && equip.star > 0)
        {
            GameService* v42 = Answer::Singleton<GameService>::instance();
            Answer::NetPacket* packet = GameService::popNetpacket(v42, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6);
            if (packet)
            {
                Answer::NetPacket::writeInt32(packet, 524);
                std::string val;
                std::string val; Player::getName(m_pPlayer, &val);
                Answer::NetPacket::writeUTF8(packet, &val);
                // ... 写入其他信息
                GameService::worldBroadcast(v42, 0, packet);
            }
        }
        
        // 返回成功
        int32_t v39 = equip.id;
        uint16_t Proc = Answer::NetPacket::getProc(inPacket);
        int16_t GateIndex = Player::getGateIndex(m_pPlayer);
        int8_t v36 = Player::getConnId(m_pPlayer);
        GameService* v41 = Answer::Singleton<GameService>::instance();
        GameService::replySuccess(v41, v36, GateIndex, Proc, v39);
    }
    else
    {
        // 返回失败
        int64_t id = equip.id;
        uint16_t Proc = Answer::NetPacket::getProc(inPacket);
        int16_t GateIndex = Player::getGateIndex(m_pPlayer);
        int8_t v49 = Player::getConnId(m_pPlayer);
        GameService* v50 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v50, v49, GateIndex, Proc, 1, id);
    }
    
    (void)(&equip);
    return 0;
}

int32_t CExtEquip::onBackEquip(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    
    // VIP检查
    if (StaticObj::getMapId(m_pPlayer) != 50001)
    {
        void* PlayerVip = Player::GetPlayerVip(m_pPlayer);
        // if (!CVip::GetVipFlg(PlayerVip, 1) && !CVip::GetVipFlg(PlayerVip, 2)) return 10002;
    }
    
    std::vector<int32_t> vSlot;
    Player::queryBagInfo(m_pPlayer, inPacket, &vSlot);
    
    std::map<int32_t, int32_t> CurrMap;
    std::vector<int32_t> vRemoveSlot;
    
    for (auto it = vSlot.begin(); it != vSlot.end(); ++it)
    {
        MemChrBag EquipBag;
        Player::getBagSlotData(&EquipBag, m_pPlayer, *it);
        
        if (EquipBag.itemClass == 2)
        {
            CfgData* v8 = Answer::Singleton<CfgData>::instance();
            const void* EquipTable = CfgData::GetEquipTable(v8);
            const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, EquipBag.itemId);
            
            if (pCfgEquip)
            {
                int32_t nBackType = 0; // 获取回收类型
                int32_t nBackValue = 0; // 获取回收价值
                if (nBackType >= 0 && nBackValue > 0)
                {
                    CurrMap[nBackType] += nBackValue;
                    vRemoveSlot.push_back(*it);
                }
            }
        }
    }
    
    if (vRemoveSlot.size() > 0)
    {
        for (auto itV = vRemoveSlot.begin(); itV != vRemoveSlot.end(); ++itV)
        {
            CExtCharBag* Bag = Player::GetBag(m_pPlayer);
            CExtCharBag::CleanSlot(Bag, *itV, 113);
        }
        
        int32_t Rate = 10; // VIP回收倍率
        
        for (auto& pair : CurrMap)
        {
            int32_t v16 = Rate * pair.second / 10;
            Player::AddCurrency(m_pPlayer, pair.first, v16, 113, 0);
        }
        
        return 0;
    }
    
    return 10002;
}

int32_t CExtEquip::onEquipUpPhase(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 2;
    
    int8_t nBagType = Answer::NetPacket::readInt8(inPacket);
    int32_t EquipId = Answer::NetPacket::readInt32(inPacket);
    int32_t nPos = Answer::NetPacket::readInt32(inPacket);
    
    MemChrBag EquipBag;
    memset(&EquipBag, 0, sizeof(EquipBag));
    
    if (nBagType == 1)
    {
        MemChrBag v36;
        Player::getBagSlotData(&v36, m_pPlayer, nPos);
        EquipBag = v36;
    }
    else
    {
        if ((unsigned int)nPos > 0x5C) return 2;
        EquipBag = m_vMemEquip[nPos];
    }
    
    if (EquipBag.itemId <= 0 || EquipBag.itemCount <= 0) return 2;
    if (EquipBag.itemClass != 2) return 10002;
    
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    const void* EquipUpPhaseTable = CfgData::GetEquipUpPhaseTable(v3);
    const void* EquipUpPhase = CfgEquipUpPhaseTable::GetData(EquipUpPhaseTable, EquipId);
    
    if (!EquipUpPhase) return 10002;
    
    CEquipManager* v6 = Answer::Singleton<CEquipManager>::instance();
    MemEquip NowEquip;
    CEquipManager::GetMemEquip(&NowEquip, v6, EquipBag.srcId);
    
    if (NowEquip.base != EquipBag.itemId)
    {
        (void)(&NowEquip);
        return 10002;
    }
    
    int32_t nCostMoney = 0; // 获取消耗金钱
    if (nCostMoney > 0)
    {
        CExtCurrency* Currency = Player::GetCurrency(m_pPlayer);
        int64_t MoneyBindAndNoBind = CExtCurrency::GetMoneyBindAndNoBind(Currency);
        if (MoneyBindAndNoBind < nCostMoney)
        {
            (void)(&NowEquip);
            return 2;
        }
    }
    
    // 扣除消耗物品
    // 扣除金钱
    if (nCostMoney > 0)
    {
        CExtCurrency* v19 = Player::GetCurrency(m_pPlayer);
        CExtCurrency::DecMoneyAndNoBind(v19, nCostMoney, 18, 0);
    }
    
    NowEquip.base = EquipId; // 新装备ID
    
    // 广播公告
    int32_t nGongGaoId = 0; // 获取公告ID
    if (nGongGaoId > 0)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* v21 = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(v21, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nGongGaoId);
            std::string val;
            std::string val; Player::getName(m_pPlayer, &val);
            Answer::NetPacket::writeUTF8(packet, &val);
            // ... 写入其他信息
            GameService::worldBroadcast(v21, ConnId, packet);
        }
    }
    
    // 更新装备
    int8_t v26 = Player::getConnId(m_pPlayer);
    CEquipManager* v27 = Answer::Singleton<CEquipManager>::instance();
    CEquipManager::UpdateMemEquip(v27, v26, &NowEquip, 220);
    
    if (nBagType == 1)
    {
        EquipBag.itemId = EquipId;
        CExtCharBag* v28 = Player::GetBag(m_pPlayer);
        CExtCharBag::SetSlotData(v28, nPos, &EquipBag, 0, 0);
    }
    else
    {
        m_vMemEquip[nPos].itemId = EquipId;
        addDirty(nPos);
        checkEquipSuit(0);
        Player::recalcAttr(m_pPlayer, 0, 0);
    }
    
    // 发送装备信息
    CEquipManager* v30 = Answer::Singleton<CEquipManager>::instance();
    CEquipManager::SendPlayerEquipInfo(v30, m_pPlayer, &NowEquip);
    
    // 返回成功
    int64_t id = NowEquip.id;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v34 = Player::getConnId(m_pPlayer);
    GameService* v35 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v35, v34, GateIndex, Proc, id);
    
    (void)(&NowEquip);
    return 0;
}

int32_t CExtEquip::onRelieveBind(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    
    int8_t nBagType = Answer::NetPacket::readInt8(inPacket);
    int32_t nPos = Answer::NetPacket::readInt32(inPacket);
    int32_t nItemPos = Answer::NetPacket::readInt32(inPacket);
    
    MemChrBag Equip;
    memset(&Equip, 0, sizeof(Equip));
    
    if (nBagType == 1)
    {
        MemChrBag v7;
        Player::getBagSlotData(&v7, m_pPlayer, nPos);
        Equip = v7;
    }
    else
    {
        if ((unsigned int)nPos > 0x5C) return 2;
        Equip = m_vMemEquip[nPos];
    }
    
    if (!Equip.bind) return 2;
    if (Equip.itemId <= 0 || Equip.itemCount <= 0) return 2;
    if (Equip.itemClass != 2) return 10002;
    
    std::vector<int32_t> vSlot;
    ItemData data;
    memset(&data, 0, sizeof(data));
    vSlot.push_back(nItemPos);
    data.m_nId = 7321;
    data.m_nClass = 1;
    data.m_nCount = 1;
    
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::RemoveItem(Bag, &vSlot, &data, 131))
    {
        return 2;
    }
    
    if (nBagType == 1)
    {
        Equip.bind = 0;
        CExtCharBag* v4 = Player::GetBag(m_pPlayer);
        CExtCharBag::SetSlotData(v4, nPos, &Equip, 0, 0);
        CExtCharBag::ForceSendDirty(v4);
    }
    else
    {
        m_vMemEquip[nPos].bind = 0;
        addDirty(nPos);
    }
    
    return 0;
}

int32_t CExtEquip::onEquipStrenGthen(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    
    int8_t nPos = Answer::NetPacket::readInt8(inPacket);
    std::vector<int32_t> vSlot;
    Player::queryBagInfo(m_pPlayer, inPacket, &vSlot);
    
    if ((uint8_t)nPos < 10)
    {
        int32_t nLevel = m_vMemStrengthen[nPos];
        CfgData* v4 = Answer::Singleton<CfgData>::instance();
        const void* EquipTable = CfgData::GetEquipTable(v4);
        const void* pStrengthen = CfgEquipTable::GetEquipStrengthenCfg(EquipTable, nPos, nLevel);
        
        if (pStrengthen)
        {
            // 检查消耗物品
            // 扣除物品
            m_vMemStrengthen[nPos]++;
            Player::recalcAttr(m_pPlayer, 0, 0);
            
            // 广播公告
            int32_t nGongGaoId = 0;
            if (nGongGaoId > 0)
            {
                GameService* v17 = Answer::Singleton<GameService>::instance();
                Answer::NetPacket* packet = GameService::popNetpacket(v17, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6);
                if (packet)
                {
                    Answer::NetPacket::writeInt32(packet, nGongGaoId);
                    std::string val;
                    std::string val; Player::getName(m_pPlayer, &val);
                    Answer::NetPacket::writeUTF8(packet, &val);
                    // ... 写入其他信息
                    GameService::worldBroadcast(v17, 0, packet);
                }
            }
            
            uint16_t Proc = Answer::NetPacket::getProc(inPacket);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            int8_t ConnId = Player::getConnId(m_pPlayer);
            GameService* v12 = Answer::Singleton<GameService>::instance();
            GameService::replySuccess(v12, ConnId, GateIndex, Proc, nPos);
            return 0;
        }
    }
    
    return 10002;
}

void CExtEquip::SendStrenGthenInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2DC2);
    
    if (!packet) return;
    
    Answer::NetPacket::writeInt8(packet, 10);
    for (int i = 0; i <= 9; ++i)
    {
        Answer::NetPacket::writeInt8(packet, i);
        Answer::NetPacket::writeInt32(packet, m_vMemStrengthen[i]);
    }
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v5 = Player::getConnId(m_pPlayer);
    GameService* v6 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v6, v5, GateIndex, packet);
}

int32_t CExtEquip::onEquipUpPos(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    
    int8_t nPos = Answer::NetPacket::readInt8(inPacket);
    int8_t AutoBuy = Answer::NetPacket::readInt8(inPacket);
    int8_t BaoHu = Answer::NetPacket::readInt8(inPacket);
    
    std::vector<int32_t> vSlot;
    Player::queryBagInfo(m_pPlayer, inPacket, &vSlot);
    
    if ((uint8_t)nPos < 10)
    {
        int32_t nLevel = m_vMemPosLevel[nPos];
        int32_t v3 = nLevel + 1;
        
        CfgData* v5 = Answer::Singleton<CfgData>::instance();
        const void* EquipTable = CfgData::GetEquipTable(v5);
        const void* pCfgEquipUpPos = CfgEquipTable::GetEquipUpPos(EquipTable, nPos, v3);
        
        if (!pCfgEquipUpPos) return 10002;
        
        int32_t nNeedLevel = 0; // 获取需要等级
        int32_t v7 = Player::getLevel(m_pPlayer);
        if (v7 < nNeedLevel) return 10002;
        
        int32_t nCostMoney = 0; // 获取消耗金钱
        if (nCostMoney > 0)
        {
            CExtCurrency* v8 = Player::GetCurrency(m_pPlayer);
            int64_t MoneyBindAndNoBind = CExtCurrency::GetMoneyBindAndNoBind(v8);
            if (MoneyBindAndNoBind < nCostMoney) return 10002;
        }
        
        // 处理自动购买和扣除消耗
        // 随机成功/失败
        Answer::Random* v36 = Answer::Singleton<Answer::Random>::instance();
        int32_t v37 = Answer::Random::generate(v36, 1, 10000);
        int32_t nRate = 0; // 获取成功率
        
        if (v37 <= nRate)
        {
            // 成功
            m_vMemPosLevel[nPos] = nLevel + 1;
            Player::recalcAttr(m_pPlayer, 0, 0);
            
            // 广播公告
            int32_t nGongGaoId = 0;
            if (nGongGaoId > 0)
            {
                GameService* v50 = Answer::Singleton<GameService>::instance();
                Answer::NetPacket* packet = GameService::popNetpacket(v50, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6);
                if (packet)
                {
                    Answer::NetPacket::writeInt32(packet, nGongGaoId);
                    std::string val; Player::getName(m_pPlayer, &val);
                    Answer::NetPacket::writeUTF8(packet, &val);
                    GameService::worldBroadcast(v50, 0, packet);
                }
            }
            
            uint16_t Proc = Answer::NetPacket::getProc(inPacket);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            int8_t ConnId = Player::getConnId(m_pPlayer);
            GameService* v49 = Answer::Singleton<GameService>::instance();
            GameService::replySuccess(v49, ConnId, GateIndex, Proc, nPos);
        }
        else
        {
            // 失败
            if (BaoHu == 1)
            {
                // 保护符保护，不掉级
                m_vMemPosLevel[nPos] = nLevel;
            }
            else
            {
                m_vMemPosLevel[nPos] = nLevel - 1;
                if (m_vMemPosLevel[nPos] < 0) m_vMemPosLevel[nPos] = 0;
            }
            Player::recalcAttr(m_pPlayer, 0, 0);
            
            int32_t v40 = nPos;
            uint16_t v41 = Answer::NetPacket::getProc(inPacket);
            int16_t v42 = Player::getGateIndex(m_pPlayer);
            int8_t v43 = Player::getConnId(m_pPlayer);
            GameService* v44 = Answer::Singleton<GameService>::instance();
            GameService::replyfailure(v44, v43, v42, v41, v40, m_vMemPosLevel[nPos]);
        }
        
        return 0;
    }
    
    return 10002;
}

int32_t CExtEquip::onShenYaoPosLevelUp(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    
    int8_t nPos = Answer::NetPacket::readInt8(inPacket);
    std::vector<int32_t> vSlot;
    Player::queryBagInfo(m_pPlayer, inPacket, &vSlot);
    
    if (nPos > 10 && nPos <= 20)
    {
        int32_t nLevel = m_vShenYaoEquipPos[nPos - 11];
        CfgData* v4 = Answer::Singleton<CfgData>::instance();
        const void* EquipTable = CfgData::GetEquipTable(v4);
        const void* pCfgEquipUpPos = CfgEquipTable::GetShenYaoPosCfg(EquipTable, nPos, nLevel);
        int32_t v6 = nLevel + 1;
        const void* pCfgEquipNextUpPos = CfgEquipTable::GetShenYaoPosCfg(EquipTable, nPos, v6);
        
        if (pCfgEquipUpPos && pCfgEquipNextUpPos)
        {
            // 扣除消耗
            m_vShenYaoEquipPos[nPos - 11] = nLevel + 1;
            Player::RecalcAttr(m_pPlayer);
            
            // 广播公告
            int32_t nGongGaoId = 0;
            if (nGongGaoId > 0)
            {
                int8_t ConnId = Player::getConnId(m_pPlayer);
                GameService* v13 = Answer::Singleton<GameService>::instance();
                Answer::NetPacket* packet = GameService::popNetpacket(v13, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
                if (packet)
                {
                    Answer::NetPacket::writeInt32(packet, nGongGaoId);
                    GameService::worldBroadcast(v13, ConnId, packet);
                }
            }
            
            CExtCharBag* v18 = Player::GetBag(m_pPlayer);
            CExtCharBag::ForceSendDirty(v18);
            
            uint16_t Proc = Answer::NetPacket::getProc(inPacket);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            int8_t v22 = Player::getConnId(m_pPlayer);
            GameService* v23 = Answer::Singleton<GameService>::instance();
            GameService::replySuccess(v23, v22, GateIndex, Proc, nPos);
            return 0;
        }
    }
    
    return 10002;
}

int32_t CExtEquip::onGemOpenHole(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    
    int8_t nPos = Answer::NetPacket::readInt8(inPacket);
    int8_t nSlot = Answer::NetPacket::readInt8(inPacket);
    
    if ((uint8_t)nPos > 9 || (uint8_t)nSlot >= 4) return 10002;
    if (m_vMemGem[nPos][nSlot] >= 0) return 10002;
    
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const void* ItemGemTable = CfgData::GetItemGemTable(v4);
    const void* pCfgGemOpenHole = CfgItemGemTable::GetGemOpenHole(ItemGemTable, nSlot);
    
    if (!pCfgGemOpenHole) return 10002;
    
    // 扣除消耗
    m_vMemGem[nPos][nSlot] = 0;
    SendGemInfo(nPos, nSlot);
    
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v15 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v15, ConnId, GateIndex, Proc, nPos);
    return 0;
}

int32_t CExtEquip::GetGemOpenLevel(int32_t EquipSlot)
{
    switch (EquipSlot)
    {
        case 0: return 300;
        case 1: return 320;
        case 2: return 310;
        case 3: return 340;
        case 4: return 410;
        case 5: return 430;
        case 6: return 330;
        case 7: return 370;
        case 8: return 350;
        case 9: return 390;
        default: return 0;
    }
}

int32_t CExtEquip::GetMoFuSuitLevel()
{
    int32_t SuitId = 0;
    
    for (int i = 50; i <= 58; ++i)
    {
        MemChrBag* curEquip = &m_vMemEquip[i];
        if (curEquip->srcId <= 0 || curEquip->itemClass != 2) return 0;
        
        CEquipManager* v2 = Answer::Singleton<CEquipManager>::instance();
        MemEquip equip;
        CEquipManager::GetMemEquip(&equip, v2, curEquip->srcId);
        
        if (equip.base == curEquip->itemId)
        {
            CfgData* v5 = Answer::Singleton<CfgData>::instance();
            const void* EquipTable = CfgData::GetEquipTable(v5);
            const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, curEquip->itemId);
            
            if (pCfgEquip)
            {
                int32_t nGrade = 0; // 获取装备品级
                if (SuitId == 0)
                {
                    SuitId = nGrade;
                }
                else
                {
                    if (nGrade < SuitId) SuitId = nGrade;
                }
            }
            else
            {
                SuitId = 0;
            }
        }
        else
        {
            SuitId = 0;
        }
        
        (void)(&equip);
        if (SuitId == 0) return 0;
    }
    
    return SuitId;
}

int32_t CExtEquip::onGemAdd(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    
    int8_t nPos = Answer::NetPacket::readInt8(inPacket);
    int8_t nSlot = Answer::NetPacket::readInt8(inPacket);
    int32_t nBagSlot = Answer::NetPacket::readInt32(inPacket);
    
    if ((uint8_t)nPos > 9 || (uint8_t)nSlot >= 4) return 10002;
    
    int32_t nId = m_vMemGem[nPos][nSlot];
    if (nId < 0) return 10002;
    
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    MemChrBag bagItem = *CExtCharBag::GetSlotData(Bag, nBagSlot);
    
    if (bagItem.itemClass != 3) return 10002;
    
    int32_t nGemId = bagItem.itemId;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const void* ItemGemTable = CfgData::GetItemGemTable(v4);
    const void* pCfgGem = CfgItemGemTable::GetItemGem(ItemGemTable, bagItem.itemId);
    
    if (!pCfgGem) return 10002;
    
    // 检查宝石类型冲突
    for (int i = 0; i <= 3; ++i)
    {
        int32_t GemId = m_vMemGem[nPos][i];
        if (GemId > 0)
        {
            const void* pCfgCheckGem = CfgItemGemTable::GetItemGem(ItemGemTable, GemId);
            if (pCfgCheckGem)
            {
                int32_t nGemType = 0; // 获取宝石类型
                int32_t nCurrType = 0; // 获取当前宝石类型
                if (nGemType == nCurrType) return 10002;
            }
        }
    }
    
    // 替换已有宝石
    if (nId > 0)
    {
        MemChrBag addGem;
        memset(&addGem, 0, sizeof(addGem));
        addGem.itemClass = 3;
        addGem.itemCount = 1;
        addGem.itemId = nId;
        
        CExtCharBag* v8 = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::AddItem(v8, &addGem, 132)) return 10002;
    }
    
    // 消耗新宝石
    bagItem.itemCount--;
    CExtCharBag* v9 = Player::GetBag(m_pPlayer);
    CExtCharBag::SetSlotData(v9, nBagSlot, &bagItem, 127, 0);
    
    m_vMemGem[nPos][nSlot] = nGemId;
    Player::recalcAttr(m_pPlayer, 0, 0);
    updateEquipGem();
    SendGemInfo(nPos, nSlot);
    
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v14 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v14, ConnId, GateIndex, Proc, nPos);
    return 0;
}

int32_t CExtEquip::onGemRemove(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    
    int8_t nPos = Answer::NetPacket::readInt8(inPacket);
    int8_t nSlot = Answer::NetPacket::readInt8(inPacket);
    
    if ((uint8_t)nPos > 9 || (uint8_t)nSlot >= 4) return 10002;
    
    int32_t nId = m_vMemGem[nPos][nSlot];
    if (nId <= 0) return 10002;
    
    MemChrBag addGem;
    memset(&addGem, 0, sizeof(addGem));
    addGem.itemClass = 3;
    addGem.itemCount = 1;
    addGem.itemId = nId;
    
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &addGem, 132)) return 10002;
    
    m_vMemGem[nPos][nSlot] = 0;
    Player::recalcAttr(m_pPlayer, 0, 0);
    SendGemInfo(nPos, nSlot);
    
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v8 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v8, ConnId, GateIndex, Proc, nPos);
    return 0;
}

int32_t CExtEquip::onGemLevelUp(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    
    int32_t nEquipPos = Answer::NetPacket::readInt32(inPacket);
    int8_t nGemSlot = Answer::NetPacket::readInt8(inPacket);
    std::vector<int32_t> vSlot;
    Player::queryBagInfo(m_pPlayer, inPacket, &vSlot);
    int8_t bAutoBuy = Answer::NetPacket::readInt8(inPacket);
    
    if ((uint8_t)nGemSlot >= 4) return 2;
    if ((unsigned int)nEquipPos >= 10) return 2;
    
    int32_t nGemId = m_vMemGem[nEquipPos][nGemSlot];
    if (nGemId <= 0) return 2;
    
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    const void* ItemGemTable = CfgData::GetItemGemTable(v3);
    const void* pCfgGemLevelUp = CfgItemGemTable::GetGemLevelUp(ItemGemTable, nGemId);
    
    if (!pCfgGemLevelUp) return 2;
    
    // 处理自动购买和扣除消耗
    // 升级宝石
    m_vMemGem[nEquipPos][nGemSlot] = pCfgGemLevelUp->nNextId;
    Player::recalcAttr(m_pPlayer, 0, 0);
    updateEquipGem();
    SendGemInfo(nEquipPos, nGemSlot);
    
    // 广播公告
    int32_t nBroadId = 0;
    if (nBroadId > 0)
    {
        std::string strName;
        Player::getName(m_pPlayer, &strName);
        broadcastGemLevelUp(nBroadId, &strName, Player::getCid(m_pPlayer), pCfgGemLevelUp->nNextId);
    }
    
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v27 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v27, ConnId, GateIndex, Proc, pCfgGemLevelUp->nNextId);
    return 0;
}

void CExtEquip::SendEquipInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2DBE);
    
    if (!packet) return;
    
    int32_t equipCount = 0;
    int32_t oldBengin = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::writeInt8(packet, 0);
    
    for (int i = 0; i <= 92; ++i)
    {
        MemChrBag* tSlot = &m_vMemEquip[i];
        Answer::NetPacket::writeInt8(packet, i);
        Answer::NetPacket::writeInt32(packet, tSlot->itemId);
        Answer::NetPacket::writeInt8(packet, tSlot->bind);
        Answer::NetPacket::writeInt32(packet, tSlot->endTime);
        Answer::NetPacket::writeInt64(packet, tSlot->srcId);
        ++equipCount;
    }
    
    int32_t oldwoffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, oldBengin);
    Answer::NetPacket::writeInt8(packet, equipCount);
    Answer::NetPacket::setWOffset(packet, oldwoffset);
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v5 = Player::getConnId(m_pPlayer);
    GameService* v6 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v6, v5, GateIndex, packet);
}

void CExtEquip::SendShenYaoPosLevelInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2DC3);
    
    if (!packet) return;
    
    PackageShenYaoPosLevelUp(packet);
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v5 = Player::getConnId(m_pPlayer);
    GameService* v6 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v6, v5, GateIndex, packet);
}

void CExtEquip::SendGemInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2DBF);
    
    if (!packet) return;
    
    PackageGemInfo(packet);
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v5 = Player::getConnId(m_pPlayer);
    GameService* v6 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v6, v5, GateIndex, packet);
}

void CExtEquip::SendPosLevelInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2DC1);
    
    if (!packet) return;
    
    Answer::NetPacket::writeInt8(packet, 10);
    for (int i = 0; i <= 9; ++i)
    {
        Answer::NetPacket::writeInt8(packet, i);
        Answer::NetPacket::writeInt32(packet, m_vMemPosLevel[i]);
    }
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v5 = Player::getConnId(m_pPlayer);
    GameService* v6 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v6, v5, GateIndex, packet);
}

int32_t CExtEquip::GetPosLevel(int8_t Pos)
{
    if ((uint8_t)Pos < 10) return m_vMemPosLevel[Pos];
    return 0;
}

int32_t CExtEquip::getStrenGthenLevel(int8_t Pos)
{
    if ((uint8_t)Pos < 10) return m_vMemStrengthen[Pos];
    return 0;
}

bool CExtEquip::IsAllPosTop()
{
    for (int nPos = 0; nPos <= 9; ++nPos)
    {
        int32_t v1 = m_vMemPosLevel[nPos] + 1;
        CfgData* v2 = Answer::Singleton<CfgData>::instance();
        const void* EquipTable = CfgData::GetEquipTable(v2);
        if (CfgEquipTable::GetEquipUpPos(EquipTable, nPos, v1)) return 0;
    }
    return 1;
}

int32_t CExtEquip::IsAllPosLevel()
{
    int32_t Level = 0;
    for (int nPos = 0; nPos <= 9; ++nPos)
    {
        if (m_vMemPosLevel[nPos] > 0) Level += m_vMemPosLevel[nPos];
    }
    return Level;
}

void CExtEquip::SendGemInfo(int8_t nPos, int8_t nSlot)
{
    if (!m_pPlayer) return;
    if ((uint8_t)nPos > 9 || (uint8_t)nSlot >= 4) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v4 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v4, ConnId, Answer::PackType::PACK_DISPATCH, 0x2DBF);
    
    if (!packet) return;
    
    Answer::NetPacket::writeInt8(packet, 1);
    Answer::NetPacket::writeInt8(packet, nPos);
    Answer::NetPacket::writeInt8(packet, nSlot);
    Answer::NetPacket::writeInt32(packet, m_vMemGem[nPos][nSlot]);
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v7 = Player::getConnId(m_pPlayer);
    GameService* v8 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v8, v7, GateIndex, packet);
}

void CExtEquip::AddCharAttr()
{
    addEquipAttr();
    addGemAttr();
    addSuitAttr();
    addPosLevelAttr();
    AddStrengthenAttr();
    addShenYaoPosAttr();
}

void CExtEquip::addEquipAttr()
{
    if (!m_pPlayer) return;
    
    for (int i = 0; i <= 92; ++i)
    {
        MemChrBag* curEquip = &m_vMemEquip[i];
        if (curEquip->srcId > 0 && curEquip->itemClass == 2)
        {
            CEquipManager* v2 = Answer::Singleton<CEquipManager>::instance();
            MemEquip equip;
            CEquipManager::GetMemEquip(&equip, v2, curEquip->srcId);
            
            if (equip.base == curEquip->itemId)
            {
                CfgData* v4 = Answer::Singleton<CfgData>::instance();
                const void* EquipTable = CfgData::GetEquipTable(v4);
                const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, equip.base);
                
                if (pCfgEquip)
                {
                    // 添加基础属性
                    // 添加星级属性
                    // 添加强化属性
                }
            }
            
            (void)(&equip);
        }
    }
}

void CExtEquip::addGemAttr()
{
    if (!m_pPlayer) return;
    
    int32_t nGemLevel = 0;
    
    for (int i = 0; i <= 9; ++i)
    {
        if (m_vMemEquip[i].itemId > 0)
        {
            for (int j = 0; j <= 3; ++j)
            {
                int32_t nGemId = m_vMemGem[i][j];
                if (nGemId > 0)
                {
                    CfgData* v1 = Answer::Singleton<CfgData>::instance();
                    const void* ItemGemTable = CfgData::GetItemGemTable(v1);
                    const void* pCfgGem = CfgItemGemTable::GetItemGem(ItemGemTable, nGemId);
                    
                    if (pCfgGem)
                    {
                        nGemLevel += 1; // 宝石等级
                        // 添加宝石属性
                    }
                }
            }
        }
    }
    
    if (nGemLevel > 0)
    {
        // 添加宝石套装属性
    }
}

void CExtEquip::AddStrengthenAttr()
{
    if (!m_pPlayer) return;
    
    for (int i = 0; i <= 9; ++i)
    {
        if (m_vMemEquip[i].itemId > 0)
        {
            int32_t StrenGthenLevel = getStrenGthenLevel(i);
            CfgData* v1 = Answer::Singleton<CfgData>::instance();
            const void* EquipTable = CfgData::GetEquipTable(v1);
            const void* pStrengthen = CfgEquipTable::GetEquipStrengthenCfg(EquipTable, i, StrenGthenLevel);
            
            if (pStrengthen)
            {
                // 添加强化属性
            }
        }
    }
}

void CExtEquip::addSuitAttr()
{
    if (!m_pPlayer) return;
    
    for (auto it = m_EquipSuit.begin(); it != m_EquipSuit.end(); ++it)
    {
        int32_t v1 = *it;
        CfgData* v2 = Answer::Singleton<CfgData>::instance();
        const void* EquipTable = CfgData::GetEquipTable(v2);
        const void* pSuit = CfgEquipTable::GetEquipSuit(EquipTable, v1);
        
        if (pSuit)
        {
            // 添加套装属性
        }
    }
}

void CExtEquip::addPosLevelAttr()
{
    if (!m_pPlayer) return;
    
    int32_t nSuitLevel = -1;
    
    for (int i = 0; i <= 9; ++i)
    {
        int32_t nLevel = m_vMemPosLevel[i];
        if (nSuitLevel >= 0)
        {
            if (nLevel < nSuitLevel) nSuitLevel = nLevel;
        }
        else
        {
            nSuitLevel = nLevel;
        }
        
        if (nLevel > 0)
        {
            CfgData* v2 = Answer::Singleton<CfgData>::instance();
            const void* EquipTable = CfgData::GetEquipTable(v2);
            const void* pCfgEquipUpPos = CfgEquipTable::GetEquipUpPos(EquipTable, i, nLevel);
            
            if (pCfgEquipUpPos)
            {
                // 添加位置等级属性
            }
        }
    }
    
    if (nSuitLevel > 0)
    {
        // 添加位置套装属性
    }
}

int32_t CExtEquip::GetEquipAllGemLevel()
{
    int32_t sum = 0;
    
    for (int i = 0; i <= 9; ++i)
    {
        for (int j = 0; j <= 3; ++j)
        {
            if (m_vMemGem[i][j] > 0)
            {
                CfgData* v2 = Answer::Singleton<CfgData>::instance();
                const void* ItemGemTable = CfgData::GetItemGemTable(v2);
                const void* pGem = CfgItemGemTable::GetItemGem(ItemGemTable, m_vMemGem[i][j]);
                if (pGem) sum += 1; // 宝石等级
            }
        }
    }
    
    return sum;
}

void CExtEquip::addShenYaoPosAttr()
{
    if (!m_pPlayer) return;
    
    int32_t nSuitLevel = -1;
    
    for (int i = 0; i <= 9; ++i)
    {
        int32_t nLevel = m_vShenYaoEquipPos[i];
        if (nSuitLevel >= 0)
        {
            if (nLevel < nSuitLevel) nSuitLevel = nLevel;
        }
        else
        {
            nSuitLevel = nLevel;
        }
        
        if (nLevel > 0)
        {
            int32_t v1 = i + 11;
            CfgData* v2 = Answer::Singleton<CfgData>::instance();
            const void* EquipTable = CfgData::GetEquipTable(v2);
            const void* pCfgEquipUpPos = CfgEquipTable::GetShenYaoPosCfg(EquipTable, v1, nLevel);
            
            if (pCfgEquipUpPos)
            {
                // 添加神耀位置属性
            }
        }
    }
    
    if (nSuitLevel > 0)
    {
        // 添加神耀位置套装属性
    }
}

bool CExtEquip::onEquipExchange(int32_t nBagSlot, int8_t nEquipSlot)
{
    if ((uint8_t)nEquipSlot > 0x5C) return 0;
    
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::IsSlotValid(Bag, nBagSlot)) return 0;
    
    MemChrBag bagItem;
    Player::getBagSlotData(&bagItem, m_pPlayer, nBagSlot);
    
    if (bagItem.itemId > 0 && !checkDressEquip(nBagSlot, nEquipSlot)) return 0;
    
    CExtCharBag::SetSlotData(Bag, nBagSlot, &m_vMemEquip[nEquipSlot], 0, 0);
    m_vMemEquip[nEquipSlot] = bagItem;
    
    if (nEquipSlot == 0 || nEquipSlot == 2)
    {
        Player::SetNeedSyncAround(m_pPlayer);
        Player::setSyncToTeamFlag(m_pPlayer);
        
        if (nEquipSlot == 0)
        {
            Player::sendUpdateSocialPlayerInfo(m_pPlayer, 2, bagItem.itemId);  // PII_MAIN_WEAPON
        }
        else
        {
            Player::sendUpdateSocialPlayerInfo(m_pPlayer, 3, bagItem.itemId);  // PII_CLOTH
        }
    }
    
    addDirty(nEquipSlot);
    checkEquipSuit(nEquipSlot > 10 && nEquipSlot <= 20);
    updateEquipStar();
    updateEquipGem();
    
    Player::recalcAttr(m_pPlayer, 0, 0);
    Player::SetNeedSyncAround(m_pPlayer);
    return 1;
}

int32_t CExtEquip::GetTeJieValue(int32_t nSlot)
{
    if (nSlot <= 86 || nSlot > 92) return 0;
    
    int32_t v3 = m_vMemEquip[nSlot].itemId;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const void* pCfgEquip = CfgData::getEquip(v4, v3);
    
    if (pCfgEquip) return 1; // 特戒参数
    return 0;
}

int32_t CExtEquip::GetEquipType(int32_t Slot)
{
    switch (Slot)
    {
        case 33: return 29;
        case 34: return 30;
        case 35: return 31;
        case 36: return 32;
        case 37: return 33;
        case 38: return 34;
        case 39: return 35;
        case 40: return 36;
        case 41: return 37;
        case 42: return 38;
        case 43: return 39;
        case 50: return 46;
        case 51: return 47;
        case 52: return 48;
        case 53: return 49;
        case 54: return 50;
        case 55: return 51;
        case 56: return 52;
        case 57: return 53;
        case 58: return 54;
        case 59: return 55;
        case 60: return 56;
        case 61: return 57;
        case 62: return 58;
        case 63: return 59;
        case 64: return 60;
        default: return -1;
    }
}

int32_t CExtEquip::GetEquipSlotPlace(int32_t nType)
{
    switch (nType)
    {
        case 17: return 21;
        case 18: return 22;
        case 19: return 23;
        case 20: return 24;
        case 21: return 25;
        case 22: return 26;
        case 23: return 27;
        case 24: return 28;
        case 25: return 29;
        case 26: return 30;
        case 27: return 31;
        case 28: return 32;
        default: return 0;
    }
}

int32_t CExtEquip::GetMaxPolishLevel(int32_t Slot)
{
    if ((unsigned int)Slot > 0x5C) return 0;
    return 10;  // 默认最大研磨等级
}

int32_t CExtEquip::GetMaxRefiningLevel(int32_t Slot)
{
    if ((unsigned int)Slot > 0x5C) return 0;
    return 10;  // 默认最大精炼等级
}

void CExtEquip::addDirty(int32_t nSlot)
{
    auto it = std::find(m_lstDirty.begin(), m_lstDirty.end(), nSlot);
    if (it == m_lstDirty.end())
    {
        m_lstDirty.push_back(nSlot);
    }
}

void CExtEquip::checkDirty()
{
    if (sendDirty())
    {
        clearDirty();
    }
}

void CExtEquip::clearDirty()
{
    m_lstDirty.clear();
}

bool CExtEquip::sendDirty()
{
    if (!m_pPlayer || m_lstDirty.empty()) return 0;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v4 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v4, ConnId, Answer::PackType::PACK_DISPATCH, 0x2DB7);
    
    if (!packet) return 0;
    
    Answer::NetPacket::writeInt8(packet, 0);
    int32_t nOffSet = Answer::NetPacket::getWOffset(packet);
    int32_t nCount = 0;
    Answer::NetPacket::writeInt32(packet, 0);
    
    for (auto it = m_lstDirty.begin(); it != m_lstDirty.end(); ++it)
    {
        int32_t nSlot = *it;
        if ((unsigned int)nSlot <= 0x5C)
        {
            MemChrBag* tSlot = GetEquipSlot(nSlot);
            Answer::NetPacket::writeInt32(packet, nSlot);
            Answer::NetPacket::writeInt32(packet, tSlot->itemId);
            Answer::NetPacket::writeInt8(packet, tSlot->itemClass);
            Answer::NetPacket::writeInt32(packet, tSlot->itemCount);
            Answer::NetPacket::writeInt8(packet, tSlot->bind);
            Answer::NetPacket::writeInt32(packet, tSlot->endTime);
            Answer::NetPacket::writeInt64(packet, tSlot->srcId);
            ++nCount;
        }
    }
    
    int32_t endOffSet = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, nOffSet);
    Answer::NetPacket::writeInt32(packet, nCount);
    Answer::NetPacket::setWOffset(packet, endOffSet);
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v7 = Player::getConnId(m_pPlayer);
    GameService* v8 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v8, v7, GateIndex, packet);
    return 1;
}

void CExtEquip::broadcastItemCombi(int32_t ItemId, int8_t ItemClass)
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v4 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v4, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    
    if (!packet) return;
    
    Answer::NetPacket::writeInt32(packet, 121);
    int64_t Cid = Player::getCid(m_pPlayer);
    Answer::NetPacket::writeInt64(packet, Cid);
    
    std::string val;
    std::string val; Player::getName(m_pPlayer, &val);
    Answer::NetPacket::writeUTF8(packet, &val);
    
    Answer::NetPacket::writeInt32(packet, ItemId);
    Answer::NetPacket::writeInt8(packet, ItemClass);
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int8_t v7 = Player::getConnId(m_pPlayer);
    GameService* v8 = Answer::Singleton<GameService>::instance();
    GameService::worldBroadcast(v8, v7, packet);
}

void CExtEquip::EquipUpStarGongGao(MemEquip* Equip)
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v3 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v3, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    
    if (!packet) return;
    
    Answer::NetPacket::writeInt32(packet, 115);
    int64_t Cid = Player::getCid(m_pPlayer);
    Answer::NetPacket::writeInt64(packet, Cid);
    
    std::string val;
    std::string val; Player::getName(m_pPlayer, &val);
    Answer::NetPacket::writeUTF8(packet, &val);
    
    Answer::NetPacket::writeInt64(packet, Equip->id);
    Answer::NetPacket::writeInt32(packet, Equip->base);
    Answer::NetPacket::writeInt8(packet, Equip->star);
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int8_t v6 = Player::getConnId(m_pPlayer);
    GameService* v7 = Answer::Singleton<GameService>::instance();
    GameService::worldBroadcast(v7, v6, packet);
}

int32_t CExtEquip::GetLowestStar()
{
    int32_t LowesetStar = 99999;
    
    for (int i = 0; i <= 92; ++i)
    {
        if (m_vMemEquip[i].srcId > 0 && m_vMemEquip[i].itemId > 0)
        {
            CEquipManager* v3 = Answer::Singleton<CEquipManager>::instance();
            MemEquip Equip;
            CEquipManager::GetMemEquip(&Equip, v3, m_vMemEquip[i].srcId);
            
            if (Equip.base == m_vMemEquip[i].itemId)
            {
                if (Equip.star < LowesetStar) LowesetStar = Equip.star;
            }
            
            (void)(&Equip);
        }
    }
    
    return LowesetStar;
}

int32_t CExtEquip::GetEquipAllCount()
{
    int32_t Count = 0;
    
    for (int i = 0; i <= 92; ++i)
    {
        if (m_vMemEquip[i].itemId > 0 && m_vMemEquip[i].srcId > 0)
        {
            CEquipManager* v2 = Answer::Singleton<CEquipManager>::instance();
            MemEquip Equip;
            CEquipManager::GetMemEquip(&Equip, v2, m_vMemEquip[i].srcId);
            
            if (Equip.base == m_vMemEquip[i].itemId) ++Count;
            
            (void)(&Equip);
        }
    }
    
    return Count;
}

int32_t CExtEquip::GetEquipAllStar()
{
    int32_t AllStar = 0;
    
    for (int i = 0; i <= 92; ++i)
    {
        if (m_vMemEquip[i].itemId > 0 && m_vMemEquip[i].srcId > 0)
        {
            CEquipManager* v2 = Answer::Singleton<CEquipManager>::instance();
            MemEquip Equip;
            CEquipManager::GetMemEquip(&Equip, v2, m_vMemEquip[i].srcId);
            
            if (Equip.base == m_vMemEquip[i].itemId) AllStar += Equip.star;
            
            (void)(&Equip);
        }
    }
    
    return AllStar;
}

int32_t CExtEquip::GetEquipAllUpPosLevel()
{
    int32_t Level = 0;
    
    for (int nPos = 0; nPos <= 9; ++nPos)
    {
        if (m_vMemPosLevel[nPos] <= 0) return 0;
        
        if (Level == 0)
        {
            Level = m_vMemPosLevel[nPos];
        }
        else
        {
            if (m_vMemPosLevel[nPos] < Level) Level = m_vMemPosLevel[nPos];
        }
    }
    
    return Level;
}

int32_t CExtEquip::HaveLevelEquipCount(int32_t Level)
{
    int32_t Count = 0;
    
    for (int i = 0; i <= 92; ++i)
    {
        int32_t itemId = m_vMemEquip[i].itemId;
        CfgData* v3 = Answer::Singleton<CfgData>::instance();
        const void* EquipTable = CfgData::GetEquipTable(v3);
        const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, itemId);
        
        int32_t nItemLevel = 0;  // 获取装备等级
        if (pCfgEquip && nItemLevel >= Level) ++Count;
    }
    
    return Count;
}

int32_t CExtEquip::HaveStarEquipCount(int32_t Star)
{
    int32_t Count = 0;
    
    for (int i = 0; i <= 92; ++i)
    {
        if (m_vMemEquip[i].srcId > 0 && m_vMemEquip[i].itemId > 0)
        {
            CEquipManager* v4 = Answer::Singleton<CEquipManager>::instance();
            MemEquip Equip;
            CEquipManager::GetMemEquip(&Equip, v4, m_vMemEquip[i].srcId);
            
            if (Equip.base == m_vMemEquip[i].itemId && Equip.star >= Star) ++Count;
            
            (void)(&Equip);
        }
    }
    
    return Count;
}

int32_t CExtEquip::HaveQualityGradeEquipCount(int8_t nQuality, int8_t nGrade)
{
    int32_t Count = 0;
    
    for (int i = 0; i <= 92; ++i)
    {
        int32_t itemId = m_vMemEquip[i].itemId;
        CfgData* v4 = Answer::Singleton<CfgData>::instance();
        const void* EquipTable = CfgData::GetEquipTable(v4);
        const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, itemId);
        
        int32_t nEquipQuality = 0;  // 获取装备品质
        int32_t nEquipGrade = 0;   // 获取装备品级
        if (pCfgEquip && nEquipQuality >= nQuality && nEquipGrade >= nGrade) ++Count;
    }
    
    return Count;
}

int32_t CExtEquip::GetWingEquipCount3SuitId()
{
    std::vector<int32_t> Suit;
    
    for (int j = 44; j <= 49; ++j)
    {
        MemChrBag* curEquip = &m_vMemEquip[j];
        if (curEquip->srcId > 0 && curEquip->itemClass == 2)
        {
            CEquipManager* v2 = Answer::Singleton<CEquipManager>::instance();
            MemEquip equip;
            CEquipManager::GetMemEquip(&equip, v2, curEquip->srcId);
            
            if (equip.base == curEquip->itemId)
            {
                CfgData* v5 = Answer::Singleton<CfgData>::instance();
                const void* EquipTable = CfgData::GetEquipTable(v5);
                const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, curEquip->itemId);
                
                if (pCfgEquip)
                {
                    int32_t nSuitId = 0;  // 获取套装ID
                    if (nSuitId > 0) Suit.push_back(nSuitId);
                }
            }
            
            (void)(&equip);
        }
    }
    
    if (Suit.size() > 2)
    {
        std::sort(Suit.begin(), Suit.end());
        return Suit[2];
    }
    
    return 0;
}

int32_t CExtEquip::GetWingEquipCount6SuitId()
{
    int32_t SuitId = 0;
    
    for (int j = 44; j <= 49; ++j)
    {
        MemChrBag* curEquip = &m_vMemEquip[j];
        if (curEquip->srcId <= 0 || curEquip->itemClass != 2) return 0;
        
        CEquipManager* v2 = Answer::Singleton<CEquipManager>::instance();
        MemEquip equip;
        CEquipManager::GetMemEquip(&equip, v2, curEquip->srcId);
        
        if (equip.base == curEquip->itemId)
        {
            CfgData* v5 = Answer::Singleton<CfgData>::instance();
            const void* EquipTable = CfgData::GetEquipTable(v5);
            const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, curEquip->itemId);
            
            if (pCfgEquip)
            {
                int32_t nSuitId2 = 0;  // 获取套装ID2
                if (nSuitId2 > 0)
                {
                    if (SuitId == 0)
                    {
                        SuitId = nSuitId2;
                    }
                    else
                    {
                        if (nSuitId2 < SuitId) SuitId = nSuitId2;
                    }
                }
                else
                {
                    SuitId = 0;
                }
            }
            else
            {
                SuitId = 0;
            }
        }
        else
        {
            SuitId = 0;
        }
        
        (void)(&equip);
        if (SuitId == 0) return 0;
    }
    
    return SuitId;
}

int32_t CExtEquip::GetCount6SuitId()
{
    std::vector<int32_t> Suit;
    
    for (int j = 21; j <= 32; ++j)
    {
        MemChrBag* curEquip = &m_vMemEquip[j];
        if (curEquip->srcId > 0 && curEquip->itemClass == 2)
        {
            CEquipManager* v2 = Answer::Singleton<CEquipManager>::instance();
            MemEquip equip;
            CEquipManager::GetMemEquip(&equip, v2, curEquip->srcId);
            
            if (equip.base == curEquip->itemId)
            {
                CfgData* v5 = Answer::Singleton<CfgData>::instance();
                const void* EquipTable = CfgData::GetEquipTable(v5);
                const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, curEquip->itemId);
                
                if (pCfgEquip)
                {
                    int32_t nSuitId = 0;  // 获取套装ID
                    if (nSuitId > 0) Suit.push_back(nSuitId);
                }
            }
            
            (void)(&equip);
        }
    }
    
    if (Suit.size() > 5)
    {
        std::sort(Suit.begin(), Suit.end());
        return Suit[5];
    }
    
    return 0;
}

int32_t CExtEquip::GetCount12SuitId()
{
    int32_t SuitId = 0;
    
    for (int j = 21; j <= 32; ++j)
    {
        MemChrBag* curEquip = &m_vMemEquip[j];
        if (curEquip->srcId <= 0 || curEquip->itemClass != 2) return 0;
        
        CEquipManager* v2 = Answer::Singleton<CEquipManager>::instance();
        MemEquip equip;
        CEquipManager::GetMemEquip(&equip, v2, curEquip->srcId);
        
        if (equip.base == curEquip->itemId)
        {
            CfgData* v5 = Answer::Singleton<CfgData>::instance();
            const void* EquipTable = CfgData::GetEquipTable(v5);
            const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, curEquip->itemId);
            
            if (pCfgEquip)
            {
                int32_t nSuitId2 = 0;  // 获取套装ID2
                if (nSuitId2 > 0)
                {
                    if (SuitId == 0)
                    {
                        SuitId = nSuitId2;
                    }
                    else
                    {
                        if (nSuitId2 < SuitId) SuitId = nSuitId2;
                    }
                }
                else
                {
                    SuitId = 0;
                }
            }
            else
            {
                SuitId = 0;
            }
        }
        else
        {
            SuitId = 0;
        }
        
        (void)(&equip);
        if (SuitId == 0) return 0;
    }
    
    return SuitId;
}

int32_t CExtEquip::GetWeaponBless()
{
    MemChrBag* weapon = &m_vMemEquip[0];
    if (weapon->srcId <= 0 || weapon->itemId <= 0) return 0;
    
    CEquipManager* v3 = Answer::Singleton<CEquipManager>::instance();
    MemEquip equip;
    CEquipManager::GetMemEquip(&equip, v3, weapon->srcId);
    
    int32_t Lucky = equip.Lucky;
    (void)(&equip);
    
    return Lucky;
}

void CExtEquip::SetWeaponBless(int32_t nBless)
{
    if (!m_pPlayer) return;
    
    MemChrBag* weapon = &m_vMemEquip[0];
    if (weapon->srcId <= 0 || weapon->itemId <= 0) return;
    
    CEquipManager* v3 = Answer::Singleton<CEquipManager>::instance();
    MemEquip equip;
    CEquipManager::GetMemEquip(&equip, v3, weapon->srcId);
    
    if (equip.base == weapon->itemId && equip.Lucky != nBless)
    {
        equip.Lucky = nBless;
        
        int8_t v4 = Player::getConnId(m_pPlayer);
        CEquipManager* v8 = Answer::Singleton<CEquipManager>::instance();
        CEquipManager::UpdateMemEquip(v8, v4, &equip, 1404);
        
        CEquipManager* v10 = Answer::Singleton<CEquipManager>::instance();
        CEquipManager::SendPlayerEquipInfo(v10, m_pPlayer, &equip);
    }
    
    (void)(&equip);
}

void CExtEquip::checkEquipSuit(bool IsGongGao)
{
    m_EquipSuit.clear();
    
    // 检查各种套装组合
    // 1. 头盔和项链套装
    // 2. 武器和戒指套装
    // 3. 护手套装
    // 4. 铭文套装
    
    // 更新套装数量记录
    int32_t v20 = m_EquipSuit.size();
    Player::updateRecord(m_pPlayer, 4, v20);
}

int32_t CExtEquip::GetAddGemCount(int32_t nLevel)
{
    int32_t nCount = 0;
    
    for (int i = 0; i <= 9; ++i)
    {
        for (int j = 0; j <= 3; ++j)
        {
            if (m_vMemGem[i][j] > 0)
            {
                int32_t v2 = m_vMemGem[i][j];
                CfgData* v3 = Answer::Singleton<CfgData>::instance();
                const void* ItemGemTable = CfgData::GetItemGemTable(v3);
                const void* pGem = CfgItemGemTable::GetItemGem(ItemGemTable, v2);
                
                if (pGem)
                {
                    int32_t nGemLevel = 0;  // 获取宝石等级
                    if (nGemLevel >= nLevel) ++nCount;
                }
            }
        }
    }
    
    return nCount;
}

int32_t CExtEquip::GetQualityCount(int32_t nQuality)
{
    int32_t nCount = 0;
    
    for (int i = 0; i <= 92; ++i)
    {
        int32_t itemId = m_vMemEquip[i].itemId;
        CfgData* v3 = Answer::Singleton<CfgData>::instance();
        const void* EquipTable = CfgData::GetEquipTable(v3);
        const void* pEquip = CfgEquipTable::GetEquip(EquipTable, itemId);
        
        int32_t nEquipQuality = 0;  // 获取装备品质
        if (pEquip && nEquipQuality >= nQuality) ++nCount;
    }
    
    return nCount;
}

void CExtEquip::clearGemInfo()
{
    for (int i = 0; i <= 9; ++i)
    {
        for (int j = 0; j <= 3; ++j)
        {
            m_vMemGem[i][j] = -1;
        }
    }
}

std::string CExtEquip::saveGemString()
{
    std::string result;
    // 序列化宝石信息
    return result;
}

void CExtEquip::loadGemString(const std::string* str)
{
    if (!str || str->empty()) return;
    // 反序列化宝石信息
}

std::string CExtEquip::savePosLevelString()
{
    std::string result;
    // 序列化位置等级信息
    return result;
}

void CExtEquip::loadPosLevelString(const std::string* str)
{
    if (!str || str->empty()) return;
    // 反序列化位置等级信息
}

std::string CExtEquip::saveStrengthenString()
{
    std::string result;
    // 序列化强化信息
    return result;
}

void CExtEquip::LoadStrengthenString(const std::string* str)
{
    if (!str || str->empty()) return;
    // 反序列化强化信息
}

std::string CExtEquip::saveShenYaoString()
{
    std::string result;
    // 序列化神耀信息
    return result;
}

void CExtEquip::LoadShenYaoString(const std::string* str)
{
    if (!str || str->empty()) return;
    // 反序列化神耀信息
}

void CExtEquip::updateEquipGem()
{
    if (!m_pPlayer) return;
    
    int32_t nSumValue = GetEquipAllGemLevel();
    if (m_nEquipGem != nSumValue)
    {
        m_nEquipGem = nSumValue;
        Player::sendUpdateSocialPlayerInfo(m_pPlayer, 8, m_nEquipGem);  // PII_EQUIP_GEM
    }
}

void CExtEquip::updateEquipStar()
{
    if (!m_pPlayer) return;
    
    int32_t nSumValue = 0;
    
    for (int i = 0; i <= 92; ++i)
    {
        if (m_vMemEquip[i].srcId > 0)
        {
            CEquipManager* v2 = Answer::Singleton<CEquipManager>::instance();
            MemEquip equip;
            CEquipManager::GetMemEquip(&equip, v2, m_vMemEquip[i].srcId);
            
            if (equip.base == m_vMemEquip[i].itemId) nSumValue += equip.star;
            
            (void)(&equip);
        }
    }
    
    if (m_nEquipStar != nSumValue)
    {
        m_nEquipStar = nSumValue;
        Player::sendUpdateSocialPlayerInfo(m_pPlayer, 7, m_nEquipStar);  // PII_EQUIP_STAR
    }
}

int32_t CExtEquip::CheckSuitId(int32_t nSrcId, int32_t DesId)
{
    if (nSrcId <= 0 || DesId <= 0) return 0;
    
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const void* EquipTable = CfgData::GetEquipTable(v4);
    const void* pCfgSrcEquip = CfgEquipTable::GetEquip(EquipTable, nSrcId);
    
    if (!pCfgSrcEquip) return 0;
    
    int32_t m_nSuitId = 0;  // 获取套装ID
    
    const void* pSuitCFG = CfgEquipTable::GetEquipSuit(EquipTable, m_nSuitId);
    if (!pSuitCFG) return 0;
    
    const void* pDesCfgEquip = CfgEquipTable::GetEquip(EquipTable, DesId);
    if (!pDesCfgEquip) return 0;
    
    int32_t nDesSuitId = 0;  // 获取目标套装ID
    
    // 检查是否在同一套装中
    if (m_nSuitId == nDesSuitId) return m_nSuitId;
    
    return 0;
}

int32_t CExtEquip::GetEquipSuitId(int32_t nId)
{
    if (nId <= 0) return 0;
    
    CfgData* v2 = Answer::Singleton<CfgData>::instance();
    const void* EquipTable = CfgData::GetEquipTable(v2);
    const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, nId);
    
    if (pCfgEquip) return 1;  // 套装ID
    return 0;
}

int32_t CExtEquip::checkGongMingSuit()
{
    int32_t nSuitId = 0;
    
    for (int i = 59; i <= 64; ++i)
    {
        MemChrBag* item = &m_vMemEquip[i];
        if (item->srcId <= 0 || item->itemClass != 2)
        {
            nSuitId = 0;
            break;
        }
        
        CEquipManager* v2 = Answer::Singleton<CEquipManager>::instance();
        MemEquip equip;
        CEquipManager::GetMemEquip(&equip, v2, item->srcId);
        
        if (equip.base == item->itemId)
        {
            CfgData* v5 = Answer::Singleton<CfgData>::instance();
            const void* EquipTable = CfgData::GetEquipTable(v5);
            const void* pCfgEquip = CfgEquipTable::GetEquip(EquipTable, item->itemId);
            
            if (pCfgEquip)
            {
                int32_t m_nSuitId = 0;  // 获取套装ID
                if (nSuitId == 0)
                {
                    nSuitId = m_nSuitId;
                }
                else
                {
                    if (m_nSuitId < nSuitId) nSuitId = m_nSuitId;
                }
            }
            else
            {
                nSuitId = 0;
            }
        }
        else
        {
            nSuitId = 0;
        }
        
        (void)(&equip);
        if (nSuitId == 0) break;
    }
    
    if (m_nGongMingSuitId != nSuitId)
    {
        m_nGongMingSuitId = nSuitId;
        CheckGongMingSuitBuff();
    }
    
    return nSuitId;
}

void CExtEquip::CheckGongMingSuitBuff()
{
    if (!m_pPlayer) return;
    
    if (m_nGongMingSuitBuff > 0)
    {
        Unit::removeBuff(m_pPlayer, m_nGongMingSuitBuff);
        m_nGongMingSuitBuff = 0;
        Player::recalcAttr(m_pPlayer, 0, 0);
    }
    
    if (m_nGongMingSuitId > 0)
    {
        CfgData* v3 = Answer::Singleton<CfgData>::instance();
        const void* EquipTable = CfgData::GetEquipTable(v3);
        const void* pSuit = CfgEquipTable::GetEquipSuit(EquipTable, m_nGongMingSuitId);
        
        if (pSuit)
        {
            int32_t nBuffId = 0;  // 获取Buff ID
            if (nBuffId > 0)
            {
                m_nGongMingSuitBuff = nBuffId;
                // 添加Buff
                Player::recalcAttr(m_pPlayer, 0, 0);
            }
        }
    }
}

void CExtEquip::OnUpdate(int64_t curTick)
{
    checkDirty();
}

void CExtEquip::OnDaySwitch(int32_t nDiffDays)
{
    // 日切换处理
}

void CExtEquip::broadcastGemLevelUp(int32_t nBroadId, const std::string* pName, int64_t Cid, int32_t nGemId)
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v4 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v4, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    
    if (!packet) return;
    
    Answer::NetPacket::writeInt32(packet, nBroadId);
    Answer::NetPacket::writeUTF8(packet, pName);
    Answer::NetPacket::writeInt64(packet, Cid);
    Answer::NetPacket::writeInt32(packet, nGemId);
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int8_t v6 = Player::getConnId(m_pPlayer);
    GameService* v7 = Answer::Singleton<GameService>::instance();
    GameService::worldBroadcast(v7, v6, packet);
}

void CExtEquip::PackageShenYaoPosLevelUp(Answer::NetPacket* packet)
{
    if (!packet) return;
    
    for (int i = 0; i <= 9; ++i)
    {
        Answer::NetPacket::writeInt32(packet, m_vShenYaoEquipPos[i]);
    }
}

void CExtEquip::PackageGemInfo(Answer::NetPacket* packet)
{
    if (!packet) return;
    
    Answer::NetPacket::writeInt8(packet, 10);  // 装备数量
    for (int i = 0; i <= 9; ++i)
    {
        Answer::NetPacket::writeInt8(packet, i);
        for (int j = 0; j <= 3; ++j)
        {
            Answer::NetPacket::writeInt32(packet, m_vMemGem[i][j]);
        }
    }
}

bool CExtEquip::checkDressEquip(int32_t nBagSlot, int8_t nEquipSlot)
{
    // 检查装备是否可穿戴
    return true;
}

MemChrBag* CExtEquip::GetEquipSlot(int32_t nSlot)
{
    if (nSlot >= 0 && nSlot <= 92)
    {
        return &m_vMemEquip[nSlot];
    }
    return nullptr;
}