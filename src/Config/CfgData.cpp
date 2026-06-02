#include "Config/CfgData.h"







#include "Answer/Singleton.h"







#include "Answer/Logger.h"







#include "Answer/DayTime.h"







#include "Answer/MySqlDBGuard.h"







#include "Answer/MySqlQuery.h"







#include "CTimer.h"







#include "DB/DBService.h"







#include "GameService/GameService.h"







#include "ItemEffectManager.h"







#include "COpenBeta.h"







#include "CCardGroupBoxManager.h"







#include "CItemHelper.h"







#include "CDBCFile.h"







#include "Config/CfgFestivalActivityTable.h"







#include "Utility/StringUtility.h"







#include "Common/Random.h"







#include <cstring>







#include <algorithm>















// ==================== 构造函数 ====================















CfgData::CfgData()







{}















// ==================== 析构函数 ====================















CfgData::~CfgData()







{}















// ==================== 单例与初始化 ====================















// ==================== Stub implementations ====================















void CfgData::parseEquipItemString(MemChrEquipBagVector* result, CfgData* cfg, int32_t nIndex, const std::string* strItems)







{







    // TODO: implement from gameserver.cc







    (void)result; (void)cfg; (void)nIndex; (void)strItems;







}















void CfgData::parseGongGaoString(std::list<CfgGongGao>* result, int32_t nIndex, const std::string* strItems)







{







    // TODO: implement from gameserver.cc







    (void)result; (void)nIndex; (void)strItems;







}















// ==================== 单例与初始化 ====================















CfgData *CfgData::instance()







{







    return Answer::Singleton<CfgData>::instance();







}















bool CfgData::init(int32_t line, int32_t debug)







{







    fetchServerConfig(line);







    fetchActivity();







    fetchBuff();







    fetchDungeon();







    fetchDungeonEvent();







    fetchDungeonMonster();







    fetchDungeonPlant();







    fetchDungeonTrap();







    fetchDungeonNpc();







    fetchItem(0);







    fetchJob();







    fetchLevelAttr();







    fetchMap();







    fetchMapMonster();







    fetchMapPlant();







    fetchChrShop();







    fetchMapRegion();







    fetchMonster();







    fetchMonsterAI();







    fetchMonsterAdjustTable();







    fetchMonsterDropGroup();







    fetchMonsterGroupDrop();







    fetchMonsterTaskDrop();







    fetchNpc();







    fetchMovie();







    fetchPlant();







    fetchTask();







    fetchTrap();







    fetchLevelExp();







    fetchSignReward();







    InitGroupIconTable();







    InitActiveSkillTable();







    InitPassiveSkillTable();







    InitTrigSkillTable();







    InitTalentTable();







    InitTalentPageTable();







    InitFamilySkillTable();







    InitTalentActiveTable();







    InitEquipTable();







    InitEquipUpStarTable();







    InitEquipSuitTable();







    InitEquipDecomposeTable();







    InitEquipUpPosTable();







    InitEquipPosSuitTable();







    InitEquipBoxTable();







    InitItemGemTable();







    InitGemOpenHoleTable();







    InitGemLevelUpTable();







    InitGemSuitTable();







    InitItemCombiTable();







    InitMonsterRandTable();







    InitCarrierTable();







    InitCarrierAttrTable();







    InitJueWeiTable();







    InitPetTable();







    InitPetAttrTable();







    InitPetUpStarTable();







    InitSkillLevelUpTable();







    InitTaskCycleStarTable();







    InitTaskCycleRewardTable();







    InitFamilyTable();







    InitFamilyWarJoinRewardTable();







    InitFamilyWarReliveTable();







    InitFamilyLightExpTable();







    InitHorseRacingRewardTable();







    InitCampWarRewardTable();







    InitCampWarContKillTable();







    InitCityWarContRewardTable();







    InitHallOfFameRewardTable();







    InitTitleTable();







    InitGoldEggTable();







    InitLimitTimeTable();







    InitMysteryShopTable();







    InitBuyGiftTable();







    InitExchangeTable();







    InitMysteryGiftTable();







    InitDrawTable();







    InitMapRoadTable();







    InitTrailerTable();







    InitMYSJRewardTable();







    InitMaintainCompensateTable();







    InitWishRewardTable();







    InitBFZLEnterCostTable();







    InitBlacketMarketTable();







    InitLevelChatTimesTable();







    InitSuperMemberTable();







    InitSouGouSkinTable();







    InitPetEquipTable();







    InitWeiXinTable();







    InitAdultGiftTable();







    InitQQZoneRewardTable();







    InitQQGameRewardTable();







    InitYellowRewardTable();







    InitYellowDailyRewardTable();







    InitYellowLevelRewardTable();







    InitBlueRewardTable();







    InitBlueDailyRewardTable();







    InitBlueLevelRewardTable();







    InitTGPRewardTable();







    InitTGPDailyRewardTable();







    InitTGPLevelRewardTable();







    InitFamilyDungeonTable();







    InitDropRecordTable();







    InitDungeonScoreTable();







    InitDungeonSummon();







    InitSysMail();







    InitBossInfo();







    InitFaBaoTable();







    InitDaTiHD();







    InitShangChengTable();







    InitOnLimeReward();







    InitSevenLoginReward();







    InitLevelGift();







    InitWeekOnlineReward();







    InitOffLineExpTable();







    InitVipCardTable();







    InitFunctionOpenTable();







    InitPlantEventTable();







    InitZiYuanZhaoHuiTable();







    InitMoHuaHuanYiTable();







    InitShouChongLiBao();







    InitNewServerFavorable();







    InitMobilePhoneGiftTable();







    InitEveryDayChongZhi();







    InitKaiFuHuoDongTable();







    InitHuoYueDuTable();







    InitAppendAttrTable();







    InitFamilyRewardTable();







    InitTotalChongZhiTable();







    InitSpecialTreasureMapRandTable();







    InitMonsterAddAttrTable();







    InitDaZheQuanTable();







    Init360RewardTable();







    Init360RewardTypeTable();







    InitVplanTable();







    InitShunWangTable();







    InitSpeed360Reward();







    InitTencentSevenDayLoginTable();







    InitGoblinData();







    InitSpecialMonster();







    InitEquipUpPhase();







    InitTouZiTable();







    InitGroupMonster();







    InitPkDropRateTable();







    InitEquipBackTable();







    InitStrengthenTable();







    InitBuyFaBaoResTable();







    InitXingMaiTable();







    InitXingMaiSlotTable();







    InitTianLingTable();







    InitGongMinTable();







    InitBeastShrineEnterCostTable();







    InitDamnationTable();







    InitAttrBattleTable();







    InitScoreShopTable();







    InitChouJiangTable();







    InitWingCfgTable();







    InitLibraryTable();







    InitTreasureMapTabale();







    InitEquipBackTask();







    InitSpeciaEquipCfgMap();







    InitSpeciaSkillDistanceTable();







    InitLuckDropTable();







    InitLevelRefiningTable();







    InitBossFirstKilledTable();







    InitUltimateChallengeCfg();







    InitCachetCfg();







    InitBossKilledReward();







    InitSystemOpenReward();







    InitSpecialMap();







    InitTestServerReward();







    InitEquipBlessTable();







    InitGuWuCfgMap();







    InitFunctionOpenMailMap();







    InitXianYaoTaskTable();







    InitShenWeiTaskTable();







    InitMiniClientTable();







    InitWuHunShopTable();







    InitWuHunItemTable();







    InitWuHunCreateTable();







    InitBossDistribution();







    InitSpecialBossMapCfgMap();







    InitDuiHuanLimitTable();







    InitSuperTeHuiTable();







    InitJewelPavilionTable();







    InitGoblinTable();







    InitShouHuRefining();







    InitWingEquipPolish();







    InitGuiGuDaoRenTable();







    InitShiZhuangTable();







    InitShiZhuangLevelTable();







    InitMonthlyChouJiangTable();







    InitActDropTable();







    InitSevenTaskTable();







    CQiFuTable::InitQiFuTable(&this->m_QiFuTable);







    VipTable::InitVipTable(&this->m_VipTable);







    InitOutLinkFestivalTable();







    InitFestivalActivityTable();







    InitYYDaTing();







    InitYYVip();







    InitSouGouDaTing();







    InitXunLeiTable();







    InitCycleTowerTable();







    InitPlatformDaTingMap();







    InitMiniClientMap();







    InitPlatformVipMap();







    InitShangRenCfgMap();







    InitSelectItemCfgMap();







    InitGuardPrivilegeTable();







    InitPlatformReward();







    InitDiligenceCfgMap();







    InitFamilyDonateTable();







    InitFamilyMedalTable();







    InitHoeTable();







    InitMingGeTable();







    InitCrossTowerCfgMap();







    InitKunLingTable();







    InitLaDaShiHuiYuan();







    InitFlopTable();







    InitMoFuTable();







    InitChristmasDuiHuanTable();







    InitBaoKuFuBen();







    InitChargeDungeon();







    InitShenYaoPosTable();







    InitShenYaoPosSuitTable();







    InitTongTianChiRanTable();







    InitXinMoTable();







    InitGameTable();







    InitRefreshMonsterCfgListMap();







    InitLittleHelperCfg();







    InitShiZhuLevelUp();







    InitNationalDayTask();







    InitRongHeCfg();







    this->m_MapMonsterId = 100000;







    this->m_debug = debug;







    return 1;







}















void CfgData::reload()







{







    ItemEffectManager *v1;















    fetchItem(1);







    v1 = Answer::Singleton<ItemEffectManager>::instance();







    v1->init();







}















// ==================== Getter 方法示例 ====================















int32_t CfgData::GetMapMonsterId()







{







    return ++this->m_MapMonsterId;







}















CfgActivity *CfgData::getActivity(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_activities.find(ida);







    if (it != this->m_activities.end())







    {







        return &it->second;







    }







    return nullptr;







}







// ==================== Getter 方法 ====================















const CfgMapEventListT *CfgData::getActivityEvents(int32_t activity_id, MapId_t nMapId)







{







    MapId_t nMapIda = nMapId;







    int32_t activity_ida = activity_id;







    







    auto it = this->m_activityEvents.find(activity_ida);







    if (it == this->m_activityEvents.end())







    {







        return &this->m_emptyEvents;







    }







    







    auto itMap = it->second.find(nMapIda);







    if (itMap == it->second.end())







    {







        return &this->m_emptyEvents;







    }







    







    return &itMap->second;







}















CfgActivityMonster *CfgData::getActivityMonster(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_activityMonsters.find(ida);







    if (it != this->m_activityMonsters.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgActivityNpc *CfgData::getActivityNpc(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_activityNpcs.find(ida);







    if (it != this->m_activityNpcs.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgActivityPlant *CfgData::getActivityPlant(int32_t Id)







{







    int32_t Ida = Id;







    auto it = this->m_activityPlants.find(Ida);







    if (it != this->m_activityPlants.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgActivityTrap *CfgData::getActivityTrap(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_activityTraps.find(ida);







    if (it != this->m_activityTraps.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgBuff *CfgData::getBuff(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_buffs.find(ida);







    if (it != this->m_buffs.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgDungeon *CfgData::getDungeon(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_dungeons.find(ida);







    if (it != this->m_dungeons.end())







    {







        return &it->second;







    }







    return nullptr;







}















const CfgDungeon *CfgData::GetUpTowerDungeon(int32_t nFloor)







{







    int32_t nFloora = nFloor;







    auto it = this->m_mUpTowerDungeon.find(nFloora);







    if (it != this->m_mUpTowerDungeon.end())







    {







        return &it->second;







    }







    return nullptr;







}















const CfgMapEventListT *CfgData::getDungeonEvent(int32_t dungonid)







{







    int32_t dungonida = dungonid;







    auto it = this->m_dungeonEvents.find(dungonida);







    if (it != this->m_dungeonEvents.end())







    {







        return &it->second;







    }







    return &this->m_emptyEvents;







}















CfgDungeonMonster *CfgData::getDungeonMonster(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_dungeonMonsters.find(ida);







    if (it != this->m_dungeonMonsters.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgDungeonPlant *CfgData::getDungeonPlant(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_dungeonPlants.find(ida);







    if (it != this->m_dungeonPlants.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgDungeonTrap *CfgData::getDungeonTrap(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_dungeonTraps.find(ida);







    if (it != this->m_dungeonTraps.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgDungeonNpc *CfgData::getDungeonNpc(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_dungeonNpcs.find(ida);







    if (it != this->m_dungeonNpcs.end())







    {







        return &it->second;







    }







    return nullptr;







}















const CfgEquip *CfgData::getEquip(int32_t id)







{







    return static_cast<const CfgEquip*>(CfgEquipTable::GetEquip(&this->m_cfgEquip, id));







}















CfgChrShop *CfgData::getChrShop(int32_t Index)







{







    int32_t Indexa = Index;







    auto it = this->m_chrShops.find(Indexa);







    if (it != this->m_chrShops.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgItemTable *CfgData::getAllItem()







{







    Answer::RwLockRdGuard lock(&this->m_itemsLock);







    return &this->m_items;







}















CfgItem *CfgData::getItem(int32_t id)







{







    if (id <= 0)







        return nullptr;







    







    int32_t ida = id;







    Answer::RwLockRdGuard lock(&this->m_itemsLock);







    







    auto it = this->m_items.find(ida);







    if (it != this->m_items.end())







    {







        CfgItem *second = it->second;







            return second;







    }







    







    return nullptr;







}















CfgItemGiftVector *CfgData::getItemGift(int32_t id)







{







    int32_t ida = id;







    Answer::RwLockRdGuard lock(&this->m_itemGiftsLock);







    







    auto it = this->m_itemGifts.find(ida);







    if (it != this->m_itemGifts.end())







    {







        CfgItemGiftVector *second = it->second;







            return second;







    }







    







    return nullptr;







}















CfgItemGiftRandomVector *CfgData::getItemGiftRandom(int32_t id)







{







    int32_t ida = id;







    Answer::RwLockRdGuard lock(&this->m_itemGiftRandomsLock);







    







    auto it = this->m_itemGiftRandoms.find(ida);







    if (it != this->m_itemGiftRandoms.end())







    {







        CfgItemGiftRandomVector *second = it->second;







            return second;







    }







    







    return nullptr;







}















CfgJob *CfgData::getJob(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_jobs.find(ida);







    if (it != this->m_jobs.end())







    {







        return &it->second;







    }







    return nullptr;







}















bool CfgData::getMovie(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_movie.find(ida);







    return it != this->m_movie.end();







}















CfgMap *CfgData::getMap(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_maps.find(ida);







    if (it != this->m_maps.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMapMonsterVector *CfgData::getMonstersOnMap(int32_t mapid)







{







    int32_t mapida = mapid;







    auto it = this->m_mapMonsters.find(mapida);







    if (it != this->m_mapMonsters.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMapMonster *CfgData::GetMapMonsterInfo(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_CfgMapMonsters.find(ida);







    if (it != this->m_CfgMapMonsters.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMapPlantVector *CfgData::getPlantOnMap(int32_t mapid)







{







    int32_t mapida = mapid;







    auto it = this->m_mapPlants.find(mapida);







    if (it != this->m_mapPlants.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMapPlant *CfgData::GetMapPlantInfo(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_mMapPlants.find(ida);







    if (it != this->m_mMapPlants.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMapRegion *CfgData::getMapRegion(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_mapRegions.find(ida);







    if (it != this->m_mapRegions.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMapRegionVector *CfgData::getRegionOnMap(int32_t mapid)







{







    int32_t mapida = mapid;







    auto it = this->m_mapRegionsByMapId.find(mapida);







    if (it != this->m_mapRegionsByMapId.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMonster *CfgData::getMonster(int32_t mid)







{







    int32_t mida = mid;







    auto it = this->m_monsters.find(mida);







    if (it != this->m_monsters.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMonsterAI *CfgData::GetMonsterAI(int32_t nId)







{







    int32_t nIda = nId;







    auto it = this->m_mMonsterAI.find(nIda);







    if (it != this->m_mMonsterAI.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMonsterAdjust *CfgData::GetMonsterAdjust(int32_t mid, int32_t adj_level, bool bAutoLow)







{







    auto key = std::make_pair(mid, adj_level);







    auto it = this->m_mMonsterAdjust.find(key);







    if (it != this->m_mMonsterAdjust.end())







    {







        return const_cast<CfgMonsterAdjust*>(&it->second);







    }







    







    if (bAutoLow)







    {







        int32_t nLevel = 0;







        CfgMonsterAdjust *pAutoLow = nullptr;







        







        for (auto& pair : this->m_mMonsterAdjust)







        {







            if (pair.first.first == mid && pair.first.second > nLevel && pair.first.second < adj_level)







            {







                nLevel = pair.first.second;







                pAutoLow = const_cast<CfgMonsterAdjust*>(&pair.second);







            }







        }







        return pAutoLow;







    }







    







    return nullptr;







}















bool CfgData::isMonsterBroadcast(int32_t mid)







{







    int32_t mida = mid;







    auto it = std::find(this->m_monsterBroadcasts.begin(), this->m_monsterBroadcasts.end(), mida);







    return it != this->m_monsterBroadcasts.end();







}















CfgDungeonReward *CfgData::getDungeonReward(int32_t dungeonID)







{







    int32_t dungeonIDa = dungeonID;







    auto it = this->m_dungeonReward.find(dungeonIDa);







    if (it != this->m_dungeonReward.end())







    {







        return &it->second;







    }







    return nullptr;







}















const CfgDungeonDrop *CfgData::randDungeonDrop(int32_t dungeonID)







{







    int32_t dungeonIDa = dungeonID;







    auto it = this->m_dungeonDropGroup.find(dungeonIDa);







    if (it == this->m_dungeonDropGroup.end())







    {







        return nullptr;







    }







    return it->second.RandDrop();







}















CfgMonsterDropGroupVector *CfgData::getMonsterDropGroup(int32_t group_id)







{







    int32_t group_ida = group_id;







    auto it = this->m_monsterDropGroups.find(group_ida);







    if (it != this->m_monsterDropGroups.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMonsterGroupDropVector *CfgData::getMonsterGroupDrop(int32_t mid)







{







    int32_t mida = mid;







    auto it = this->m_monsterGroupDrops.find(mida);







    if (it != this->m_monsterGroupDrops.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgMonsterTaskDropVector *CfgData::getMonsterTaskDrop(int32_t mid)







{







    int32_t mida = mid;







    auto it = this->m_monsterTaskDrops.find(mida);







    if (it != this->m_monsterTaskDrops.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgNpc *CfgData::getNpc(int32_t npcid)







{







    int32_t npcida = npcid;







    auto it = this->m_npcs.find(npcida);







    if (it != this->m_npcs.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgNpcAirport *CfgData::getNpcAirport(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_npcAirports.find(ida);







    if (it != this->m_npcAirports.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgPlant *CfgData::getPlant(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_plants.find(ida);







    if (it != this->m_plants.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgTask *CfgData::getTask(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_tasks.find(ida);







    if (it != this->m_tasks.end())







    {







        return &it->second;







    }







    return nullptr;







}















int8_t CfgData::GetTaskType(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_tasks.find(ida);







    if (it != this->m_tasks.end())







    {







        return it->second.type;







    }







    return 0;







}















CfgTrap *CfgData::getTrap(int32_t id)







{







    int32_t ida = id;







    auto it = this->m_traps.find(ida);







    if (it != this->m_traps.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgLevelExp *CfgData::getUpgradeExp(int32_t level)







{







    int32_t levela = level;







    auto it = this->m_levelExps.find(levela);







    if (it != this->m_levelExps.end())







    {







        return &it->second;







    }







    return nullptr;







}















int64_t CfgData::getNeedLevelExp(int32_t level)







{







    int32_t levela = level;







    auto it = this->m_levelExps.find(levela);







    if (it != this->m_levelExps.end())







    {







        return it->second.upgrade_exp;







    }







    return INT64_MAX;







}















int64_t CfgData::getMaxExp(int32_t level)







{







    int32_t levela = level;







    auto it = this->m_levelExps.find(levela);







    if (it != this->m_levelExps.end())







    {







        return it->second.max_exp;







    }







    return INT64_MAX;







}















int32_t CfgData::GetPetExp(int32_t level)







{







    int32_t levela = level;







    auto it = this->m_levelExps.find(levela);







    if (it != this->m_levelExps.end())







    {







        return it->second.pet_exp;







    }







    return INT32_MAX;







}















int32_t CfgData::GetTalentPoint(int32_t level)







{







    int32_t levela = level;







    auto it = this->m_levelExps.find(levela);







    if (it != this->m_levelExps.end())







    {







        return it->second.talent_point;







    }







    return 0;







}















int32_t CfgData::GetAttrPoint(int32_t level)







{







    int32_t levela = level;







    auto it = this->m_levelExps.find(levela);







    if (it != this->m_levelExps.end())







    {







        return it->second.attr_point;







    }







    return 0;







}















CfgLevelAttr *CfgData::getLevelAttr(int32_t job, int32_t level)







{







    int key = (job << 16) | level;







    auto it = this->m_levelAttrs.find(key);







    if (it != this->m_levelAttrs.end())







    {







        return &it->second;







    }







    return nullptr;







}















int32_t CfgData::getBaseJob(int32_t job)







{







    if (job == 2) return 2;







    if (job == 3) return 3;







    return (job == 1) ? 1 : 0;







}















// ==================== Init 方法 1-10 ====================















void CfgData::InitGroupIconTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/GroupIcon.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_GROUP_ICON_TABLE fail, please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = readFile.GetRecordsNum();







        int32_t iBaseColumnCount = readFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgGroupIcon icon{};                







                icon.nId = readFile.Search_Posistion( i, 0)->iValue;







                icon.bShowInCross = readFile.Search_Posistion( i, 18)->iValue;







                







                const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, 19);







                icon.platfrom = v1->pString;







                







                auto *v2 = &this->m_cfgGroupIcons[icon.nId];







                *v2 = icon;







                        }







        }







    }







}















bool CfgData::IsShowIcon(int32_t nIconId, const std::string *const platform)







{







    int32_t nIconIda = nIconId;







    auto __it = this->m_cfgGroupIcons.find(nIconIda);







    if (__it == this->m_cfgGroupIcons.end())







    {







        return false;







    }







    







    const CfgGroupIcon *icon = &__it->second;







    GameService *v4 = Answer::Singleton<GameService>::instance();







    if (GameService::getLine(v4) == 9 && !icon->bShowInCross)







    {







        return false;







    }







    







    return !(icon->platfrom != "-1" && *platform != icon->platfrom);







}















void CfgData::InitFamilyRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/FamilyTaskRing.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_FAMILY_TASK_RING failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                FamilyTaskReward stu{};







                







                int32_t nIndex = 0;







                stu.Index = TabFile.Search_Posistion( i, nIndex++)->iValue;







                stu.NeedCount = TabFile.Search_Posistion( i, nIndex++)->iValue;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                stu.Rewards = __x;







                                                    







                auto *v2 = &this->m_FamilyTaskReward[stu.Index];







                *v2 = stu;







                        }







        }







    }







}















FamilyTaskReward *CfgData::GetFamilyReward(int32_t Index)







{







    int32_t Indexa = Index;







    auto it = this->m_FamilyTaskReward.find(Indexa);







    if (it != this->m_FamilyTaskReward.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitAppendAttrTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/AdditionalAttribute.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_APPEND_ATTR_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgAppendAttr stu{};







                







                int32_t nIndex = 0;







                stu.Id = TabFile.Search_Posistion( i, nIndex++)->iValue;







                stu.Job = TabFile.Search_Posistion( i, nIndex++)->iValue;







                







                for (int32_t j = 0; j <= 9; ++j)







                {







                    AddAttribute Attr;







                    Attr.m_nAddAttrType = TabFile.Search_Posistion( i, nIndex++)->iValue;







                    Attr.m_nAddAttrValue = TabFile.Search_Posistion( i, nIndex++)->iValue;







                    if (Attr.m_nAddAttrValue > 0)







                    {







                        stu.AttrList.push_back(Attr);







                    }







                }







                







                this->m_AppendAttrTable.push_back(stu);







                        }







        }







    }







}















void CfgData::InitHuoYueDuTable()







{







    CDBCFile TabFile;







    bool ret = TabFile.OpenFromTXT( "./ServerConfig/Tables/HuoYueDu.txt");







    







    if (!ret)







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_HUO_YUE_DU_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgHuoYueDu stu;







            CfgOutLinkFestival stu;

                memset(&stu, 0, sizeof(stu));







                stu.Index = TabFile.Search_Posistion( i, 0)->iValue;







                stu.Type = TabFile.Search_Posistion( i, 1)->iValue;







                stu.Effect = TabFile.Search_Posistion( i, 2)->iValue;







                stu.Count = TabFile.Search_Posistion( i, 3)->iValue;







                stu.Gold = TabFile.Search_Posistion( i, 4)->iValue;







                stu.AddHuoYueDu = TabFile.Search_Posistion( i, 5)->iValue;







                stu.IsCanSec = TabFile.Search_Posistion( i, 6)->iValue > 0;







                







                auto *v1 = &this->m_HuoYueDuTable[stu.Index];







                *v1 = stu;







            }







            







            CDBCFile TabFileReward;







                    ret = TabFileReward.OpenFromTXT( "./ServerConfig/Tables/HuoYueDuReward.txt");







            







            if (!ret)







            {







                Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_HUO_YUE_DU_REWARD_TABLE failed,please check!!!\n");







            }







            else







            {







                iBaseTableCount = TabFileReward.GetRecordsNum();







                iBaseColumnCount = TabFileReward.GetFieldsNum();







                







                if (iBaseColumnCount > 0)







                {







                    for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)







                    {







                        CfgHuoYueDuReward stu_0{};







                        







                        stu_0.Id = TabFileReward.Search_Posistion( i_0, 0)->iValue;







                        stu_0.NeedHuoYueDu = TabFileReward.Search_Posistion( i_0, 1)->iValue;







                        







                        std::string strItems;







                        char v10;







                                            const CDBCFile::FIELD *v2 = TabFileReward.Search_Posistion( i_0, 2);







                        strItems = v2->pString;







                        







                        MemChrBagVector __x;







                        CItemHelper().parseItemVectorString(&__x, &strItems);







                        stu_0.Items = __x;







                                                                                    







                        char __k = stu_0.Id;







                        auto *v3 = &this->m_HuoYueDuRewardTable[__k];







                        *v3 = stu_0;







                                        }







                }







            }







                }







    }







}















CfgHuoYueDu *CfgData::GetHuoYueDuCfg(int32_t Index)







{







    int32_t Indexa = Index;







    auto it = this->m_HuoYueDuTable.find(Indexa);







    if (it != this->m_HuoYueDuTable.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgHuoYueDuReward *CfgData::GetHuoYueDuReward(int32_t Index)







{







    char __x = Index;







    auto it = this->m_HuoYueDuRewardTable.find(__x);







    if (it != this->m_HuoYueDuRewardTable.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitKaiFuHuoDongTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/NewServer.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_KAI_FU_HUO_DONG failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgKaiFuHuoDongData stu{};







                int32_t Index = 0;







                stu.Index = TabFile.Search_Posistion( i, 0)->iValue;







                stu.Type = TabFile.Search_Posistion( i, ++Index)->iValue;







                stu.Level = TabFile.Search_Posistion( i, ++Index)->iValue;







                stu.StartDay = TabFile.Search_Posistion( i, ++Index)->iValue;







                stu.EndDay = TabFile.Search_Posistion( i, ++Index)->iValue;







                stu.Conditions = TabFile.Search_Posistion( i, ++Index)->iValue;







                ++Index;







                







                if (stu.Type == 17)







                {







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, Index);







                    stu.ItemString = v1->pString;







                }







                else







                {







                    std::string strItems;







                    char v9;







                                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, Index);







                    strItems = v2->pString;







                    







                    MemChrBagVector __x;







                    CItemHelper().parseItemVectorString(&__x, &strItems);







                    stu.ItemVector = __x;







                                                                }







                







                stu.PetAttr = TabFile.Search_Posistion( i, ++Index)->iValue;







                stu.GongGaoId = TabFile.Search_Posistion( i, ++Index)->iValue;







                stu.Limit = TabFile.Search_Posistion( i, ++Index)->iValue;







                stu.PetEffect = TabFile.Search_Posistion( i, ++Index)->iValue;







                ++Index;







                Index += 5;







                stu.Conditions1 = TabFile.Search_Posistion( i, Index++)->iValue;







                







                const CDBCFile::FIELD *v3 = TabFile.Search_Posistion( i, Index);







                stu.FlopString = v3->pString;







                







                auto *v4 = &this->m_KaiFuHuoDongTable[stu.Index];







                *v4 = stu;







                        }







        }







    }







}







void CfgData::InitDropRecordTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/DropRecord.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_DROP_RECORD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                CfgDropRecord record;







                memset(&record, 0, sizeof(record));







                record.nIndex = TabFile.Search_Posistion( i, 0)->iValue;







                record.nItemId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







                record.nItemClass = TabFile.Search_Posistion( i, ++nIndex)->iValue;







                record.nSpecial = TabFile.Search_Posistion( i, ++nIndex)->iValue;







                ++nIndex;







                







                auto *v1 = &this->m_cfgDropRecordTable[record.nIndex];







                *v1 = record;







            }







        }







    }







}















const CfgDropRecord *CfgData::GetDropRecord(int32_t nIndex)







{







    int32_t nIndexa = nIndex;







    auto iter = this->m_cfgDropRecordTable.find(nIndexa);







    if (iter != this->m_cfgDropRecordTable.end())







    {







        return &iter->second;







    }







    return nullptr;







}















void CfgData::InitDungeonScoreTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/DungeonScore.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_DUNGEON_SCORE_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                CfgDungeonScore score;







                memset(&score, 0, sizeof(score));







                score.nDungeonId = TabFile.Search_Posistion( i, 0)->iValue;







                score.nKillCount = TabFile.Search_Posistion( i, ++nIndex)->iValue;







                ++nIndex;







                







                std::string strItem;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItem = v1->pString;







                







                MemChrBag v2;







                CItemHelper().parseItemString(v2, &strItem);







                score.Item = v2;







                







                                        ++nIndex;







                







                // TODO: // TODO: implement AddDungeonScore







                    // this->m_cfgDungeonScoreTable.AddDungeonScore(score);







            }







        }







    }







}















void CfgData::InitDungeonSummon()







{







    CDBCFile DungeonTrapFile;







    







    if (!DungeonTrapFile.OpenFromTXT( "./ServerConfig/Tables/cfg_dungeon_summon.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_DUNGEON_SUMMON_TABLE.txt failed,please check!!");







    }







    else







    {







        int32_t iBaseTableCount = DungeonTrapFile.GetRecordsNum();







        int32_t iBaseColumnCount = DungeonTrapFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgDungeonSummon stu;







                







                int32_t nIndex = 0;







                stu.nDungeon = DungeonTrapFile.Search_Posistion( i, nIndex++)->iValue;







                stu.nIndex = DungeonTrapFile.Search_Posistion( i, nIndex++)->iValue;







                stu.nGold = DungeonTrapFile.Search_Posistion( i, nIndex++)->iValue;







                stu.nLimit = DungeonTrapFile.Search_Posistion( i, nIndex++)->iValue;







                







                std::string path;







                char v9;







                            const CDBCFile::FIELD *v1 = DungeonTrapFile.Search_Posistion(i, nIndex);







                path = v1->pString;







                







                std::list<int> __x;







                paraseInt32List(&__x, &path, 0, nullptr);







                stu.lMonsters = __x;







                                                    ++nIndex;







                







                std::string strItem;







                char v13;







                            const CDBCFile::FIELD *v2 = DungeonTrapFile.Search_Posistion( i, nIndex);







                strItem = v2->pString;







                







                ItemData v20;







                v20 = CItemHelper().parseItemDataString(&strItem);







                stu.ConstItem.m_nId = v20.m_nId;







                stu.ConstItem.m_nClass = v20.m_nClass;







                stu.ConstItem.m_nCount = v20.m_nCount;







                







                                        stu.FreeCount = DungeonTrapFile.Search_Posistion( i, ++nIndex)->iValue;







                ++nIndex;







                







                auto __k = std::make_pair(stu.nDungeon, stu.nIndex);







                this->m_cfgDungeonSummon[__k] = stu;







                        }







        }







    }







}















const CfgDungeonSummon *CfgData::GetDungeonSummon(int32_t nDungeon, int32_t nIndex)







{







    auto __x = std::make_pair(nDungeon, nIndex);







    auto iter = this->m_cfgDungeonSummon.find(__x);







    if (iter != this->m_cfgDungeonSummon.end())







    {







        return &iter->second;







    }







    return nullptr;







}















void CfgData::InitSpecialTreasureMapRandTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/SpecialTreasureMap.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SPECIAL_TREASURE_MAP_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                SpecialTreasureMapRandCfg stu{};







                







                stu.nItemId = TabFile.Search_Posistion( i, 0)->iValue;







                stu.nType = TabFile.Search_Posistion( i, 1)->iValue;







                







                std::string strItems;







                char v9;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 2);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                stu.vGetItems = __x;







                                                    







                stu.nRate = TabFile.Search_Posistion( i, 3)->iValue;







                







                std::string strItem;







                char v11;







                            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, 4);







                strItem = v2->pString;







                







                MemChrBag v3;







                CItemHelper().parseItemString(v3, &strItem);







                stu.vShowItem = v3;







                







                                        stu.nGongGaoId = TabFile.Search_Posistion( i, 5)->iValue;







                







                this->m_SpecialTreasureMapRandCfgList.push_back(stu);







                        }







        }







    }







}















void CfgData::InitMonsterAddAttrTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/MonstAddAttr.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MONSTER_ADD_ATTR failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t Mid = TabFile.Search_Posistion( i, 0)->iValue;







                







                CfgMonsterAddAttr AddAttrs{};







                AddAttrs.WorldBossLevelMin = TabFile.Search_Posistion( i, 1)->iValue;







                AddAttrs.WorldBossLevelMax = TabFile.Search_Posistion( i, 2)->iValue;







                







                for (int32_t j = 0; j <= 6; ++j)







                {







                    AttrAddon stu;







                    memset(&stu, 0, sizeof(stu));







                    stu.index = TabFile.Search_Posistion( i, 2 * j + 3)->iValue;







                    stu.addon = TabFile.Search_Posistion( i, 2 * (j + 2))->iValue;







                    AddAttrs.AttrVector.push_back(stu);







                }







                







                auto &v1 = this->m_MonstAddAttrMap[Mid];







                v1.push_back(AddAttrs);







                        }







        }







    }







}















AttrAddonVector *CfgData::GetAddMonsterAttrs(int32_t Mid, int32_t WorldLevel)







{







    int32_t Mida = Mid;







    auto it = this->m_MonstAddAttrMap.find(Mida);







    







    if (it != this->m_MonstAddAttrMap.end())







    {







        for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)







        {







            if (iter->WorldBossLevelMin <= WorldLevel && iter->WorldBossLevelMax >= WorldLevel)







            {







                return &iter->AttrVector;







            }







        }







    }







    return nullptr;







}















void CfgData::InitDaZheQuanTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/ItemDiscount.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_DA_ZHE_QUAN_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                DaZheQuan stu{};







                







                stu.Index = TabFile.Search_Posistion( i, 0)->iValue;







                stu.CurrencyType = TabFile.Search_Posistion( i, 1)->iValue;







                stu.CurrencyValues = TabFile.Search_Posistion( i, 2)->iValue;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 3);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                stu.Items = __x;







                                                    







                auto *v2 = &this->m_DaZheQuanMap[stu.Index];







                *v2 = stu;







                        }







        }







    }







}















DaZheQuan *CfgData::GetDaZheQuanCfg(int32_t Index)







{







    int32_t Indexa = Index;







    auto it = this->m_DaZheQuanMap.find(Indexa);







    if (it != this->m_DaZheQuanMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::Init360RewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/GameLobby.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_360_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                Wan360Reward stu{};







                







                stu.Index = TabFile.Search_Posistion( i, 0)->iValue;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 1);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                stu.Items = __x;







                                                    







                auto *v2 = &this->m_Wan360RewardMap[stu.Index];







                *v2 = stu;







                        }







        }







    }







}















void CfgData::Init360RewardTypeTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/GameLobbyType.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_360_REWARD_TYPE_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                std::string platform;







                char v5;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 0);







                platform = v1->pString;







                            







                int32_t icon = TabFile.Search_Posistion( i, 1)->iValue;







                auto *v2 = &this->m_Wan360RewardTypeMap[platform];







                *v2 = icon;







                        }







        }







    }







}















Wan360Reward *CfgData::Get360RewardCfg(int32_t Index)







{







    int32_t Indexa = Index;







    auto it = this->m_Wan360RewardMap.find(Indexa);







    if (it != this->m_Wan360RewardMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















int32_t CfgData::Get360RewardIcon(const std::string *const platform)







{







    return this->m_Wan360RewardTypeMap[*platform];







}















void CfgData::InitTotalChongZhiTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/ChongZhiHuiKui.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TOTAL_CHONG_ZHI_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount - 1; ++i)







            {







                CfgTotalChongZhi stu{};







                







                stu.Index = TabFile.Search_Posistion( i, 0)->iValue;







                stu.NeedGold = TabFile.Search_Posistion( i, 1)->iValue;







                







                std::string strItems;







                char v8;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 2);







                strItems = v1->pString;







                







                MemChrEquipBagVector __x;







                CfgData::parseEquipItemString(&__x, this, stu.Index, &strItems);







                stu.ItemVector = __x;







                                                    







                std::string v10;







                char v11;







                            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, 3);







                v10 = v2->pString;







                







                std::list<CfgGongGao> v9;







                CfgData::parseGongGaoString(&v9, stu.Index, &v10);







                stu.GongGaoInfo = v9;







                                                    







                stu.NewServerday = TabFile.Search_Posistion( i, 4)->iValue;







                







                auto *v3 = &this->m_TotalChongZhiTable[stu.Index];







                *v3 = stu;







                        }







        }







    }







}















CfgTotalChongZhi *CfgData::GetTotalChongZhiCfg(int8_t Index)







{







    int8_t Indexa[4] = {Index};







    auto it = this->m_TotalChongZhiTable.find(Indexa[0]);







    if (it != this->m_TotalChongZhiTable.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitEveryDayChongZhi()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/MeiRiChongZhi.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_EVERYDAY_CHONG_ZHI failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgEverydayChongZhi stu{};







                







                stu.Index = TabFile.Search_Posistion( i, 0)->iValue;







                stu.Type = TabFile.Search_Posistion( i, 1)->iValue;







                stu.NeedGold = TabFile.Search_Posistion( i, 2)->iValue;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 3);







                strItems = v1->pString;







                







                MemChrEquipBagVector __x;







                CfgData::parseEquipItemString(&__x, this, stu.Index, &strItems);







                stu.ItemVector = __x;







                                                    







                std::string v9;







                char v10;







                            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, 4);







                v9 = v2->pString;







                







                std::list<CfgGongGao> v8;







                CfgData::parseGongGaoString(&v8, stu.Index, &v9);







                stu.GongGaoInfo = v8;







                                                    







                this->m_EveryDayChongZhiTable.push_back(stu);







                        }







        }







    }







}















CfgEverydayChongZhi *CfgData::GetEveryDayChongZhiCfg(int8_t nType, int8_t Index)







{







    for (auto it = this->m_EveryDayChongZhiTable.begin(); it != this->m_EveryDayChongZhiTable.end(); ++it)







    {







        if (it->Type == nType && it->Index == Index)







        {







            return const_cast<CfgEverydayChongZhi*>(&(*it));







        }







    }







    return nullptr;







}















void CfgData::InitNewServerFavorable()







{







    CDBCFile TabFile;







    bool ret = TabFile.OpenFromTXT( "./ServerConfig/Tables/TeHui.txt");







    







    if (!ret)







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_NEW_SERVER_FAVORABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                NewServerFavorable stu;







                







                stu.Index = TabFile.Search_Posistion( i, 0)->iValue;







                







                std::string strItems;







                char v11;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 1);







                strItems = v1->pString;







                







                MemChrEquipBagVector __x;







                CfgData::parseEquipItemString(&__x, this, stu.Index, &strItems);







                stu.ItemVector = __x;







                                                    







                stu.NeedGold = TabFile.Search_Posistion( i, 2)->iValue;







                







                std::string v13;







                char v14;







                            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, 3);







                v13 = v2->pString;







                







                std::list<CfgGongGao> v12;







                CfgData::parseGongGaoString(&v12, stu.Index, &v13);







                stu.GongGaoInfo = v12;







                                                    







                stu.nLimitTime = TabFile.Search_Posistion( i, 4)->iValue;







                







                auto *v3 = &this->m_NewServerFavorable[stu.Index];







                *v3 = stu;







                        }







            







            CDBCFile TabFile2;







                    ret = TabFile2.OpenFromTXT( "./ServerConfig/Tables/SanChong.txt");







            







            if (!ret)







            {







                Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_THREE_PET_GIFT failed,please check!!!\n");







            }







            else







            {







                iBaseTableCount = TabFile2.GetRecordsNum();







                iBaseColumnCount = TabFile2.GetFieldsNum();







                







                if (iBaseColumnCount > 0)







                {







                    for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)







                    {







                        NewServerFavorable stu;







                        







                        this->m_ThreePetGift.Index = TabFile2.Search_Posistion( i_0, 0)->iValue;







                        







                        std::string v16;







                        char v17;







                                            const CDBCFile::FIELD *v4 = TabFile2.Search_Posistion( i_0, 1);







                        v16 = v4->pString;







                        







                        MemChrEquipBagVector v15;







                        CfgData::parseEquipItemString(&v15, this, stu.Index, &v16);







                        this->m_ThreePetGift.ItemVector = v15;







                                                                                    







                        this->m_ThreePetGift.NeedGold = TabFile2.Search_Posistion( i_0, 2)->iValue;







                        







                        std::string v19;







                        char v20;







                                            const CDBCFile::FIELD *v5 = TabFile2.Search_Posistion( i_0, 3);







                        v19 = v5->pString;







                        







                        std::list<CfgGongGao> v18;







                        CfgData::parseGongGaoString(&v18, stu.Index, &v19);







                        this->m_ThreePetGift.GongGaoInfo = v18;







                                                                                    







                        this->m_ThreePetGift.nLimitTime = TabFile2.Search_Posistion( i_0, 4)->iValue;







                                        }







                }







            }







                }







    }







}















NewServerFavorable *CfgData::GetNewServerFavorable(int8_t Index)







{







    int8_t Indexa[4] = {Index};







    auto it = this->m_NewServerFavorable.find(Indexa[0]);







    if (it != this->m_NewServerFavorable.end())







    {







        return &it->second;







    }







    return nullptr;







}







void CfgData::InitShouChongLiBao()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/ShouChong.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SHOU_CHONG_LI_BAO failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgShouChong stu;







                







                stu.nIndex = TabFile.Search_Posistion( i, 0)->iValue;







                stu.nGold = TabFile.Search_Posistion( i, 1)->iValue;







                







                std::string strItems;







                char v6;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 2);







                strItems = v1->pString;







                







                MemChrEquipBagVector __x;







                CfgData::parseEquipItemString(&__x, this, 1, &strItems);







                stu.vReward = __x;







                                                    







                stu.nBroadcast = TabFile.Search_Posistion( i, 3)->iValue;







                







                CfgShouChongTable::Add(&this->m_cfgShouChongTable, &stu);







                        }







        }







    }







}















void CfgData::InitMoHuaHuanYiTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/MoHuaZhuanYi.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MO_HUA_ZHUAN_YI_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgEquipExchange stu;







                memset(&stu, 0, sizeof(stu));







                







                stu.Level = TabFile.Search_Posistion( i, 0)->iValue;







                stu.Money = TabFile.Search_Posistion( i, 1)->iValue;







                







                std::string strItem;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 2);







                strItem = v1->pString;







                







                ItemData v12 = CItemHelper().parseItemDataString(&strItem);







                stu.CostItem.m_nId = v12.m_nId;







                stu.CostItem.m_nClass = v12.m_nClass;







                stu.CostItem.m_nCount = v12.m_nCount;







                







                                        







                auto *v2 = &this->m_CfgEquipExchangeMap[stu.Level];







                *v2 = stu;







            }







        }







    }







}















CfgEquipExchange *CfgData::GetEquipExchange(int8_t Level)







{







    int8_t Levela[4] = {Level};







    auto it = this->m_CfgEquipExchangeMap.find(Levela[0]);







    if (it != this->m_CfgEquipExchangeMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitZiYuanZhaoHuiTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/ZiYuanZhaoHui.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ZYZH_TABLE failed,please check!!");







    }







    else







    {







        int32_t iBaseTableCount = readFile.GetRecordsNum();







        int32_t iBaseColumnCount = readFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                CfgSearchBack stu{};







                







                stu.nId = readFile.Search_Posistion( i, nIndex++)->iValue;







                stu.nType = readFile.Search_Posistion( i, nIndex++)->iValue;







                stu.nTimes = readFile.Search_Posistion( i, nIndex++)->iValue;







                stu.nSubType = readFile.Search_Posistion( i, nIndex++)->iValue;







                stu.nOpenType = readFile.Search_Posistion( i, nIndex++)->iValue;







                stu.nMinLevel = readFile.Search_Posistion( i, nIndex++)->iValue;







                stu.nMaxLevel = readFile.Search_Posistion( i, nIndex++)->iValue;







                stu.nGoldCost = readFile.Search_Posistion( i, nIndex++)->iValue;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                stu.vFreeReward = __x;







                                                    ++nIndex;







                







                std::string v9;







                char v10;







                            const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







                v9 = v2->pString;







                







                MemChrBagVector v8;







                CItemHelper().parseItemVectorString(&v8, &v9);







                stu.vItemReward = v8;







                                                    ++nIndex;







                







                // TODO: this->m_cfgSearchBackTable.AddSearchBack(stu);







                        }







        }







    }







}















void CfgData::InitPlantEventTable()







{







    CDBCFile ItemGiftFile;







    







    if (!ItemGiftFile.OpenFromTXT( "./ServerConfig/Tables/PlantEvent.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_PLANT_EVENT_TABLE failed,please check!!");







    }







    else







    {







        int32_t iBaseTableCount = ItemGiftFile.GetRecordsNum();







        int32_t iBaseColumnCount = ItemGiftFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgPlantEventEffect Event;







                







                Event.EventId = ItemGiftFile.Search_Posistion( i, 0)->iValue;







                Event.EventType = ItemGiftFile.Search_Posistion( i, 1)->iValue;







                







                const CDBCFile::FIELD *v1 = ItemGiftFile.Search_Posistion( i, 2);







                Event.EventEffect = v1->pString;







                Event.GongGaoId = ItemGiftFile.Search_Posistion( i, 3)->iValue;







                







                auto *v2 = &this->m_PlantEventMap[Event.EventId];







                *v2 = Event;







                        }







        }







    }







}















CfgPlantEventEffect *CfgData::GetPlantEvent(int32_t EventId)







{







    int32_t EventIda = EventId;







    auto it = this->m_PlantEventMap.find(EventIda);







    if (it != this->m_PlantEventMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitFunctionOpenTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_unlock.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_FUNCTION_OPEN_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                FunctionOpenCfg stu;







                memset(&stu, 0, sizeof(stu));







                







                stu.Type = TabFile.Search_Posistion( i, 2)->iValue;







                stu.TaskId = TabFile.Search_Posistion( i, 1)->iValue;







                stu.Level = TabFile.Search_Posistion( i, 3)->iValue;







                







                auto *v1 = &this->m_FunctionOpenCfg[stu.Type];







                *v1 = stu;







            }







        }







    }







}















FunctionOpenCfg *CfgData::GetOpenFunctionCfg(int32_t FunctionId)







{







    int32_t FunctionIda = FunctionId;







    auto it = this->m_FunctionOpenCfg.find(FunctionIda);







    if (it != this->m_FunctionOpenCfg.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitVipCardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/Privilege.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_VIP_CARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                VipCardCfg VipCard{};







                







                int32_t nIndex = 0;







                VipCard.VipCardId = TabFile.Search_Posistion( i, nIndex++)->iValue;







                VipCard.AddVipTime = TabFile.Search_Posistion( i, nIndex++)->iValue;







                VipCard.NeedGold = TabFile.Search_Posistion( i, nIndex++)->iValue;







                







                std::string v8;







                char v9;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                v8 = v1->pString;







                







                std::list<AddAttribute> __x;







                CfgData::parseAddAttribues(&__x, &v8, i, nullptr);







                VipCard.AddAttr = __x;







                                                    







                VipCard.VipLevel = TabFile.Search_Posistion( i, ++nIndex)->iValue;







                ++nIndex;







                nIndex += 2;







                VipCard.ReNeedGold = TabFile.Search_Posistion( i, nIndex++)->iValue;







                VipCard.Money = TabFile.Search_Posistion( i, nIndex++)->iValue;







                VipCard.TianShuJinHua = TabFile.Search_Posistion( i, nIndex++)->iValue;







                VipCard.WeiWang = TabFile.Search_Posistion( i, nIndex++)->iValue;







                







                int __k = VipCard.VipCardId;







                auto *v2 = &this->m_VipCardTable[__k];







                *v2 = VipCard;







                        }







        }







    }







}















VipCardCfg *CfgData::GetVipCardCfg(int8_t VipType)







{







    int __x = VipType;







    auto it = this->m_VipCardTable.find(__x);







    if (it != this->m_VipCardTable.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitOffLineExpTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/OfflineExp.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_OFF_LINE_EXP_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgOffLineExp reward;







                memset(&reward, 0, sizeof(reward));







                







                reward.Level = TabFile.Search_Posistion( i, 1)->iValue;







                reward.MinuteExp = TabFile.Search_Posistion( i, 2)->iValue;







                reward.TwoTimes = TabFile.Search_Posistion( i, 3)->iValue;







                reward.ThreeTimes = TabFile.Search_Posistion( i, 4)->iValue;







                reward.BaseExp = TabFile.Search_Posistion( i, 5)->iValue;







                







                auto *v1 = &this->m_OffLineExpTable[reward.Level];







                *v1 = reward;







            }







        }







    }







}















CfgOffLineExp *CfgData::GetOfflineExpCfg(int32_t Level)







{







    int32_t Levela = Level;







    auto it = this->m_OffLineExpTable.lower_bound(Levela);







    if (it != this->m_OffLineExpTable.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitWeekOnlineReward()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/WeekOnlineReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WEEK_ONLINE_REWARD failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgWeekOnlineReward reward;







                memset(&reward, 0, sizeof(reward));







                







                reward.Week = TabFile.Search_Posistion( i, 0)->iValue;







                reward.Cash = TabFile.Search_Posistion( i, 1)->iValue;







                reward.UpperLimit = TabFile.Search_Posistion( i, 2)->iValue;







                







                auto *v1 = &this->m_WeekOnlineReward[reward.Week];







                *v1 = reward;







            }







        }







    }







}















CfgWeekOnlineReward *CfgData::GetWeekOnlineReward(int32_t Week)







{







    if (Week <= 0) return nullptr;







    







    auto it = this->m_WeekOnlineReward.find(Week);







    if (it != this->m_WeekOnlineReward.end())







    {







        return &it->second;







    }







    







    if (!this->m_WeekOnlineReward.empty())







    {







        return &this->m_WeekOnlineReward.rbegin()->second;







    }







    return nullptr;







}















void CfgData::InitLevelGift()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_level_gift.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_LEVEL_GIFT failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgLevelGift reward{};







                







                reward.Index = TabFile.Search_Posistion( i, 0)->iValue;







                reward.Level = TabFile.Search_Posistion( i, 1)->iValue;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 2);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                reward.ItemVector = __x;







                                                    







                int __k = reward.Index;







                auto *v2 = &this->m_LevelGift[__k];







                *v2 = reward;







                        }







        }







    }







}















CfgLevelGift *CfgData::GetLevelGiftCfg(int8_t Index)







{







    int __x = Index;







    auto it = this->m_LevelGift.find(__x);







    if (it != this->m_LevelGift.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitSevenLoginReward()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_seven_login_reward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SEVEN_LOGIN_REWARD failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgSevenLoginRewrad reward{};







                







                reward.Day = TabFile.Search_Posistion( i, 0)->iValue;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 1);







                strItems = v1->pString;







                







                MemChrEquipBagVector __x;







                CfgData::parseEquipItemString(&__x, this, reward.Day, &strItems);







                reward.ItemVector = __x;







                                                    







                auto *v2 = &this->m_SevenLoginReward[reward.Day];







                *v2 = reward;







                        }







        }







    }







}















CfgSevenLoginRewrad *CfgData::GetSevenLoginRewardCfg(int8_t Day)







{







    int8_t Daya[4] = {Day};







    auto it = this->m_SevenLoginReward.find(Daya[0]);







    if (it != this->m_SevenLoginReward.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitOnLimeReward()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_online_reward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ONLINE_REWARD failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgOnlineReward reward{};







                







                reward.Id = TabFile.Search_Posistion( i, 0)->iValue;







                reward.NeedTime = TabFile.Search_Posistion( i, 1)->iValue;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 2);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                reward.ItemVector = __x;







                                                    







                auto *v2 = &this->m_OnLineReward[reward.Id];







                *v2 = reward;







                        }







        }







    }







}















CfgOnlineReward *CfgData::GetOnlineRewardCfg(int8_t id)







{







    int8_t ida[4] = {id};







    auto it = this->m_OnLineReward.find(ida[0]);







    if (it != this->m_OnLineReward.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::fetchSignReward()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_sign_reward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SIGIN_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgSignReward reward{};







                







                reward.count = TabFile.Search_Posistion( i, 0)->iValue;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 1);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                reward.ItemVector = __x;







                                                    







                char __k = reward.count;







                auto *v2 = &this->m_signReward[__k];







                *v2 = reward;







                        }







        }







    }







}















CfgMemChrBagVector *CfgData::GetSignReward(int8_t SiginCount)







{







    auto it = this->m_signReward.find(SiginCount);







    if (it != this->m_signReward.end())







    {







        return &it->second.ItemVector;







    }







    return nullptr;







}







void CfgData::InitQQZoneRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/QQZone.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_QQZONE_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                CfgQQGift gift;







                







                CfgTGPGift gift;



                gift.nIndex = TabFile.Search_Posistion( i, nIndex++)->iValue;







                gift.nType = TabFile.Search_Posistion( i, nIndex++)->iValue;







                ++nIndex;







                







                std::string strItems;







                char v6;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                gift.vReward = __x;







                                                    







                gift.nCondition = TabFile.Search_Posistion( i, ++nIndex)->iValue;







                ++nIndex;







                







                // TODO: this->m_cfgTencentTable.AddQQZoneGift(gift);







                        }







        }







    }







}















void CfgData::InitQQGameRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/QQGameDaTing.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_QQGAME_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                CfgQQGift gift;







                







                gift.nIndex = TabFile.Search_Posistion( i, nIndex++)->iValue;







                gift.nType = TabFile.Search_Posistion( i, nIndex++)->iValue;







                ++nIndex;







                







                std::string strItems;







                char v6;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                gift.vReward = __x;







                                                    







                gift.nCondition = TabFile.Search_Posistion( i, ++nIndex)->iValue;







                ++nIndex;







                







                // TODO: this->m_cfgTencentTable.AddQQGameGift(gift);







                        }







        }







    }







}















void CfgData::InitYellowRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/YellowReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_YELLOW_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                int32_t nId = TabFile.Search_Posistion( i, 0)->iValue;







                ++nIndex;







                







                std::string strItems;







                char v5;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector vReward;







                CItemHelper().parseItemVectorString(&vReward, &strItems);







                                        ++nIndex;







                







                // TODO: this->m_cfgTencentTable.SetYellowNewerGift(vReward);







                        }







        }







    }







}















void CfgData::InitTencentSevenDayLoginTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/TencentSevenDayLogin.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TENCENT_SEVEN_DAY_LOGIN_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                CfgTencentSevenDayLogin stu;







                







                stu.nDays = TabFile.Search_Posistion( i, nIndex++)->iValue;







                







                std::string strItems;







                char v6;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                stu.vReward = __x;







                                                    ++nIndex;







                







                // TODO: this->m_cfgTencentTable.AddSevenDayLogin(stu);







                        }







        }







    }







}















void CfgData::InitYellowDailyRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/YellowEverydayReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_YELLOW_DAILY_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                CfgTencentGift daily;







                CfgTencentGift year;







                







                int32_t nId = TabFile.Search_Posistion( i, nIndex++)->iValue;







                int32_t nLevel = TabFile.Search_Posistion( i, nIndex++)->iValue;







                







                std::string strItems;







                char v8;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                daily.vRewards = __x;







                                                    ++nIndex;







                







                std::string v10;







                char v11;







                            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







                v10 = v2->pString;







                







                MemChrBagVector v9;







                CItemHelper().parseItemVectorString(&v9, &v10);







                year.vRewards = v9;







                                                    ++nIndex;







                







                daily.nLevel = nLevel;







                year.nLevel = nLevel;







                







                // TODO: this->m_cfgTencentTable.AddYellowDailyGift(daily);







                // TODO: this->m_cfgTencentTable.AddYellowYearGift(year);







                







                                    }







        }







    }







}















void CfgData::InitYellowLevelRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/YellowLevelReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_YELLOW_LEVEL_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                CfgTencentGift gift;







                







                int32_t nId = TabFile.Search_Posistion( i, nIndex++)->iValue;







                gift.nLevel = TabFile.Search_Posistion( i, nIndex++)->iValue;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                gift.vRewards = __x;







                                                    ++nIndex;







                







                std::string v9;







                char v10;







                            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







                v9 = v2->pString;







                







                MemChrBagVector v8;







                CItemHelper().parseItemVectorString(&v8, &v9);







                gift.vVipRewards = v8;







                                                    ++nIndex;







                







                // TODO: this->m_cfgTencentTable.AddYellowLevelGift(nId, gift);







                        }







        }







    }







}















void CfgData::InitBlueRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/BlueReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BLUE_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                int32_t nId = TabFile.Search_Posistion( i, 0)->iValue;







                ++nIndex;







                







                std::string strItems;







                char v5;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector vReward;







                CItemHelper().parseItemVectorString(&vReward, &strItems);







                                        ++nIndex;







                







                // TODO: this->m_cfgTencentTable.SetBlueNewerGift(vReward);







                        }







        }







    }







}















void CfgData::InitBlueDailyRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/BlueEverydayReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BLUE_DAILY_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                CfgTencentGift daily;







                CfgTencentGift year;







                CfgTencentGift high;







                







                int32_t nId = TabFile.Search_Posistion( i, nIndex++)->iValue;







                int32_t nLevel = TabFile.Search_Posistion( i, nIndex++)->iValue;







                







                std::string strItems;







                char v10;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                daily.vRewards = __x;







                                                    ++nIndex;







                







                std::string v12;







                char v13;







                            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







                v12 = v2->pString;







                







                MemChrBagVector v11;







                CItemHelper().parseItemVectorString(&v11, &v12);







                year.vRewards = v11;







                                                    ++nIndex;







                







                std::string v15;







                char v16;







                            const CDBCFile::FIELD *v3 = TabFile.Search_Posistion( i, nIndex);







                v15 = v3->pString;







                







                MemChrBagVector v14;







                CItemHelper().parseItemVectorString(&v14, &v15);







                high.vRewards = v14;







                                                    ++nIndex;







                







                daily.nLevel = nLevel;







                year.nLevel = nLevel;







                high.nLevel = nLevel;







                







                // TODO: this->m_cfgTencentTable.AddBlueDailyGift(daily);







                // TODO: this->m_cfgTencentTable.AddBlueYearGift(year);







                // TODO: this->m_cfgTencentTable.AddBlueHighGift(high);







                







                                                }







        }







    }







}















void CfgData::InitBlueLevelRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/BlueLevelReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BLUE_LEVEL_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                CfgTencentGift gift;







                







                int32_t nId = TabFile.Search_Posistion( i, nIndex++)->iValue;







                gift.nLevel = TabFile.Search_Posistion( i, nIndex++)->iValue;







                







                std::string strItems;







                char v6;







                            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                strItems = v1->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                gift.vRewards = __x;







                                                    ++nIndex;







                







                // TODO: this->m_cfgTencentTable.AddBlueLevelGift(nId, gift);







                        }







        }







    }







}















void CfgData::InitTGPRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/TGPReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TGP_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                







                const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                CfgTGPGift gift;







                gift.strPF = v1->pString;







                ++nIndex;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







                strItems = v2->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                gift.vReward = __x;







                                                    ++nIndex;







                











                gift.strPF = v1->pString;







                // TODO: this->m_cfgTencentTable.AddTGPNewerGift(gift);







                        }







        }







    }







}















void CfgData::InitTGPDailyRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/TGPEverydayReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TGP_DAILY_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;







                







                const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                CfgTGPGift gift;







                gift.strPF = v1->pString;







                ++nIndex;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







                strItems = v2->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                gift.vReward = __x;







                                                    ++nIndex;







                







                // TODO: this->m_cfgTencentTable.AddTGPDailyGift(gift);







                        }







        }







    }







}















void CfgData::InitTGPLevelRewardTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/TGPLevelReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TGP_LEVEL_REWARD_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                int32_t nIndex = 0;                CfgTGPGift gift;



                gift.nIndex = TabFile.Search_Posistion( i, nIndex++)->iValue;



                



                



                const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







                gift.strPF = v1->pString;







                gift.nLevel = TabFile.Search_Posistion( i, ++nIndex)->iValue;







                ++nIndex;







                







                std::string strItems;







                char v7;







                            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







                strItems = v2->pString;







                







                MemChrBagVector __x;







                CItemHelper().parseItemVectorString(&__x, &strItems);







                gift.vReward = __x;







                                                    ++nIndex;







                







                // TODO: this->m_cfgTencentTable.AddTGPLevelGift(gift);







                        }







        }







    }







}















void CfgData::InitFamilyDungeonTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/FamilyDungeon.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_FAMILY_DUNGEON_TABLE failed,please check!!!\n");







    }







    else







    {







        int32_t iBaseTableCount = TabFile.GetRecordsNum();







        int32_t iBaseColumnCount = TabFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount; ++i)







            {







                CfgFamilyDungeon stu;







                







                stu.nHard = TabFile.Search_Posistion( i, 0)->iValue;







                stu.nDungeonId = TabFile.Search_Posistion( i, ++i)->iValue;







                stu.nFamilyLevel = TabFile.Search_Posistion( i, ++i)->iValue;







                stu.nCostMoney = TabFile.Search_Posistion( i, ++i)->iValue;







                ++i;







                i += 7;







                stu.nBaoKuFuBen = TabFile.Search_Posistion( i, ++i)->iValue;







                stu.nNpcId = TabFile.Search_Posistion( i, ++i)->iValue;







                stu.X = TabFile.Search_Posistion( i, ++i)->iValue;







                stu.Y = TabFile.Search_Posistion( i, ++i)->iValue;







                stu.nTime = TabFile.Search_Posistion( i, ++i)->iValue;







                







                // TODO: this->m_cfgFamilyDungeonTable.AddDungeon(stu);







            }







        }







    }







}







void CfgData::fetchActivity()







{







    CDBCFile ActivityFile;







    







    if (!ActivityFile.OpenFromTXT( "./ServerConfig/Tables/cfg_activity.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_activity.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = ActivityFile.GetRecordsNum();







    int32_t iBaseColumnCount = ActivityFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgActivity cfg;







            







            int32_t nIndex = 0;







            cfg.id = ActivityFile.Search_Posistion( i, 0)->iValue;







            cfg.typeId = ActivityFile.Search_Posistion( i, 5)->iValue;







            int32_t weekday = ActivityFile.Search_Posistion( i, 9)->iValue;







            cfg.begin_date = ActivityFile.Search_Posistion( i, 10)->iValue;







            cfg.end_date = ActivityFile.Search_Posistion( i, 11)->iValue;







            cfg.line = ActivityFile.Search_Posistion( i, 12)->iValue;







            







            std::string maps;







            std::string start_hour;







            std::string position;







            std::string awards;







            std::string daily;







            







            char v120;







                    const CDBCFile::FIELD *v1 = ActivityFile.Search_Posistion( i, 13);







            maps = v1->pString;







                    







            char v121;







                    const CDBCFile::FIELD *v2 = ActivityFile.Search_Posistion( i, 14);







            start_hour = v2->pString;







                    







            cfg.duration = ActivityFile.Search_Posistion( i, 15)->iValue;







            cfg.level = ActivityFile.Search_Posistion( i, 16)->iValue;







            







            char v122;







                    const CDBCFile::FIELD *v3 = ActivityFile.Search_Posistion( i, 17);







            position = v3->pString;







                    







            char v123;







                    const CDBCFile::FIELD *v4 = ActivityFile.Search_Posistion( i, 18);







            awards = v4->pString;







                    







            cfg.iconid = ActivityFile.Search_Posistion( i, 21)->iValue;







            cfg.world_event_chapter = ActivityFile.Search_Posistion( i, 23)->iValue;







            int32_t target_mapid = ActivityFile.Search_Posistion( i, 24)->iValue;







            int32_t target_regiona = ActivityFile.Search_Posistion( i, 25)->iValue;







            







            char v124;







                    const CDBCFile::FIELD *v5 = ActivityFile.Search_Posistion( i, 26);







            daily = v5->pString;







                    







            cfg.StartGongGao = ActivityFile.Search_Posistion( i, 30)->iValue;







            







            // 解析星期几







            while (weekday > 0)







            {







                int __x = weekday % 10;







                cfg.weekday.push_back(__x);







                weekday /= 10;







            }







            







            // 解析奖励







            if (awards.size() > 0 && awards.c_str()[0] != '\0')







            {







                std::string delims;







                std::string str;







                char v127;







                            delims = ":";







                







                char v129;







                            str = awards.c_str();







                







                StringVector strParams;







                StringUtility::split(strParams, str, delims);







                                                                







                int16_t nsize = (int16_t)strParams.size();







                switch (cfg.typeId)







                {







                    case 1: nsize = 8; break;







                    case 6: nsize = 1; break;







                    case 8: nsize = 2; break;







                    case 11: nsize = 6; break;







                    case 12: nsize = 5; break;







                    default: break;







                }







                







                std::vector<int> v77;







                v77.resize(nsize, 0);







                for (int32_t j = 0; j < nsize; ++j)







                {







                    std::string v8 = strParams[j];







                    int val = atoi(v8.c_str());







                    v77[j] = val;







                }







                







                // 根据类型设置 gift_id







                for (int32_t j = 0; j < nsize; ++j)







                {







                    cfg.gift_id.push_back(v77[j]);







                }







            }







            







            // 解析每日奖励







            if (daily.size() > 0 && daily.c_str()[0] != '\0')







            {







                std::string v130;







                std::string v132;







                char v131;







                            v130 = ":";







                







                char v133;







                            v132 = daily.c_str();







                







                StringVector strParams_0;







                StringUtility::split(strParams_0, v132, v130);







                                                                







                int16_t nsize_0 = (int16_t)strParams_0.size();







                std::vector<int> v78;







                v78.resize(nsize_0, 0);







                for (int32_t j_0 = 0; j_0 < nsize_0; ++j_0)







                {







                    std::string v12 = strParams_0[j_0];







                    int val = atoi(v12.c_str());







                    v78[j_0] = val;







                }







                







                for (int32_t j_0 = 0; j_0 < nsize_0; ++j_0)







                {







                    cfg.daily_reward.push_back(v78[j_0]);







                }







            }







            







            // 解析地图列表







            {







                std::string v134;







                std::string v136;







                char v135;







                            v134 = ":";







                







                char v137;







                            v136 = maps.c_str();







                







                StringVector strMaps;







                StringUtility::split(strMaps, v136, v134);







                                                                







                for (size_t j_1 = 0; j_1 < strMaps.size(); ++j_1)







                {







                    std::string v15 = strMaps[j_1];







                    int val = atoi(v15.c_str());







                    cfg.maps.push_back(val);







                }







            }







            







            // 解析目标区域







            {







                std::string v139;







                std::string v141;







                char v140;







                            v139 = ":";







                







                char v142;







                            v141 = position.c_str();







                







                StringVector strParams_1;







                StringUtility::split(strParams_1, v141, v139);







                                                                







                if (strParams_1.size() == 2)







                {







                    std::string v19 = strParams_1[0];







                    cfg.target_mapid = atoi(v19.c_str());







                    







                    std::string v143;







                    std::string v145;







                    char v144;







                                    v143 = "|";







                    







                    char v146;







                                    std::string v21 = strParams_1[1];







                    v145 = v21.c_str();







                    







                    StringVector stritemParams;







                    StringUtility::split(stritemParams, v145, v143);







                                                                                    







                    for (size_t j_2 = 0; j_2 < stritemParams.size(); ++j_2)







                    {







                        std::string v23 = stritemParams[j_2];







                        int val = atoi(v23.c_str());







                        cfg.target_regiona.push_back(val);







                    }







                }







                else if (strParams_1.size() == 3)







                {







                    std::string v25 = strParams_1[0];







                    cfg.target_mapid = atoi(v25.c_str());







                    







                    std::string v148;







                    std::string v150;







                    char v149;







                                    v148 = "|";







                    







                    char v151;







                                    std::string v27 = strParams_1[1];







                    v150 = v27.c_str();







                    







                    StringVector stritemParams_0;







                    StringUtility::split(stritemParams_0, v150, v148);







                                                                                    







                    for (size_t j_3 = 0; j_3 < stritemParams_0.size(); ++j_3)







                    {







                        std::string v29 = stritemParams_0[j_3];







                        int val = atoi(v29.c_str());







                        cfg.target_regiona.push_back(val);







                    }







                    







                    std::string v154;







                    std::string v156;







                    char v155;







                                    v154 = "|";







                    







                    char v157;







                                    std::string v31 = strParams_1[2];







                    v156 = v31.c_str();







                    







                    StringVector v153;







                    StringUtility::split(v153, v156, v154);







                    stritemParams_0.operator=(v153);







                                                                                    







                    for (size_t k = 0; k < stritemParams_0.size(); ++k)







                    {







                        std::string v33 = stritemParams_0[k];







                        int val = atoi(v33.c_str());







                        cfg.target_regionb.push_back(val);







                    }







                }







            }







            







            // 解析开始时间







            {







                std::string v159;







                char v160;







                            v159 = ":";







                







                StringVector vStartHour;







                StringUtility::split(vStartHour, start_hour, v159);







                                        







                for (size_t j_4 = 0; j_4 < vStartHour.size(); ++j_4)







                {







                    std::string v35 = vStartHour[j_4];







                    int tstart = atoi(v35.c_str());







                    if (tstart >= 0 && cfg.duration >= 0 && cfg.duration + tstart <= 1439)







                    {







                        cfg.start_hour.push_back(tstart);







                    }







                    else







                    {







                        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "wrong activity data with id=%d\n", cfg.id);







                    }







                }







            }







            







            // 存储到 map







            auto *v38 = &this->m_activities[cfg.id];







            *v38 = cfg;







            







            // 清理







        }







        







        // 加载活动事件







        CDBCFile ActivityEventFile;







            







        if (ActivityEventFile.OpenFromTXT( "./ServerConfig/Tables/cfg_activity_event.txt"))







        {







            int32_t iBaseTableCount_Event = ActivityEventFile.GetRecordsNum();







            int32_t iBaseColumnCount_Event = ActivityEventFile.GetFieldsNum();







            







            if (iBaseColumnCount_Event > 0)







            {







                for (int32_t i_0 = 0; i_0 < iBaseTableCount_Event; ++i_0)







                {







                    CfgMapEvent event;







                    







                    int32_t nIndex = 0;







                    event.id = ActivityEventFile.Search_Posistion( i_0, 0)->iValue;







                    event.activity_id = ActivityEventFile.Search_Posistion( i_0, ++nIndex)->iValue;







                    event.plant_id = ActivityEventFile.Search_Posistion( i_0, ++nIndex)->iValue;







                    event.bInitOpen = ActivityEventFile.Search_Posistion( i_0, ++nIndex)->iValue == 0;







                    event.count = ActivityEventFile.Search_Posistion( i_0, ++nIndex)->iValue;







                    ++nIndex;







                    







                    std::string triggerParam;







                    char v161;







                                    const CDBCFile::FIELD *v39 = ActivityEventFile.Search_Posistion( i_0, nIndex);







                    triggerParam = v39->pString;







                                    







                    event.mapid = ActivityEventFile.Search_Posistion( i_0, ++nIndex)->iValue;







                    







                    const CDBCFile::FIELD *v40 = ActivityEventFile.Search_Posistion( i_0, ++nIndex);







                    event.effect = v40->pString;







                    ++nIndex;







                    







                    // 解析 trigger_param







                    std::string v162;







                    std::string v164;







                    char v163;







                                    v162 = ":";







                    







                    char v165;







                                    v164 = triggerParam.c_str();







                    







                    StringVector strParams_2;







                    StringUtility::split(strParams_2, v164, v162);







                                                                                    







                    for (auto& strParam : strParams_2)







                    {







                        int val = atoi(strParam.c_str());







                        event.trigger_param.push_back(val);







                    }







                    







                    // 存储事件







                    auto& v44 = this->m_activityEvents[event.id];







                    auto& v45 = v44[event.mapid];







                    v45.push_back(event);







                    







                }







            }







                    







            // 加载活动怪物







            CDBCFile ActivityMonsterFile;







                    







            if (ActivityMonsterFile.OpenFromTXT( "./ServerConfig/Tables/cfg_activity_monster.txt"))







            {







                int32_t iBaseTableCount_Monster = ActivityMonsterFile.GetRecordsNum();







                int32_t iBaseColumnCount_Monster = ActivityMonsterFile.GetFieldsNum();







                







                if (iBaseColumnCount_Monster > 0)







                {







                    for (int32_t i_1 = 0; i_1 < iBaseTableCount_Monster; ++i_1)







                    {







                        CfgActivityMonster monster;







                        







                        monster.id = ActivityMonsterFile.Search_Posistion( i_1, 0)->iValue;







                        monster.wave = ActivityMonsterFile.Search_Posistion( i_1, 1)->iValue;







                        monster.mid = ActivityMonsterFile.Search_Posistion( i_1, 2)->iValue;







                        monster.count = ActivityMonsterFile.Search_Posistion( i_1, 3)->iValue;







                        monster.side = ActivityMonsterFile.Search_Posistion( i_1, 4)->iValue;







                        







                        std::string road;







                        char v168;







                                            const CDBCFile::FIELD *v46 = ActivityMonsterFile.Search_Posistion( i_1, 5);







                        road = v46->pString;







                                            







                        monster.x = ActivityMonsterFile.Search_Posistion( i_1, 6)->iValue;







                        monster.y = ActivityMonsterFile.Search_Posistion( i_1, 7)->iValue;







                        monster.delay = ActivityMonsterFile.Search_Posistion( i_1, 8)->iValue;







                        monster.times = ActivityMonsterFile.Search_Posistion( i_1, 9)->iValue;







                        monster.buff = ActivityMonsterFile.Search_Posistion( i_1, 10)->iValue;







                        







                        std::string randpos;







                        char v169;







                                            const CDBCFile::FIELD *v47 = ActivityMonsterFile.Search_Posistion( i_1, 11);







                        randpos = v47->pString;







                                            







                        monster.left = ActivityMonsterFile.Search_Posistion( i_1, 12)->iValue;







                        







                        // 解析路线







                        if (road.size() > 3)







                        {







                            std::string v170;







                            char v171;







                                                    v170 = ":";







                            







                            StringVector vRoad;







                            StringUtility::split(vRoad, road, v170);







                                                                            







                            for (size_t j_5 = 0; j_5 < vRoad.size(); ++j_5)







                            {







                                std::string v172;







                                char v173;







                                                            v172 = ",";







                                







                                StringVector vPos;







                                StringUtility::split(vPos, vRoad[j_5], v172);







                                                                                        







                                if (vPos.size() == 2)







                                {







                                    Position pos;







                                    pos.x = atoi(vPos[0].c_str());







                                    pos.y = atoi(vPos[1].c_str());







                                    monster.road.push_back(pos);







                                }







                            }







                        }







                        







                        // 解析随机位置







                        if (randpos.size() > 3)







                        {







                            std::string v175;







                            char v176;







                                                    v175 = "|";







                            







                            StringVector vRandPos;







                            StringUtility::split(vRandPos, randpos, v175);







                                                                            







                            for (size_t j_6 = 0; j_6 < vRandPos.size(); ++j_6)







                            {







                                std::string v177;







                                char v178;







                                                            v177 = ":";







                                







                                StringVector vPos_0;







                                StringUtility::split(vPos_0, vRandPos[j_6], v177);







                                                                                        







                                if (vPos_0.size() == 2)







                                {







                                    Position pos;







                                    pos.x = atoi(vPos_0[0].c_str());







                                    pos.y = atoi(vPos_0[1].c_str());







                                    monster.randpos.push_back(pos);







                                }







                            }







                        }







                        







                        auto *v64 = &this->m_activityMonsters[monster.id];







                        *v64 = monster;







                        







                    }







                }







                        }







            







            // 加载活动NPC







            CDBCFile ActivityNpcFile;







                    







            if (ActivityNpcFile.OpenFromTXT( "./ServerConfig/Tables/cfg_activity_npc.txt"))







            {







                int32_t iBaseTableCount_Npc = ActivityNpcFile.GetRecordsNum();







                int32_t iBaseColumnCount_Npc = ActivityNpcFile.GetFieldsNum();







                







                if (iBaseColumnCount_Npc > 0)







                {







                    for (int32_t i_2 = 0; i_2 < iBaseTableCount_Npc; ++i_2)







                    {







                        CfgActivityNpc npc;







                        







                        npc.id = ActivityNpcFile.Search_Posistion( i_2, 0)->iValue;







                        npc.activity_id = ActivityNpcFile.Search_Posistion( i_2, 1)->iValue;







                        npc.npc_id = ActivityNpcFile.Search_Posistion( i_2, 2)->iValue;







                        npc.count = ActivityNpcFile.Search_Posistion( i_2, 3)->iValue;







                        







                        std::string regionId;







                        char v180;







                                            const CDBCFile::FIELD *v65 = ActivityNpcFile.Search_Posistion( i_2, 4);







                        regionId = v65->pString;







                                            







                        std::string v181;







                        char v182;







                                            v181 = ":";







                        







                        char v184;







                                            std::string v183;







                        v183 = regionId.c_str();







                        







                        StringVector strRegions;







                        StringUtility::split(strRegions, v183, v181);







                                                                                                        







                        for (size_t it_0 = 0; it_0 < strRegions.size(); ++it_0)







                        {







                            std::string v67 = strRegions[it_0];







                            int val = atoi(v67.c_str());







                            npc.region_id.push_back(val);







                        }







                        







                        auto *v69 = &this->m_activityNpcs[npc.id];







                        *v69 = npc;







                    }







                }







                        }







            







            // 加载活动植物







            CDBCFile ActivityPlantFile;







                    







            if (ActivityPlantFile.OpenFromTXT( "./ServerConfig/Tables/cfg_activity_plant.txt"))







            {







                int32_t iBaseTableCount_Plant = ActivityPlantFile.GetRecordsNum();







                int32_t iBaseColumnCount_Plant = ActivityPlantFile.GetFieldsNum();







                







                if (iBaseColumnCount_Plant > 0)







                {







                    for (int32_t i_3 = 0; i_3 < iBaseTableCount_Plant; ++i_3)







                    {







                        CfgActivityPlant plant;







                        







                        plant.id = ActivityPlantFile.Search_Posistion( i_3, 0)->iValue;







                        plant.activity_id = ActivityPlantFile.Search_Posistion( i_3, 1)->iValue;







                        plant.plant_id = ActivityPlantFile.Search_Posistion( i_3, 2)->iValue;







                        plant.count = ActivityPlantFile.Search_Posistion( i_3, 3)->iValue;







                        plant.wave = ActivityPlantFile.Search_Posistion( i_3, 4)->iValue;







                        plant.region_id = ActivityPlantFile.Search_Posistion( i_3, 5)->iValue;







                        plant.whoplant = ActivityPlantFile.Search_Posistion( i_3, 6)->iValue;







                        







                        std::string strPos;







                        char v189;







                                            const CDBCFile::FIELD *v70 = ActivityPlantFile.Search_Posistion( i_3, 7);







                        strPos = v70->pString;







                        







                        std::vector<Position> v187;







                        CfgData::paresPosition(v187, &strPos);







                        plant.EnterPosVector = v187;







                                            







                        plant.life_time = ActivityPlantFile.Search_Posistion( i_3, 8)->iValue;







                        







                        auto *v71 = &this->m_activityPlants[plant.id];







                        *v71 = plant;







                    }







                }







                        }







            







            // 加载活动掉落







            CDBCFile ActivityDropFile;







                    







            if (ActivityDropFile.OpenFromTXT( "./ServerConfig/Tables/cfg_activity_drop.txt"))







            {







                int32_t iBaseTableCount_Drop = ActivityDropFile.GetRecordsNum();







                int32_t iBaseColumnCount_Drop = ActivityDropFile.GetFieldsNum();







                







                if (iBaseColumnCount_Drop > 0)







                {







                    for (int32_t i_4 = 0; i_4 < iBaseTableCount_Drop; ++i_4)







                    {







                        CfgActivityDrop drop;







                        drop.id = ActivityDropFile.Search_Posistion( i_4, 0)->iValue;







                        drop.activity_id = ActivityDropFile.Search_Posistion( i_4, 1)->iValue;







                        drop.monster_min_level = ActivityDropFile.Search_Posistion( i_4, 2)->iValue;







                        drop.drop_group_id = ActivityDropFile.Search_Posistion( i_4, 3)->iValue;







                        drop.probability = ActivityDropFile.Search_Posistion( i_4, 4)->iValue;







                        drop.bind_type = ActivityDropFile.Search_Posistion( i_4, 5)->iValue;







                        







                        auto *v72 = &this->m_activityDrops[drop.id];







                        *v72 = drop;







                    }







                }







                        }







            







            // 加载活动陷阱







            CDBCFile ActivityTrapFile;







                    







            if (ActivityTrapFile.OpenFromTXT( "./ServerConfig/Tables/cfg_activity_trap.txt"))







            {







                int32_t iBaseTableCount_Trap = ActivityTrapFile.GetRecordsNum();







                int32_t iBaseColumnCount_Trap = ActivityTrapFile.GetFieldsNum();







                







                if (iBaseColumnCount_Trap > 0)







                {







                    for (int32_t i_5 = 0; i_5 < iBaseTableCount_Trap; ++i_5)







                    {







                        CfgActivityTrap trap;







                        trap.id = ActivityTrapFile.Search_Posistion( i_5, 0)->iValue;







                        trap.tid = ActivityTrapFile.Search_Posistion( i_5, 1)->iValue;







                        trap.x = ActivityTrapFile.Search_Posistion( i_5, 2)->iValue;







                        trap.y = ActivityTrapFile.Search_Posistion( i_5, 3)->iValue;







                        







                        auto *v73 = &this->m_activityTraps[trap.id];







                        *v73 = trap;







                    }







                }







                        }







        }







    }







}







void CfgData::fetchBuff()







{







    CDBCFile BuffFile;







    







    if (!BuffFile.OpenFromTXT( "./ServerConfig/Tables/cfg_buff.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_buff.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = BuffFile.GetRecordsNum();







    int32_t iBaseColumnCount = BuffFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgBuff buff;







            







            int32_t nIndex = 0;







            buff.id = BuffFile.Search_Posistion( i, nIndex++)->iValue;







            buff.groupid = BuffFile.Search_Posistion( i, nIndex++)->iValue;







            buff.attack_type = BuffFile.Search_Posistion( i, nIndex++)->iValue;







            buff.level = BuffFile.Search_Posistion( i, nIndex++)->iValue;







            ++nIndex;







            buff.beneficial = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.special = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.duration = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.interval = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string str;







            char v8;







                    const CDBCFile::FIELD *v1 = BuffFile.Search_Posistion( i, nIndex);







            str = v1->pString;







            







            BuffAttrVector __x;







            CfgData::paraseBuffAttr(__x, &str);







            buff.buffAttr = __x;







                    ++nIndex;







            







            buff.isShow = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            buff.modify = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.control = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.hpValue = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.hpPecent = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.shieldValue = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.shieldRatio = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.effectType = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.effectTimes = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.canRemove = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            buff.canRecover = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            ++nIndex;







            ++nIndex;







            







            std::string v9;







            char v10;







                    const CDBCFile::FIELD *v2 = BuffFile.Search_Posistion( i, nIndex);







            v9 = v2->pString;







            buff.angry = atoi(v9.c_str());







                    







            buff.battle = BuffFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto *v3 = &this->m_buffs[buff.id];







            *v3 = buff;







        }







    }







}















void CfgData::paraseBuffAttr(BuffAttrVector& retstr, const std::string *const str)







{







    retstr.clear();







    







    if (str->size() > 4)







    {







        std::string delims;







        char v16;







            delims = "|";







        







        StringVector vstr;







        StringUtility::split(vstr, *str, delims);







            







        if (!vstr.empty())







        {







            int32_t nSize = (int32_t)vstr.size();







            retstr.reserve(nSize);







            







            for (int32_t i = 0; i < nSize; ++i)







            {







                std::string v17;







                char v18;







                            v17 = ":";







                







                StringVector vBuff;







                StringUtility::split(vBuff, vstr[i], v17);







                            







                if (vBuff.size() == 3)







                {







                    BuffAttr stu;







                    stu.attr = atoi(vBuff[0].c_str());







                    stu.ratio = atoi(vBuff[1].c_str());







                    stu.addon = atoi(vBuff[2].c_str());







                    retstr->push_back(stu);







                }







            }







        }







    }







    return;







}















void CfgData::fetchDungeon()







{







    CDBCFile DungeonFile;







    







    if (!DungeonFile.OpenFromTXT( "./ServerConfig/Tables/cfg_dungeon.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_dungeon.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = DungeonFile.GetRecordsNum();







    int32_t iBaseColumnCount = DungeonFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            







            int32_t nIndex = 0;







            CfgDungeon dungeon;



            dungeon.id = DungeonFile.Search_Posistion( i, nIndex++)->iValue;







            







            const CDBCFile::FIELD *v1 = DungeonFile.Search_Posistion( i, nIndex);







            dungeon.name = v1->pString;







            ++nIndex;







            ++nIndex;







            







            dungeon.mapid = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.x = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.y = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.type = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.sequence = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.group_id = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.duration = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.job = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.level = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.maxLevel = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.vip = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.player_num = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            int32_t weekday = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.start_hour = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.end_hour = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.daily_count = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            dungeon.last_id = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.next_id = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.player_buff = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            dungeon.star = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.double_cost = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.reward_time = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            







            std::string strItem;







            char v21;







                    const CDBCFile::FIELD *v2 = DungeonFile.Search_Posistion( i, nIndex);







            strItem = v2->pString;







            







            ItemData v55 = CItemHelper().parseItemDataString(&strItem);







            dungeon.costItem.m_nId = v55.m_nId;







            dungeon.costItem.m_nClass = v55.m_nClass;







            dungeon.costItem.m_nCount = v55.m_nCount;







                    ++nIndex;







            







            std::string strItems;







            char v24;







                    const CDBCFile::FIELD *v3 = DungeonFile.Search_Posistion( i, nIndex);







            strItems = v3->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            dungeon.rewardItem = __x;







                    ++nIndex;







            







            std::string v26;







            char v27;







                    const CDBCFile::FIELD *v4 = DungeonFile.Search_Posistion( i, nIndex);







            v26 = v4->pString;







            







            MemChrBagVector v25;







            CItemHelper().parseItemVectorString(&v25, &v26);







            dungeon.rewardOnce = v25;







                    ++nIndex;







            







            dungeon.Battle = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.costMoney = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.costGold = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            dungeon.buyTimes = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.buyCost = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.quickDoneCost = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.quickDoneDrop = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.needUpTowerStar = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.summon_cost = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.summon_boss = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            dungeon.rand_time = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            ++nIndex;







            dungeon.task_id = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            dungeon.stay_position = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string path;







            std::string str;







            char v30;







                    path = "./ServerConfig/Tables/cfg_dungeon.txt";







            







            char v32;







                    const CDBCFile::FIELD *v5 = DungeonFile.Search_Posistion( i, nIndex);







            str = v5->pString;







            







            Int32Vector v28;







            CfgData::paraseInt32Vector(v28, &str, &path, 0);







            dungeon.win_star = v28;







                    ++nIndex;







            







            std::string v34;







            char v35;







                    const CDBCFile::FIELD *v6 = DungeonFile.Search_Posistion( i, nIndex);







            v34 = v6->pString;







            







            MemChrBagVector v33;







            CItemHelper().parseItemVectorString(&v33, &v34);







            dungeon.star_reward = v33;







                    ++nIndex;







            







            std::string v37;







            char v38;







                    v37 = "./ServerConfig/Tables/cfg_dungeon.txt";







            







            char v40;







                    const CDBCFile::FIELD *v7 = DungeonFile.Search_Posistion( i, nIndex);







            std::string v39;



            v39 = v7->pString;







            







            Int32Vector v36;







            CfgData::paraseInt32Vector(v36, &v39, &v37, 0);







            dungeon.star_ratio = v36;







                    ++nIndex;







            







            dungeon.TeQuan = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.backMapId = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.backX = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.backY = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            dungeon.CanEnter = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            dungeon.BuffId = DungeonFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            while (weekday > 0)







            {







                dungeon.weekday |= 1 << (weekday % 10);







                weekday /= 10;







            }







            







            auto *v8 = &this->m_dungeons[dungeon.id];







            *v8 = dungeon;







            







            if (dungeon.type == 16)







            {







                auto *v9 = &this->m_mUpTowerDungeon[dungeon.star];







                *v9 = dungeon;







            }







            







        }







        







        // 加载副本奖励







        CDBCFile DungeonRewardFile;







            







        if (DungeonRewardFile.OpenFromTXT( "./ServerConfig/Tables/cfg_dungeon_reward.txt"))







        {







            int32_t iBaseTableCount_Reward = DungeonRewardFile.GetRecordsNum();







            int32_t iBaseColumnCount_Reward = DungeonRewardFile.GetFieldsNum();







            







            if (iBaseColumnCount_Reward > 0)







            {







                for (int32_t i_0 = 0; i_0 < iBaseTableCount_Reward; ++i_0)







                {







                    CfgDungeonReward reward;







                    reward.dungeonID = DungeonRewardFile.Search_Posistion( i_0, 0)->iValue;







                    reward.exp = DungeonRewardFile.Search_Posistion( i_0, 1)->iValue;







                    reward.money = DungeonRewardFile.Search_Posistion( i_0, 2)->iValue;







                    reward.cash = DungeonRewardFile.Search_Posistion( i_0, 3)->iValue;







                    reward.groupID = DungeonRewardFile.Search_Posistion( i_0, 4)->iValue;







                    







                    auto *v10 = &this->m_dungeonReward[reward.dungeonID];







                    *v10 = reward;







                }







            }







                }







        







        // 加载副本掉落组







        CDBCFile DungeonDropFile;







            







        if (DungeonDropFile.OpenFromTXT( "./ServerConfig/Tables/cfg_dungeon_drop_group.txt"))







        {







            int32_t iBaseTableCount_Drop = DungeonDropFile.GetRecordsNum();







            int32_t iBaseColumnCount_Drop = DungeonDropFile.GetFieldsNum();







            







            if (iBaseColumnCount_Drop > 0)







            {







                for (int32_t i_1 = 0; i_1 < iBaseTableCount_Drop; ++i_1)







                {







                    int32_t id = DungeonDropFile.Search_Posistion( i_1, 0)->iValue;







                    CfgDungeonDrop group;







                    group.type = DungeonDropFile.Search_Posistion( i_1, 1)->iValue;







                    group.item_id = DungeonDropFile.Search_Posistion( i_1, 3)->iValue;







                    group.item_class = DungeonDropFile.Search_Posistion( i_1, 4)->iValue;







                    group.bind_type = DungeonDropFile.Search_Posistion( i_1, 5)->iValue;







                    group.count = DungeonDropFile.Search_Posistion( i_1, 6)->iValue;







                    group.weight = DungeonDropFile.Search_Posistion( i_1, 7)->iValue;







                    group.probability = DungeonDropFile.Search_Posistion( i_1, 8)->iValue;







                    







                    auto *v11 = &this->m_dungeonDropGroup[id];







                    v11->Add(&group);







                }







            }







                }







    }







}







void CfgData::fetchItem(bool bSend)







{







    CfgItemTable newItems;







    // Already default-constructed







    







    Answer::RwLock::WriteLock _m_itemsLock_wrlock(reinterpret_cast<Answer::RwLock*>(&this->m_itemsLock));







    







    CDBCFile ItemFile;







    







    if (!ItemFile.OpenFromTXT( "./ServerConfig/Tables/cfg_item.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_item.txt failed,please check!!");







    }







    else







    {







        int32_t iBaseTableCount = ItemFile.GetRecordsNum();







        int32_t iBaseColumnCount = ItemFile.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i_1 = 0; i_1 < iBaseTableCount; ++i_1)







            {







                CfgItem *pItem = new CfgItem();







                







                pItem->id = ItemFile.Search_Posistion( i_1, 0)->iValue;







                







                const CDBCFile::FIELD *v3 = ItemFile.Search_Posistion( i_1, 1);







                pItem->name = v3->pString;







                







                const CDBCFile::FIELD *v4 = ItemFile.Search_Posistion( i_1, 3);







                pItem->desc = v4->pString;







                







                pItem->type = ItemFile.Search_Posistion( i_1, 4)->iValue;







                pItem->level = ItemFile.Search_Posistion( i_1, 5)->iValue;







                pItem->grade = ItemFile.Search_Posistion( i_1, 6)->iValue;







                pItem->job = ItemFile.Search_Posistion( i_1, 7)->iValue;







                pItem->in_value = ItemFile.Search_Posistion( i_1, 8)->iValue;







                pItem->out_value = ItemFile.Search_Posistion( i_1, 9)->iValue;







                pItem->bind = ItemFile.Search_Posistion( i_1, 10)->iValue;







                pItem->combine = ItemFile.Search_Posistion( i_1, 11)->iValue;







                pItem->quality = ItemFile.Search_Posistion( i_1, 12)->iValue;







                







                const CDBCFile::FIELD *v14 = ItemFile.Search_Posistion( i_1, 13);







                pItem->url = v14->pString;







                







                const CDBCFile::FIELD *v15 = ItemFile.Search_Posistion( i_1, 14);







                pItem->drop_url = v15->pString;







                







                const CDBCFile::FIELD *v16 = ItemFile.Search_Posistion( i_1, 15);







                pItem->effect = v16->pString;







                







                const CDBCFile::FIELD *v17 = ItemFile.Search_Posistion( i_1, 16);







                pItem->use_method = v17->pString;







                







                pItem->downgrade = ItemFile.Search_Posistion( i_1, 19)->iValue;







                pItem->group_id = ItemFile.Search_Posistion( i_1, 20)->iValue;







                pItem->cd_group = ItemFile.Search_Posistion( i_1, 21)->iValue;







                pItem->overlay = ItemFile.Search_Posistion( i_1, 22)->iValue;







                pItem->can_sell = ItemFile.Search_Posistion( i_1, 23)->iValue;







                pItem->broadcast = ItemFile.Search_Posistion( i_1, 24)->iValue;







                pItem->valid_time = ItemFile.Search_Posistion( i_1, 25)->iValue;







                pItem->item_Grade = ItemFile.Search_Posistion( i_1, 26)->iValue;







                pItem->CanDrop = ItemFile.Search_Posistion( i_1, 32)->iValue;







                pItem->useBroadcast = ItemFile.Search_Posistion( i_1, 33)->iValue;







                pItem->useInCarrier = ItemFile.Search_Posistion( i_1, 35)->iValue;







                pItem->GongGaoId = ItemFile.Search_Posistion( i_1, 40)->iValue;







                pItem->KunLingJingHua = ItemFile.Search_Posistion( i_1, 42)->iValue;







                pItem->XinMoBag = ItemFile.Search_Posistion( i_1, 43)->iValue;







                







                std::string str;







                char v51;







                            const CDBCFile::FIELD *v33 = ItemFile.Search_Posistion( i_1, 44);







                str = v33->pString;







                pItem->RongHeReceovery = atoi(str.c_str());







                            







                auto it_1 = this->m_items.find(pItem->id);







                if (it_1 == this->m_items.end())







                {







                    std::pair<int, CfgItem *> v54(pItem->id, pItem);







                    newItems.insert(v54);







                }







                







                auto *v34 = this->m_items[pItem->id];







                *v34 = *pItem;







            }







        }







    }







    







    







    if (!newItems.empty() && bSend)







    {







        sendNewItems(&newItems);







    }







    







    







    // 加载物品礼包







    Answer::RwLock::WriteLock _m_itemGiftsLock_wrlock(reinterpret_cast<Answer::RwLock*>(&this->m_itemGiftsLock));







    this->m_itemGifts.clear();







    







    CDBCFile ItemGiftFile;







    







    if (!ItemGiftFile.OpenFromTXT( "./ServerConfig/Tables/cfg_item_gift.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_item_gift.txt failed,please check!!");







    }







    else







    {







        int32_t iBaseTableCount_Gift = ItemGiftFile.GetRecordsNum();







        int32_t iBaseColumnCount_Gift = ItemGiftFile.GetFieldsNum();







        







        if (iBaseColumnCount_Gift > 0)







        {







            for (int32_t i_0 = 0; i_0 < iBaseTableCount_Gift; ++i_0)







            {







                CfgItemGift itemGift;







                memset(&itemGift, 0, sizeof(itemGift));







                itemGift.id = ItemGiftFile.Search_Posistion( i_0, 0)->iValue;







                itemGift.item = ItemGiftFile.Search_Posistion( i_0, 2)->iValue;







                itemGift.type = ItemGiftFile.Search_Posistion( i_0, 3)->iValue;







                itemGift.count = ItemGiftFile.Search_Posistion( i_0, 4)->iValue;







                itemGift.bind = ItemGiftFile.Search_Posistion( i_0, 5)->iValue;







                itemGift.job = ItemGiftFile.Search_Posistion( i_0, 6)->iValue;







                itemGift.time = ItemGiftFile.Search_Posistion( i_0, 7)->iValue;







                







                auto it_0 = this->m_itemGifts.find(itemGift.id);







                if (it_0 == this->m_itemGifts.end())







                {







                    CfgItemGiftVector *v36 = new std::vector<CfgItemGift>();







                    this->m_itemGifts[itemGift.id] = v36;







                }







                







                auto v37 = this->m_itemGifts[itemGift.id];







                v37->push_back(itemGift);







            }







        }







    }







    







    // 加载随机物品礼包







    Answer::RwLock::WriteLock _m_itemGiftRandomsLock_wrlock(reinterpret_cast<Answer::RwLock*>(&this->m_itemGiftRandomsLock));







    this->m_itemGiftRandoms.clear();







    







    CDBCFile ItemGiftRandFile;







    







    if (!ItemGiftRandFile.OpenFromTXT( "./ServerConfig/Tables/cfg_item_gift_random.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_item_gift_random.txt failed,please check!!");







    }







    else







    {







        int32_t iBaseTableCount_GiftRand = ItemGiftRandFile.GetRecordsNum();







        int32_t iBaseColumnCount_GiftRand = ItemGiftRandFile.GetFieldsNum();







        







        if (iBaseColumnCount_GiftRand > 0)







        {







            for (int32_t i = 0; i < iBaseTableCount_GiftRand; ++i)







            {







                CfgItemGiftRandom itemGiftRandom;







                itemGiftRandom.id = ItemGiftRandFile.Search_Posistion( i, 0)->iValue;







                itemGiftRandom.item = ItemGiftRandFile.Search_Posistion( i, 1)->iValue;







                itemGiftRandom.type = ItemGiftRandFile.Search_Posistion( i, 2)->iValue;







                itemGiftRandom.count = ItemGiftRandFile.Search_Posistion( i, 3)->iValue;







                itemGiftRandom.bind = ItemGiftRandFile.Search_Posistion( i, 4)->iValue;







                itemGiftRandom.static_probability = ItemGiftRandFile.Search_Posistion( i, 5)->iValue;







                itemGiftRandom.sum_probability = ItemGiftRandFile.Search_Posistion( i, 6)->iValue;







                itemGiftRandom.job = ItemGiftRandFile.Search_Posistion( i, 7)->iValue;







                itemGiftRandom.MinLevel = ItemGiftRandFile.Search_Posistion( i, 8)->iValue;







                itemGiftRandom.MaxLevel = ItemGiftRandFile.Search_Posistion( i, 9)->iValue;







                







                auto it = this->m_itemGiftRandoms.find(itemGiftRandom.id);







                if (it == this->m_itemGiftRandoms.end())







                {







                    CfgItemGiftRandomVector *v38 = new std::vector<CfgItemGiftRandom>();







                    this->m_itemGiftRandoms[itemGiftRandom.id] = v38;







                }







                







                auto v39 = this->m_itemGiftRandoms[itemGiftRandom.id];







                v39->push_back(itemGiftRandom);







            }







        }







    }







    







}















void CfgData::sendNewItems(const CfgItemTable *const items)







{







    GameService *v2 = Answer::Singleton<GameService>::instance();







    Answer::NetPacket *packet = GameService::popNetpacket(v2, 0, Answer::PackType::PACK_DISPATCH, 0x2752u);







    







    if (!packet) return;







    







    int32_t v3 = (int32_t)items->size();







    packet->writeInt32(v3);







    







    for (auto it = items->begin(); it != items->end(); ++it)







    {







        CfgItem *pCfgItem = it->second;







        if (!pCfgItem) continue;







        







        packet->writeInt32(pCfgItem->id);







        packet->writeUTF8(pCfgItem->name);







        packet->writeUTF8(pCfgItem->desc);







        packet->writeInt32(pCfgItem->type);







        packet->writeInt32(pCfgItem->level);







        packet->writeInt32(pCfgItem->job);







        packet->writeInt32(pCfgItem->in_value);







        packet->writeInt32(pCfgItem->out_value);







        packet->writeInt32(pCfgItem->bind);







        packet->writeInt32(pCfgItem->combine);







        packet->writeInt32(pCfgItem->quality);







        packet->writeUTF8(pCfgItem->url);







        packet->writeUTF8(pCfgItem->drop_url);







        packet->writeUTF8(pCfgItem->effect);







        packet->writeUTF8(pCfgItem->use_method);







        packet->writeInt32(pCfgItem->group_id);







        packet->writeInt32(pCfgItem->cd_group);







        packet->writeInt32(pCfgItem->overlay);







        packet->writeInt32(pCfgItem->can_sell);







        packet->writeInt32(pCfgItem->valid_time);







    }







    







    uint32_t WOffset = packet->getWOffset();







    packet->setSize(WOffset);







    







    GameService *v5 = Answer::Singleton<GameService>::instance();







    v5->broadcast(packet);







}















void CfgData::fetchServerConfig(int32_t line)







{







    if (line == 9) return;







    







    char szSQL[4096];







    memset(szSQL, 0, sizeof(szSQL));







    snprintf(szSQL, sizeof(szSQL) - 1, "SELECT * FROM `sys_server_start`");







    







    Answer::DBPool *v2 = Answer::Singleton<Answer::DBPool>::instance();







    Answer::MySqlDBGuard db;







    







    







    Answer::MySqlQuery *v3 = db.query(szSQL);







    Answer::MySqlQuery result;







    result = Answer::MySqlQuery(v3);







    







    if (!result.eof())







    {







        this->m_heFuTime = result.getIntValue("he_fu_time", 0);







        this->m_kaiFuTime = result.getIntValue("kai_fu_time", 0);







        this->m_serverType = result.getIntValue("type", 0);







    }







    







}















int32_t CfgData::getServerStartTime(SERVER_TYPE nType)







{







    if (nType == SERVER_TYPE::SVT_UNITE)







        return this->m_heFuTime;







    else







        return this->m_kaiFuTime;







}















int32_t CfgData::getServerStartDayZeroTime(SERVER_TYPE nType)







{







    if (nType == SERVER_TYPE::SVT_UNITE)







        return Answer::DayTime::dayzero(this->m_heFuTime);







    else







        return Answer::DayTime::dayzero(this->m_kaiFuTime);







}















int32_t CfgData::getServerDiffDay(SERVER_TYPE nType)







{







    if (nType == SERVER_TYPE::SVT_UNITE)







        return Answer::DayTime::daydiff(this->m_heFuTime);







    else







        return Answer::DayTime::daydiff(this->m_kaiFuTime);







}















void CfgData::updateServerStartTime(int32_t kaiFuTime)







{







    if (getServerType() != 0) return;







    







    this->m_kaiFuTime = kaiFuTime;







    COpenBeta *v2 = Answer::Singleton<COpenBeta>::instance();







    COpenBeta::updateStartTime(v2);







}







void CfgData::fetchDungeonEvent()







{







    CDBCFile DungeonEventFile;







    







    if (!DungeonEventFile.OpenFromTXT( "./ServerConfig/Tables/cfg_dungeon_event.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_dungeon_event.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = DungeonEventFile.GetRecordsNum();







    int32_t iBaseColumnCount = DungeonEventFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMapEvent dungeonEvent;







            







            dungeonEvent.id = DungeonEventFile.Search_Posistion( i, 0)->iValue;







            dungeonEvent.trigger_id = DungeonEventFile.Search_Posistion( i, 1)->iValue;







            dungeonEvent.bInitOpen = DungeonEventFile.Search_Posistion( i, 2)->iValue == 0;







            dungeonEvent.trigger_type = DungeonEventFile.Search_Posistion( i, 3)->iValue;







            







            std::string triggerParam;







            char v12;







                    const CDBCFile::FIELD *v1 = DungeonEventFile.Search_Posistion( i, 4);







            triggerParam = v1->pString;







                    







            dungeonEvent.event_type = DungeonEventFile.Search_Posistion( i, 5)->iValue;







            







            const CDBCFile::FIELD *v2 = DungeonEventFile.Search_Posistion( i, 6);







            dungeonEvent.effect = v2->pString;







            dungeonEvent.repeate = DungeonEventFile.Search_Posistion( i, 7)->iValue;







            







            std::string delims;







            std::string str;







            char v14;







                    delims = ":";







            







            char v16;







                    str = triggerParam.c_str();







            







            StringVector strTriggerParam;







            StringUtility::split(strTriggerParam, str, delims);







                    







            for (auto& param : strTriggerParam)







            {







                int val = atoi(param.c_str());







                dungeonEvent.trigger_param.push_back(val);







            }







            







            if (!dungeonEvent.trigger_param.empty())







            {







                auto v6 = this->m_dungeonEvents[dungeonEvent.id];







                v6.push_back(dungeonEvent);







            }







            







        }







    }







}















void CfgData::fetchDungeonMonster()







{







    CDBCFile DungeonMonsterFile;







    







    if (!DungeonMonsterFile.OpenFromTXT( "./ServerConfig/Tables/cfg_dungeon_monster.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_dungeon_monster.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = DungeonMonsterFile.GetRecordsNum();







    int32_t iBaseColumnCount = DungeonMonsterFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgDungeonMonster monster;







            







            monster.id = DungeonMonsterFile.Search_Posistion( i, 0)->iValue;







            monster.wave = DungeonMonsterFile.Search_Posistion( i, 1)->iValue;







            














            char v32;







                    const CDBCFile::FIELD *v2 = DungeonMonsterFile.Search_Posistion( i, 2);







            int32_t nIndex = 0;















            nIndex = atoi(v2->pString);







            







            std::list<Param2> __x;







            CfgData::paraseParam2List(&__x, nullptr, nIndex, nullptr);







            monster.mids = __x;







                    







            monster.x = DungeonMonsterFile.Search_Posistion( i, 3)->iValue;







            monster.y = DungeonMonsterFile.Search_Posistion( i, 4)->iValue;







            monster.count = DungeonMonsterFile.Search_Posistion( i, 5)->iValue;







            monster.side = DungeonMonsterFile.Search_Posistion( i, 6)->iValue;







            







            std::string road;







            char v35;







                    const CDBCFile::FIELD *v3 = DungeonMonsterFile.Search_Posistion( i, 7);







            road = v3->pString;







                    







            monster.delay = DungeonMonsterFile.Search_Posistion( i, 8)->iValue;







            monster.times = DungeonMonsterFile.Search_Posistion( i, 9)->iValue;







            monster.money = DungeonMonsterFile.Search_Posistion( i, 10)->iValue;







            monster.life = DungeonMonsterFile.Search_Posistion( i, 11)->iValue;







            







            std::string randpos;







            char v36;







                    const CDBCFile::FIELD *v4 = DungeonMonsterFile.Search_Posistion( i, 12);







            randpos = v4->pString;







                    







            monster.wait = DungeonMonsterFile.Search_Posistion( i, 13)->iValue;







            







            // 解析路线







            if (road.size() > 3)







            {







                std::string delims;







                char v38;







                            delims = ":";







                







                StringVector vRoad;







                StringUtility::split(vRoad, road, delims);







                            







                for (size_t j = 0; j < vRoad.size(); ++j)







                {







                    std::string v39;







                    char v40;







                                    v39 = ",";







                    







                    StringVector vPos;







                    StringUtility::split(vPos, vRoad[j], v39);







                                    







                    if (vPos.size() == 2)







                    {







                        Position pos;







                        pos.x = atoi(vPos[0].c_str());







                        pos.y = atoi(vPos[1].c_str());







                        monster.road.push_back(pos);







                    }







                }







            }







            







            // 解析随机位置







            if (randpos.size() > 3)







            {







                std::string v42;







                char v43;







                            v42 = "|";







                







                StringVector vRandPos;







                StringUtility::split(vRandPos, randpos, v42);







                            







                for (size_t j_0 = 0; j_0 < vRandPos.size(); ++j_0)







                {







                    std::string v44;







                    char v45;







                                    v44 = ":";







                    







                    StringVector vPos_0;







                    StringUtility::split(vPos_0, vRandPos[j_0], v44);







                                    







                    if (vPos_0.size() == 2)







                    {







                        Position pos;







                        pos.x = atoi(vPos_0[0].c_str());







                        pos.y = atoi(vPos_0[1].c_str());







                        monster.randpos.push_back(pos);







                    }







                }







            }







            







            auto *v21 = &this->m_dungeonMonsters[monster.id];







            *v21 = monster;







            







        }







    }







}















void CfgData::fetchMap()







{







    CDBCFile MapFile;







    







    if (!MapFile.OpenFromTXT( "./ServerConfig/Tables/cfg_map.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_map.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = MapFile.GetRecordsNum();







    int32_t iBaseColumnCount = MapFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMap map;







            







            map.id = MapFile.Search_Posistion( i, 0)->iValue;







            







            const CDBCFile::FIELD *v1 = MapFile.Search_Posistion( i, 1);







            map.name = v1->pString;







            







            map.type = MapFile.Search_Posistion( i, 2)->iValue;







            map.param = MapFile.Search_Posistion( i, 3)->iValue;







            map.width = MapFile.Search_Posistion( i, 4)->iValue;







            map.height = MapFile.Search_Posistion( i, 5)->iValue;







            map.revive = MapFile.Search_Posistion( i, 6)->iValue;







            map.pk_mode = MapFile.Search_Posistion( i, 7)->iValue;







            map.anti_protect = MapFile.Search_Posistion( i, 8)->iValue;







            map.isMount = MapFile.Search_Posistion( i, 9)->iValue;







            map.isFly = MapFile.Search_Posistion( i, 10)->iValue;







            map.isVicegeneral = MapFile.Search_Posistion( i, 11)->iValue;







            map.player_level = MapFile.Search_Posistion( i, 12)->iValue;







            map.player_level_max = MapFile.Search_Posistion( i, 13)->iValue;







            map.team_member = MapFile.Search_Posistion( i, 14)->iValue;







            map.hide_mini = MapFile.Search_Posistion( i, 15)->iValue;







            map.runnerId = MapFile.Search_Posistion( i, 23)->iValue;







            map.jump = MapFile.Search_Posistion( i, 30)->iValue;







            map.GongGaoId = MapFile.Search_Posistion( i, 33)->iValue;







            map.reviveTime = MapFile.Search_Posistion( i, 35)->iValue;







            map.pvp = MapFile.Search_Posistion( i, 36)->iValue;







            map.cross = MapFile.Search_Posistion( i, 38)->iValue;







            map.x = MapFile.Search_Posistion( i, 44)->iValue;







            map.y = MapFile.Search_Posistion( i, 45)->iValue;







            map.drop_rate = MapFile.Search_Posistion( i, 47)->iValue;







            map.XingMaiLevel = MapFile.Search_Posistion( i, 48)->iValue;







            map.SuitId = MapFile.Search_Posistion( i, 49)->iValue;







            map.JueWeiLevel = MapFile.Search_Posistion( i, 50)->iValue;







            map.VipLevel = MapFile.Search_Posistion( i, 51)->iValue;







            map.NeedBroadcast = MapFile.Search_Posistion( i, 54)->iValue;







            map.MapCanUsePet = MapFile.Search_Posistion( i, 55)->iValue;







            map.SignOut = MapFile.Search_Posistion( i, 56)->iValue;







            map.Rdx = MapFile.Search_Posistion( i, 58)->iValue;







            map.Refresh = MapFile.Search_Posistion( i, 60)->iValue;







            map.CanRand = MapFile.Search_Posistion( i, 61)->iValue;







            map.LimitDay = MapFile.Search_Posistion( i, 62)->iValue;







            map.XinMoAct = MapFile.Search_Posistion( i, 63)->iValue;







            







            auto *v2 = &this->m_maps[map.id];







            *v2 = map;







        }







    }







}















void CfgData::fetchMapMonster()







{







    CDBCFile MapMonsterFile;







    







    if (!MapMonsterFile.OpenFromTXT( "./ServerConfig/Tables/cfg_map_monster.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_map_monster.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = MapMonsterFile.GetRecordsNum();







    int32_t iBaseColumnCount = MapMonsterFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMapMonster mapmonster;







            memset(&mapmonster, 0, sizeof(mapmonster));







            mapmonster.id = MapMonsterFile.Search_Posistion( i, 0)->iValue;







            mapmonster.mapid = MapMonsterFile.Search_Posistion( i, 1)->iValue;







            mapmonster.monsterid = MapMonsterFile.Search_Posistion( i, 2)->iValue;







            mapmonster.x = MapMonsterFile.Search_Posistion( i, 3)->iValue;







            mapmonster.y = MapMonsterFile.Search_Posistion( i, 4)->iValue;







            mapmonster.hide = MapMonsterFile.Search_Posistion( i, 5)->iValue;







            mapmonster.side = MapMonsterFile.Search_Posistion( i, 6)->iValue;







            mapmonster.boss = MapMonsterFile.Search_Posistion( i, 7)->iValue;







            mapmonster.Day = MapMonsterFile.Search_Posistion( i, 8)->iValue;







            







            auto v1 = this->m_mapMonsters[mapmonster.mapid];







            v1.push_back(mapmonster);







            







            auto *v2 = &this->m_CfgMapMonsters[mapmonster.id];







            *v2 = mapmonster;







        }







    }







}















void CfgData::fetchMapPlant()







{







    CDBCFile MapPlantFile;







    







    if (!MapPlantFile.OpenFromTXT( "./ServerConfig/Tables/cfg_map_plant.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_map_plant.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = MapPlantFile.GetRecordsNum();







    int32_t iBaseColumnCount = MapPlantFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            int32_t id = MapPlantFile.Search_Posistion( i, 0)->iValue;







            CfgMapPlant mapPlant;







            mapPlant.mapid = MapPlantFile.Search_Posistion( i, 1)->iValue;







            mapPlant.plantid = MapPlantFile.Search_Posistion( i, 2)->iValue;







            mapPlant.x = MapPlantFile.Search_Posistion( i, 3)->iValue;







            mapPlant.y = MapPlantFile.Search_Posistion( i, 4)->iValue;







            







            auto v1 = this->m_mapPlants[mapPlant.mapid];







            v1.push_back(mapPlant);







            







            auto *v2 = &this->m_mMapPlants[id];







            *v2 = mapPlant;







        }







    }







}















void CfgData::fetchMapRegion()







{







    CDBCFile MapRegionFile;







    







    if (!MapRegionFile.OpenFromTXT( "./ServerConfig/Tables/cfg_map_region.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_map_region.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = MapRegionFile.GetRecordsNum();







    int32_t iBaseColumnCount = MapRegionFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMapRegion mapRegion;







            mapRegion.id = MapRegionFile.Search_Posistion( i, 0)->iValue;







            mapRegion.mapid = MapRegionFile.Search_Posistion( i, 2)->iValue;







            mapRegion.min_x = MapRegionFile.Search_Posistion( i, 3)->iValue;







            mapRegion.min_y = MapRegionFile.Search_Posistion( i, 4)->iValue;







            mapRegion.max_x = MapRegionFile.Search_Posistion( i, 5)->iValue;







            mapRegion.max_y = MapRegionFile.Search_Posistion( i, 6)->iValue;







            mapRegion.type = MapRegionFile.Search_Posistion( i, 7)->iValue;







            mapRegion.mId = MapRegionFile.Search_Posistion( i, 9)->iValue;







            







            auto *v1 = &this->m_mapRegions[mapRegion.id];







            *v1 = mapRegion;







            







            auto v2 = this->m_mapRegionsByMapId[mapRegion.mapid];







            v2.push_back(mapRegion);







        }







    }







}















void CfgData::fetchMonster()







{







    CDBCFile MonsterFile;







    







    if (!MonsterFile.OpenFromTXT( "./ServerConfig/Tables/Monster.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MONSTER_TABLE failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = MonsterFile.GetRecordsNum();







    int32_t iBaseColumnCount = MonsterFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMonster monster{};







            







            int32_t nIndex = 0;







            monster.mid = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            monster.group_id = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.level = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.quality = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            







            for (int j = 1; j <= 28; ++j)







            {







                monster.vAttr[j] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            }







            







            monster.rand_count = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string randtypes;







            char v19;







                    const CDBCFile::FIELD *v1 = MonsterFile.Search_Posistion( i, nIndex);







            randtypes = v1->pString;







                    







            monster.exp = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.type = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.revive_time = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.skill_id = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string strSkill;







            char v21;







                    const CDBCFile::FIELD *v2 = MonsterFile.Search_Posistion( i, nIndex);







            strSkill = v2->pString;







            parseMonsterSkill(monster.mid, &monster.unique_skill, &strSkill);







                    ++nIndex;







            







            std::string v22;







            char v23;







                    const CDBCFile::FIELD *v3 = MonsterFile.Search_Posistion( i, nIndex);







            v22 = v3->pString;







            parseMonsterSkill(monster.mid, &monster.random_skill, &v22);







                    







            monster.hpPercent = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.ai = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            nIndex += 9;







            monster.broadcast = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            monster.drop_free = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            nIndex += 2;







            monster.boss_sign = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            nIndex += 5;







            monster.BossSocre = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            monster.TaskShare = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            nIndex += 4;







            monster.camp = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            nIndex += 2;







            monster.kill_point = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            monster.revive_skin = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            ++nIndex;







            monster.PortalRatio = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.PortalId = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.PortalDuration = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            monster.prestige = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.dust = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.is_building = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.PureDamage = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            monster.IfCurse = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            monster.Score = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.IsShowOwner = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            nIndex += 2;







            monster.DiligenceType = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            monster.DiligenceValue = MonsterFile.Search_Posistion( i, nIndex++)->iValue;







            







            const CDBCFile::FIELD *v4 = MonsterFile.Search_Posistion( i, nIndex);







            monster.MonsterHp = strtoll(v4->pString, nullptr, 10);







            monster.CanTunshi = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.Mid2 = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.FamilyValue = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            monster.vAttr[41] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.vAttr[42] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.vAttr[43] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.vAttr[44] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.IsXinMoMonster = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.QieGe = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.vAttr[49] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.Energy = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string strItem;







            char v25;







                    const CDBCFile::FIELD *v5 = MonsterFile.Search_Posistion( i, nIndex);







            strItem = v5->pString;







            CItemHelper().parseItemString(monster.cItem, &strItem);







                    







            monster.IsSunAndMoon = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            monster.corpse_time = 3000;







            







            // 解析随机类型







            if (!randtypes.empty() && randtypes != "-1")







            {







                std::string delims;







                char v27;







                            delims = ":";







                







                StringVector vRand;







                StringUtility::split(vRand, randtypes, delims);







                            







                for (auto& randStr : vRand)







                {







                    int val = atoi(randStr.c_str());







                    monster.rand_types.push_back(val);







                }







            }







            







            auto *v9 = &this->m_monsters[monster.mid];







            *v9 = monster;







            







        }







        







        // 加载怪物广播列表







        CDBCFile MonsterBroadcastFile;







            







        if (MonsterBroadcastFile.OpenFromTXT( "./ServerConfig/Tables/cfg_monster_broadcast.txt"))







        {







            int32_t iBaseTableCountBroadcast = MonsterBroadcastFile.GetRecordsNum();







            int32_t iBaseColumnCountBroadcast = MonsterBroadcastFile.GetFieldsNum();







            







            if (iBaseColumnCountBroadcast > 0)







            {







                for (int32_t i_0 = 0; i_0 < iBaseTableCountBroadcast; ++i_0)







                {







                    const CDBCFile::FIELD *v10 = MonsterBroadcastFile.Search_Posistion( i_0, 1);







                    this->m_monsterBroadcasts.push_back(v10->iValue);







                }







            }







                }







    }







}







void CfgData::fetchNpc()







{







    CDBCFile TabFile;







    bool ret = TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_npc.txt");







    







    if (!ret)







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_npc.txt failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgNpc npc;







            







            npc.id = TabFile.Search_Posistion( i, 0)->iValue;







            npc.npcid = TabFile.Search_Posistion( i, 4)->iValue;







            







            // 解析地图列表







            std::string delims;







            std::string str;







            char v43;







                    delims = "|";







            







            char v45;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 6);







            str = v1->pString;







            







            StringVector mapids;







            StringUtility::split(mapids, str, delims);







                    







            for (auto& mapidStr : mapids)







            {







                int val = atoi(mapidStr.c_str());







                npc.mapids.push_back(val);







            }







            







            npc.x = TabFile.Search_Posistion( i, 7)->iValue;







            npc.y = TabFile.Search_Posistion( i, 8)->iValue;







            npc.func = TabFile.Search_Posistion( i, 9)->iValue;







            npc.func_extra = TabFile.Search_Posistion( i, 10)->iValue;







            







            std::string param;







            char v48;







                    const CDBCFile::FIELD *v4 = TabFile.Search_Posistion( i, 21);







            param = v4->pString;







                    







            std::string v49;







            char v50;







                    v49 = ":";







            







            StringVector vparam;







            StringUtility::split(vparam, param, v49);







                    







            for (size_t j = 0; j < vparam.size(); ++j)







            {







                int val = atoi(vparam[j].c_str());







                npc.params.push_back(val);







            }







            







            npc.unite_flag = TabFile.Search_Posistion( i, 23)->iValue;







            







            std::string platform;







            char v52;







                    const CDBCFile::FIELD *v8 = TabFile.Search_Posistion( i, 25);







            platform = v8->pString;







                    







            if (platform != "0")







            {







                std::string v53;







                char v54;







                            v53 = "|";







                







                StringVector vpf;







                StringUtility::split(vpf, platform, v53);







                            







                for (auto& pf : vpf)







                {







                    npc.platforms.push_back(pf);







                }







            }







            







            // 解析消耗物品







            std::string v55;







            char v56;







                    v55 = "|";







            







            char v58;







                    const CDBCFile::FIELD *v11 = TabFile.Search_Posistion( i, 27);







            std::string v57;







            v57 = v11->pString;







            







            StringVector CostVector;







            StringUtility::split(CostVector, v57, v55);







                    







            for (auto& costStr : CostVector)







            {







                std::string v60;







                char v61;







                            v60 = ":";







                







                StringVector CostItem;







                StringUtility::split(CostItem, costStr, v60);







                            







                if (CostItem.size() == 3)







                {







                    CfgDungeonNpcCost NpcCost;







                    NpcCost.ItemType = atoi(CostItem[0].c_str());







                    NpcCost.ItemId = atoi(CostItem[1].c_str());







                    NpcCost.ItemCount = atoi(CostItem[2].c_str());







                    npc.m_vNpcCost.push_back(NpcCost);







                }







            }







            







            auto *v19 = &this->m_npcs[npc.id];







            *v19 = npc;







        }







        







        // 加载NPC机场







        CDBCFile TabFileAir;







            ret = TabFileAir.OpenFromTXT( "./ServerConfig/Tables/cfg_npc_airport.txt");







        







        if (ret)







        {







            iBaseTableCount = TabFileAir.GetRecordsNum();







            iBaseColumnCount = TabFileAir.GetFieldsNum();







            







            if (iBaseColumnCount > 0)







            {







                for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)







                {







                    CfgNpcAirport airport;







                    airport.id = TabFileAir.Search_Posistion( i_0, 0)->iValue;







                    airport.npcid = TabFileAir.Search_Posistion( i_0, 1)->iValue;







                    airport.player_kingdom = TabFileAir.Search_Posistion( i_0, 3)->iValue;







                    airport.kingdom_id = TabFileAir.Search_Posistion( i_0, 4)->iValue;







                    airport.map_id = TabFileAir.Search_Posistion( i_0, 5)->iValue;







                    airport.map_x = TabFileAir.Search_Posistion( i_0, 6)->iValue;







                    airport.map_y = TabFileAir.Search_Posistion( i_0, 7)->iValue;







                    airport.cost = TabFileAir.Search_Posistion( i_0, 8)->iValue;







                    







                    auto *v20 = &this->m_npcAirports[airport.id];







                    *v20 = airport;







                }







            }







                }







    }







}















void CfgData::fetchPlant()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_plant.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_plant.txt failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgPlant plant{};







            







            plant.id = TabFile.Search_Posistion( i, 0)->iValue;







            plant.type = TabFile.Search_Posistion( i, 3)->iValue;







            plant.level = TabFile.Search_Posistion( i, 4)->iValue;







            







            std::string delims;







            std::string str;







            char v22;







                    delims = "|";







            







            char v24;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 5);







            str = v1->pString;







            







            StringVector strItems;







            StringUtility::split(strItems, str, delims);







                    







            for (auto& eventStr : strItems)







            {







                std::string v26;







                char v27;







                            v26 = ":";







                







                StringVector EventVt;







                StringUtility::split(EventVt, eventStr, v26);







                            







                if (EventVt.size() == 2)







                {







                    CfgPlantEvent Event;







                    Event.EventId = atoi(EventVt[0].c_str());







                    Event.Probability = atoi(EventVt[1].c_str());







                    plant.EventMaxRate += Event.Probability;







                    plant.Events.push_back(Event);







                }







            }







            







            plant.item_cost = TabFile.Search_Posistion( i, 6)->iValue;







            plant.start_hour = TabFile.Search_Posistion( i, 7)->iValue;







            plant.end_hour = TabFile.Search_Posistion( i, 8)->iValue;







            plant.gather_time = TabFile.Search_Posistion( i, 9)->iValue;







            plant.revive_time = TabFile.Search_Posistion( i, 10)->iValue;







            plant.hide_time = TabFile.Search_Posistion( i, 16)->iValue;







            plant.get_points = TabFile.Search_Posistion( i, 18)->iValue;







            plant.boss_id = TabFile.Search_Posistion( i, 19)->iValue;







            plant.BuffId = TabFile.Search_Posistion( i, 20)->iValue;







            plant.revive_skin = TabFile.Search_Posistion( i, 21)->iValue;







            plant.TaskId = TabFile.Search_Posistion( i, 22)->iValue;







            







            std::string v28;







            char v29;







                    v28 = "|";







            







            char v31;







                    const CDBCFile::FIELD *v7 = TabFile.Search_Posistion( i, 23);







            std::string v30;







            v30 = v7->pString;







            







            StringVector ItemString;







            StringUtility::split(ItemString, v30, v28);







                    







            for (auto& itemStr : ItemString)







            {







                std::string v33;







                char v34;







                            v33 = ":";







                







                StringVector RateVt;







                StringUtility::split(RateVt, itemStr, v33);







                            







                if (RateVt.size() == 2)







                {







                    int val = atoi(RateVt[0].c_str());







                    plant.ItemVt.push_back(val);







                }







            }







            







            auto *v11 = &this->m_plants[plant.id];







            *v11 = plant;







        }







    }







}















void CfgData::fetchTask()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_task.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_task.txt failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgTask task;







            







            task.id = TabFile.Search_Posistion( i, 0)->iValue;







            







            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 1);







            snprintf(task.name, sizeof(task.name), "%s", v1->pString);







            







            task.type = TabFile.Search_Posistion( i, 2)->iValue;







            task.group = TabFile.Search_Posistion( i, 3)->iValue;







            task.can_giveup = TabFile.Search_Posistion( i, 4)->iValue;







            task.pretask = TabFile.Search_Posistion( i, 5)->iValue;







            task.posttask = TabFile.Search_Posistion( i, 6)->iValue;







            task.main_order = TabFile.Search_Posistion( i, 7)->iValue;







            task.job = TabFile.Search_Posistion( i, 8)->iValue;







            task.level = TabFile.Search_Posistion( i, 9)->iValue;







            task.max_level = TabFile.Search_Posistion( i, 10)->iValue;







            task.kingdom = TabFile.Search_Posistion( i, 11)->iValue;







            task.start_npc = TabFile.Search_Posistion( i, 12)->iValue;







            task.end_npc = TabFile.Search_Posistion( i, 13)->iValue;







            task.dungeon = TabFile.Search_Posistion( i, 14)->iValue;







            







            std::string strItems;







            char v12;







                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, 15);







            strItems = v2->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            task.items_receive = __x;







                    







            task.award_exp = TabFile.Search_Posistion( i, 16)->iValue;







            task.award_money = TabFile.Search_Posistion( i, 17)->iValue;







            task.gold = TabFile.Search_Posistion( i, 18)->iValue;







            task.dilong = TabFile.Search_Posistion( i, 19)->iValue;







            task.rongyu = TabFile.Search_Posistion( i, 20)->iValue;







            task.fuwen = TabFile.Search_Posistion( i, 21)->iValue;







            task.BossScore = TabFile.Search_Posistion( i, 22)->iValue;







            







            std::string v14;







            char v15;







                    const CDBCFile::FIELD *v3 = TabFile.Search_Posistion( i, 23);







            v14 = v3->pString;







            







            MemChrBagVector v13;







            CItemHelper().parseItemVectorString(&v13, &v14);







            task.award_item = v13;







                    







            std::string v17;







            char v18;







                    const CDBCFile::FIELD *v4 = TabFile.Search_Posistion( i, 24);







            v17 = v4->pString;







            







            MemChrJobBagVector v16;







            CfgData::parseTaskItemJobString(v16, task.id, &v17);







            task.award_optional = v16;







                    







            task.condition = TabFile.Search_Posistion( i, 35)->iValue;







            







            std::string strRequest;







            char v20;







                    const CDBCFile::FIELD *v5 = TabFile.Search_Posistion( i, 36);







            strRequest = v5->pString;







            task.request = parseTaskCondition(task.id, task.condition, &strRequest);







                    







            task.GongXian = TabFile.Search_Posistion( i, 50)->iValue;







            task.JunTuanZiJin = TabFile.Search_Posistion( i, 51)->iValue;







            task.Double = TabFile.Search_Posistion( i, 52)->iValue;







            task.acScore = TabFile.Search_Posistion( i, 53)->iValue;







            task.quickDoneCost = TabFile.Search_Posistion( i, 55)->iValue;







            task.quality = TabFile.Search_Posistion( i, 61)->iValue;







            task.ratio = TabFile.Search_Posistion( i, 62)->iValue;







            task.dust = TabFile.Search_Posistion( i, 63)->iValue;







            task.activity_score = TabFile.Search_Posistion( i, 65)->iValue;







            task.talent_point = TabFile.Search_Posistion( i, 67)->iValue;







            







            std::string path;







            char v23;







                    path = "./ServerConfig/Tables/cfg_task.txt";







            







            char v25;







                    const CDBCFile::FIELD *v6 = TabFile.Search_Posistion( i, 68);







            std::string v22;







            v22 = v6->pString;







            







            std::list<TaskDrop> v21;







            CfgData::parseTaskDrop((CfgData *const)&v21, task.id, &v22);







            task.drop_list = v21;







                    







            task.DoubleGold = TabFile.Search_Posistion( i, 69)->iValue;







            task.BuffId = TabFile.Search_Posistion( i, 70)->iValue;







            







            auto *v7 = &this->m_tasks[task.id];







            *v7 = task;







            







            if (task.type == 3)







            {







                CfgTaskCycleTable::AddTask(&this->m_cfgTaskCycleTable, task.id, task.level, task.max_level);







            }







            if (task.type == 37)







            {







                CfgFamilyTask::AddFamilyTask(&this->m_FamilyTaskTable, task.id, task.level, task.max_level);







            }







            if (task.type == 41)







            {







                CfgActivityTaskTable::AddTask(&this->m_cfgActivityTaskTable, task.id, task.level, task.max_level, task.ratio);







            }







            







        }







    }







}















void CfgData::fetchTrap()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_trap.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_trap.txt failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgTrap trap;







            







            trap.id = TabFile.Search_Posistion( i, 0)->iValue;







            trap.cd = TabFile.Search_Posistion( i, 2)->iValue;







            trap.delay = TabFile.Search_Posistion( i, 3)->iValue;







            trap.event_type = TabFile.Search_Posistion( i, 4)->iValue;







            







            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 6);







            trap.effect = v1->pString;







            trap.item_cost = TabFile.Search_Posistion( i, 7)->iValue;







            trap.life = TabFile.Search_Posistion( i, 12)->iValue;







            







            auto *v2 = &this->m_traps[trap.id];







            *v2 = trap;







        }







    }







}















void CfgData::fetchLevelExp()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_level_exp.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_level_exp.txt failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgLevelExp levelExp;







            levelExp.level = TabFile.Search_Posistion( i, 0)->iValue;







            







            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 1);







            levelExp.upgrade_exp = strtoll(v1->pString, nullptr, 10);







            







            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, 2);







            levelExp.max_exp = strtoll(v2->pString, nullptr, 10);







            







            levelExp.pet_exp = TabFile.Search_Posistion( i, 3)->iValue;







            levelExp.mount_exp = TabFile.Search_Posistion( i, 4)->iValue;







            levelExp.vicegeneral_id = TabFile.Search_Posistion( i, 5)->iValue;







            levelExp.attr_point = TabFile.Search_Posistion( i, 6)->iValue;







            levelExp.talent_point = TabFile.Search_Posistion( i, 7)->iValue;







            







            auto *v3 = &this->m_levelExps[levelExp.level];







            *v3 = levelExp;







        }







    }







}















void CfgData::fetchLevelAttr()







{







    CDBCFile LevelAttrFile;







    







    if (!LevelAttrFile.OpenFromTXT( "./ServerConfig/Tables/cfg_level_attr.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_PLAYER_LEVEL_ATTR_TABLE failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = LevelAttrFile.GetRecordsNum();







    int32_t iBaseColumnCount = LevelAttrFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgLevelAttr levelAttr;







            







            levelAttr.level = LevelAttrFile.Search_Posistion( i, 0)->iValue;







            levelAttr.job = LevelAttrFile.Search_Posistion( i, 1)->iValue;







            







            std::string path;







            std::string addonAttr;







            char v8;







                    path = "./ServerConfig/Tables/cfg_level_attr.txt";







            







            char v10;







                    const CDBCFile::FIELD *v1 = LevelAttrFile.Search_Posistion( i, 2);







            addonAttr = v1->pString;







            







            AttrAddonVector __x;







            CfgData::paraseAttrAddon(__x, &addonAttr, i, &path);







            levelAttr.addonattr = __x;







                    







            std::string v12;







            char v13;







                    v12 = "./ServerConfig/Tables/cfg_level_attr.txt";







            







            char v15;







                    const CDBCFile::FIELD *v2 = LevelAttrFile.Search_Posistion( i, 3);







            std::string v14;







            v14 = v2->pString;







            







            AttrAddonVector v11;







            CfgData::paraseAttrAddon(v11, &v14, i, &v12);







            levelAttr.addonPoint = v11;







                    







            int __k = (levelAttr.job << 16) | levelAttr.level;







            auto *v3 = &this->m_levelAttrs[__k];







            *v3 = levelAttr;







        }







    }







}















void CfgData::fetchJob()







{







    CDBCFile JobFile;







    







    if (!JobFile.OpenFromTXT( "./ServerConfig/Tables/cfg_job.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_job.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = JobFile.GetRecordsNum();







    int32_t iBaseColumnCount = JobFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgJob job;







            job.id = JobFile.Search_Posistion( i, 0)->iValue;







            job.job_task = JobFile.Search_Posistion( i, 4)->iValue;







            job.attack_attr = JobFile.Search_Posistion( i, 5)->iValue;







            job.base_skill = JobFile.Search_Posistion( i, 6)->iValue;







            







            auto *v1 = &this->m_jobs[job.id];







            *v1 = job;







        }







    }







}















void CfgData::fetchChrShop()







{







    CDBCFile ChrShopFile;







    







    if (!ChrShopFile.OpenFromTXT( "./ServerConfig/Tables/cfg_chr_shop.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_chr_shop.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = ChrShopFile.GetRecordsNum();







    int32_t iBaseColumnCount = ChrShopFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgChrShop chrShop;







            chrShop.Index = ChrShopFile.Search_Posistion( i, 0)->iValue;







            chrShop.ItemId = ChrShopFile.Search_Posistion( i, 1)->iValue;







            chrShop.ItemClass = ChrShopFile.Search_Posistion( i, 2)->iValue;







            chrShop.IsBind = ChrShopFile.Search_Posistion( i, 3)->iValue;







            chrShop.LimitCount = ChrShopFile.Search_Posistion( i, 4)->iValue;







            chrShop.Price = ChrShopFile.Search_Posistion( i, 5)->iValue;







            chrShop.ConstType = ChrShopFile.Search_Posistion( i, 9)->iValue;







            chrShop.QiQinglevel = ChrShopFile.Search_Posistion( i, 10)->iValue;







            







            auto *v1 = &this->m_chrShops[chrShop.Index];







            *v1 = chrShop;







        }







    }







}







void CfgData::fetchMovie()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_movie.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_movie.txt failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMovie movie;







            movie.id = TabFile.Search_Posistion( i, 0)->iValue;







            int32_t MoveId = TabFile.Search_Posistion( i, 2)->iValue;







            







            if (movie.id == MoveId)







            {







                auto *v1 = &this->m_movie[movie.id];







                v1->id = movie.id;







            }







        }







    }







}















void CfgData::fetchMonsterAI()







{







    CDBCFile MonsterFile;







    







    if (!MonsterFile.OpenFromTXT( "./ServerConfig/Tables/MonsterAi.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MONSTER_AI_TABLE failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = MonsterFile.GetRecordsNum();







    int32_t iBaseColumnCount = MonsterFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMonsterAI ai;







            memset(&ai, 0, sizeof(ai));







            







            int32_t nIndex = 0;







            ai.id = MonsterFile.Search_Posistion( i, 0)->iValue;







            ai.style = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.target = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.escape_hp = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.view_range = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.move_range = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.rest_range = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.rest_time_min = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.rest_time_max = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.rest_ratio = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.run_distance = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.run_range = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.run_cd = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ai.pursuit_range = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto *v1 = &this->m_mMonsterAI[ai.id];







            *v1 = ai;







        }







    }







}















void CfgData::fetchMonsterAdjustTable()







{







    CDBCFile MonsterFile;







    







    if (!MonsterFile.OpenFromTXT( "./ServerConfig/Tables/MonsterAdjust.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MONSTER_ADJUST_TABLE failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = MonsterFile.GetRecordsNum();







    int32_t iBaseColumnCount = MonsterFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMonsterAdjust monster;







            memset(&monster, 0, sizeof(monster));







            







            int32_t nIndex = 0;







            monster.mid = MonsterFile.Search_Posistion( i, 0)->iValue;







            monster.adj_level = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.level = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            







            for (int j = 1; j <= 28; ++j)







            {







                monster.vAttr[j] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            }







            







            monster.vAttr[41] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.vAttr[42] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.vAttr[43] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            monster.vAttr[44] = MonsterFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto key = std::make_pair(monster.mid, monster.adj_level);







            auto result = this->m_mMonsterAdjust.insert(std::make_pair(key, monster));







        }







    }







    







    // 加载 Boss 成长表







    CDBCFile MonsterFile2;







    







    if (MonsterFile2.OpenFromTXT( "./ServerConfig/Tables/BossGrow.txt"))







    {







        iBaseTableCount = MonsterFile2.GetRecordsNum();







        iBaseColumnCount = MonsterFile2.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)







            {







                CfgMonsterAdjust monster;







                memset(&monster, 0, sizeof(monster));







                







                int32_t nIndex_0 = 0;







                monster.mid = MonsterFile2.Search_Posistion( i_0, 0)->iValue;







                monster.adj_level = MonsterFile2.Search_Posistion( i_0, ++nIndex_0)->iValue;







                monster.exp = MonsterFile2.Search_Posistion( i_0, ++nIndex_0)->iValue;







                ++nIndex_0;







                







                for (int j = 1; j <= 28; ++j)







                {







                    monster.vAttr[j] = MonsterFile2.Search_Posistion( i_0, ++nIndex_0)->iValue;







                }







                







                monster.vAttr[41] = MonsterFile2.Search_Posistion( i_0, ++nIndex_0)->iValue;







                monster.vAttr[42] = MonsterFile2.Search_Posistion( i_0, ++nIndex_0)->iValue;







                monster.vAttr[43] = MonsterFile2.Search_Posistion( i_0, ++nIndex_0)->iValue;







                monster.vAttr[44] = MonsterFile2.Search_Posistion( i_0, ++nIndex_0)->iValue;







                ++nIndex_0;







                







                auto key = std::make_pair(monster.mid, monster.adj_level);







                this->m_mMonsterAdjust.insert(std::make_pair(key, monster));







            }







        }







        }







}















void CfgData::fetchMonsterDropGroup()







{







    CDBCFile MonsterDropFile;







    







    if (!MonsterDropFile.OpenFromTXT( "./ServerConfig/Tables/cfg_monster_drop_group.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_monster_drop_group.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = MonsterDropFile.GetRecordsNum();







    int32_t iBaseColumnCount = MonsterDropFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMonsterDropGroup monsterDropGroup;







            memset(&monsterDropGroup, 0, sizeof(monsterDropGroup));







            







            int32_t nIndex = 0;







            monsterDropGroup.group_id = MonsterDropFile.Search_Posistion( i, 0)->iValue;







            monsterDropGroup.item_id = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterDropGroup.item_class = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterDropGroup.item_count = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterDropGroup.bind_type = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterDropGroup.weight = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterDropGroup.probability = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterDropGroup.cost_type = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterDropGroup.cost_value = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterDropGroup.limit_time = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterDropGroup.daily_limit = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string p_StringTime;







            char v7;







                    const CDBCFile::FIELD *v1 = MonsterDropFile.Search_Posistion( i, nIndex);







            p_StringTime = v1->pString;







            monsterDropGroup.start_date = Answer::DayTime::StringToIntTime(&p_StringTime);







                    ++nIndex;







            







            std::string v8;







            char v9;







                    const CDBCFile::FIELD *v2 = MonsterDropFile.Search_Posistion( i, nIndex);







            v8 = v2->pString;







            monsterDropGroup.end_date = Answer::DayTime::StringToIntTime(&v8);







                    







            monsterDropGroup.record = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterDropGroup.festival_group = MonsterDropFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto v3 = this->m_monsterDropGroups[monsterDropGroup.group_id];







            v3.push_back(monsterDropGroup);







        }







    }







}















void CfgData::fetchMonsterGroupDrop()







{







    CDBCFile MonsterGroupFile;







    







    if (!MonsterGroupFile.OpenFromTXT( "./ServerConfig/Tables/cfg_monster_group_drop.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_monster_group_drop.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = MonsterGroupFile.GetRecordsNum();







    int32_t iBaseColumnCount = MonsterGroupFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMonsterGroupDrop monsterGroupDrop;







            memset(&monsterGroupDrop, 0, sizeof(monsterGroupDrop));







            







            int32_t nIndex = 0;







            monsterGroupDrop.mid = MonsterGroupFile.Search_Posistion( i, 0)->iValue;







            monsterGroupDrop.group_id = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.probability = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.begin_time = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.end_time = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.repeat = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.job = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.grow_level = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.dropType = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.bind_type = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.hard = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.quality = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.world_event = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            monsterGroupDrop.DropLimit = MonsterGroupFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            if (monsterGroupDrop.begin_time < 1440 && monsterGroupDrop.end_time < 1440 &&







                monsterGroupDrop.begin_time <= monsterGroupDrop.end_time && monsterGroupDrop.repeat > 0)







            {







                auto v1 = this->m_monsterGroupDrops[monsterGroupDrop.mid];







                v1.push_back(monsterGroupDrop);







            }







            else







            {







                Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, 







                    "wrong cfg_monster_group_drop data with mid = %d, group_id = %d\n", 







                    monsterGroupDrop.mid, monsterGroupDrop.group_id);







            }







        }







    }







}















void CfgData::fetchMonsterTaskDrop()







{







    CDBCFile MonsterTaskFile;







    







    if (!MonsterTaskFile.OpenFromTXT( "./ServerConfig/Tables/cfg_monster_task_drop.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_monster_task_drop.txt failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = MonsterTaskFile.GetRecordsNum();







    int32_t iBaseColumnCount = MonsterTaskFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMonsterTaskDrop monsterTaskDrop;







            monsterTaskDrop.mid = MonsterTaskFile.Search_Posistion( i, 0)->iValue;







            monsterTaskDrop.tid = MonsterTaskFile.Search_Posistion( i, 1)->iValue;







            monsterTaskDrop.item = MonsterTaskFile.Search_Posistion( i, 2)->iValue;







            monsterTaskDrop.probability = MonsterTaskFile.Search_Posistion( i, 3)->iValue;







            







            auto v1 = this->m_monsterTaskDrops[monsterTaskDrop.mid];







            v1.push_back(monsterTaskDrop);







        }







    }







}















// ==================== 辅助方法 ====================















void CfgData::paresPosition(std::vector<Position>& retstr, const std::string *const strPos)







{







    retstr.clear();







    







    if (!strPos->empty())







    {







        std::string delims;







        char v15;







            delims = "|";







        







        StringVector PosString;







        StringUtility::split(PosString, *strPos, delims);







            







        for (auto& posStr : PosString)







        {







            std::string v17;







            char v18;







                    v17 = ":";







            







            StringVector Pos;







            StringUtility::split(Pos, posStr, v17);







                    







            if (Pos.size() == 2)







            {







                Position stu;







                stu.x = atoi(Pos[0].c_str());







                stu.y = atoi(Pos[1].c_str());







                retstr->push_back(stu);







            }







        }







    }







    return;







}















Param2 CfgData::paraseParam2(const std::string *const str)







{







    if (str->empty() || *str == "-1")







    {







        Param2 result;







        result.nParam1 = 0;







        result.nParam2 = 0;







        return result;







    }







    







    std::string delims;







    char v13;







    delims = ":";







    







    StringVector vParam;







    StringUtility::split(vParam, *str, delims);







    







    Param2 result;







    if (vParam.size() == 2)







    {







        result.nParam1 = atoi(vParam[0].c_str());







        result.nParam2 = atoi(vParam[1].c_str());







    }







    else







    {







        result.nParam1 = 0;







        result.nParam2 = 0;







    }







    







    return result;







}















void CfgData::paraseInt32Vector(CfgInt32Vector& retstr, const std::string *const str, const std::string *const path, int32_t size)







{







    retstr.clear();







    







    if (str->empty() || *str == "-1") return;







    







    std::string delims;







    char v19;







    delims = ":";







    







    StringVector vstr;







    StringUtility::split(vstr, *str, delims);







    







    if (size > 0 && (int32_t)vstr.size() != size)







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, 







            "CfgData::paraseInt32Vector() check size err from %s, where size = %d, str = %s\n",







            path->c_str(), size, str->c_str());







    }







    else







    {







        retstr.reserve(vstr.size());







        for (auto& valStr : vstr)







        {







            int val = atoi(valStr.c_str());







            retstr->push_back(val);







        }







    }







    







    return;







}















void CfgData::paraseAttrAddon(AttrAddonVector& retstr, const std::string *const addonAttr, int32_t nIndex, const std::string *const path)







{







    retstr.clear();







    







    if (addonAttr->empty() || *addonAttr == "-1" || *addonAttr == "0") return;







    







    std::string delims;







    char v22;







    delims = "|";







    







    StringVector strAttrAddons;







    StringUtility::split(strAttrAddons, *addonAttr, delims);







    







    for (auto& addonStr : strAttrAddons)







    {







        std::string v24;







        char v25;







            v24 = ":";







        







        StringVector strAttrAddon;







        StringUtility::split(strAttrAddon, addonStr, v24);







            







        if (strAttrAddon.size() == 2)







        {







            AttrAddon attrAddon;







            attrAddon.index = atoi(strAttrAddon[0].c_str());







            attrAddon.addon = atoi(strAttrAddon[1].c_str());







            if (attrAddon.index > 0 && attrAddon.addon > 0)







            {







                retstr->push_back(attrAddon);







            }







        }







        else







        {







            Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,







                "CfgData::paraseAttrAddon() wrong data from %s, where index = %d, str = %s\n",







                path->c_str(), nIndex, addonAttr->c_str());







        }







    }







    







    return;







}















int32_t CfgData::getOverlay(int32_t nId, int8_t nClass)







{







    if (nClass == 3)







    {







        const CfgItemGem *cfgGem = CfgItemGemTable::GetItemGem(&this->m_cfgItemGem, nId);







        if (!cfgGem) return 0;







        int32_t overlay = cfgGem->m_nLayNum;







        return (overlay == 0) ? 1 : overlay;







    }







    else if (nClass == 1)







    {







        CfgItem *cfgItem = getItem(nId);







        if (!cfgItem) return 0;







        int32_t overlay = cfgItem->overlay;







        return (overlay == 0) ? 1 : overlay;







    }







    else if (nClass == 8)







    {







        WuHunItem *pWuHun = GetWuHunItem(nId);







        if (!pWuHun) return 0;







        int32_t overlay = pWuHun->overlay;







        return (overlay == 0) ? 1 : overlay;







    }







    else if (nClass == 11)







    {







        return 99;







    }







    return 1;







}















int32_t CfgData::getOutValue(int32_t nId, int8_t nClass)







{







    if (nClass == 2)







    {







        const CfgEquip *pEquip = getEquip(nId);







        if (pEquip) return pEquip->m_nPrice;







    }







    else if (nClass == 3)







    {







        const CfgItemGem *pGem = CfgItemGemTable::GetItemGem(&this->m_cfgItemGem, nId);







        if (pGem) return pGem->m_nPrice;







    }







    else if (nClass == 7)







    {







        const CfgPetEquip *pPetEquip = CfgPetEquipTable::GetEquip(&this->m_cfgPetEquipTable, nId);







        if (pPetEquip) return pPetEquip->nPrice;







    }







    else if (nClass == 1)







    {







        CfgItem *pItem = getItem(nId);







        if (pItem) return pItem->out_value;







    }







    return -1;







}















bool CfgData::dropBroadcast(int32_t nId, int8_t nClass)







{







    switch (nClass)







    {







    case 2:







    {







        const CfgEquip *pEquip = getEquip(nId);







        if (pEquip) return pEquip->m_nBroadcast > 0;







        break;







    }







    case 7:







    {







        const CfgPetEquip *pPetEquip = CfgPetEquipTable::GetEquip(&this->m_cfgPetEquipTable, nId);







        if (pPetEquip) return pPetEquip->nBroadcast > 0;







        break;







    }







    case 1:







    {







        CfgItem *pItem = getItem(nId);







        if (pItem) return pItem->broadcast > 0;







        break;







    }







    }







    return false;







}















bool CfgData::canSell(int32_t nId, int8_t nClass)







{







    if (nClass != 1) return true;







    CfgItem *pCfgItem = getItem(nId);







    return (pCfgItem == nullptr) || (pCfgItem->can_sell != 0);







}















int32_t CfgData::InitXuWuTask(int32_t Level, int32_t Times)







{







    std::vector<int> TaskIdVt;







    







    for (auto& pair : this->m_tasks)







    {







        const CfgTask& task = pair.second;







        if (task.type == 9 && task.level <= Level && task.max_level >= Level && task.group == Times)







        {







            TaskIdVt.push_back(task.id);







        }







    }







    







    if (TaskIdVt.empty()) return 0;







    







    std::random_shuffle(TaskIdVt.begin(), TaskIdVt.end());







    return TaskIdVt.front();







}















int32_t CfgData::InitXinMoTask(int32_t Level, int32_t Times)







{







    std::vector<int> TaskIdVt;







    







    for (auto& pair : this->m_tasks)







    {







        const CfgTask& task = pair.second;







        if (task.type == 42 && task.level <= Level && task.max_level >= Level && task.group == Times)







        {







            TaskIdVt.push_back(task.id);







        }







    }







    







    if (TaskIdVt.empty()) return 0;







    







    std::random_shuffle(TaskIdVt.begin(), TaskIdVt.end());







    return TaskIdVt.front();







}















int32_t CfgData::RandPdbfTask(int32_t Level)







{







    std::vector<int> TaskVt;







    







    for (auto& pair : this->m_tasks)







    {







        const CfgTask& task = pair.second;







        if (task.type == 4 && task.level <= Level && task.max_level >= Level)







        {







            TaskVt.push_back(task.id);







        }







    }







    







    if (TaskVt.empty()) return 0;







    







    std::random_shuffle(TaskVt.begin(), TaskVt.end());







    return TaskVt.front();







}







void CfgData::InitActiveSkillTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/cfg_skill_info.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_ACTIVE_SKILL_TABLE fail, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgActiveSkill skill;







            memset(&skill, 0, sizeof(skill));







            new (&skill.summon_attr) std::vector<AttrAddon>();







            







            int32_t nIndex = 0;







            skill.id = TabFile.Search_Posistion( i, 0)->iValue;







            ++nIndex;







            skill.talent = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.groupid = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.job = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.kind = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.distance = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.range = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.area = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.self = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.target_num = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.beneficial = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.tar_type = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.addon_skill = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.addon_time = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.addon_cd = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.addon_delay = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.addon_trig_times = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.cd = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.cd_adjust = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.mp = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.power = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.special = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.attack_type = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.attack_modify = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.building_modify = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.buff_rate = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.buff = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            skill.chantTime = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.summon_id = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            skill.summon_delay = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string path;







            std::string addonAttr;







            char v6;







                    path = "./ServerConfig/Tables/cfg_skill_info.txt";







            







            char v8;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            addonAttr = v1->pString;







            







            AttrAddonVector __x;







            CfgData::paraseAttrAddon(__x, &addonAttr, i, &path);







            skill.summon_attr = __x;







                    







            skill.summon_limit = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            nIndex += 17;







            skill.boss_addon_damage = TabFile.Search_Posistion( i, nIndex++)->iValue;







            skill.append_value = TabFile.Search_Posistion( i, nIndex++)->iValue;







            skill.shu_lian_du = TabFile.Search_Posistion( i, nIndex++)->iValue;







            skill.cd += skill.cd_adjust;







            







            CfgSkillTable::AddActiveSkill(&this->m_cfgSkillTable, &skill);







        }







    }







}















void CfgData::InitPassiveSkillTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/SkillPassiveAttr.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_PASSIVE_SKILL_TABLE fail, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgPassiveSkill stu;







            memset(&stu, 0, sizeof(stu));







            new (&stu.vAttrs) std::vector<AttrAddon>();







            stu.lTalentAddon.clear();







            







            int32_t nIndex = 0;







            stu.id = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.type = readFile.Search_Posistion( i, nIndex++)->iValue;







            ++nIndex;







            







            std::string path;







            std::string addonAttr;







            char v7;







                    path = "./ServerConfig/Tables/SkillPassiveAttr.txt";







            







            char v9;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            addonAttr = v1->pString;







            







            AttrAddonVector __x;







            CfgData::paraseAttrAddon(__x, &addonAttr, i, &path);







            stu.vAttrs = __x;







                    ++nIndex;







            







            std::string v11;







            char v12;







                    v11 = "./ServerConfig/Tables/SkillPassiveAttr.txt";







            







            char v14;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            std::string v13;







            v13 = v2->pString;







            







            std::list<TalentAddon> v10;







            CfgData::paraseTalentAddon(&v10, &v13, i, &v11);







            stu.lTalentAddon = v10;







                    







            stu.dropMoneyRate = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            CfgSkillTable::AddPassiveSkill(&this->m_cfgSkillTable, &stu);







        }







    }







}















void CfgData::InitTrigSkillTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/SkillTrig.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_TRIG_SKILL_TABLE fail, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgTrigSkill stu;







            







            int32_t nIndex = 0;







            stu.id = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.groupid = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.trigType = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            stu.trigParam = v1->pString;







            stu.targetType = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.trigRate = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.cdTime = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.trigBuff = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.special = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.specialParam = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.IsPvp = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            CfgSkillTable::AddTrigSkill(&this->m_cfgSkillTable, &stu);







        }







    }







}















void CfgData::InitTalentTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/SkillTalent.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_TALENT_TABLE fail, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgTalent stu;







            







            int32_t nIndex = 0;







            stu.id = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.Level = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.skillid = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.maxLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.Playerlevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            ++nIndex;







            







            std::string bCombi;







            char v7;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            bCombi = v1->pString;







            







            std::list<ItemData> strItems;







            // TODO: fix CItemHelper::parseItemDataListString call







            stu.costItem = strItems;







                    ++nIndex;







            







            std::string size;







            char v10;







                    size = "./ServerConfig/Tables/SkillTalent.txt";







            







            std::string path;







            char v12;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            path = v2->pString;







            







            std::list<int> __x;







            paraseInt32List(&__x, &path, atoi(size.c_str()), nullptr);







            stu.powerSkills = __x;







                    ++nIndex;







            ++nIndex;







            







            stu.battle = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            stu.Point = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.GongGaoId = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            CfgTalentTable::AddTalent(&this->m_cfgTalentTable, &stu);







        }







    }







}















void CfgData::InitTalentPageTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/SkillTree.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_TALENT_PAGE_TABLE fail, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgTalentPage stu;







            







            int32_t nIndex = 0;







            stu.job = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string size;







            char v6;







                    size = "./ServerConfig/Tables/SkillTree.txt";







            







            std::string path;







            char v8;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            path = v1->pString;







            







            std::list<int> __x;







            paraseInt32List(&__x, &path, atoi(size.c_str()), nullptr);







            stu.talents = __x;







                    ++nIndex;







            







            CfgTalentTable::AddTalentPage(&this->m_cfgTalentTable, &stu);







        }







    }







}















void CfgData::InitFamilySkillTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/FamilySkill.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_FAMILY_SKILL_TABLE fail, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgFamilySkill stu;







            







            int32_t nIndex = 0;







            stu.nId = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nLevel = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nFamilyLevel = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nCostValue = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string path;







            std::string addonAttr;







            char v6;







                    path = "./ServerConfig/Tables/FamilySkill.txt";







            







            char v8;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            addonAttr = v1->pString;







            







            AttrAddonVector __x;







            CfgData::paraseAttrAddon(__x, &addonAttr, i, &path);







            stu.vAttrAddon = __x;







                    







            stu.nCostMoney = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.PlayerLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            CfgSkillTable::AddFamilySkill(&this->m_cfgSkillTable, &stu);







        }







    }







}















void CfgData::InitTalentActiveTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/SkillActive.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_ACTIVE_TALENT_TABLE fail, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgTalentActive stu;







            







            int32_t nIndex = 0;







            stu.nId = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string bCombi;







            char v6;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            bCombi = v1->pString;







            







            std::list<ItemData> strItems;







            // TODO: fix CItemHelper::parseItemDataListString call







            stu.lItems = strItems;







                    ++nIndex;







            







            CfgSkillTable::AddTalentActive(&this->m_cfgSkillTable, &stu);







        }







    }







}







void CfgData::InitEquipTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/Equip.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ITEM_EQUIP_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgEquip equip{};







            







            int32_t nIndex = 0;







            equip.m_nId = readFile.Search_Posistion( i, 0)->iValue;







            ++nIndex;







            equip.m_nType = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_nGrade = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_nLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_nJob = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_nQuality = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_nSuitId = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_nSuitId2 = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_nPrice = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string path;







            std::string addonAttr;







            char v8;







                    path = "./ServerConfig/Tables/Equip.txt";







            







            char v10;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            addonAttr = v1->pString;







            







            AttrAddonVector __x;







            CfgData::paraseAttrAddon(__x, &addonAttr, i, &path);







            equip.m_vAttrAddon = __x;







                    ++nIndex;







            







            std::string v12;







            char v13;







                    v12 = "./ServerConfig/Tables/Equip.txt";







            







            char v15;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            std::string v14;







            v14 = v2->pString;







            







            AttrAddonVector v11;







            CfgData::paraseAttrAddon(v11, &v14, i, &v12);







            equip.m_vElement = v11;







                    ++nIndex;







            nIndex += 10;







            equip.m_DropLuck = readFile.Search_Posistion( i, nIndex++)->iValue;







            equip.m_DropRate = readFile.Search_Posistion( i, nIndex++)->iValue;







            equip.m_BackType = readFile.Search_Posistion( i, nIndex++)->iValue;







            equip.m_BackValue = readFile.Search_Posistion( i, nIndex++)->iValue;







            equip.m_nBroadcast = readFile.Search_Posistion( i, nIndex++)->iValue;







            equip.m_backIndex = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            ++nIndex;







            equip.m_nLimitStar = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_BackGold = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string v17;







            char v18;







                    v17 = "./ServerConfig/Tables/Equip.txt";







            







            char v20;







                    const CDBCFile::FIELD *v3 = readFile.Search_Posistion( i, nIndex);







            std::string v19;







            v19 = v3->pString;







            







            AttrAddonVector v16;







            CfgData::paraseAttrAddon(v16, &v19, i, &v17);







            equip.m_BaseAttr = v16;







                    







            equip.m_PolishLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            equip.m_MoFuHuiShou = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_SellDay = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_GongMingLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_WingEquipRefiningLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_XinMoExp = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_XinMoBag = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_CanRongHe = readFile.Search_Posistion( i, ++nIndex)->iValue;







            equip.m_TeJieParam = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            CfgEquipTable::AddEquip(&this->m_cfgEquip, &equip);







        }







    }







}















void CfgData::InitEquipUpStarTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/EquipUpStar.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ITEM_EQUIP_UP_STAR_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgEquipUpStar stu;







            







            int32_t nIndex = 0;







            stu.m_nType = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.m_nStar = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.m_nRate = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.m_nSuccessAddStar = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.m_nFailLostStar = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string bCombi;







            char v7;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            bCombi = v1->pString;







            







            std::list<ItemData> strItems;







            // TODO: fix CItemHelper::parseItemDataListString call







            stu.m_lCosItem = strItems;







                    







            stu.m_nCostMoney = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string path;







            std::string addonAttr;







            char v10;







                    path = "./ServerConfig/Tables/EquipUpStar.txt";







            







            char v12;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            addonAttr = v2->pString;







            







            AttrAddonVector __x;







            CfgData::paraseAttrAddon(__x, &addonAttr, i, &path);







            stu.m_vAttrAddon = __x;







                    ++nIndex;







            ++nIndex;







            







            stu.m_nCostXingMai = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.m_RongLianAttr = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            CfgEquipTable::AddEquipUpStar(&this->m_cfgEquip, &stu);







        }







    }







}















void CfgData::InitWingCfgTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/AttributeWing.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WING_CFG_TABEL failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            WingCfg stu;







            memset(&stu, 0, sizeof(stu));







            new (&stu.ConstItems) std::list<ItemData>();







            new (&stu.AddonVector) std::vector<AttrAddon>();







            







            int32_t nIndex = 0;







            stu.Level = readFile.Search_Posistion( i, 0)->iValue;







            ++nIndex;







            







            std::string bCombi;







            char v7;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            bCombi = v1->pString;







            







            std::list<ItemData> strItems;







            // TODO: fix CItemHelper::parseItemDataListString call







            stu.ConstItems = strItems;







                    







            stu.StartPoints = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.SuccessPoints = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.MaxPoints = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.Rate = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.FailAddPoints = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.SkillId = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.SkillLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.IsClear = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.GongGaoId = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string path;







            std::string addonAttr;







            char v10;







                    path = "./ServerConfig/Tables/AttributeWing.txt";







            







            char v12;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            addonAttr = v2->pString;







            







            AttrAddonVector __x;







            CfgData::paraseAttrAddon(__x, &addonAttr, i, &path);







            stu.AddonVector = __x;







                    ++nIndex;







            







            WingCfg p_stu;







            WingCfg::WingCfg(&p_stu, &stu);







            CfgWingTable::AddWingCfg(&this->m_cfgWing, &p_stu);







        }







    }







}















void CfgData::InitCarrierTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/Carrier.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_CARRIER_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgCarrier stu;







//             CfgCarrier stu;
            memset(&stu, 0, sizeof(stu));







            new (&stu.lSkills) std::list<int>();







            







            int32_t nIndex = 0;







            stu.nId = readFile.Search_Posistion( i, 0)->iValue;







            ++nIndex;







            







            std::string size;







            char v6;







                    size = "./ServerConfig/Tables/Carrier.txt";







            







            std::string path;







            char v8;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            path = v1->pString;







            







            std::list<int> __x;







            paraseInt32List(&__x, &path, atoi(size.c_str()), nullptr);







            stu.lSkills = __x;







                    ++nIndex;







            







            CfgCarrierTable::AddCarrier(&this->m_cfgCarrierTable, &stu);







        }







    }







}















void CfgData::InitPetTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/Pet.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_PET_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgPetData pet;







            







            int32_t nIndex = 0;







            pet.m_nPetId = readFile.Search_Posistion( i, nIndex++)->iValue;







            pet.m_Quality = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            for (int32_t j = 0; j <= 8; ++j)







            {







                pet.m_vSkill[j] = readFile.Search_Posistion( i, nIndex++)->iValue;







                pet.m_vSkillOpen[j] = readFile.Search_Posistion( i, nIndex++)->iValue;







            }







            







            CfgPetTable::Add(&this->m_cfgPetTable, &pet);







        }







    }







}















void CfgData::InitFamilyTable()







{







    CDBCFile readFile;







    bool ret = readFile.OpenFromTXT( "./ServerConfig/Tables/Family.txt");







    







    if (!ret)







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_FAMILY_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        CfgFamily family;







        







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgFamily::CleanUp(&family);







            







            int32_t nIndex = 0;







            family.nLevel = readFile.Search_Posistion( i, nIndex++)->iValue;







            family.nExp = readFile.Search_Posistion( i, nIndex++)->iValue;







            family.nMaxMembers = readFile.Search_Posistion( i, nIndex++)->iValue;







            family.vPosition[1] = readFile.Search_Posistion( i, nIndex++)->iValue;







            family.vPosition[2] = readFile.Search_Posistion( i, nIndex++)->iValue;







            family.nMaxBossPoints = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            CfgFamilyTable::Add(&this->m_cfgFamilyTable, &family);







        }







    }







    







    // 加载家族Boss表







    CDBCFile readFile2;







    ret = readFile2.OpenFromTXT( "./ServerConfig/Tables/FamilyBoss.txt");







    







    if (ret)







    {







        iBaseTableCount = readFile2.GetRecordsNum();







        iBaseColumnCount = readFile2.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)







            {







                CfgFamilyBoss stu;







                int32_t nIndex_0 = 0;








                stu.nFamilyLevel = readFile2.Search_Posistion( i_0, 0)->iValue;







                stu.BossMid = readFile2.Search_Posistion( i_0, ++nIndex_0)->iValue;







                stu.FamilyExp = readFile2.Search_Posistion( i_0, ++nIndex_0)->iValue;







                stu.GongGaoId = readFile2.Search_Posistion( i_0, ++nIndex_0)->iValue;







                ++nIndex_0;







                







                CfgFamilyTable::AddFamilyBoss(&this->m_cfgFamilyTable, &stu);







            }







        }







        }







}















void CfgData::InitHoeTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/Hoe.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_HOE_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            HoeCfg Stu;







            int32_t nIndex = 0;







            Stu.nId = readFile.Search_Posistion( i, 0)->iValue;







            Stu.nNextId = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            Stu.nDouble = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto *v1 = &this->m_HoeCfgMap[Stu.nId];







            *v1 = Stu;







        }







    }







}















HoeCfg *CfgData::GetHoeCfg(int32_t nId)







{







    auto it = this->m_HoeCfgMap.find(nId);







    if (it != this->m_HoeCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitTitleTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/Title.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TITLE_TABLE failed,please check!!");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgTitle title;







            







            int32_t nIndex = 0;







            title.nId = readFile.Search_Posistion( i, 0)->iValue;







            ++nIndex;







            title.nType = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string params;







            char v14;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            params = v1->pString;







                    







            const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, ++nIndex);







            title.sPlatform = v2->pString;







            title.nJob = readFile.Search_Posistion( i, ++nIndex)->iValue;







            title.nSex = readFile.Search_Posistion( i, ++nIndex)->iValue;







            title.nSpecial = readFile.Search_Posistion( i, ++nIndex)->iValue;







            title.nPriority = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string getttr;







            char v15;







                    const CDBCFile::FIELD *v3 = readFile.Search_Posistion( i, nIndex);







            getttr = v3->pString;







                    ++nIndex;







            







            std::string dressattr;







            char v16;







                    const CDBCFile::FIELD *v4 = readFile.Search_Posistion( i, nIndex);







            dressattr = v4->pString;







                    ++nIndex;







            







            std::string delims;







            char v18;







                    delims = ":";







            







            StringVector strParams;







            StringUtility::split(strParams, params, delims);







                    







            for (auto& param : strParams)







            {







                int val = atoi(param.c_str());







                title.vParams.push_back(val);







            }







            







            std::string path;







            char v23;







                    path = "./ServerConfig/Tables/Title.txt";







            







            AttrAddonVector v21;







            CfgData::paraseAttrAddon(v21, &getttr, i, &path);







            title.vGetAttr = v21;







                    







            std::string v25;







            char v26;







                    v25 = "./ServerConfig/Tables/Title.txt";







            







            AttrAddonVector v24;







            CfgData::paraseAttrAddon(v24, &dressattr, i, &v25);







            title.vDressAttr = v24;







                    







            







            CfgTitleTable::Add(&this->m_cfgTitleTable, &title);







        }







    }







}







void CfgData::InitGoldEggTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/ItemGoldEggOpen.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_GOLD_EGG_OPEN_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgGoldEgg stu;







            







            int32_t nIndex = 0;







            stu.nId = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nGroupId = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nOpenTimes = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nCostMoney = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nCostGold = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            CfgGoldEggTable::AddGoldEgg(&this->m_cfgGoldEggTable, &stu);







        }







    }







    







    // 加载金蛋产出表







    CDBCFile readFile2;







    if (readFile2.OpenFromTXT( "./ServerConfig/Tables/ItemGoldEgg.txt"))







    {







        iBaseTableCount = readFile2.GetRecordsNum();







        iBaseColumnCount = readFile2.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)







            {







                int32_t nId = readFile2.Search_Posistion( i_0, 0)->iValue;







                CfgGoldEggItem stu;







                stu.nId = readFile2.Search_Posistion( i_0, 1)->iValue;







                stu.nGroupId = readFile2.Search_Posistion( i_0, 2)->iValue;







                stu.nOpenTimes = readFile2.Search_Posistion( i_0, 3)->iValue;







                stu.nCostMoney = readFile2.Search_Posistion( i_0, 4)->iValue;







                stu.nCostGold = readFile2.Search_Posistion( i_0, 5)->iValue;







                







                CfgGoldEggTable::AddGoldEggProduce(&this->m_cfgGoldEggTable, nId, &stu);







            }







        }







        }







}















void CfgData::InitLimitTimeTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/LimitTime.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_LIMIT_TIME_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgLimitTime stu;







            int32_t nIndex = 0;







            stu.nIndex = readFile.Search_Posistion( i, 0)->iValue;







            stu.nType = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string strTime;







            char v6;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            strTime = v1->pString;







                    ++nIndex;







            







            if (stu.nType == 2)







            {







                stu.nTime = Answer::DayTime::StringToIntTime(&strTime);







            }







            else if (stu.nType == 3 || stu.nType == 1)







            {







                stu.nTime = atoi(strTime.c_str());







            }







            







            CfgLimitTimeTable::AddLimitTime(&this->m_cfgLimitTimeTable, &stu);







        }







    }







}















void CfgData::InitMysteryShopTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/MysteriousShop.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MYSTERY_SHOP_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMysteryShop stu;







            memset(&stu, 0, sizeof(stu));







            







            int32_t nIndex = 0;







            stu.nId = readFile.Search_Posistion( i, 0)->iValue;







            stu.nType = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string strItem;







            char v9;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            strItem = v1->pString;







            







            MemChrBag v4;







            CItemHelper().parseItemString(v4, &strItem);







            stu.item = v4;







                    







            stu.nCostType = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nPrice = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            stu.nRate = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nMinLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nMaxLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nBroad = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string v10;







            char v11;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            v10 = v2->pString;







            







            ItemData v17 = CItemHelper().parseItemDataString(&v10);







            stu.exchange.m_nId = v17.m_nId;







            stu.exchange.m_nClass = v17.m_nClass;







            stu.exchange.m_nCount = v17.m_nCount;







                    ++nIndex;







            







            CfgMysteryShopTable::Add(&this->m_cfgMysteryShopTable, &stu);







        }







    }







}















void CfgData::InitBuyGiftTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/TimeLimit.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BUY_GIFT_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgBuyGift stu;







            







            int32_t nIndex = 0;







            stu.nIndex = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nGold = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string items;







            char v6;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            items = v1->pString;







                    ++nIndex;







            nIndex += 3;







            stu.nBroad = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &items);







            stu.vGift = __x;







            







            auto *v2 = &this->m_cfgBuyGiftTable[stu.nIndex];







            *v2 = stu;







        }







    }







}















void CfgData::InitExchangeTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/JiZiDuiJiang.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_EXCHANGE_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgExchange stu;







            







            int32_t nIndex = 0;







            stu.nIndex = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nType = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string items;







            char v7;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            items = v1->pString;







                    ++nIndex;







            







            std::string rewards;







            char v8;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            rewards = v2->pString;







                    







            stu.nLimit = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::list<ItemData> strItems;







            // TODO: fix CItemHelper::parseItemDataListString call







            stu.vCost = strItems;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &rewards);







            stu.vReward = __x;







            







            CfgExchangeTable::Add(&this->m_cfgExchangeTable, &stu);







        }







    }







}















void CfgData::InitMysteryGiftTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/MysteriousGift.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MYSTERY_GIFT_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMysteryGift stu;







            







            int32_t nIndex = 0;







            stu.nIndex = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nType = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nCondition = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string strItems;







            char v6;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            strItems = v1->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.vItem = __x;







                    ++nIndex;







            nIndex += 3;







            stu.nBroadId = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            CfgMysteryGiftTable::Add(&this->m_cfgMysteryGiftTable, &stu);







        }







    }







}















void CfgData::InitDrawTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/Draw.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_DRAW_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgDrawReward stu;







            







            int32_t nIndex = 0;







            stu.nIndex = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nType = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nRate = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            MemChrBag item;







            memset(&item, 0, sizeof(item));







            item.itemClass = readFile.Search_Posistion( i, nIndex++)->iValue;







            item.itemId = readFile.Search_Posistion( i, nIndex++)->iValue;







            item.itemCount = readFile.Search_Posistion( i, nIndex++)->iValue;







            item.bind = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nBroad = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nId = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.vItem.push_back(item);







            







            CfgDrawTable::Add(&this->m_cfgDrawTable, &stu);







        }







    }







}















void CfgData::InitMapRoadTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/MapRoad.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MAP_ROAD_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMapRoad stu;







            







            int32_t nIndex = 0;







            stu.nIndex = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nNextIndex = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nMapId = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string strRoad;







            char v13;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            strRoad = v1->pString;







                    ++nIndex;







            







            std::string delims;







            char v15;







                    delims = "|";







            







            StringVector vRoad;







            StringUtility::split(vRoad, strRoad, delims);







                    







            for (auto& roadStr : vRoad)







            {







                std::string v17;







                char v18;







                            v17 = ":";







                







                StringVector vPos;







                StringUtility::split(vPos, roadStr, v17);







                            







                if (vPos.size() == 2)







                {







                    Position __x;







                    __x.x = atoi(vPos[0].c_str());







                    __x.y = atoi(vPos[1].c_str());







                    stu.road.push_back(__x);







                }







            }







            







            CfgMapRoadTable::Add(&this->m_cfgMapRoadTable, &stu);







        }







    }







}















void CfgData::InitTrailerTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/Trailer.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TRAILER_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgTrailer stu;







            







            int32_t nIndex = 0;







            stu.nId = TabFile.Search_Posistion( i, 0)->iValue;







            stu.nAttrList = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.TrailerQuality = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nTaskId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.PureDamage = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            CfgTrailerTable::Add(&this->m_cfgTrailerTable, &stu);







        }







    }







}















void CfgData::InitMYSJRewardTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/MoYuShiJieReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MYSJ_REWARD_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        int32_t nGroupId = 0;







        int32_t nGroupIndex = 0;







        std::list<CardGroup> cardList;







        







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMYSJReward stu;







            memset(&stu, 0, sizeof(stu));







            







            int32_t nIndex = 0;







            int32_t nId = readFile.Search_Posistion( i, 0)->iValue;







            stu.nId = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nClass = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nCount = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nBind = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nWeight = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nShow = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nGroup = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nMin = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nMax = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nBroad = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nSpecial = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            if (stu.nGroup > 0 && stu.nWeight > 0)







            {







                if (nGroupId && stu.nGroup != nGroupId)







                {







                    CCardGroupBoxManager *v1 = Answer::Singleton<CCardGroupBoxManager>::instance();







                    v1->Add(nGroupId, &cardList);







                    nGroupId = stu.nGroup;







                    nGroupIndex = 0;







                    cardList.clear();







                }







                else if (!nGroupId)







                {







                    nGroupId = stu.nGroup;







                }







                stu.nIndex = ++nGroupIndex;







                







                CardGroup card;







                card.id = nGroupIndex;







                card.count = stu.nWeight;







                card.min = stu.nMin;







                card.max = stu.nMax;







                cardList.push_back(card);







            }







            







            CfgMYSJRewardTable::Add(&this->m_cfgMYSJRewardTable, nId, &stu);







        }







        







        if (nGroupId > 0)







        {







            CCardGroupBoxManager *v2 = Answer::Singleton<CCardGroupBoxManager>::instance();







            v2->Add(nGroupId, &cardList);







        }







    }







}















void CfgData::InitMaintainCompensateTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/MaintainCompensate.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MAINTAIN_COMPENSATE_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgMaintainCompensate stu;







            







            int32_t nIndex = 0;







            stu.nIndex = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string p_StringTime;







            char v6;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            p_StringTime = v1->pString;







            stu.nTime = Answer::DayTime::StringToIntTime(&p_StringTime);







                    ++nIndex;







            







            std::string strItems;







            char v9;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            strItems = v2->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.vItems = __x;







                    ++nIndex;







            







            CfgMaintainCompensateTable::Add(&this->m_cfgMaintainCompensateTable, &stu);







        }







    }







}















void CfgData::InitWishRewardTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/WishReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WISH_REWARD_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgWishReward stu;







            







            int32_t nIndex = 0;







            int32_t nId = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nTime = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string strItems;







            char v6;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            strItems = v1->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.vReward = __x;







                    ++nIndex;







            







            CfgWishRewardTable::Add(&this->m_cfgWishRewardTable, nId, &stu);







        }







    }







}















void CfgData::InitBFZLEnterCostTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/BingFengZouLang.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BFZL_ENTER_COST_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            int32_t nTimes = readFile.Search_Posistion( i, 1)->iValue;







            int32_t nIndex = 0;

            ++nIndex;







            







            std::string bCombi;







            char v5;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            bCombi = v1->pString;







            







            std::list<ItemData> vItem;







            // TODO: fix CItemHelper::parseItemDataListString call







                    ++nIndex;







            







            CfgBFZLEnterCostTable::AddEnterCost(&this->m_cfgBFZLEnterCostTable, nTimes, &vItem);







        }







    }







}















void CfgData::InitBlacketMarketTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/BlackMarket.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BLACK_MARKET_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            int32_t nDays = readFile.Search_Posistion( i, 0)->iValue;







            int8_t nOrder = readFile.Search_Posistion( i, 1)->iValue;







            int32_t nIndex = 0;

            ++nIndex;







            







            std::string strItem;







            char v9;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            strItem = v1->pString;







            







            MemChrBag item1;







            CItemHelper().parseItemString(item1, &strItem);







                    ++nIndex;







            







            std::string v10;







            char v11;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            v10 = v2->pString;







            







            MemChrBag item2;







            CItemHelper().parseItemString(item2, &v10);







                    ++nIndex;







            







            std::string v12;







            char v13;







                    const CDBCFile::FIELD *v3 = readFile.Search_Posistion( i, nIndex);







            v12 = v3->pString;







            







            MemChrBag item3;







            CItemHelper().parseItemString(item3, &v12);







                    







            int32_t nOldPrice = readFile.Search_Posistion( i, ++nIndex)->iValue;







            int32_t nPrice = readFile.Search_Posistion( i, ++nIndex)->iValue;







            int32_t nBroadcast = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            CfgBlacketMarketTable::Add(&this->m_cfgBlacketMarketTable, nDays, nOrder, 1, &item1, nPrice, nBroadcast);







            CfgBlacketMarketTable::Add(&this->m_cfgBlacketMarketTable, nDays, nOrder, 2, &item2, nPrice, nBroadcast);







            CfgBlacketMarketTable::Add(&this->m_cfgBlacketMarketTable, nDays, nOrder, 3, &item3, nPrice, nBroadcast);







        }







    }







}















void CfgData::InitLevelChatTimesTable()







{
    int32_t nIndex = 0;






    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/PublicChatTimes.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_LEVEL_CHAT_TIMES_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            int32_t nLevel = readFile.Search_Posistion( i, 0)->iValue;







            int32_t nTimes = readFile.Search_Posistion( i, 1)->iValue;







            ++nIndex;







            







            auto *v1 = &this->m_cfgLevelChatTable[nLevel];







            *v1 = nTimes;







        }







    }







}















int32_t CfgData::GetChatTimes(int32_t nLevel)







{







    auto it = this->m_cfgLevelChatTable.find(nLevel);







    if (it != this->m_cfgLevelChatTable.end())







    {







        return it->second;







    }







    return -1;







}















void CfgData::InitSuperMemberTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/SuperMember.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SUPER_MEMBER_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            std::string platform;







            char v6;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, 0);







            platform = v1->pString;







                    







            CfgSuperMember stu;







            stu.nGold = readFile.Search_Posistion( i, 1)->iValue;







            stu.nQQ = readFile.Search_Posistion( i, 2)->iValue;







            int32_t nIndex = 0;
            ++nIndex;







            stu.nIcon = readFile.Search_Posistion( i, 4)->iValue;







            ++nIndex;







            







            auto *v2 = &this->m_cfgSuperMember[platform];







            *v2 = stu;







        }







    }







}















const CfgSuperMember *CfgData::GetSuperMember(const std::string *const platform)







{







    auto it = this->m_cfgSuperMember.find(*platform);







    if (it != this->m_cfgSuperMember.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitSouGouSkinTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/SogouSkin.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SOU_GOU_SKIN_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgSouGouSkin stu{};







            







            int32_t nIndex = 0;







            std::string platform;







            char v7;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            platform = v1->pString;







                    ++nIndex;







            







            std::string strItems;







            char v10;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            strItems = v2->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.vReward = __x;







                    







            stu.nIcon = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto *v3 = &this->m_cfgSouGouSkin[platform];







            *v3 = stu;







        }







    }







}















const CfgSouGouSkin *CfgData::GetSouGouSkin(const std::string *const platform)







{







    auto it = this->m_cfgSouGouSkin.find(*platform);







    if (it != this->m_cfgSouGouSkin.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitPetEquipTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/PetEquip.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_PET_EQUIP_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgPetEquip stu{};







            







            int32_t nIndex = 0;







            stu.nId = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nType = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nPrice = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nQuality = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nSuitId = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string path;







            std::string addonAttr;







            char v7;







                    path = "./ServerConfig/Tables/PetEquip.txt";







            







            char v9;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            addonAttr = v1->pString;







            







            AttrAddonVector __x;







            CfgData::paraseAttrAddon(__x, &addonAttr, i, &path);







            stu.vAttr = __x;







                    ++nIndex;







            nIndex += 5;







            stu.nBroadcast = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.bCanDrop = readFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nGrade = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string v11;







            char v12;







                    v11 = "./ServerConfig/Tables/PetEquip.txt";







            







            char v14;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            std::string v13;







            v13 = v2->pString;







            







            AttrAddonVector v10;







            CfgData::paraseAttrAddon(v10, &v13, i, &v11);







            stu.vOwnerAttr = v10;







                    







            stu.nNeedStar = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            CfgPetEquipTable::AddEquip(&this->m_cfgPetEquipTable, &stu);







        }







    }







}















void CfgData::InitWeiXinTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/WeiXin.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WEI_XIN_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            







            int32_t nIndex = 0;







            const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, 0);







            CfgWeiXingGift stu;
            stu.strPlatform = v1->pString;







            stu.nIconId = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            ++nIndex;







            ++nIndex;







            







            std::string strItems;







            char v7;







                    const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, nIndex);







            strItems = v2->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.vReward = __x;







                    ++nIndex;







            







            CfgWeiXinTable::Add(&this->m_cfgWeiXinTable, &stu);







        }







    }







}















void CfgData::InitAdultGiftTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/ShenFenYanZheng.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ADULT_GIFT_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            







            int32_t nIndex = 1;







            







            std::string strItems;







            char v7;







                    const CDBCFile::FIELD *v1 = readFile.Search_Posistion( i, nIndex);







            strItems = v1->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);






            CfgAdultGift stu;

            stu.vReward = __x;







                    







            const CDBCFile::FIELD *v2 = readFile.Search_Posistion( i, ++nIndex);







stu.strPlatfrom = v2->pString;







            stu.nIconId = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            CfgAdultGiftTable::Add(&this->m_cfgAdultGiftTable, &stu);







        }







    }







}















// ==================== 解析辅助方法 ====================















void CfgData::parseMonsterSkill(int32_t id, MonsterSkill (*const vSkill)[10], const std::string *const strSkill)







{







    if (strSkill->empty() || strSkill->size() <= 3) return;







    







    std::string delims;







    char v22;







    delims = "|";







    







    StringVector skills;







    StringUtility::split(skills, *strSkill, delims);







    







    int32_t isize = (int32_t)skills.size();







    if (isize > 9) isize = 10;







    







    for (int32_t i = 0; i < isize; ++i)







    {







        std::string v23;







        char v24;







            v23 = ":";







        







        StringVector skill;







        StringUtility::split(skill, skills[i], v23);







            







        if (skill.size() == 3)







        {







            (*vSkill)[i].maxHp = atoi(skill[0].c_str());







            (*vSkill)[i].minHp = atoi(skill[1].c_str());







            (*vSkill)[i].skillid = atoi(skill[2].c_str());







        }







        else







        {







            Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,







                "CfgData::parseMonsterSkill wrong data with id = %d, string = %s\n",







                id, strSkill->c_str());







        }







    }







}















TaskRequest CfgData::parseTaskCondition(int32_t id, int32_t condition, const std::string *const strRequest)







{







    TaskRequest request;







    memset(&request, 0, sizeof(request));







    







    std::string delims;







    char v21;







    delims = ":";







    







    StringVector requests;







    StringUtility::split(requests, *strRequest, delims);







    







    int32_t nSize = (int32_t)requests.size();







    







    switch (nSize)







    {







    case 3:







        request.param1 = atoi(requests[0].c_str());







        request.param2 = atoi(requests[1].c_str());







        request.param3 = atoi(requests[2].c_str());







        break;







    case 2:







        request.param1 = atoi(requests[0].c_str());







        request.param2 = atoi(requests[1].c_str());







        break;







    case 1:







        request.param1 = atoi(requests[0].c_str());







        break;







    default:







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,







            "CfgData::parseTaskCondition wrong data with id = %d, string = %s\n",







            id, strRequest->c_str());







        break;







    }







    







    return request;







}















void CfgData::parseTaskDrop(int32_t id, const std::string *const strDrop)







{







    // 解析任务掉落







    if (strDrop->empty() || *strDrop == "-1") return;







    







    // 实现解析逻辑...







}















int32_t CfgData::GetMonsterReviveTime(int32_t Time, int32_t BossId)







{







    if (BossId != 1) return Time;







    if (Time <= 0) return Time;







    







    CfgData *v4 = Answer::Singleton<CfgData>::instance();







    int32_t diffDay = CfgData::getServerDiffDay(v4, SERVER_TYPE::SVT_NORMAL);







    







    switch (diffDay)







    {







    case 0: return 10 * (Time / 100);







    case 1:







    case 2: return 30 * (Time / 100);







    case 3: return 50 * (Time / 100);







    case 4: return 60 * (Time / 100);







    case 5: return 70 * (Time / 100);







    case 6: return 80 * (Time / 100);







    default: return Time;







    }







}















void CfgData::InitMobilePhoneGiftTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/ShouJi.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MOBILE_PHONE_GIFT_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            







            int32_t nIndex = 1;







            







            std::string strItems;







            char v8;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            strItems = v1->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);





            CfgMobilePhoneGift stu;


            stu.vItem = __x;







                    







            const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, ++nIndex);







            stu = v2->pString;







            stu.nIcon = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto *v3 = &this->m_CfgMobilePhoneGift[stu.strPlatfrom];







            *v3 = stu;







        }







    }







}







void CfgData::InitMiniClientTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/WeiDuan.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MINI_CLIENT_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {
            CfgMiniClient stu;







            







            int32_t nIndex = 0;







            const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            stu.strPlatfrom = v1->pString;







            stu.nIconDownload = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nIconLogin = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string strItems;







            char v8;







                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







            strItems = v2->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.vReward = __x;







                    ++nIndex;







            







            auto *v3 = &this->m_CfgMiniClient[stu.strPlatfrom];







            *v3 = stu;







        }







    }







}















void CfgData::InitWuHunShopTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/MysterShop.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WU_HUN_SHOP_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgWuHunShop stu;







            memset(&stu, 0, sizeof(stu));







            







            int32_t nIndex = 0;







            stu.Index = TabFile.Search_Posistion( i, 0)->iValue;







            int32_t ShopId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string strItem;







            char v10;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            strItem = v1->pString;







            







            MemChrBag v4;







            CItemHelper().parseItemString(v4, &strItem);







            stu.Item = v4;







                    







            stu.Rate = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            stu.Const = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto v2 = this->m_CfgWuHunShopMap[ShopId];







            v2.push_back(stu);







            







            auto *v3 = &this->m_CfgWuHunShopItemMap[stu.Index];







            *v3 = stu;







        }







    }







}















void CfgData::InitWuHunItemTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/WuHun.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WU_HUN_ITEM_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            WuHunItem stu{};







            







            int32_t nIndex = 0;







            stu.nId = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nLevel = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nType = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nQuality = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nNeedQuality = TabFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string v6;







            char v7;







                    v6 = "./ServerConfig/Tables/WuHun.txt";







            







            std::string v8;







            char v9;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            v8 = v1->pString;







            







            std::list<AddAttribute> __x;







            CfgData::parseAddAttribues(&__x, &v8, i, &v6);







            stu.lAttrList = __x;







                    







            stu.nTalentId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nTalentLevel = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            nIndex += 8;







            stu.overlay = TabFile.Search_Posistion( i, nIndex++)->iValue;







            nIndex += 3;







            stu.nDressLevel = TabFile.Search_Posistion( i, nIndex++)->iValue;







            







            auto *v2 = &this->m_WuHunItemMap[stu.nId];







            *v2 = stu;







        }







    }







}















void CfgData::InitWuHunCreateTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/WuHunMake.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_CREATE_WU_HUN_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CreateWuHun stu{};







            







            int32_t nIndex = 0;







            stu.nId = TabFile.Search_Posistion( i, nIndex++)->iValue;







            nIndex += 2;







            







            std::string bCombi;







            char v11;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            bCombi = v1->pString;







            







            std::list<ItemData> strItems;







            // TODO: fix CItemHelper::parseItemDataListString call







            stu.ConstItem = strItems;







                    ++nIndex;







            







            std::string v13;







            char v14;







                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







            v13 = v2->pString;







            







            std::list<RateItem> v12;







            // TODO: fix CItemHelper::parseRateItemDataListString call







            stu.GetItemRate = v12;







                    ++nIndex;







            nIndex += 3;







            







            std::string strItem;







            char v16;







                    const CDBCFile::FIELD *v3 = TabFile.Search_Posistion( i, nIndex);







            strItem = v3->pString;







            







            ItemData v25 = CItemHelper().parseItemDataString(&strItem);







            stu.SpecialCost.m_nId = v25.m_nId;







            stu.SpecialCost.m_nClass = v25.m_nClass;







            stu.SpecialCost.m_nCount = v25.m_nCount;







                    ++nIndex;







            







            std::string v18;







            char v19;







                    const CDBCFile::FIELD *v4 = TabFile.Search_Posistion( i, nIndex);







            v18 = v4->pString;







            







            std::list<RateItem> v17;







            // TODO: fix CItemHelper::parseRateItemDataListString call







            stu.GetItemRate2 = v17;







                    ++nIndex;







            







            auto *v5 = &this->m_CreateWuHunMap[stu.nId];







            *v5 = stu;







        }







    }







}















void CfgData::InitBossDistribution()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/MonsterRefreshInMaps.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BOSS_LEVEL_IFNO_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            BossLevelInfo stu;







            memset(&stu, 0, sizeof(stu));







            new (&stu.BossMapList) std::list<int>();







            







            int32_t nIndex = 0;







            stu.BossLevel = TabFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string BossMapListString;







            char v9;







                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







            BossMapListString = v2->pString;







                    ++nIndex;







            







            std::list<int> __x;







            paraseInt32List(&__x, &BossMapListString, 0, nullptr);







            stu.BossMapList = __x;







            







            BossLevelInfo p_stu;







            BossLevelInfo::BossLevelInfo(&p_stu, &stu);







            BossDistribution::AddBossLevelInfo(&this->m_BossDistribution, &p_stu);







        }







    }







    







    // 加载地图Boss信息







    CDBCFile TabFile2;







    if (TabFile2.OpenFromTXT( "./ServerConfig/Tables/MapClassInfo.txt"))







    {







        iBaseTableCount = TabFile2.GetRecordsNum();







        iBaseColumnCount = TabFile2.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)







            {







                MapBossInfo stu;







                memset(&stu, 0, sizeof(stu));







                new (&stu.BossMapList) std::list<int>();







                







                int32_t nIndex_0 = 0;







                stu.nId = TabFile2.Search_Posistion( i_0, nIndex_0++)->iValue;







                







                std::string BossMapListString_0;







                char v12;







                            const CDBCFile::FIELD *v3 = TabFile2.Search_Posistion( i_0, nIndex_0);







                BossMapListString_0 = v3->pString;







                            ++nIndex_0;







                







                std::list<int> v13;







                paraseInt32List(&v13, &BossMapListString_0, 0, nullptr);







                stu.BossMapList = v13;







                







                MapBossInfo v14;







                MapBossInfo::MapBossInfo(&v14, &stu);







                BossDistribution::AddMapBossInfo(&this->m_BossDistribution, &v14);







            }







        }







        }







}















void CfgData::InitSpecialBossMapCfgMap()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/MapConsume.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MAP_BOSS_INFO_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            SpecialBossMapCfg stu;







            memset(&stu, 0, sizeof(stu));







            







            int32_t nIndex = 0;







            stu.MapId = TabFile.Search_Posistion( i, 0)->iValue;







            ++nIndex;







            







            std::string strItem;







            char v7;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            strItem = v1->pString;







            







            ItemData v13 = CItemHelper().parseItemDataString(&strItem);







            stu.ConstItem.m_nId = v13.m_nId;







            stu.ConstItem.m_nClass = v13.m_nClass;







            stu.ConstItem.m_nCount = v13.m_nCount;







                    







            stu.StartCD = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.ContinuedTime = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto *v2 = &this->m_SpecialBossMapCfgMap[stu.MapId];







            *v2 = stu;







        }







    }







}















void CfgData::InitDuiHuanLimitTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/ExchangeShengYaoBi.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MAP_BOSS_INFO_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        this->m_DuiHuanLimit.clear();







        







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            DuiHuanLimit stu;







            int32_t nIndex = 1;







            int32_t MaxLevel = TabFile.Search_Posistion( i, 1)->iValue;







            stu.ShengYaoBi = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.ConstGold = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.ConstCurrency = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.Limit = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto *v1 = &this->m_DuiHuanLimit[MaxLevel];







            *v1 = stu;







        }







    }







}















DuiHuanLimit *CfgData::GetDuiHuanLimitCount(int32_t Level)







{







    auto it = this->m_DuiHuanLimit.lower_bound(Level);







    if (it != this->m_DuiHuanLimit.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitSuperTeHuiTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/SuperDiscount.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SUPER_DISCOUNT_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        this->m_SuperTeHuiCfgMap.clear();







        







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            SuperTeHuiCfg stu{};







            







            int32_t nIndex = 0;







            stu.nIndex = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nNeedVip = TabFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string strItems;







            char v7;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            strItems = v1->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.Items = __x;







                    







            stu.nPrice = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nGongGaoId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto *v2 = &this->m_SuperTeHuiCfgMap[stu.nIndex];







            *v2 = stu;







        }







    }







}















SuperTeHuiCfg *CfgData::GetSuperTeHuiCfg(int32_t nIndex)







{







    auto it = this->m_SuperTeHuiCfgMap.find(nIndex);







    if (it != this->m_SuperTeHuiCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitJewelPavilionTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/TreasureShop.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TREASURE_SHOP_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        this->m_JewelPavilionCfgMap.clear();







        







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            JewelPavilionCfg stu;







            memset(&stu, 0, sizeof(stu));







            







            int32_t nIndex = 0;







            stu.nDay = TabFile.Search_Posistion( i, 0)->iValue;







            stu.nIndex = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string strItem;







            char v8;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            strItem = v1->pString;







            







            MemChrBag v3;







            CItemHelper().parseItemString(v3, &strItem);







            stu.Item = v3;







                    







            stu.nPrice = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto key = std::make_pair(stu.nDay, stu.nIndex);







            auto result = this->m_JewelPavilionCfgMap.insert(std::make_pair(key, stu));







        }







    }







}















JewelPavilionCfg *CfgData::GetJewelPavilionCfg(int32_t nDay, int32_t nIndex)







{







    auto key = std::make_pair(nDay, nIndex);







    auto it = this->m_JewelPavilionCfgMap.find(key);







    if (it != this->m_JewelPavilionCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitGoblinTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/GoblinUp.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_GOBLIN_SUIT_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        this->m_GoblinCfgMap.clear();







        







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            GoblinCfg stu;







            int32_t nIndex = 0;







            int32_t nType = TabFile.Search_Posistion( i, 0)->iValue;







            int32_t nLevel = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.UpAttr = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.SuitId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            stu.ConstCurr = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto key = std::make_pair(nType, nLevel);







            this->m_GoblinCfgMap.insert(std::make_pair(key, stu));







        }







    }







}















GoblinCfg *CfgData::GetGoblinCfg(int32_t nType, int32_t nLevel)







{







    auto key = std::make_pair(nType, nLevel);







    auto it = this->m_GoblinCfgMap.find(key);







    if (it != this->m_GoblinCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitShouHuRefining()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/StarSpaceLevel.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_GOBLIN_SUIT_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        this->m_ShouHuRefinishingCfgMap.clear();







        







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            ShouHuRefinishingCfg stu{};







            







            int32_t nIndex = 0;







            int32_t nType = TabFile.Search_Posistion( i, nIndex++)->iValue;







            int32_t nLevel = TabFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string bCombi;







            char v12;







                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







            bCombi = v2->pString;







            







            std::list<ItemData> strItems;







            // TODO: fix CItemHelper::parseItemDataListString call







            stu.lCostList = strItems;







                    ++nIndex;







            







            std::string v14;







            char v15;







                    v14 = "./ServerConfig/Tables/StarSpaceLevel.txt";







            







            std::string v16;







            char v17;







                    const CDBCFile::FIELD *v3 = TabFile.Search_Posistion( i, nIndex);







            v16 = v3->pString;







            







            std::list<AddAttribute> __x;







            CfgData::parseAddAttribues(&__x, &v16, i, &v14);







            stu.lAttrList = __x;







                    







            stu.SuitId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto key = std::make_pair(nType, nLevel);







            this->m_ShouHuRefinishingCfgMap.insert(std::make_pair(key, stu));







        }







    }







}















ShouHuRefinishingCfg *CfgData::GetShouHuRefinishingCfg(int32_t nType, int32_t nLevel)







{







    auto key = std::make_pair(nType, nLevel);







    auto it = this->m_ShouHuRefinishingCfgMap.find(key);







    if (it != this->m_ShouHuRefinishingCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitWingEquipPolish()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/WingEquipPolish.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_GOBLIN_SUIT_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        this->m_WingEquipPolishCfgMap.clear();







        







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            WingEquipPolish stu{};







            







            int32_t nIndex = 0;







            int32_t nType = TabFile.Search_Posistion( i, nIndex++)->iValue;







            int32_t nLevel = TabFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string v18;







            char v19;







                    v18 = "./ServerConfig/Tables/StarSpaceLevel.txt";







            







            std::string v20;







            char v21;







                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







            v20 = v2->pString;







            







            std::list<AddAttribute> __x;







            CfgData::parseAddAttribues(&__x, &v20, i, &v18);







            stu.lAttrList = __x;







                    ++nIndex;







            







            std::string bCombi;







            char v24;







                    const CDBCFile::FIELD *v3 = TabFile.Search_Posistion( i, nIndex);







            bCombi = v3->pString;







            







            std::list<ItemData> strItems;







            // TODO: fix CItemHelper::parseItemDataListString call







            stu.lCostList = strItems;







                    







            stu.nConstMoney = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.SuitId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.GongGaoId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto key = std::make_pair(nType, nLevel);







            this->m_WingEquipPolishCfgMap.insert(std::make_pair(key, stu));







        }







    }







    







    // 加载套装属性







    CDBCFile TabFile2;







    if (TabFile2.OpenFromTXT( "./ServerConfig/Tables/WingEquipPolishSuit.txt"))







    {







        iBaseTableCount = TabFile2.GetRecordsNum();







        iBaseColumnCount = TabFile2.GetFieldsNum();







        







        if (iBaseColumnCount > 0)







        {







            this->m_WingEquipPolishSuitMap.clear();







            







            for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)







            {







                int32_t nId = TabFile2.Search_Posistion( i_0, 0)->iValue;







                int32_t nIndex = 0;
                ++nIndex;







                







                std::string v28;







                char v29;







                            v28 = "./ServerConfig/Tables/StarSpaceSuit.txt";







                







                std::string v30;







                char v31;







                            const CDBCFile::FIELD *v5 = TabFile2.Search_Posistion( i_0, nIndex);







                v30 = v5->pString;







                







                std::list<AddAttribute> AddAttrs;







                CfgData::parseAddAttribues(&AddAttrs, &v30, i_0, &v28);







                            ++nIndex;







                







                auto v6 = this->m_WingEquipPolishSuitMap[nId];







                v6 = AddAttrs;







            }







        }







        }







}















WingEquipPolish *CfgData::GetWingEquipPolishCfg(int32_t nType, int32_t nLevel)







{







    auto key = std::make_pair(nType, nLevel);







    auto it = this->m_WingEquipPolishCfgMap.find(key);







    if (it != this->m_WingEquipPolishCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitGuiGuDaoRenTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/TaoistTask.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_GUI_GU_DAO_REN_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        this->m_GuiGuDaoRenCfgMap.clear();







        







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            GuiGuDaoRenCfg stu{};







            







            int32_t nIndex = 0;







            stu.nNpcId = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nMaxCount = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string RefreshMonsterString;







            char v23;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            RefreshMonsterString = v1->pString;







                    ++nIndex;







            







            std::string delims;







            char v25;







                    delims = "|";







            







            StringVector SplitStr;







            StringUtility::split(SplitStr, RefreshMonsterString, delims);







                    







            for (auto& monsterStr : SplitStr)







            {







                std::string v28;







                char v29;







                            v28 = ":";







                







                StringVector vstr;







                StringUtility::split(vstr, monsterStr, v28);







                            







                if (vstr.size() == 4)







                {







                    RefreshMonster tmpStu;







                    tmpStu.nCount = atoi(vstr[0].c_str());







                    tmpStu.BossId = atoi(vstr[1].c_str());







                    tmpStu.AliveTime = atoi(vstr[2].c_str());







                    tmpStu.GongGaoId = atoi(vstr[3].c_str());







                    stu.lRefreshMonster.push_back(tmpStu);







                }







            }







            







            stu.vItemData.reserve(3);







            stu.vItem.reserve(3);







            







            for (int32_t j = 0; j <= 2; ++j)







            {







                std::string strItem;







                char v32;







                            const CDBCFile::FIELD *v11 = TabFile.Search_Posistion( i, nIndex);







                strItem = v11->pString;







                







                ItemData __x = CItemHelper().parseItemDataString(&strItem);







                stu.vItemData.push_back(__x);







                            ++nIndex;







                







                std::string v34;







                char v35;







                            const CDBCFile::FIELD *v12 = TabFile.Search_Posistion( i, nIndex);







                v34 = v12->pString;







                







                MemChrBag v33;







                CItemHelper().parseItemString(v33, &v34);







                stu.vItem.push_back(v33);







                            ++nIndex;







            }







            







            std::string path;







            char v38;







                    path = "./ServerConfig/Tables/TaoistTask.txt";







            







            std::string str;







            char v40;







                    const CDBCFile::FIELD *v13 = TabFile.Search_Posistion( i, nIndex);







            str = v13->pString;







            







            Int32Vector v36;







            CfgData::paraseInt32Vector(v36, &str, &path, 0);







            stu.vMapId = v36;







                    ++nIndex;







            







            auto *v14 = &this->m_GuiGuDaoRenCfgMap[stu.nNpcId];







            *v14 = stu;







        }







    }







}















GuiGuDaoRenCfg *CfgData::GetGuiGuDaoRenCfg(int32_t NpcId)







{







    auto it = this->m_GuiGuDaoRenCfgMap.find(NpcId);







    if (it != this->m_GuiGuDaoRenCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitShiZhuangTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/ShiZhuang.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SHIZHUANG_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgShiZhuang stu;







            







            int32_t nIndex = 0;







            stu.nId = readFile.Search_Posistion( i, 0)->iValue;







            ++nIndex;







            stu.nType = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            for (int32_t j = 0; j <= 6; ++j)







            {







                AddAttribute AddAttr;







                AddAttr.m_nAddAttrType = readFile.Search_Posistion( i, nIndex++)->iValue;







                AddAttr.m_nAddAttrValue = readFile.Search_Posistion( i, nIndex++)->iValue;







                if (AddAttr.m_nAddAttrValue > 0)







                {







                    stu.vAttr.push_back(AddAttr);







                }







            }







            nIndex += 7;







            stu.nSuitId = readFile.Search_Posistion( i, nIndex++)->iValue;







            







            CfgShiZhuangTable::AddShiZhuang(&this->m_cfgShiZhuangTable, &stu);







        }







    }







}















void CfgData::InitShiZhuangLevelTable()







{







    CDBCFile readFile;







    







    if (!readFile.OpenFromTXT( "./ServerConfig/Tables/ShiZhuangLevel.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SHIZHUANG_LEVEL_TABLE failed,please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = readFile.GetRecordsNum();







    int32_t iBaseColumnCount = readFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgShiZhuangLevel stu;







            







            int32_t nIndex = 0;







            stu.nType = readFile.Search_Posistion( i, 0)->iValue;







            stu.nLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nLevelExp = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nNeedLevel = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nCostItem = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nGetExp = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nSmallCritRate = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nLargeCritRate = readFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nBroadcast = readFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            for (int32_t j = 0; j <= 6; ++j)







            {







                AddAttribute AddAttr;







                AddAttr.m_nAddAttrType = readFile.Search_Posistion( i, nIndex++)->iValue;







                AddAttr.m_nAddAttrValue = readFile.Search_Posistion( i, nIndex++)->iValue;







                if (AddAttr.m_nAddAttrValue > 0)







                {







                    stu.vAttr.push_back(AddAttr);







                }







            }







            







            CfgShiZhuangTable::AddShiZhuangLevel(&this->m_cfgShiZhuangTable, &stu);







        }







    }







}















void CfgData::InitMonthlyChouJiangTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/TurntableReward.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TURNTABLE_REWARD_TABLE failed, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            int32_t nIndex = 0;







            int32_t Month = TabFile.Search_Posistion( i, 0)->iValue;







            int32_t nId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string strItem;







            char v7;







                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







            strItem = v2->pString;







            







            RateItem ItemRate;







            CItemHelper().parseRateItemDataString(ItemRate, &strItem);







                    ++nIndex;







            







            MonthlyChouJiangTable::AddMonthlyChouJiangItemMap(&this->m_MonthlyChouJiangTable, Month, nId, ItemRate);







        }







    }







}















void CfgData::InitActDropTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/shoujihuodong.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ACTDROP_TABLE failed, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            ActDropItem stu;







            memset(&stu, 0, sizeof(stu));







            







            int32_t nIndex = 0;







            int32_t Type = TabFile.Search_Posistion( i, 0)->iValue;







            stu.nMinLevel = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nMaxLevel = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nMapType = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nId = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nClass = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nCount = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            stu.nBind = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            std::string p_StringTime;







            char v8;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            p_StringTime = v1->pString;







            stu.nStartTime = Answer::DayTime::StringToIntTime(&p_StringTime);







                    ++nIndex;







            







            std::string v9;







            char v10;







                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







            v9 = v2->pString;







            stu.nEndTime = Answer::DayTime::StringToIntTime(&v9);







                    







            stu.nProbability = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            ++nIndex;







            







            auto v3 = this->m_ActDropItemListMap[Type];







            v3.push_back(stu);







        }







    }







}















void CfgData::InitOutLinkFestivalTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/OutLinkFestival.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_OUT_LINK_FESTIVAL_TABLE failed, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgOutLinkFestival stu;







            memset(&stu, 0, sizeof(stu));







            stu.strPlatfrom.clear();







            







            int32_t nIndex = 0;







            stu.nIndex = TabFile.Search_Posistion( i, nIndex++)->iValue;







            







            std::string p_StringTime;







            char v7;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, nIndex);







            p_StringTime = v1->pString;







            stu.nStartTime = Answer::DayTime::StringToIntTime(&p_StringTime);







                    ++nIndex;







            







            std::string v8;







            char v9;







                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, nIndex);







            v8 = v2->pString;







            stu.nEndTime = Answer::DayTime::StringToIntTime(&v8);







                    







            stu.nIcon = TabFile.Search_Posistion( i, ++nIndex)->iValue;







            







            const CDBCFile::FIELD *v3 = TabFile.Search_Posistion( i, ++nIndex);







            stu.strPlatfrom = v3->pString;







            ++nIndex;







            







            CfgOutLinkFestivalTable::Add(&this->m_cfgOutLinkFestivalTable, &stu);







        }







    }







}















void CfgData::InitFestivalActivityTable()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/FestivalActivity.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_FESTIVAL_ACTIVITY_TABLE failed, please check!!!");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgFestivalActivity stu{};







            







            int32_t nIndex = 0;







            stu.nIcon = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nStartDay = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nEndDay = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nPlantId = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nPlantCount = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nNpcId = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nNpcCount = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nTimes = TabFile.Search_Posistion( i, nIndex++)->iValue;







            stu.nPlantTime = TabFile.Search_Posistion( i, nIndex++)->iValue;







            







            // 解析开始/结束天数列表







            {







                std::string strStartDay;







                char vAlloc;







                            const CDBCFile::FIELD *pField = TabFile.Search_Posistion( i, nIndex++);







                strStartDay = pField->pString;







                







                std::vector<int> vStart;







                paraseInt32List(&vStart, &strStartDay, 0, nullptr);







                for (size_t idx = 0; idx < vStart.size() && idx < 5; ++idx)







                    stu.vStartDayList.push_back(vStart[idx]);







                                                }







            







            {







                std::string strEndDay;







                char vAlloc;







                            const CDBCFile::FIELD *pField = TabFile.Search_Posistion( i, nIndex++);







                strEndDay = pField->pString;







                







                std::vector<int> vEnd;







                paraseInt32List(&vEnd, &strEndDay, 0, nullptr);







                for (size_t idx = 0; idx < vEnd.size() && idx < 5; ++idx)







                    stu.vEndDayList.push_back(vEnd[idx]);







                                                }







            







            // 解析植物位置 (MapId:PosX:PosY|MapId:PosX:PosY)







            {







                std::string strPlantPos;







                char vAlloc;







                            const CDBCFile::FIELD *pField = TabFile.Search_Posistion( i, nIndex++);







                strPlantPos = pField->pString;







                







                std::vector<std::string> vPlantPos;







                CDBCFile::_ConvertStringToVector(strPlantPos.c_str(), &vPlantPos, "|", 1, 1);







                for (const auto& posStr : vPlantPos)







                {







                    std::vector<std::string> parts;







                    CDBCFile::_ConvertStringToVector(posStr.c_str(), &parts, ":", 1, 0);







                    if (parts.size() >= 3)







                    {







                        CfgFestivalMapPos pos;







                        pos.nMapId = atoi(parts[0].c_str());







                        pos.nX = atoi(parts[1].c_str());







                        pos.nY = atoi(parts[2].c_str());







                        stu.vPlantPosList.push_back(pos);







                    }







                }







                                    }







            







            // 解析NPC位置







            {







                std::string strNpcPos;







                char vAlloc;







                            const CDBCFile::FIELD *pField = TabFile.Search_Posistion( i, nIndex++);







                strNpcPos = pField->pString;







                







                std::vector<std::string> vNpcPos;







                CDBCFile::_ConvertStringToVector(strNpcPos.c_str(), &vNpcPos, "|", 1, 1);







                for (const auto& posStr : vNpcPos)







                {







                    std::vector<std::string> parts;







                    CDBCFile::_ConvertStringToVector(posStr.c_str(), &parts, ":", 1, 0);







                    if (parts.size() >= 3)







                    {







                        CfgFestivalMapPos pos;







                        pos.nMapId = atoi(parts[0].c_str());







                        pos.nX = atoi(parts[1].c_str());







                        pos.nY = atoi(parts[2].c_str());







                        stu.vNpcPosList.push_back(pos);







                    }







                }







                                    }







            







            







            // 解析每日/活动奖励物品







            {







                std::string strDayGift;







                char vAlloc;







                            const CDBCFile::FIELD *pField = TabFile.Search_Posistion( i, nIndex++);







                strDayGift = pField->pString;







                







                CItemHelper().parseItemString(stu.dayGift, &strDayGift);







                







                                    }







            







            {







                std::string strActGift;







                char vAlloc;







                            const CDBCFile::FIELD *pField = TabFile.Search_Posistion( i, nIndex++);







                strActGift = pField->pString;







                







                CItemHelper().parseItemString(stu.actGift, &strActGift);







                







                                    }







            







            // 解析兑换配置 (Times:CostGold|GetItems|CostItems#Times:CostGold|GetItems|CostItems)







            {







                std::string strChangeCfg;







                char vAlloc;







                            const CDBCFile::FIELD *pField = TabFile.Search_Posistion( i, nIndex++);







                strChangeCfg = pField->pString;







                







                std::vector<std::string> vCfgEntries;







                CDBCFile::_ConvertStringToVector(strChangeCfg.c_str(), &vCfgEntries, "#", 1, 1);







                







                for (const auto& entry : vCfgEntries)







                {







                    std::vector<std::string> parts;







                    CDBCFile::_ConvertStringToVector(entry.c_str(), &parts, "|", 1, 1);







                    if (parts.size() >= 4)







                    {







                        CfgChangeItemCfg cfgItem;







                        cfgItem.nTimes = atoi(parts[0].c_str());







                        cfgItem.nCostGold = atoi(parts[1].c_str());







                        







                        // Parse get items (separated by ~)







                        {







                            std::vector<std::string> vGetItems;







                            CDBCFile::_ConvertStringToVector(parts[2].c_str(), &vGetItems, "~", 1, 1);







                            for (const auto& itemStr : vGetItems)







                            {







                                MemChrBag item;







                                std::string strItem = itemStr;







                                CItemHelper().parseItemString(item, &strItem);







                                cfgItem.vGetItems.push_back(item);







                            }







                        }







                        







                        // Parse cost items (separated by ~)







                        {







                            std::vector<std::string> vCostItems;







                            CDBCFile::_ConvertStringToVector(parts[3].c_str(), &vCostItems, "~", 1, 1);







                            for (const auto& itemStr : vCostItems)







                            {







                                MemChrBag item;







                                std::string strItem = itemStr;







                                CItemHelper().parseItemString(item, &strItem);







                                cfgItem.vCostItems.push_back(item);







                            }







                        }







                        







                        stu.vChangeCfgList.push_back(cfgItem);







                    }







                }







                







                                    }







            







            // 解析答题列表 (answers separated by |, groups separated by #)







            {







                std::string strAnswer;







                char vAlloc;







                            const CDBCFile::FIELD *pField = TabFile.Search_Posistion( i, nIndex++);







                strAnswer = pField->pString;







                







                std::vector<std::string> vAnswerGroups;







                CDBCFile::_ConvertStringToVector(strAnswer.c_str(), &vAnswerGroups, "#", 1, 1);







                







                for (const auto& group : vAnswerGroups)







                {







                    std::vector<std::string> vAnswers;







                    CDBCFile::_ConvertStringToVector(group.c_str(), &vAnswers, "|", 1, 1);







                    stu.vAnswerList.push_back(vAnswers);







                }







                







                                    }







            this->m_festivalActivityTable.Add(stu);







        }







    }







}















CfgFestivalActivityTable *CfgData::GetFestivalActivityTable()







{







    return &this->m_festivalActivityTable;







}















void CfgData::InitYYDaTing()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/YYDaTing.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_YY_DATING_TABLE failed, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgYYGameApp stu;







            memset(&stu, 0, sizeof(stu));







            new (&stu.Rewards) std::vector<MemChrBag>();







            







            stu.nIndex = TabFile.Search_Posistion( i, 0)->iValue;







            stu.nType = TabFile.Search_Posistion( i, 1)->iValue;







            







            std::string strItems;







            char v7;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 3);







            strItems = v1->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.Rewards = __x;







                    







            stu.nCondition = TabFile.Search_Posistion( i, 4)->iValue;







            







            auto *v2 = &this->m_CfgYYGameAppMap[stu.nIndex];







            *v2 = stu;







        }







    }







}















CfgYYGameApp *CfgData::GetYYGameApp(int32_t nIndex)







{







    auto it = this->m_CfgYYGameAppMap.find(nIndex);







    if (it != this->m_CfgYYGameAppMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitLaDaShiHuiYuan()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/LuDaShiHuiYuan.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_LUDA_SHI_HUI_YUAN_TABLE failed, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            LuDaShiVip stu{};







            







            stu.nIndex = TabFile.Search_Posistion( i, 0)->iValue;







            stu.nType = TabFile.Search_Posistion( i, 1)->iValue;







            







            std::string strItems;







            char v7;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 3);







            strItems = v1->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.Rewards = __x;







                    







            stu.nCondition = TabFile.Search_Posistion( i, 4)->iValue;







            stu.nVipType = TabFile.Search_Posistion( i, 8)->iValue;







            stu.nMaxCondition = TabFile.Search_Posistion( i, 11)->iValue;







            







            auto *v2 = &this->m_LuDaShiVipMap[stu.nIndex];







            *v2 = stu;







        }







    }







}















LuDaShiVip *CfgData::GetLaDaShiHuiYuan(int32_t nIndex)







{







    auto it = this->m_LuDaShiVipMap.find(nIndex);







    if (it != this->m_LuDaShiVipMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitYYVip()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/YYHuiYuan.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_YY_VIP_TABLE failed, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgYYVip stu;







            memset(&stu, 0, sizeof(stu));







            new (&stu.Rewards) std::vector<MemChrBag>();







            







            stu.nIndex = TabFile.Search_Posistion( i, 0)->iValue;







            stu.nType = TabFile.Search_Posistion( i, 1)->iValue;







            







            std::string strItems;







            char v11;







                    const CDBCFile::FIELD *v2 = TabFile.Search_Posistion( i, 3);







            strItems = v2->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.Rewards = __x;







                    







            stu.nCondition = TabFile.Search_Posistion( i, 4)->iValue;







            stu.nPric = TabFile.Search_Posistion( i, 5)->iValue;







            stu.nLimit = TabFile.Search_Posistion( i, 6)->iValue;







            







            auto *v3 = &this->m_CfgYYVipMap[stu.nIndex];







            *v3 = stu;







        }







    }







}















CfgYYVip *CfgData::GetYYVip(int32_t nIndex)







{







    auto it = this->m_CfgYYVipMap.find(nIndex);







    if (it != this->m_CfgYYVipMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















void CfgData::InitSouGouDaTing()







{







    CDBCFile TabFile;







    







    if (!TabFile.OpenFromTXT( "./ServerConfig/Tables/SouGouDaTing.txt"))







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SOU_GOU_DATING_TABLE failed, please check!!!\n");







            return;







    }







    







    int32_t iBaseTableCount = TabFile.GetRecordsNum();







    int32_t iBaseColumnCount = TabFile.GetFieldsNum();







    







    if (iBaseColumnCount > 0)







    {







        for (int32_t i = 0; i < iBaseTableCount; ++i)







        {







            CfgSgGameApp stu;







            memset(&stu, 0, sizeof(stu));







            // stu.vRewards initialized by default constructor







            







            stu.nIndex = TabFile.Search_Posistion( i, 0)->iValue;







            stu.nType = TabFile.Search_Posistion( i, 1)->iValue;







            







            std::string strItems;







            char v7;







                    const CDBCFile::FIELD *v1 = TabFile.Search_Posistion( i, 3);







            strItems = v1->pString;







            







            MemChrBagVector __x;







            CItemHelper().parseItemVectorString(&__x, &strItems);







            stu.vRewards = __x;







                    







            stu.nCondition = TabFile.Search_Posistion( i, 4)->iValue;







            







            auto *v2 = &this->m_CfgSgGameAppMap[stu.nIndex];







            *v2 = stu;







        }







    }







}















CfgSgGameApp *CfgData::GetSoGouGameApp(int32_t nIndex)







{







    auto it = this->m_CfgSgGameAppMap.find(nIndex);







    if (it != this->m_CfgSgGameAppMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















// ==================== 解析辅助方法（续） ====================















void CfgData::parseAddAttribues(const std::string *const addonAttr, 







                                 int32_t nIndex, const std::string *const path)







{







    // 实现解析添加属性列表







    // 此方法在多个地方被调用，用于解析 AddAttribute 列表







}















const CfgMiniClient *CfgData::GetMiniClient(const std::string *const platform)







{







    auto it = this->m_CfgMiniClient.find(*platform);







    if (it != this->m_CfgMiniClient.end())







    {







        return &it->second;







    }







    return nullptr;







}







// ==================== 解析辅助方法 ====================















void CfgData::parseAddAttribues(std::list<AddAttribute> *result, 







                                 const std::string *const addonAttr, int32_t nIndex, const std::string *const path)







{







    if (addonAttr->empty() || *addonAttr == "-1" || *addonAttr == "0") return;







    







    std::string delims;







    char v22;







    delims = "|";







    







    StringVector strAttrAddons;







    StringUtility::split(strAttrAddons, *addonAttr, delims);







    







    for (auto& addonStr : strAttrAddons)







    {







        std::string v24;







        char v25;







            v24 = ":";







        







        StringVector strAttrAddon;







        StringUtility::split(strAttrAddon, addonStr, v24);







            







        if (strAttrAddon.size() == 2)







        {







            AddAttribute attr;







            attr.m_nAddAttrType = atoi(strAttrAddon[0].c_str());







            attr.m_nAddAttrValue = atoi(strAttrAddon[1].c_str());







            if (attr.m_nAddAttrValue > 0)







            {







                result->push_back(attr);







            }







        }







        else







        {







            Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,







                "CfgData::parseAddAttribues() wrong data from %s, where index = %d, str = %s\n",







                path->c_str(), nIndex, addonAttr->c_str());







        }







    }







}















void CfgData::paraseTalentAddon(std::list<TalentAddon> *result,







                                 const std::string *const str, int32_t nIndex, const std::string *const path)







{







    if (str->empty() || *str == "-1") return;







    







    std::string delims;







    char v10;







    delims = "|";







    







    StringVector strTalentAddons;







    StringUtility::split(strTalentAddons, *str, delims);







    







    for (auto& talentStr : strTalentAddons)







    {







        std::string v12;







        char v13;







            v12 = ":";







        







        StringVector vParam;







        StringUtility::split(vParam, talentStr, v12);







            







        if (vParam.size() == 2)







        {







            TalentAddon addon;







            addon.nSkillId = atoi(vParam[0].c_str());







            addon.nSkillLevel = atoi(vParam[1].c_str());







            result->push_back(addon);







        }







    }







}















void CfgData::paraseParam2List(std::list<Param2> *result,







                                const std::string *const str, int32_t nIndex, const std::string *const path)







{







    if (str->empty() || *str == "-1") return;







    







    std::string delims;







    char v14;







    delims = "|";







    







    StringVector strParams;







    StringUtility::split(strParams, *str, delims);







    







    for (auto& paramStr : strParams)







    {







        std::string v16;







        char v17;







            v16 = ":";







        







        StringVector vParam;







        StringUtility::split(vParam, paramStr, v16);







            







        if (vParam.size() == 2)







        {







            Param2 param;







            param.nParam1 = atoi(vParam[0].c_str());







            param.nParam2 = atoi(vParam[1].c_str());







            result->push_back(param);







        }







    }







}















void CfgData::paraseInt32List(std::list<int> *result,







                               const std::string *const str, int32_t size, const std::string *const path)







{







    if (str->empty() || *str == "-1") return;







    







    std::string delims;







    char v19;







    delims = ":";







    







    StringVector vstr;







    StringUtility::split(vstr, *str, delims);







    







    if (size > 0 && (int32_t)vstr.size() != size)







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,







            "CfgData::paraseInt32List() check size err from %s, where size = %d, str = %s\n",







            path ? path->c_str() : "unknown", size, str->c_str());







    }







    else







    {







        for (auto& valStr : vstr)







        {







            int val = atoi(valStr.c_str());







            result->push_back(val);







        }







    }







}















CfgInt32Vector *CfgData::paraseInt32Vector2(const std::string *const str, const std::string *const path, int32_t size)







{







    CfgInt32Vector *retstr = new CfgInt32Vector();







    if (str->empty() || *str == "-1") return retstr;







    







    std::string delims;







    char v19;







    delims = "|";







    







    StringVector vstr;







    StringUtility::split(vstr, *str, delims);







    







    if (size > 0 && (int32_t)vstr.size() != size)







    {







        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,







            "CfgData::paraseInt32Vector2() check size err from %s, where size = %d, str = %s\n",







            path->c_str(), size, str->c_str());







    }







    else







    {







        retstr->reserve(vstr.size());







        for (auto& valStr : vstr)







        {







            int val = atoi(valStr.c_str());







            retstr->push_back(val);







        }







    }







    return retstr;







}















CfgInt32VtVector *CfgData::paraseInt32VtVector(const std::string *const str, const std::string *const path)







{







    CfgInt32VtVector *retstr = new CfgInt32VtVector();







    if (str->empty() || *str == "-1") return retstr;







    







    std::string delims;







    char v18;







    delims = "|";







    







    StringVector SplitStr;







    StringUtility::split(SplitStr, *str, delims);







    







    retstr->reserve(SplitStr.size());







    







    for (auto& splitItem : SplitStr)







    {







        std::string v21;







        char v22;







            v21 = ":";







        







        StringVector vstr;







        StringUtility::split(vstr, splitItem, v21);







            







        std::vector<int> probability;







        probability.reserve(vstr.size());







        







        for (auto& valStr : vstr)







        {







            int val = atoi(valStr.c_str());







            probability.push_back(val);







        }







        retstr->push_back(probability);







    }







    return retstr;







}















void CfgData::parseTaskItemJobString(MemChrJobBagVector& retstr,







                                      int32_t id, const std::string *const strItems)







{







    retstr.clear();







    if (strItems->empty() || strItems->size() <= 4) return;







    







    std::string delims;







    char v47;







    delims = "|";







    







    StringVector items_receive;







    StringUtility::split(items_receive, *strItems, delims);







    







    for (auto& itemStr : items_receive)







    {







        std::string v49;







        char v50;







            v49 = ":";







        







        StringVector item;







        StringUtility::split(item, itemStr, v49);







            







        if (item.size() == 4)







        {







            MemChrJobBag itemData;







            memset(&itemData, 0, sizeof(itemData));







            itemData.id = atoi(item[0].c_str());







            itemData.type = atoi(item[1].c_str());







            itemData.count = atoi(item[2].c_str());







            itemData.job = atoi(item[3].c_str());







            retstr->push_back(itemData);







        }







        else if (item.size() == 5)







        {







            MemChrJobBag itemData;







            memset(&itemData, 0, sizeof(itemData));







            itemData.id = atoi(item[0].c_str());







            itemData.type = atoi(item[1].c_str());







            itemData.count = atoi(item[2].c_str());







            itemData.bind = atoi(item[3].c_str());







            itemData.job = atoi(item[4].c_str());







            retstr->push_back(itemData);







        }







        else if (item.size() == 6)







        {







            MemChrJobBag itemData;







            memset(&itemData, 0, sizeof(itemData));







            itemData.id = atoi(item[0].c_str());







            itemData.type = atoi(item[1].c_str());







            itemData.count = atoi(item[2].c_str());







            itemData.bind = atoi(item[3].c_str());







            itemData.job = atoi(item[4].c_str());







            itemData.time = atoi(item[5].c_str());







            retstr->push_back(itemData);







        }







        else







        {







            Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,







                "CfgData::parseTaskItemJobString wrong data with id = %d, string = %s\n",







                id, strItems->c_str());







        }







    }







}















MemJobItemTable *CfgData::parseGambleEquip(int32_t id, const std::string *const strItems)







{







    MemJobItemTable *retstr = new MemJobItemTable();







    if (strItems->empty()) return retstr;







    







    std::string delims;







    char v19;







    delims = "|";







    







    StringVector items_receive;







    StringUtility::split(items_receive, *strItems, delims);







    







    for (auto& itemStr : items_receive)







    {







        std::string v21;







        char v22;







            v21 = ":";







        







        StringVector item;







        StringUtility::split(item, itemStr, v21);







            







        if (item.size() == 2)







        {







            MemJobItem itemData;







            itemData.job = atoi(item[0].c_str());







            itemData.item = atoi(item[1].c_str());







            auto *v9 = &retstr[itemData.job];







            *v9 = itemData;







        }







        else







        {







            Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,







                "CfgData::parseGambleEquip wrong data with id = %d, string = %s\n",







                id, strItems->c_str());







        }







    }







    return retstr;







}















// ==================== 剩余的 Getter 方法 ====================































































































































































































QuestionsVector *CfgData::GetAllQuestions(int8_t nType)







{







    auto it = this->m_mQuestions.find(nType);







    if (it != this->m_mQuestions.end())







    {







        return &it->second;







    }







    return nullptr;







}















BossInfo *CfgData::GetBossInfo(int32_t BossId)







{







    auto it = this->m_BossInfo.find(BossId);







    if (it != this->m_BossInfo.end())







    {







        return &it->second;







    }







    return nullptr;







}















PkDropRate *CfgData::GetPkDropRate(int32_t PkValues)







{







    for (auto& item : this->m_PkDropRateList)







    {







        if (item.PkValues >= PkValues)







        {







            return &item;







        }







    }







    return nullptr;







}















GroupMonsterVector *CfgData::GetGroupMonsterVector(int32_t MapId)







{







    auto it = this->m_GroupMonsterMap.find(MapId);







    if (it != this->m_GroupMonsterMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgSysMail *CfgData::GetSysMail(int32_t Id)







{







    auto it = this->m_sysMail.find(Id);







    if (it != this->m_sysMail.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgGameShop *CfgData::GetGameShop(int32_t nShopId)







{







    auto it = this->m_GameShopMap.find(nShopId);







    if (it != this->m_GameShopMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgGameShop *CfgData::GetGameShopItem(int8_t Class, int32_t Id)







{







    for (auto& pair : this->m_GameShopMap)







    {







        CfgGameShop& shop = pair.second;







        if (shop.ItemClass == Class && shop.ItemId == Id && shop.ShopType != 4)







        {







            return &shop;







        }







    }







    return nullptr;







}















BuyFaBaoResCfg *CfgData::GetBuyFaBaoResCfg(int32_t Id)







{







    auto it = this->m_FaBaoResMap.find(Id);







    if (it != this->m_FaBaoResMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















XingMaiCfg *CfgData::GetXingMaiCfg(int32_t XingMaiLevel)







{







    auto it = this->m_XingMaiCfgTable.find(XingMaiLevel);







    if (it != this->m_XingMaiCfgTable.end())







    {







        return &it->second;







    }







    return nullptr;







}















const CfgXingMaiSlot *CfgData::GetXingMaiSlot(int8_t nIndex)







{







    auto it = this->m_cfgXingMaiSlotTable.find(nIndex);







    if (it != this->m_cfgXingMaiSlotTable.end())







    {







        return &it->second;







    }







    return nullptr;







}















TianLingCfg *CfgData::GetTianLingCfg(int32_t TianLingLevel)







{







    auto it = this->m_TianLingCfgTable.find(TianLingLevel);







    if (it != this->m_TianLingCfgTable.end())







    {







        return &it->second;







    }







    return nullptr;







}















GongMingCfg *CfgData::GetGongMingCfg(int32_t GongMinLevel)







{







    auto it = this->m_GongMingCfgMap.find(GongMinLevel);







    if (it != this->m_GongMingCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















DamnationCfg *CfgData::GetDamnationCfg(int32_t Level)







{







    auto it = this->m_DamnationCfgTable.find(Level);







    if (it != this->m_DamnationCfgTable.end())







    {







        return &it->second;







    }







    return nullptr;







}















int32_t CfgData::GetAttrBattle(int32_t nAttr)







{







    if (nAttr <= 0 || nAttr > 49) return 0;







    return this->m_cfgAttrBattle[nAttr];







}















ScoreShopCfg *CfgData::GetScoreShopCfg(int32_t Index)







{







    auto it = this->m_ScoreShopCfgTable.find(Index);







    if (it != this->m_ScoreShopCfgTable.end())







    {







        return &it->second;







    }







    return nullptr;







}















LevelRefinCfg *CfgData::GetRefining(int32_t Level)







{







    auto it = this->m_LevelRefinCfgMap.find(Level);







    if (it != this->m_LevelRefinCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















UltimateChallengeCfg *CfgData::GetUltimateChallengeCfgMap(int32_t MapId)







{







    auto it = this->m_UltimateChallengeCfgMap.find(MapId);







    if (it != this->m_UltimateChallengeCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















CfgCachet *CfgData::GetCfgCachet(int32_t level)







{







    auto it = this->m_CfgCachetMap.find(level);







    if (it != this->m_CfgCachetMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















int32_t CfgData::GetCachetLevel(int64_t Exp)







{







    int32_t Level = 1;







    for (auto& pair : this->m_CfgCachetMap)







    {







        if (pair.second.nNeedCacht <= Exp && pair.second.nLevel > Level)







        {







            Level = pair.second.nLevel;







        }







    }







    return Level;







}















BossKilledReward *CfgData::GetBossKilledReward(int32_t BossId)







{







    auto it = this->m_BossKilledRewardMap.find(BossId);







    if (it != this->m_BossKilledRewardMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















SystemOpenGift *CfgData::GetSystemOpenGift(int32_t Id)







{







    auto it = this->m_SystemOpenGiftMap.find(Id);







    if (it != this->m_SystemOpenGiftMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















bool CfgData::IsSpecialMap(int32_t MapId)







{







    auto it = this->m_SpecialMapMap.find(MapId);







    return it != this->m_SpecialMapMap.end();







}















TestServerReward *CfgData::GetTestServerReward(int32_t nId)







{







    auto it = this->m_TestServerRewardMap.find(nId);







    if (it != this->m_TestServerRewardMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















GuWuCfg *CfgData::GetGuWuCfg(int32_t Level)







{







    auto it = this->m_GuWuCfgMap.find(Level);







    if (it != this->m_GuWuCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















FunctionOpenMail *CfgData::GetOpenFunctionMailnCfg(int32_t nId)







{







    auto it = this->m_FunctionOpenMailMap.find(nId);







    if (it != this->m_FunctionOpenMailMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















int32_t CfgData::RandXiangYaoTaskId(int32_t Level, bool bBest)







{







    std::map<int, int> TaskIdRate;







    int32_t MaxRate = 0;







    int32_t SpareId = 0;







    







    for (auto& task : this->m_XiangYaoTaskCfgList)







    {







        if (SpareId == 0) SpareId = task.TaskId;







        







        if (task.MinLevel <= Level && task.MaxLevel >= Level)







        {







            if (!bBest || task.Star == 4)







            {







                TaskIdRate[task.TaskId] = task.Rate;







                MaxRate += task.Rate;







            }







        }







    }







    







    if (MaxRate <= 0) return SpareId;







    







    Answer::Random *random = Answer::Singleton<Answer::Random>::instance();







    int32_t nRand = random->generate(1, MaxRate);







    







    for (auto& pair : TaskIdRate)







    {







        if (pair.second >= nRand)







        {







            return pair.first;







        }







        nRand -= pair.second;







    }







    







    return SpareId;







}















int32_t CfgData::getXiangYaoStart(int32_t TaskId)







{







    for (auto& task : this->m_XiangYaoTaskCfgList)







    {







        if (task.TaskId == TaskId)







        {







            return task.Star;







        }







    }







    return 0;







}















ShenWeiTaskCfg *CfgData::GetShenWeiTaskCfg(int32_t nId)







{







    auto it = this->m_ShenWeiTaskCfgMap.find(nId);







    if (it != this->m_ShenWeiTaskCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}























const CfgMobilePhoneGift *CfgData::GetMobilePhoneGift(const std::string *const platform)







{







    auto it = this->m_CfgMobilePhoneGift.find(*platform);







    if (it != this->m_CfgMobilePhoneGift.end())







    {







        return &it->second;







    }







    return nullptr;







}























CfgWuHunShop *CfgData::GetWuHunShopItem(int32_t nIndex)







{







    auto it = this->m_CfgWuHunShopItemMap.find(nIndex);







    if (it != this->m_CfgWuHunShopItemMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















WuHunItem *CfgData::GetWuHunItem(int32_t nId)







{







    auto it = this->m_WuHunItemMap.find(nId);







    if (it != this->m_WuHunItemMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















CreateWuHun *CfgData::GetCreateWuHun(int32_t nId)







{







    auto it = this->m_CreateWuHunMap.find(nId);







    if (it != this->m_CreateWuHunMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















SpecialBossMapCfg *CfgData::GetSpecialBossMapCfg(int32_t MapId)







{







    auto it = this->m_SpecialBossMapCfgMap.find(MapId);







    if (it != this->m_SpecialBossMapCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}























































WinRefiningCfg *CfgData::GetWingEquipRefiningCfg(int32_t nType, int32_t nLevel)







{







    auto key = std::make_pair(nType, nLevel);







    auto it = this->m_WinRefiningCfgMap.find(key);







    if (it != this->m_WinRefiningCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















































const CfgYYSuperBuff *CfgData::GetSuperBuff(int32_t SuperLevel)







{







    for (auto& buff : this->m_CfgYYSuperBuffList)







    {







        if (buff.SuperVipLevel <= SuperLevel)







        {







            return &buff;







        }







    }







    return nullptr;







}















const CfgYYSuperBuff *CfgData::Get37wanSuperBuff(int32_t SuperLevel)







{







    for (auto& buff : this->m_Cfg37wanSuperBuffList)







    {







        if (buff.SuperVipLevel <= SuperLevel)







        {







            return &buff;







        }







    }







    return nullptr;







}























PlatformDaTing *CfgData::GetGetPlatformDaTingCfg(std::string *p_Platform, int32_t nId)







{







    auto key = std::make_pair(*p_Platform, nId);







    auto it = this->m_PlatformDaTingMap.find(key);







    if (it != this->m_PlatformDaTingMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















MiniClientCfg *CfgData::GetMiniClientCfg(std::string *p_Platform, int32_t nId)







{







    auto key = std::make_pair(*p_Platform, nId);







    auto it = this->m_GetMiniClientMap.find(key);







    if (it != this->m_GetMiniClientMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















PlatformVip *CfgData::GetGetPlatformVipCfg(std::string *p_Platform, int32_t nId)







{







    auto key = std::make_pair(*p_Platform, nId);







    auto it = this->m_PlatformVipMap.find(key);







    if (it != this->m_PlatformVipMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















PlatformRewardCfg *CfgData::GetGetPlatformRewardCfg(std::string *p_Platform, int32_t nId)







{







    auto key = std::make_pair(*p_Platform, nId);







    auto it = this->m_PlatformRewardCfgMap.find(key);







    if (it != this->m_PlatformRewardCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















DiligenceCfg *CfgData::GetDiligenceCfg(int32_t nType)







{







    auto it = this->m_DiligenceCfgMap.find(nType);







    if (it != this->m_DiligenceCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















ContributionCfg *CfgData::GettFamilyDonateCfg(int32_t nId)







{







    auto it = this->m_ContributionCfgMap.find(nId);







    if (it != this->m_ContributionCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















ShangRenCfg *CfgData::GetShangRenCfg(int32_t nId)







{







    auto it = this->m_ShangRenCfgMap.find(nId);







    if (it != this->m_ShangRenCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















SelectItemCfg *CfgData::GetSelectItemCfg(int32_t nId)







{







    auto it = this->m_SelectItemCfgMap.find(nId);







    if (it != this->m_SelectItemCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















XunLeiCfg *CfgData::GetXunLeiCfg(int32_t nIndex)







{







    auto it = this->m_XunLeiCfgMap.find(nIndex);







    if (it != this->m_XunLeiCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















CrossTowerCfg *CfgData::GetCrossTowerCfg(int32_t MapId)







{







    auto it = this->m_CrossTowerCfgMap.find(MapId);







    if (it != this->m_CrossTowerCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















MoFuSkillCfg *CfgData::GetMoFuSkillCfg(int32_t nLevel)







{







    auto it = this->m_MoFuSkillCfgMap.find(nLevel);







    if (it != this->m_MoFuSkillCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















ZhuNengCfg *CfgData::GetZhuNengCfg(int32_t nType, int32_t nLevel)







{







    auto key = std::make_pair(nType, nLevel);







    auto it = this->m_ZhuNengCfgMap.find(key);







    if (it != this->m_ZhuNengCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















MoFuDuiHuan *CfgData::GetMoFuDuiHuanlCfg(int32_t nId)







{







    auto it = this->m_MoFuDuiHuanMap.find(nId);







    if (it != this->m_MoFuDuiHuanMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















ChristmasDuiHuan *CfgData::GetChristmasDuiHuanCfg(int32_t nId)







{







    auto it = this->m_ChristmasDuiHuanMap.find(nId);







    if (it != this->m_ChristmasDuiHuanMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















int32_t CfgData::GetTongTianChiReward(int32_t nId)







{







    auto it = this->m_TongTianChiRankReward.lower_bound(nId);







    if (it != this->m_TongTianChiRankReward.end())







    {







        return it->second;







    }







    return 0;







}















int32_t CfgData::GetChargeDungeonId(int32_t nId, int32_t Todaycharge)







{







    int32_t KaiFuDay = getServerDiffDay(SERVER_TYPE::SVT_NORMAL) + 1;







    







    auto it = this->m_ChargeDungeonCfgMap.find(nId);







    if (it != this->m_ChargeDungeonCfgMap.end())







    {







        CfgChargeDungeon& cfg = it->second;







        if (cfg.nMinKaiFuDay <= KaiFuDay && cfg.nMaxKaiFuDay >= KaiFuDay && cfg.nChargeValue <= Todaycharge)







        {







            return cfg.nDungeonId;







        }







    }







    return 0;







}















BaoKuFuBen *CfgData::GetBaoKuFuBen(int32_t FuBenId)







{







    auto it = this->m_BaoKuFuBenMap.find(FuBenId);







    if (it != this->m_BaoKuFuBenMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















BaoKuRandomMap *CfgData::GetBaoKuRandomMap()







{







    return &this->m_BaoKuRandomMap;







}















const TreasureHunterCfg *CfgData::GetTreasureHunterCfg(int32_t nId)







{







    auto it = this->m_TreasureHunterCfgMap.find(nId);







    if (it != this->m_TreasureHunterCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const SunAndMoonCfg *CfgData::GetSunAndMoonCfg(int32_t nLevel)







{







    auto it = this->m_SunAndMoonCfgMap.find(nLevel);







    if (it != this->m_SunAndMoonCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const ZhanHunCfg *CfgData::GetZhanHunCfg(int32_t nLevel)







{







    auto it = this->m_ZhanHunCfgMap.find(nLevel);







    if (it != this->m_ZhanHunCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const VipEquipPosLevelUp *CfgData::GetVipEquipPosLevelUp(int32_t nSlot, int32_t nLevel)







{







    auto key = std::make_pair(nSlot, nLevel);







    auto it = this->m_VipEquipPosLevelUpMap.find(key);







    if (it != this->m_VipEquipPosLevelUpMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const VipEQuipPosSuit *CfgData::GetVipEQuipPosSuit(int32_t nLevel)







{







    for (auto it = this->m_VipEQuipPosSuitList.rbegin(); it != this->m_VipEQuipPosSuitList.rend(); ++it)







    {







        if (it->nLevel <= nLevel)







        {







            return &(*it);







        }







    }







    return nullptr;







}















const CVipClubLuckyDrop *CfgData::GetCVipClubLuckyDrop(int32_t nCondition)







{







    auto it = this->m_CVipClubLuckyDropMap.find(nCondition);







    if (it != this->m_CVipClubLuckyDropMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const ZeroBuyPetCfg *CfgData::GetZeroBuyPetCfg()







{







    if (ZeroBuyPetCfg::IsEmpty(&this->m_ZeroBuyPetCfg))







    {







        return nullptr;







    }







    return &this->m_ZeroBuyPetCfg;







}















const ShiQuCfg *CfgData::GetShiQuCfg(int32_t nId)







{







    auto it = this->m_ShiQuCfgMap.find(nId);







    if (it != this->m_ShiQuCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const LittleHelperCfg *CfgData::GetLittleHelperCfg(int32_t nId)







{







    auto it = this->m_LittleHelperCfgMap.find(nId);







    if (it != this->m_LittleHelperCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const ShiZhuLevelUp *CfgData::GetShiZhuLevelUp(int32_t nId, int32_t nLevel)







{







    auto key = std::make_pair(nId, nLevel);







    auto it = this->m_ShiZhuLevelUpMap.find(key);







    if (it != this->m_ShiZhuLevelUpMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const NationalDayTaskCfg *CfgData::GetNationalDayTaskCfg(int32_t nType, int32_t nId)







{







    auto key = std::make_pair(nType, nId);







    auto it = this->m_NationalDayTaskCfgMap.find(key);







    if (it != this->m_NationalDayTaskCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const NationalDayLevelCfg *CfgData::GetNationalDayLevelCfg(int32_t nLevel)







{







    auto it = this->m_NationalDayLevelCfgMap.find(nLevel);







    if (it != this->m_NationalDayLevelCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const EquipRongHe *CfgData::GetEquipRongHe(int32_t ItemId)







{







    auto it = this->m_EquipRongHeMap.find(ItemId);







    if (it != this->m_EquipRongHeMap.end())







    {







        return &it->second;







    }







    return nullptr;







}















const RongHeCfg *CfgData::GetRongHeCfg(int32_t nId)







{







    auto it = this->m_RongHeCfgMap.find(nId);







    if (it != this->m_RongHeCfgMap.end())







    {







        return &it->second;







    }







    return nullptr;







}







void CfgData::InitAttrBattleTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitBeastShrineEnterCostTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitBossFirstKilledTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitBossInfo()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitBossKilledReward()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitBuyFaBaoResTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitCachetCfg()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitChouJiangTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitDaTiHD()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitDamnationTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitEquipBackTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitEquipBackTask()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitEquipBlessTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitEquipUpPhase()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitFaBaoTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitFunctionOpenMailMap()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitGoblinData()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitGongMinTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitGroupMonster()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitGuWuCfgMap()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitLevelRefiningTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitLibraryTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitLuckDropTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitPkDropRateTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitScoreShopTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitShangChengTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitShenWeiTaskTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitShunWangTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitSpeciaEquipCfgMap()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitSpeciaSkillDistanceTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitSpecialMap()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitSpecialMonster()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitStrengthenTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitSysMail()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitSystemOpenReward()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitTestServerReward()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitTianLingTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitTouZiTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitTreasureMapTabale()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitUltimateChallengeCfg()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitVplanTable()







{







    // TODO: Implement from IDA pseudocode







}























void CfgData::InitXianYaoTaskTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitXingMaiSlotTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitXingMaiTable()







{







    // TODO: Implement from IDA pseudocode







}















void CfgData::InitSpeed360Reward()







{







    // TODO: Implement from IDA pseudocode







}















void CQiFuTable::InitQiFuTable(CQiFuTable* table)







{







    (void)table;







    // TODO: Implement from IDA pseudocode







}















void VipTable::InitVipTable(VipTable* table)







{







    (void)table;







    // TODO: Implement from IDA pseudocode







}
