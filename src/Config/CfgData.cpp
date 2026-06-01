#include "Config/CfgData.h"
#include "Answer/Singleton.h"
#include "Answer/Logger.h"
#include "Answer/DayTime.h"
#include "CTimer.h"
#include "DB/DBService.h"
#include "GameService/GameService.h"
#include "ItemEffectManager.h"
#include "COpenBeta.h"
#include "CCardGroupBoxManager.h"
#include "CItemHelper.h"
#include "CDBCFile.h"
#include "Config/CfgFestivalActivityTable.h"
#include <cstring>
#include <algorithm>

// ==================== 构造函数 ====================

CfgData::CfgData(CfgData *const this)
{
    std::list<CfgMapEvent>::list(&this->m_emptyEvents);
    std::map<int, CfgActivity>::map(&this->m_activities);
    std::map<int, std::map<int, std::list<CfgMapEvent>>>::map(&this->m_activityEvents);
    std::map<int, CfgActivityMonster>::map(&this->m_activityMonsters);
    std::map<int, CfgActivityNpc>::map(&this->m_activityNpcs);
    std::map<int, CfgActivityPlant>::map(&this->m_activityPlants);
    std::map<int, CfgActivityDrop>::map(&this->m_activityDrops);
    std::map<int, CfgActivityTrap>::map(&this->m_activityTraps);
    std::map<int, CfgBuff>::map(&this->m_buffs);
    std::map<int, CfgDungeon>::map(&this->m_dungeons);
    std::map<int, CfgDungeon>::map(&this->m_mUpTowerDungeon);
    std::map<int, CfgDungeonReward>::map(&this->m_dungeonReward);
    std::map<int, CfgDungeonDropGroup>::map(&this->m_dungeonDropGroup);
    std::map<int, std::list<CfgMapEvent>>::map(&this->m_dungeonEvents);
    std::map<int, CfgDungeonMonster>::map(&this->m_dungeonMonsters);
    std::map<int, CfgDungeonPlant>::map(&this->m_dungeonPlants);
    std::map<int, CfgDungeonTrap>::map(&this->m_dungeonTraps);
    std::map<int, CfgDungeonNpc>::map(&this->m_dungeonNpcs);
    std::map<int, CfgItem *>::map(&this->m_items);
    Answer::RwLock::RwLock(&this->m_itemsLock);
    std::map<int, std::vector<CfgItemGift> *>::map(&this->m_itemGifts);
    Answer::RwLock::RwLock(&this->m_itemGiftsLock);
    std::map<int, std::vector<CfgItemGiftRandom> *>::map(&this->m_itemGiftRandoms);
    Answer::RwLock::RwLock(&this->m_itemGiftRandomsLock);
    std::map<int, CfgMountEquip>::map(&this->m_mountEquip);
    std::map<int, CfgJob>::map(&this->m_jobs);
    std::map<int, CfgMovie>::map(&this->m_movie);
    std::map<int, CfgLevelExp>::map(&this->m_levelExps);
    std::map<int, CfgMap>::map(&this->m_maps);
    std::map<int, std::vector<CfgMapMonster>>::map(&this->m_mapMonsters);
    std::map<int, CfgMapMonster>::map(&this->m_CfgMapMonsters);
    std::map<int, std::vector<CfgMapPlant>>::map(&this->m_mapPlants);
    std::map<int, CfgMapPlant>::map(&this->m_mMapPlants);
    std::map<int, CfgMapRegion>::map(&this->m_mapRegions);
    std::map<int, std::vector<CfgMapRegion>>::map(&this->m_mapRegionsByMapId);
    std::map<int, CfgMonster>::map(&this->m_monsters);
    std::map<int, CfgMonsterAI>::map(&this->m_mMonsterAI);
    std::map<std::pair<int, int>, CfgMonsterAdjust>::map(&this->m_mMonsterAdjust);
    std::vector<int>::vector(&this->m_monsterBroadcasts);
    std::map<int, std::vector<CfgMonsterDropGroup>>::map(&this->m_monsterDropGroups);
    std::map<int, std::vector<CfgMonsterGroupDrop>>::map(&this->m_monsterGroupDrops);
    std::map<int, std::vector<CfgMonsterTaskDrop>>::map(&this->m_monsterTaskDrops);
    std::map<int, CfgNpc>::map(&this->m_npcs);
    std::map<int, CfgNpcAirport>::map(&this->m_npcAirports);
    std::map<int, CfgChrShop>::map(&this->m_chrShops);
    std::map<int, CfgPlant>::map(&this->m_plants);
    std::map<int, CfgTask>::map(&this->m_tasks);
    std::map<int, CfgTrap>::map(&this->m_traps);
    std::map<int, CfgLevelAttr>::map(&this->m_levelAttrs);
    std::map<int, CfgGroupIcon>::map(&this->m_cfgGroupIcons);
    CfgSkillTable::CfgSkillTable(&this->m_cfgSkillTable);
    CfgTalentTable::CfgTalentTable(&this->m_cfgTalentTable);
    CfgMonsterRandTable::CfgMonsterRandTable(&this->m_cfgMonsterRandTable);
    CfgWingTable::CfgWingTable(&this->m_cfgWing);
    CfgCarrierTable::CfgCarrierTable(&this->m_cfgCarrierTable);
    CfgJueWeiTable::CfgJueWeiTable(&this->m_cfgJueWeiTable);
    CfgEquipBoxTable::CfgEquipBoxTable(&this->m_cfgEquipBoxTalbe);
    CfgActivityTaskTable::CfgActivityTaskTable(&this->m_cfgActivityTaskTable);
    CfgOutLinkFestivalTable::CfgOutLinkFestivalTable(&this->m_cfgOutLinkFestivalTable);
    std::map<int, CfgYYGameApp>::map(&this->m_CfgYYGameAppMap);
    std::map<int, CfgYYVip>::map(&this->m_CfgYYVipMap);
    std::list<CfgYYSuperBuff>::list(&this->m_CfgYYSuperBuffList);
    std::list<CfgYYSuperBuff>::list(&this->m_Cfg37wanSuperBuffList);
    std::map<int, CfgSgGameApp>::map(&this->m_CfgSgGameAppMap);
    CfgEquipTable::CfgEquipTable(&this->m_cfgEquip);
    CfgItemGemTable::CfgItemGemTable(&this->m_cfgItemGem);
    CfgItemCombiTable::CfgItemCombiTable(&this->m_cfgItemCombi);
    CfgPetTable::CfgPetTable(&this->m_cfgPetTable);
    CfgSkillLevelUpTable::CfgSkillLevelUpTable(&this->m_cfgSkillLevelUpTable);
    CfgTaskCycleTable::CfgTaskCycleTable(&this->m_cfgTaskCycleTable);
    CfgFamilyTable::CfgFamilyTable(&this->m_cfgFamilyTable);
    CfgFamilyWarTable::CfgFamilyWarTable(&this->m_cfgFamilyWarTable);
    CfgFamilyLightExpTable::CfgFamilyLightExpTable(&this->m_cfgFamilyLightExpTable);
    CfgHorseRacingRewardTable::CfgHorseRacingRewardTable(&this->m_cfgHorseRacingRewardTable);
    CfgCampWarTable::CfgCampWarTable(&this->m_cfgCampWarTable);
    CfgCityWarTable::CfgCityWarTable(&this->m_cfgCityWarTable);
    CfgHallOfFameTable::CfgHallOfFameTable(&this->m_cfgHallOfFameTable);
    CfgTitleTable::CfgTitleTable(&this->m_cfgTitleTable);
    std::map<int, CfgDropRecord>::map(&this->m_cfgDropRecordTable);
    CfgDungeonScoreTable::CfgDungeonScoreTable(&this->m_cfgDungeonScoreTable);
    std::map<std::pair<int, int>, CfgDungeonSummon>::map(&this->m_cfgDungeonSummon);
    CfgGoldEggTable::CfgGoldEggTable(&this->m_cfgGoldEggTable);
    CfgLimitTimeTable::CfgLimitTimeTable(&this->m_cfgLimitTimeTable);
    CfgMysteryShopTable::CfgMysteryShopTable(&this->m_cfgMysteryShopTable);
    std::map<int, CfgBuyGift>::map(&this->m_cfgBuyGiftTable);
    CfgExchangeTable::CfgExchangeTable(&this->m_cfgExchangeTable);
    CfgMysteryGiftTable::CfgMysteryGiftTable(&this->m_cfgMysteryGiftTable);
    CfgDrawTable::CfgDrawTable(&this->m_cfgDrawTable);
    CfgMapRoadTable::CfgMapRoadTable(&this->m_cfgMapRoadTable);
    CfgTrailerTable::CfgTrailerTable(&this->m_cfgTrailerTable);
    CfgMYSJRewardTable::CfgMYSJRewardTable(&this->m_cfgMYSJRewardTable);
    CfgMaintainCompensateTable::CfgMaintainCompensateTable(&this->m_cfgMaintainCompensateTable);
    CfgWishRewardTable::CfgWishRewardTable(&this->m_cfgWishRewardTable);
    CfgBFZLEnterCostTable::CfgBFZLEnterCostTable(&this->m_cfgBFZLEnterCostTable);
    CfgBlacketMarketTable::CfgBlacketMarketTable(&this->m_cfgBlacketMarketTable);
    std::map<int, int>::map(&this->m_cfgLevelChatTable);
    std::map<std::string, CfgSuperMember>::map(&this->m_cfgSuperMember);
    std::map<std::string, CfgSouGouSkin>::map(&this->m_cfgSouGouSkin);
    CfgPetEquipTable::CfgPetEquipTable(&this->m_cfgPetEquipTable);
    CfgWeiXinTable::CfgWeiXinTable(&this->m_cfgWeiXinTable);
    CfgAdultGiftTable::CfgAdultGiftTable(&this->m_cfgAdultGiftTable);
    CfgTencentTable::CfgTencentTable(&this->m_cfgTencentTable);
    CfgFamilyDungeonTable::CfgFamilyDungeonTable(&this->m_cfgFamilyDungeonTable);
    cfgGoblinTableData::cfgGoblinTableData(&this->m_cfgGoblinTableData);
    cfgSpecialMonsterTable::cfgSpecialMonsterTable(&this->m_cfgSpecialMonsterTable);
    cfgEquipUpPhaseTable::cfgEquipUpPhaseTable(&this->m_cfgEquipUpPhaseTable);
    CfgTouZiTable::CfgTouZiTable(&this->m_cfgTouZiTable);
    CfgSearchBackTable::CfgSearchBackTable(&this->m_cfgSearchBackTable);
    CfgEquipBlessTable::CfgEquipBlessTable(&this->m_cfgEquipBlessTable);
    std::list<PkDropRate>::list(&this->m_PkDropRateList);
    this->m_debug = 0;
    this->m_serverType = 0;
    std::map<int, CfgSysMail>::map(&this->m_sysMail);
    std::map<int, BossInfo>::map(&this->m_BossInfo);
    FaBaoTable::FaBaoTable(&this->m_FaBaoTable);
    std::map<int, std::vector<CfgQuestions>>::map(&this->m_mQuestions);
    std::map<int, CfgGameShop>::map(&this->m_GameShopMap);
    CQiFuTable::CQiFuTable(&this->m_QiFuTable);
    VipTable::VipTable(&this->m_VipTable);
    std::map<signed char, CfgSignReward>::map(&this->m_signReward);
    std::map<signed char, CfgOnlineReward>::map(&this->m_OnLineReward);
    std::map<signed char, CfgSevenLoginRewrad>::map(&this->m_SevenLoginReward);
    std::map<int, CfgLevelGift>::map(&this->m_LevelGift);
    std::map<int, CfgWeekOnlineReward>::map(&this->m_WeekOnlineReward);
    std::map<int, CfgOffLineExp>::map(&this->m_OffLineExpTable);
    std::map<int, VipCardCfg>::map(&this->m_VipCardTable);
    std::map<int, FunctionOpenCfg>::map(&this->m_FunctionOpenCfg);
    std::map<int, std::vector<CfgItemGift> *>::map(&this->m_PetGifts);
    std::map<int, CfgPlantEventEffect>::map(&this->m_PlantEventMap);
    std::map<signed char, CfgEquipExchange>::map(&this->m_CfgEquipExchangeMap);
    CfgShouChongTable::CfgShouChongTable(&this->m_cfgShouChongTable);
    std::map<signed char, NewServerFavorable>::map(&this->m_NewServerFavorable);
    NewServerFavorable::NewServerFavorable(&this->m_ThreePetGift);
    std::list<CfgEverydayChongZhi>::list(&this->m_EveryDayChongZhiTable);
    std::map<int, CfgKaiFuHuoDongData>::map(&this->m_KaiFuHuoDongTable);
    std::map<int, CfgHuoYueDu>::map(&this->m_HuoYueDuTable);
    std::map<signed char, CfgHuoYueDuReward>::map(&this->m_HuoYueDuRewardTable);
    std::list<CfgAppendAttr>::list(&this->m_AppendAttrTable);
    std::list<SpecialTreasureMapRandCfg>::list(&this->m_SpecialTreasureMapRandCfgList);
    CfgFamilyTask::CfgFamilyTask(&this->m_FamilyTaskTable);
    std::map<int, FamilyTaskReward>::map(&this->m_FamilyTaskReward);
    std::map<signed char, CfgTotalChongZhi>::map(&this->m_TotalChongZhiTable);
    std::map<int, std::list<CfgMonsterAddAttr>>::map(&this->m_MonstAddAttrMap);
    std::map<int, DaZheQuan>::map(&this->m_DaZheQuanMap);
    std::map<int, Wan360Reward>::map(&this->m_Wan360RewardMap);
    std::map<std::string, int>::map(&this->m_Wan360RewardTypeMap);
    std::set<std::string>::set(&this->m_ItemLog);
    CfgVplan::CfgVplan(&this->m_CfgVplan);
    Speed360Reward::Speed360Reward(&this->m_Speed360Reward);
    std::map<int, std::vector<GroupMonster>>::map(&this->m_GroupMonsterMap);
    std::map<int, BuyFaBaoResCfg>::map(&this->m_FaBaoResMap);
    std::map<int, XingMaiCfg>::map(&this->m_XingMaiCfgTable);
    std::map<int, CfgXingMaiSlot>::map(&this->m_cfgXingMaiSlotTable);
    std::map<int, TianLingCfg>::map(&this->m_TianLingCfgTable);
    std::map<int, DamnationCfg>::map(&this->m_DamnationCfgTable);
    std::map<int, ScoreShopCfg>::map(&this->m_ScoreShopCfgTable);
    ChouJiangTable::ChouJiangTable(&this->m_ChouJiangTable);
    CLibraryTable::CLibraryTable(&this->m_CLibraryTable);
    TreasureMapTabale::TreasureMapTabale(&this->m_TreasureMapTabale);
    std::map<int, BackEquipTask>::map(&this->m_EquipBackTable);
    std::map<int, std::map<int, std::list<Param2>>>::map(&this->m_EquipBackTaskRate);
    std::map<int, SpeciaEquipCfg>::map(&this->m_SpeciaEquipCfgMap);
    std::map<std::pair<int, int>, SpeciaSkillDistance>::map(&this->m_SpeciaSkillDistanceTable);
    LuckDropTable::LuckDropTable(&this->m_LuckDropTable);
    std::map<int, LevelRefinCfg>::map(&this->m_LevelRefinCfgMap);
    std::map<int, CfgBossFirstKilled>::map(&this->m_CfgBossFirstKilledMap);
    std::map<int, UltimateChallengeCfg>::map(&this->m_UltimateChallengeCfgMap);
    std::map<int, CfgCachet>::map(&this->m_CfgCachetMap);
    std::map<signed char, BossKilledReward>::map(&this->m_BossKilledRewardMap);
    std::map<int, SystemOpenGift>::map(&this->m_SystemOpenGiftMap);
    std::map<int, SpecialMap>::map(&this->m_SpecialMapMap);
    std::map<int, TestServerReward>::map(&this->m_TestServerRewardMap);
    std::map<int, GuWuCfg>::map(&this->m_GuWuCfgMap);
    std::map<int, FunctionOpenMail>::map(&this->m_FunctionOpenMailMap);
    std::list<XiangYaoTaskCfg>::list(&this->m_XiangYaoTaskCfgList);
    std::map<int, ShenWeiTaskCfg>::map(&this->m_ShenWeiTaskCfgMap);
    std::map<std::string, CfgMobilePhoneGift>::map(&this->m_CfgMobilePhoneGift);
    std::map<std::string, CfgMiniClient>::map(&this->m_CfgMiniClient);
    std::map<int, std::list<CfgWuHunShop>>::map(&this->m_CfgWuHunShopMap);
    std::map<int, CfgWuHunShop>::map(&this->m_CfgWuHunShopItemMap);
    std::map<int, WuHunItem>::map(&this->m_WuHunItemMap);
    std::map<int, CreateWuHun>::map(&this->m_CreateWuHunMap);
    BossDistribution::BossDistribution(&this->m_BossDistribution);
    std::map<int, SpecialBossMapCfg>::map(&this->m_SpecialBossMapCfgMap);
    std::map<int, DuiHuanLimit>::map(&this->m_DuiHuanLimit);
    std::map<std::pair<int, int>, JewelPavilionCfg>::map(&this->m_JewelPavilionCfgMap);
    std::map<int, SuperTeHuiCfg>::map(&this->m_SuperTeHuiCfgMap);
    std::map<std::pair<int, int>, GoblinCfg>::map(&this->m_GoblinCfgMap);
    std::map<int, std::list<AddAttribute>>::map(&this->m_GoblinSuitMap);
    std::map<std::pair<int, int>, ShouHuRefinishingCfg>::map(&this->m_ShouHuRefinishingCfgMap);
    std::map<int, std::list<AddAttribute>>::map(&this->m_ShouHuRefinishingSuitMap);
    std::map<std::pair<int, int>, WingEquipPolish>::map(&this->m_WingEquipPolishCfgMap);
    std::map<int, std::list<AddAttribute>>::map(&this->m_WingEquipPolishSuitMap);
    std::map<std::pair<int, int>, WinRefiningCfg>::map(&this->m_WinRefiningCfgMap);
    std::map<int, std::list<AddAttribute>>::map(&this->m_WingEquipRefiningSuitMap);
    std::map<int, GuiGuDaoRenCfg>::map(&this->m_GuiGuDaoRenCfgMap);
    CfgShiZhuangTable::CfgShiZhuangTable(&this->m_cfgShiZhuangTable);
    MonthlyChouJiangTable::MonthlyChouJiangTable(&this->m_MonthlyChouJiangTable);
    std::map<int, std::list<ActDropItem>>::map(&this->m_ActDropItemListMap);
    std::map<int, XunLeiCfg>::map(&this->m_XunLeiCfgMap);
    CfgGuardPrivilegeTable::CfgGuardPrivilegeTable(&this->m_cfgGuardPrivilegeTable);
    CycleTowerTable::CycleTowerTable(&this->m_CycleTowerTable);
    std::map<std::pair<std::string, int>, PlatformDaTing>::map(&this->m_PlatformDaTingMap);
    std::map<std::pair<std::string, int>, MiniClientCfg>::map(&this->m_GetMiniClientMap);
    std::map<std::pair<std::string, int>, PlatformVip>::map(&this->m_PlatformVipMap);
    std::map<int, ShangRenCfg>::map(&this->m_ShangRenCfgMap);
    std::map<int, SelectItemCfg>::map(&this->m_SelectItemCfgMap);
    std::map<std::pair<std::string, int>, PlatformRewardCfg>::map(&this->m_PlatformRewardCfgMap);
    std::map<int, DiligenceCfg>::map(&this->m_DiligenceCfgMap);
    std::map<int, ContributionCfg>::map(&this->m_ContributionCfgMap);
    std::map<int, HoeCfg>::map(&this->m_HoeCfgMap);
    CMingGeTable::CMingGeTable(&this->m_CMingGeTable);
    std::map<int, CrossTowerCfg>::map(&this->m_CrossTowerCfgMap);
    KunLingTable::KunLingTable(&this->m_KunLingTable);
    std::map<int, LuDaShiVip>::map(&this->m_LuDaShiVipMap);
    std::map<int, FlopDrawCfg>::map(&this->m_FlopDrawCfgMap);
    SevenTaskTable::SevenTaskTable(&this->m_SevenTaskTable);
    std::map<int, MoFuSkillCfg>::map(&this->m_MoFuSkillCfgMap);
    std::map<std::pair<int, int>, ZhuNengCfg>::map(&this->m_ZhuNengCfgMap);
    std::map<int, MoFuDuiHuan>::map(&this->m_MoFuDuiHuanMap);
    std::map<int, ChristmasDuiHuan>::map(&this->m_ChristmasDuiHuanMap);
    std::map<int, BaoKuFuBen>::map(&this->m_BaoKuFuBenMap);
    std::map<int, BaoKuRandom>::map(&this->m_BaoKuRandomMap);
    std::map<int, ChargeDungeonCfg>::map(&this->m_ChargeDungeonCfgMap);
    std::map<int, GongMingCfg>::map(&this->m_GongMingCfgMap);
    CfgBeastShrineTable::CfgBeastShrineTable(&this->m_CfgBeastShrineTable);
    std::map<int, int>::map(&this->m_TongTianChiRankReward);
    XinMoTable::XinMoTable(&this->m_XinMoTable);
    std::map<int, std::list<RefreshMonsterCfg>>::map(&this->m_RefreshMonsterCfgListMap);
    EnergyCfg::EnergyCfg(&this->m_EnergyCfg);
    std::map<std::pair<int, int>, VipEquipPosLevelUp>::map(&this->m_VipEquipPosLevelUpMap);
    std::list<VipEQuipPosSuit>::list(&this->m_VipEQuipPosSuitList);
    std::map<std::pair<int, int>, ShiZhuLevelUp>::map(&this->m_ShiZhuLevelUpMap);
    std::list<ClbAimCfg>::list(&this->m_ClbAimCfgList);
    std::map<int, CVipClubLuckyDrop>::map(&this->m_CVipClubLuckyDropMap);
    std::map<int, ZhanHunCfg>::map(&this->m_ZhanHunCfgMap);
    std::map<int, LittleHelperCfg>::map(&this->m_LittleHelperCfgMap);
    std::map<int, ShiQuCfg>::map(&this->m_ShiQuCfgMap);
    ZeroBuyPetCfg::ZeroBuyPetCfg(&this->m_ZeroBuyPetCfg);
    std::map<int, SunAndMoonCfg>::map(&this->m_SunAndMoonCfgMap);
    std::map<int, TreasureHunterCfg>::map(&this->m_TreasureHunterCfgMap);
    std::map<std::pair<int, int>, NationalDayTaskCfg>::map(&this->m_NationalDayTaskCfgMap);
    std::map<int, NationalDayLevelCfg>::map(&this->m_NationalDayLevelCfgMap);
    std::map<int, RongHeCfg>::map(&this->m_RongHeCfgMap);
    std::map<int, EquipRongHe>::map(&this->m_EquipRongHeMap);
    this->m_kaiFuTime = 0;
    this->m_heFuTime = 0;
}

// ==================== 析构函数 ====================

CfgData::~CfgData(CfgData *const this)
{
    std::map<int, EquipRongHe>::~map(&this->m_EquipRongHeMap);
    std::map<int, RongHeCfg>::~map(&this->m_RongHeCfgMap);
    std::map<int, NationalDayLevelCfg>::~map(&this->m_NationalDayLevelCfgMap);
    std::map<std::pair<int, int>, NationalDayTaskCfg>::~map(&this->m_NationalDayTaskCfgMap);
    std::map<int, TreasureHunterCfg>::~map(&this->m_TreasureHunterCfgMap);
    std::map<int, SunAndMoonCfg>::~map(&this->m_SunAndMoonCfgMap);
    ZeroBuyPetCfg::~ZeroBuyPetCfg(&this->m_ZeroBuyPetCfg);
    std::map<int, ShiQuCfg>::~map(&this->m_ShiQuCfgMap);
    std::map<int, LittleHelperCfg>::~map(&this->m_LittleHelperCfgMap);
    std::map<int, ZhanHunCfg>::~map(&this->m_ZhanHunCfgMap);
    std::map<int, CVipClubLuckyDrop>::~map(&this->m_CVipClubLuckyDropMap);
    std::list<ClbAimCfg>::~list(&this->m_ClbAimCfgList);
    std::map<std::pair<int, int>, ShiZhuLevelUp>::~map(&this->m_ShiZhuLevelUpMap);
    std::list<VipEQuipPosSuit>::~list(&this->m_VipEQuipPosSuitList);
    std::map<std::pair<int, int>, VipEquipPosLevelUp>::~map(&this->m_VipEquipPosLevelUpMap);
    EnergyCfg::~EnergyCfg(&this->m_EnergyCfg);
    std::map<int, std::list<RefreshMonsterCfg>>::~map(&this->m_RefreshMonsterCfgListMap);
    XinMoTable::~XinMoTable(&this->m_XinMoTable);
    std::map<int, int>::~map(&this->m_TongTianChiRankReward);
    CfgBeastShrineTable::~CfgBeastShrineTable(&this->m_CfgBeastShrineTable);
    std::map<int, GongMingCfg>::~map(&this->m_GongMingCfgMap);
    std::map<int, ChargeDungeonCfg>::~map(&this->m_ChargeDungeonCfgMap);
    std::map<int, BaoKuRandom>::~map(&this->m_BaoKuRandomMap);
    std::map<int, BaoKuFuBen>::~map(&this->m_BaoKuFuBenMap);
    std::map<int, ChristmasDuiHuan>::~map(&this->m_ChristmasDuiHuanMap);
    std::map<int, MoFuDuiHuan>::~map(&this->m_MoFuDuiHuanMap);
    std::map<std::pair<int, int>, ZhuNengCfg>::~map(&this->m_ZhuNengCfgMap);
    std::map<int, MoFuSkillCfg>::~map(&this->m_MoFuSkillCfgMap);
    SevenTaskTable::~SevenTaskTable(&this->m_SevenTaskTable);
    std::map<int, FlopDrawCfg>::~map(&this->m_FlopDrawCfgMap);
    std::map<int, LuDaShiVip>::~map(&this->m_LuDaShiVipMap);
    KunLingTable::~KunLingTable(&this->m_KunLingTable);
    std::map<int, CrossTowerCfg>::~map(&this->m_CrossTowerCfgMap);
    CMingGeTable::~CMingGeTable(&this->m_CMingGeTable);
    std::map<int, HoeCfg>::~map(&this->m_HoeCfgMap);
    std::map<int, ContributionCfg>::~map(&this->m_ContributionCfgMap);
    std::map<int, DiligenceCfg>::~map(&this->m_DiligenceCfgMap);
    std::map<std::pair<std::string, int>, PlatformRewardCfg>::~map(&this->m_PlatformRewardCfgMap);
    std::map<int, SelectItemCfg>::~map(&this->m_SelectItemCfgMap);
    std::map<int, ShangRenCfg>::~map(&this->m_ShangRenCfgMap);
    std::map<std::pair<std::string, int>, PlatformVip>::~map(&this->m_PlatformVipMap);
    std::map<std::pair<std::string, int>, MiniClientCfg>::~map(&this->m_GetMiniClientMap);
    std::map<std::pair<std::string, int>, PlatformDaTing>::~map(&this->m_PlatformDaTingMap);
    CycleTowerTable::~CycleTowerTable(&this->m_CycleTowerTable);
    CfgGuardPrivilegeTable::~CfgGuardPrivilegeTable(&this->m_cfgGuardPrivilegeTable);
    std::map<int, XunLeiCfg>::~map(&this->m_XunLeiCfgMap);
    std::map<int, std::list<ActDropItem>>::~map(&this->m_ActDropItemListMap);
    MonthlyChouJiangTable::~MonthlyChouJiangTable(&this->m_MonthlyChouJiangTable);
    CfgShiZhuangTable::~CfgShiZhuangTable(&this->m_cfgShiZhuangTable);
    std::map<int, GuiGuDaoRenCfg>::~map(&this->m_GuiGuDaoRenCfgMap);
    std::map<int, std::list<AddAttribute>>::~map(&this->m_WingEquipRefiningSuitMap);
    std::map<std::pair<int, int>, WinRefiningCfg>::~map(&this->m_WinRefiningCfgMap);
    std::map<int, std::list<AddAttribute>>::~map(&this->m_WingEquipPolishSuitMap);
    std::map<std::pair<int, int>, WingEquipPolish>::~map(&this->m_WingEquipPolishCfgMap);
    std::map<int, std::list<AddAttribute>>::~map(&this->m_ShouHuRefinishingSuitMap);
    std::map<std::pair<int, int>, ShouHuRefinishingCfg>::~map(&this->m_ShouHuRefinishingCfgMap);
    std::map<int, std::list<AddAttribute>>::~map(&this->m_GoblinSuitMap);
    std::map<std::pair<int, int>, GoblinCfg>::~map(&this->m_GoblinCfgMap);
    std::map<int, SuperTeHuiCfg>::~map(&this->m_SuperTeHuiCfgMap);
    std::map<std::pair<int, int>, JewelPavilionCfg>::~map(&this->m_JewelPavilionCfgMap);
    std::map<int, DuiHuanLimit>::~map(&this->m_DuiHuanLimit);
    std::map<int, SpecialBossMapCfg>::~map(&this->m_SpecialBossMapCfgMap);
    BossDistribution::~BossDistribution(&this->m_BossDistribution);
    std::map<int, CreateWuHun>::~map(&this->m_CreateWuHunMap);
    std::map<int, WuHunItem>::~map(&this->m_WuHunItemMap);
    std::map<int, CfgWuHunShop>::~map(&this->m_CfgWuHunShopItemMap);
    std::map<int, std::list<CfgWuHunShop>>::~map(&this->m_CfgWuHunShopMap);
    std::map<std::string, CfgMiniClient>::~map(&this->m_CfgMiniClient);
    std::map<std::string, CfgMobilePhoneGift>::~map(&this->m_CfgMobilePhoneGift);
    std::map<int, ShenWeiTaskCfg>::~map(&this->m_ShenWeiTaskCfgMap);
    std::list<XiangYaoTaskCfg>::~list(&this->m_XiangYaoTaskCfgList);
    std::map<int, FunctionOpenMail>::~map(&this->m_FunctionOpenMailMap);
    std::map<int, GuWuCfg>::~map(&this->m_GuWuCfgMap);
    std::map<int, TestServerReward>::~map(&this->m_TestServerRewardMap);
    std::map<int, SpecialMap>::~map(&this->m_SpecialMapMap);
    std::map<int, SystemOpenGift>::~map(&this->m_SystemOpenGiftMap);
    std::map<signed char, BossKilledReward>::~map(&this->m_BossKilledRewardMap);
    std::map<int, CfgCachet>::~map(&this->m_CfgCachetMap);
    std::map<int, UltimateChallengeCfg>::~map(&this->m_UltimateChallengeCfgMap);
    std::map<int, CfgBossFirstKilled>::~map(&this->m_CfgBossFirstKilledMap);
    std::map<int, LevelRefinCfg>::~map(&this->m_LevelRefinCfgMap);
    LuckDropTable::~LuckDropTable(&this->m_LuckDropTable);
    std::map<std::pair<int, int>, SpeciaSkillDistance>::~map(&this->m_SpeciaSkillDistanceTable);
    std::map<int, SpeciaEquipCfg>::~map(&this->m_SpeciaEquipCfgMap);
    std::map<int, std::map<int, std::list<Param2>>>::~map(&this->m_EquipBackTaskRate);
    std::map<int, BackEquipTask>::~map(&this->m_EquipBackTable);
    TreasureMapTabale::~TreasureMapTabale(&this->m_TreasureMapTabale);
    CLibraryTable::~CLibraryTable(&this->m_CLibraryTable);
    ChouJiangTable::~ChouJiangTable(&this->m_ChouJiangTable);
    std::map<int, ScoreShopCfg>::~map(&this->m_ScoreShopCfgTable);
    std::map<int, DamnationCfg>::~map(&this->m_DamnationCfgTable);
    std::map<int, TianLingCfg>::~map(&this->m_TianLingCfgTable);
    std::map<int, CfgXingMaiSlot>::~map(&this->m_cfgXingMaiSlotTable);
    std::map<int, XingMaiCfg>::~map(&this->m_XingMaiCfgTable);
    std::map<int, BuyFaBaoResCfg>::~map(&this->m_FaBaoResMap);
    std::map<int, std::vector<GroupMonster>>::~map(&this->m_GroupMonsterMap);
    Speed360Reward::~Speed360Reward(&this->m_Speed360Reward);
    CfgVplan::~CfgVplan(&this->m_CfgVplan);
    std::set<std::string>::~set(&this->m_ItemLog);
    std::map<std::string, int>::~map(&this->m_Wan360RewardTypeMap);
    std::map<int, Wan360Reward>::~map(&this->m_Wan360RewardMap);
    std::map<int, DaZheQuan>::~map(&this->m_DaZheQuanMap);
    std::map<int, std::list<CfgMonsterAddAttr>>::~map(&this->m_MonstAddAttrMap);
    std::map<signed char, CfgTotalChongZhi>::~map(&this->m_TotalChongZhiTable);
    std::map<int, FamilyTaskReward>::~map(&this->m_FamilyTaskReward);
    CfgFamilyTask::~CfgFamilyTask(&this->m_FamilyTaskTable);
    std::list<SpecialTreasureMapRandCfg>::~list(&this->m_SpecialTreasureMapRandCfgList);
    std::list<CfgAppendAttr>::~list(&this->m_AppendAttrTable);
    std::map<signed char, CfgHuoYueDuReward>::~map(&this->m_HuoYueDuRewardTable);
    std::map<int, CfgHuoYueDu>::~map(&this->m_HuoYueDuTable);
    std::map<int, CfgKaiFuHuoDongData>::~map(&this->m_KaiFuHuoDongTable);
    std::list<CfgEverydayChongZhi>::~list(&this->m_EveryDayChongZhiTable);
    NewServerFavorable::~NewServerFavorable(&this->m_ThreePetGift);
    std::map<signed char, NewServerFavorable>::~map(&this->m_NewServerFavorable);
    CfgShouChongTable::~CfgShouChongTable(&this->m_cfgShouChongTable);
    std::map<signed char, CfgEquipExchange>::~map(&this->m_CfgEquipExchangeMap);
    std::map<int, CfgPlantEventEffect>::~map(&this->m_PlantEventMap);
    std::map<int, std::vector<CfgItemGift> *>::~map(&this->m_PetGifts);
    std::map<int, FunctionOpenCfg>::~map(&this->m_FunctionOpenCfg);
    std::map<int, VipCardCfg>::~map(&this->m_VipCardTable);
    std::map<int, CfgOffLineExp>::~map(&this->m_OffLineExpTable);
    std::map<int, CfgWeekOnlineReward>::~map(&this->m_WeekOnlineReward);
    std::map<int, CfgLevelGift>::~map(&this->m_LevelGift);
    std::map<signed char, CfgSevenLoginRewrad>::~map(&this->m_SevenLoginReward);
    std::map<signed char, CfgOnlineReward>::~map(&this->m_OnLineReward);
    std::map<signed char, CfgSignReward>::~map(&this->m_signReward);
    VipTable::~VipTable(&this->m_VipTable);
    CQiFuTable::~CQiFuTable(&this->m_QiFuTable);
    std::map<int, CfgGameShop>::~map(&this->m_GameShopMap);
    std::map<int, std::vector<CfgQuestions>>::~map(&this->m_mQuestions);
    FaBaoTable::~FaBaoTable(&this->m_FaBaoTable);
    std::map<int, BossInfo>::~map(&this->m_BossInfo);
    std::map<int, CfgSysMail>::~map(&this->m_sysMail);
    std::list<PkDropRate>::~list(&this->m_PkDropRateList);
    CfgEquipBlessTable::~CfgEquipBlessTable(&this->m_cfgEquipBlessTable);
    CfgSearchBackTable::~CfgSearchBackTable(&this->m_cfgSearchBackTable);
    CfgTouZiTable::~CfgTouZiTable(&this->m_cfgTouZiTable);
    cfgEquipUpPhaseTable::~cfgEquipUpPhaseTable(&this->m_cfgEquipUpPhaseTable);
    cfgSpecialMonsterTable::~cfgSpecialMonsterTable(&this->m_cfgSpecialMonsterTable);
    cfgGoblinTableData::~cfgGoblinTableData(&this->m_cfgGoblinTableData);
    CfgFamilyDungeonTable::~CfgFamilyDungeonTable(&this->m_cfgFamilyDungeonTable);
    CfgTencentTable::~CfgTencentTable(&this->m_cfgTencentTable);
    CfgAdultGiftTable::~CfgAdultGiftTable(&this->m_cfgAdultGiftTable);
    CfgWeiXinTable::~CfgWeiXinTable(&this->m_cfgWeiXinTable);
    CfgPetEquipTable::~CfgPetEquipTable(&this->m_cfgPetEquipTable);
    std::map<std::string, CfgSouGouSkin>::~map(&this->m_cfgSouGouSkin);
    std::map<std::string, CfgSuperMember>::~map(&this->m_cfgSuperMember);
    std::map<int, int>::~map(&this->m_cfgLevelChatTable);
    CfgBlacketMarketTable::~CfgBlacketMarketTable(&this->m_cfgBlacketMarketTable);
    CfgBFZLEnterCostTable::~CfgBFZLEnterCostTable(&this->m_cfgBFZLEnterCostTable);
    CfgWishRewardTable::~CfgWishRewardTable(&this->m_cfgWishRewardTable);
    CfgMaintainCompensateTable::~CfgMaintainCompensateTable(&this->m_cfgMaintainCompensateTable);
    CfgMYSJRewardTable::~CfgMYSJRewardTable(&this->m_cfgMYSJRewardTable);
    CfgTrailerTable::~CfgTrailerTable(&this->m_cfgTrailerTable);
    CfgMapRoadTable::~CfgMapRoadTable(&this->m_cfgMapRoadTable);
    CfgDrawTable::~CfgDrawTable(&this->m_cfgDrawTable);
    CfgMysteryGiftTable::~CfgMysteryGiftTable(&this->m_cfgMysteryGiftTable);
    CfgExchangeTable::~CfgExchangeTable(&this->m_cfgExchangeTable);
    std::map<int, CfgBuyGift>::~map(&this->m_cfgBuyGiftTable);
    CfgMysteryShopTable::~CfgMysteryShopTable(&this->m_cfgMysteryShopTable);
    CfgLimitTimeTable::~CfgLimitTimeTable(&this->m_cfgLimitTimeTable);
    CfgGoldEggTable::~CfgGoldEggTable(&this->m_cfgGoldEggTable);
    std::map<std::pair<int, int>, CfgDungeonSummon>::~map(&this->m_cfgDungeonSummon);
    CfgDungeonScoreTable::~CfgDungeonScoreTable(&this->m_cfgDungeonScoreTable);
    std::map<int, CfgDropRecord>::~map(&this->m_cfgDropRecordTable);
    CfgTitleTable::~CfgTitleTable(&this->m_cfgTitleTable);
    CfgHallOfFameTable::~CfgHallOfFameTable(&this->m_cfgHallOfFameTable);
    CfgCityWarTable::~CfgCityWarTable(&this->m_cfgCityWarTable);
    CfgCampWarTable::~CfgCampWarTable(&this->m_cfgCampWarTable);
    CfgHorseRacingRewardTable::~CfgHorseRacingRewardTable(&this->m_cfgHorseRacingRewardTable);
    CfgFamilyLightExpTable::~CfgFamilyLightExpTable(&this->m_cfgFamilyLightExpTable);
    CfgFamilyWarTable::~CfgFamilyWarTable(&this->m_cfgFamilyWarTable);
    CfgFamilyTable::~CfgFamilyTable(&this->m_cfgFamilyTable);
    CfgTaskCycleTable::~CfgTaskCycleTable(&this->m_cfgTaskCycleTable);
    CfgSkillLevelUpTable::~CfgSkillLevelUpTable(&this->m_cfgSkillLevelUpTable);
    CfgPetTable::~CfgPetTable(&this->m_cfgPetTable);
    CfgItemCombiTable::~CfgItemCombiTable(&this->m_cfgItemCombi);
    CfgItemGemTable::~CfgItemGemTable(&this->m_cfgItemGem);
    CfgEquipTable::~CfgEquipTable(&this->m_cfgEquip);
    std::map<int, CfgSgGameApp>::~map(&this->m_CfgSgGameAppMap);
    std::list<CfgYYSuperBuff>::~list(&this->m_Cfg37wanSuperBuffList);
    std::list<CfgYYSuperBuff>::~list(&this->m_CfgYYSuperBuffList);
    std::map<int, CfgYYVip>::~map(&this->m_CfgYYVipMap);
    std::map<int, CfgYYGameApp>::~map(&this->m_CfgYYGameAppMap);
    CfgOutLinkFestivalTable::~CfgOutLinkFestivalTable(&this->m_cfgOutLinkFestivalTable);
    CfgActivityTaskTable::~CfgActivityTaskTable(&this->m_cfgActivityTaskTable);
    CfgEquipBoxTable::~CfgEquipBoxTable(&this->m_cfgEquipBoxTalbe);
    CfgJueWeiTable::~CfgJueWeiTable(&this->m_cfgJueWeiTable);
    CfgCarrierTable::~CfgCarrierTable(&this->m_cfgCarrierTable);
    CfgWingTable::~CfgWingTable(&this->m_cfgWing);
    CfgMonsterRandTable::~CfgMonsterRandTable(&this->m_cfgMonsterRandTable);
    CfgTalentTable::~CfgTalentTable(&this->m_cfgTalentTable);
    CfgSkillTable::~CfgSkillTable(&this->m_cfgSkillTable);
    std::map<int, CfgGroupIcon>::~map(&this->m_cfgGroupIcons);
    std::map<int, CfgLevelAttr>::~map(&this->m_levelAttrs);
    std::map<int, CfgTrap>::~map(&this->m_traps);
    std::map<int, CfgTask>::~map(&this->m_tasks);
    std::map<int, CfgPlant>::~map(&this->m_plants);
    std::map<int, CfgChrShop>::~map(&this->m_chrShops);
    std::map<int, CfgNpcAirport>::~map(&this->m_npcAirports);
    std::map<int, CfgNpc>::~map(&this->m_npcs);
    std::map<int, std::vector<CfgMonsterTaskDrop>>::~map(&this->m_monsterTaskDrops);
    std::map<int, std::vector<CfgMonsterGroupDrop>>::~map(&this->m_monsterGroupDrops);
    std::map<int, std::vector<CfgMonsterDropGroup>>::~map(&this->m_monsterDropGroups);
    std::vector<int>::~vector(&this->m_monsterBroadcasts);
    std::map<std::pair<int, int>, CfgMonsterAdjust>::~map(&this->m_mMonsterAdjust);
    std::map<int, CfgMonsterAI>::~map(&this->m_mMonsterAI);
    std::map<int, CfgMonster>::~map(&this->m_monsters);
    std::map<int, std::vector<CfgMapRegion>>::~map(&this->m_mapRegionsByMapId);
    std::map<int, CfgMapRegion>::~map(&this->m_mapRegions);
    std::map<int, CfgMapPlant>::~map(&this->m_mMapPlants);
    std::map<int, std::vector<CfgMapPlant>>::~map(&this->m_mapPlants);
    std::map<int, CfgMapMonster>::~map(&this->m_CfgMapMonsters);
    std::map<int, std::vector<CfgMapMonster>>::~map(&this->m_mapMonsters);
    std::map<int, CfgMap>::~map(&this->m_maps);
    std::map<int, CfgLevelExp>::~map(&this->m_levelExps);
    std::map<int, CfgMovie>::~map(&this->m_movie);
    std::map<int, CfgJob>::~map(&this->m_jobs);
    std::map<int, CfgMountEquip>::~map(&this->m_mountEquip);
    Answer::RwLock::~RwLock(&this->m_itemGiftRandomsLock);
    std::map<int, std::vector<CfgItemGiftRandom> *>::~map(&this->m_itemGiftRandoms);
    Answer::RwLock::~RwLock(&this->m_itemGiftsLock);
    std::map<int, std::vector<CfgItemGift> *>::~map(&this->m_itemGifts);
    Answer::RwLock::~RwLock(&this->m_itemsLock);
    std::map<int, CfgItem *>::~map(&this->m_items);
    std::map<int, CfgDungeonNpc>::~map(&this->m_dungeonNpcs);
    std::map<int, CfgDungeonTrap>::~map(&this->m_dungeonTraps);
    std::map<int, CfgDungeonPlant>::~map(&this->m_dungeonPlants);
    std::map<int, CfgDungeonMonster>::~map(&this->m_dungeonMonsters);
    std::map<int, std::list<CfgMapEvent>>::~map(&this->m_dungeonEvents);
    std::map<int, CfgDungeonDropGroup>::~map(&this->m_dungeonDropGroup);
    std::map<int, CfgDungeonReward>::~map(&this->m_dungeonReward);
    std::map<int, CfgDungeon>::~map(&this->m_mUpTowerDungeon);
    std::map<int, CfgDungeon>::~map(&this->m_dungeons);
    std::map<int, CfgBuff>::~map(&this->m_buffs);
    std::map<int, CfgActivityTrap>::~map(&this->m_activityTraps);
    std::map<int, CfgActivityDrop>::~map(&this->m_activityDrops);
    std::map<int, CfgActivityPlant>::~map(&this->m_activityPlants);
    std::map<int, CfgActivityNpc>::~map(&this->m_activityNpcs);
    std::map<int, CfgActivityMonster>::~map(&this->m_activityMonsters);
    std::map<int, std::map<int, std::list<CfgMapEvent>>>::~map(&this->m_activityEvents);
    std::map<int, CfgActivity>::~map(&this->m_activities);
    std::list<CfgMapEvent>::~list(&this->m_emptyEvents);
}

// ==================== 单例与初始化 ====================

CfgData *CfgData::instance()
{
    return Answer::Singleton<CfgData>::instance();
}

bool CfgData::init(CfgData *const this, int32_t line, int32_t debug)
{
    CfgData::fetchServerConfig(this, line);
    CfgData::fetchActivity(this);
    CfgData::fetchBuff(this);
    CfgData::fetchDungeon(this);
    CfgData::fetchDungeonEvent(this);
    CfgData::fetchDungeonMonster(this);
    CfgData::fetchDungeonPlant(this);
    CfgData::fetchDungeonTrap(this);
    CfgData::fetchDungeonNpc(this);
    CfgData::fetchItem(this, 0);
    CfgData::fetchJob(this);
    CfgData::fetchLevelAttr(this);
    CfgData::fetchMap(this);
    CfgData::fetchMapMonster(this);
    CfgData::fetchMapPlant(this);
    CfgData::fetchChrShop(this);
    CfgData::fetchMapRegion(this);
    CfgData::fetchMonster(this);
    CfgData::fetchMonsterAI(this);
    CfgData::fetchMonsterAdjustTable(this);
    CfgData::fetchMonsterDropGroup(this);
    CfgData::fetchMonsterGroupDrop(this);
    CfgData::fetchMonsterTaskDrop(this);
    CfgData::fetchNpc(this);
    CfgData::fetchMovie(this);
    CfgData::fetchPlant(this);
    CfgData::fetchTask(this);
    CfgData::fetchTrap(this);
    CfgData::fetchLevelExp(this);
    CfgData::fetchSignReward(this);
    CfgData::InitGroupIconTable(this);
    CfgData::InitActiveSkillTable(this);
    CfgData::InitPassiveSkillTable(this);
    CfgData::InitTrigSkillTable(this);
    CfgData::InitTalentTable(this);
    CfgData::InitTalentPageTable(this);
    CfgData::InitFamilySkillTable(this);
    CfgData::InitTalentActiveTable(this);
    CfgData::InitEquipTable(this);
    CfgData::InitEquipUpStarTable(this);
    CfgData::InitEquipSuitTable(this);
    CfgData::InitEquipDecomposeTable(this);
    CfgData::InitEquipUpPosTable(this);
    CfgData::InitEquipPosSuitTable(this);
    CfgData::InitEquipBoxTable(this);
    CfgData::InitItemGemTable(this);
    CfgData::InitGemOpenHoleTable(this);
    CfgData::InitGemLevelUpTable(this);
    CfgData::InitGemSuitTable(this);
    CfgData::InitItemCombiTable(this);
    CfgData::InitMonsterRandTable(this);
    CfgData::InitCarrierTable(this);
    CfgData::InitCarrierAttrTable(this);
    CfgData::InitJueWeiTable(this);
    CfgData::InitPetTable(this);
    CfgData::InitPetAttrTable(this);
    CfgData::InitPetUpStarTable(this);
    CfgData::InitSkillLevelUpTable(this);
    CfgData::InitTaskCycleStarTable(this);
    CfgData::InitTaskCycleRewardTable(this);
    CfgData::InitFamilyTable(this);
    CfgData::InitFamilyWarJoinRewardTable(this);
    CfgData::InitFamilyWarReliveTable(this);
    CfgData::InitFamilyLightExpTable(this);
    CfgData::InitHorseRacingRewardTable(this);
    CfgData::InitCampWarRewardTable(this);
    CfgData::InitCampWarContKillTable(this);
    CfgData::InitCityWarContRewardTable(this);
    CfgData::InitHallOfFameRewardTable(this);
    CfgData::InitTitleTable(this);
    CfgData::InitGoldEggTable(this);
    CfgData::InitLimitTimeTable(this);
    CfgData::InitMysteryShopTable(this);
    CfgData::InitBuyGiftTable(this);
    CfgData::InitExchangeTable(this);
    CfgData::InitMysteryGiftTable(this);
    CfgData::InitDrawTable(this);
    CfgData::InitMapRoadTable(this);
    CfgData::InitTrailerTable(this);
    CfgData::InitMYSJRewardTable(this);
    CfgData::InitMaintainCompensateTable(this);
    CfgData::InitWishRewardTable(this);
    CfgData::InitBFZLEnterCostTable(this);
    CfgData::InitBlacketMarketTable(this);
    CfgData::InitLevelChatTimesTable(this);
    CfgData::InitSuperMemberTable(this);
    CfgData::InitSouGouSkinTable(this);
    CfgData::InitPetEquipTable(this);
    CfgData::InitWeiXinTable(this);
    CfgData::InitAdultGiftTable(this);
    CfgData::InitQQZoneRewardTable(this);
    CfgData::InitQQGameRewardTable(this);
    CfgData::InitYellowRewardTable(this);
    CfgData::InitYellowDailyRewardTable(this);
    CfgData::InitYellowLevelRewardTable(this);
    CfgData::InitBlueRewardTable(this);
    CfgData::InitBlueDailyRewardTable(this);
    CfgData::InitBlueLevelRewardTable(this);
    CfgData::InitTGPRewardTable(this);
    CfgData::InitTGPDailyRewardTable(this);
    CfgData::InitTGPLevelRewardTable(this);
    CfgData::InitFamilyDungeonTable(this);
    CfgData::InitDropRecordTable(this);
    CfgData::InitDungeonScoreTable(this);
    CfgData::InitDungeonSummon(this);
    CfgData::InitSysMail(this);
    CfgData::InitBossInfo(this);
    CfgData::InitFaBaoTable(this);
    CfgData::InitDaTiHD(this);
    CfgData::InitShangChengTable(this);
    CfgData::InitOnLimeReward(this);
    CfgData::InitSevenLoginReward(this);
    CfgData::InitLevelGift(this);
    CfgData::InitWeekOnlineReward(this);
    CfgData::InitOffLineExpTable(this);
    CfgData::InitVipCardTable(this);
    CfgData::InitFunctionOpenTable(this);
    CfgData::InitPlantEventTable(this);
    CfgData::InitZiYuanZhaoHuiTable(this);
    CfgData::InitMoHuaHuanYiTable(this);
    CfgData::InitShouChongLiBao(this);
    CfgData::InitNewServerFavorable(this);
    CfgData::InitMobilePhoneGiftTable(this);
    CfgData::InitEveryDayChongZhi(this);
    CfgData::InitKaiFuHuoDongTable(this);
    CfgData::InitHuoYueDuTable(this);
    CfgData::InitAppendAttrTable(this);
    CfgData::InitFamilyRewardTable(this);
    CfgData::InitTotalChongZhiTable(this);
    CfgData::InitSpecialTreasureMapRandTable(this);
    CfgData::InitMonsterAddAttrTable(this);
    CfgData::InitDaZheQuanTable(this);
    CfgData::Init360RewardTable(this);
    CfgData::Init360RewardTypeTable(this);
    CfgData::InitVplanTable(this);
    CfgData::InitShunWangTable(this);
    CfgData::InitSpeed360Reward(this);
    CfgData::InitTencentSevenDayLoginTable(this);
    CfgData::InitGoblinData(this);
    CfgData::InitSpecialMonster(this);
    CfgData::InitEquipUpPhase(this);
    CfgData::InitTouZiTable(this);
    CfgData::InitGroupMonster(this);
    CfgData::InitPkDropRateTable(this);
    CfgData::InitEquipBackTable(this);
    CfgData::InitStrengthenTable(this);
    CfgData::InitBuyFaBaoResTable(this);
    CfgData::InitXingMaiTable(this);
    CfgData::InitXingMaiSlotTable(this);
    CfgData::InitTianLingTable(this);
    CfgData::InitGongMinTable(this);
    CfgData::InitBeastShrineEnterCostTable(this);
    CfgData::InitDamnationTable(this);
    CfgData::InitAttrBattleTable(this);
    CfgData::InitScoreShopTable(this);
    CfgData::InitChouJiangTable(this);
    CfgData::InitWingCfgTable(this);
    CfgData::InitLibraryTable(this);
    CfgData::InitTreasureMapTabale(this);
    CfgData::InitEquipBackTask(this);
    CfgData::InitSpeciaEquipCfgMap(this);
    CfgData::InitSpeciaSkillDistanceTable(this);
    CfgData::InitLuckDropTable(this);
    CfgData::InitLevelRefiningTable(this);
    CfgData::InitBossFirstKilledTable(this);
    CfgData::InitUltimateChallengeCfg(this);
    CfgData::InitCachetCfg(this);
    CfgData::InitBossKilledReward(this);
    CfgData::InitSystemOpenReward(this);
    CfgData::InitSpecialMap(this);
    CfgData::InitTestServerReward(this);
    CfgData::InitEquipBlessTable(this);
    CfgData::InitGuWuCfgMap(this);
    CfgData::InitFunctionOpenMailMap(this);
    CfgData::InitXianYaoTaskTable(this);
    CfgData::InitShenWeiTaskTable(this);
    CfgData::InitMiniClientTable(this);
    CfgData::InitWuHunShopTable(this);
    CfgData::InitWuHunItemTable(this);
    CfgData::InitWuHunCreateTable(this);
    CfgData::InitBossDistribution(this);
    CfgData::InitSpecialBossMapCfgMap(this);
    CfgData::InitDuiHuanLimitTable(this);
    CfgData::InitSuperTeHuiTable(this);
    CfgData::InitJewelPavilionTable(this);
    CfgData::InitGoblinTable(this);
    CfgData::InitShouHuRefining(this);
    CfgData::InitWingEquipPolish(this);
    CfgData::InitGuiGuDaoRenTable(this);
    CfgData::InitShiZhuangTable(this);
    CfgData::InitShiZhuangLevelTable(this);
    CfgData::InitMonthlyChouJiangTable(this);
    CfgData::InitActDropTable(this);
    CfgData::InitSevenTaskTable(this);
    CQiFuTable::InitQiFuTable(&this->m_QiFuTable);
    VipTable::InitVipTable(&this->m_VipTable);
    CfgData::InitOutLinkFestivalTable(this);
    CfgData::InitFestivalActivityTable(this);
    CfgData::InitYYDaTing(this);
    CfgData::InitYYVip(this);
    CfgData::InitSouGouDaTing(this);
    CfgData::InitXunLeiTable(this);
    CfgData::InitCycleTowerTable(this);
    CfgData::InitPlatformDaTingMap(this);
    CfgData::InitMiniClientMap(this);
    CfgData::InitPlatformVipMap(this);
    CfgData::InitShangRenCfgMap(this);
    CfgData::InitSelectItemCfgMap(this);
    CfgData::InitGuardPrivilegeTable(this);
    CfgData::InitPlatformReward(this);
    CfgData::InitDiligenceCfgMap(this);
    CfgData::InitFamilyDonateTable(this);
    CfgData::InitFamilyMedalTable(this);
    CfgData::InitHoeTable(this);
    CfgData::InitMingGeTable(this);
    CfgData::InitCrossTowerCfgMap(this);
    CfgData::InitKunLingTable(this);
    CfgData::InitLaDaShiHuiYuan(this);
    CfgData::InitFlopTable(this);
    CfgData::InitMoFuTable(this);
    CfgData::InitChristmasDuiHuanTable(this);
    CfgData::InitBaoKuFuBen(this);
    CfgData::InitChargeDungeon(this);
    CfgData::InitShenYaoPosTable(this);
    CfgData::InitShenYaoPosSuitTable(this);
    CfgData::InitTongTianChiRanTable(this);
    CfgData::InitXinMoTable(this);
    CfgData::InitGameTable(this);
    CfgData::InitRefreshMonsterCfgListMap(this);
    CfgData::InitLittleHelperCfg(this);
    CfgData::InitShiZhuLevelUp(this);
    CfgData::InitNationalDayTask(this);
    CfgData::InitRongHeCfg(this);
    this->m_MapMonsterId = 100000;
    this->m_debug = debug;
    return 1;
}

void CfgData::reload(CfgData *const this)
{
    ItemEffectManager *v1;

    CfgData::fetchItem(this, 1);
    v1 = Answer::Singleton<ItemEffectManager>::instance();
    ItemEffectManager::init(v1);
}

// ==================== Getter 方法示例 ====================

int32_t CfgData::GetMapMonsterId(CfgData *const this)
{
    return ++this->m_MapMonsterId;
}

CfgActivity *CfgData::getActivity(CfgData *const this, int32_t id)
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

const CfgMapEventList *CfgData::getActivityEvents(CfgData *const this, int32_t activity_id, MapId_t nMapId)
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

CfgActivityMonster *CfgData::getActivityMonster(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_activityMonsters.find(ida);
    if (it != this->m_activityMonsters.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgActivityNpc *CfgData::getActivityNpc(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_activityNpcs.find(ida);
    if (it != this->m_activityNpcs.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgActivityPlant *CfgData::getActivityPlant(CfgData *const this, int32_t Id)
{
    int32_t Ida = Id;
    auto it = this->m_activityPlants.find(Ida);
    if (it != this->m_activityPlants.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgActivityTrap *CfgData::getActivityTrap(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_activityTraps.find(ida);
    if (it != this->m_activityTraps.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgBuff *CfgData::getBuff(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_buffs.find(ida);
    if (it != this->m_buffs.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgDungeon *CfgData::getDungeon(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_dungeons.find(ida);
    if (it != this->m_dungeons.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgDungeon *CfgData::GetUpTowerDungeon(const CfgData *const this, int32_t nFloor)
{
    int32_t nFloora = nFloor;
    auto it = this->m_mUpTowerDungeon.find(nFloora);
    if (it != this->m_mUpTowerDungeon.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgMapEventList *CfgData::getDungeonEvent(CfgData *const this, int32_t dungonid)
{
    int32_t dungonida = dungonid;
    auto it = this->m_dungeonEvents.find(dungonida);
    if (it != this->m_dungeonEvents.end())
    {
        return &it->second;
    }
    return &this->m_emptyEvents;
}

CfgDungeonMonster *CfgData::getDungeonMonster(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_dungeonMonsters.find(ida);
    if (it != this->m_dungeonMonsters.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgDungeonPlant *CfgData::getDungeonPlant(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_dungeonPlants.find(ida);
    if (it != this->m_dungeonPlants.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgDungeonTrap *CfgData::getDungeonTrap(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_dungeonTraps.find(ida);
    if (it != this->m_dungeonTraps.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgDungeonNpc *CfgData::getDungeonNpc(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_dungeonNpcs.find(ida);
    if (it != this->m_dungeonNpcs.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgEquip *CfgData::getEquip(const CfgData *const this, int32_t id)
{
    return CfgEquipTable::GetEquip(&this->m_cfgEquip, id);
}

CfgChrShop *CfgData::getChrShop(CfgData *const this, int32_t Index)
{
    int32_t Indexa = Index;
    auto it = this->m_chrShops.find(Indexa);
    if (it != this->m_chrShops.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgItemTable *CfgData::getAllItem(CfgItemTable *__return_ptr retstr, CfgData *const this)
{
    Answer::RwLockRdGuard lock;
    
    std::map<int, CfgItem *>::map(retstr);
    Answer::RwLockRdGuard::RwLockRdGuard(&lock, &this->m_itemsLock);
    std::map<int, CfgItem *>::operator=(retstr, &this->m_items);
    Answer::RwLockRdGuard::~RwLockRdGuard(&lock);
    return retstr;
}

CfgItem *CfgData::getItem(CfgData *const this, int32_t id)
{
    if (id <= 0)
        return nullptr;
    
    int32_t ida = id;
    Answer::RwLockRdGuard lock;
    Answer::RwLockRdGuard::RwLockRdGuard(&lock, &this->m_itemsLock);
    
    auto it = this->m_items.find(ida);
    if (it != this->m_items.end())
    {
        CfgItem *second = it->second;
        Answer::RwLockRdGuard::~RwLockRdGuard(&lock);
        return second;
    }
    
    Answer::RwLockRdGuard::~RwLockRdGuard(&lock);
    return nullptr;
}

CfgItemGiftVector *CfgData::getItemGift(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    Answer::RwLockRdGuard lock;
    Answer::RwLockRdGuard::RwLockRdGuard(&lock, &this->m_itemGiftsLock);
    
    auto it = this->m_itemGifts.find(ida);
    if (it != this->m_itemGifts.end())
    {
        CfgItemGiftVector *second = it->second;
        Answer::RwLockRdGuard::~RwLockRdGuard(&lock);
        return second;
    }
    
    Answer::RwLockRdGuard::~RwLockRdGuard(&lock);
    return nullptr;
}

CfgItemGiftRandomVector *CfgData::getItemGiftRandom(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    Answer::RwLockRdGuard lock;
    Answer::RwLockRdGuard::RwLockRdGuard(&lock, &this->m_itemGiftRandomsLock);
    
    auto it = this->m_itemGiftRandoms.find(ida);
    if (it != this->m_itemGiftRandoms.end())
    {
        CfgItemGiftRandomVector *second = it->second;
        Answer::RwLockRdGuard::~RwLockRdGuard(&lock);
        return second;
    }
    
    Answer::RwLockRdGuard::~RwLockRdGuard(&lock);
    return nullptr;
}

CfgJob *CfgData::getJob(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_jobs.find(ida);
    if (it != this->m_jobs.end())
    {
        return &it->second;
    }
    return nullptr;
}

bool CfgData::getMovie(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_movie.find(ida);
    return it != this->m_movie.end();
}

CfgMap *CfgData::getMap(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_maps.find(ida);
    if (it != this->m_maps.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMapMonsterVector *CfgData::getMonstersOnMap(CfgData *const this, int32_t mapid)
{
    int32_t mapida = mapid;
    auto it = this->m_mapMonsters.find(mapida);
    if (it != this->m_mapMonsters.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMapMonster *CfgData::GetMapMonsterInfo(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_CfgMapMonsters.find(ida);
    if (it != this->m_CfgMapMonsters.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMapPlantVector *CfgData::getPlantOnMap(CfgData *const this, int32_t mapid)
{
    int32_t mapida = mapid;
    auto it = this->m_mapPlants.find(mapida);
    if (it != this->m_mapPlants.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMapPlant *CfgData::GetMapPlantInfo(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_mMapPlants.find(ida);
    if (it != this->m_mMapPlants.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMapRegion *CfgData::getMapRegion(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_mapRegions.find(ida);
    if (it != this->m_mapRegions.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMapRegionVector *CfgData::getRegionOnMap(CfgData *const this, int32_t mapid)
{
    int32_t mapida = mapid;
    auto it = this->m_mapRegionsByMapId.find(mapida);
    if (it != this->m_mapRegionsByMapId.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMonster *CfgData::getMonster(CfgData *const this, int32_t mid)
{
    int32_t mida = mid;
    auto it = this->m_monsters.find(mida);
    if (it != this->m_monsters.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMonsterAI *CfgData::GetMonsterAI(CfgData *const this, int32_t nId)
{
    int32_t nIda = nId;
    auto it = this->m_mMonsterAI.find(nIda);
    if (it != this->m_mMonsterAI.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMonsterAdjust *CfgData::GetMonsterAdjust(CfgData *const this, int32_t mid, int32_t adj_level, bool bAutoLow)
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

bool CfgData::isMonsterBroadcast(CfgData *const this, int32_t mid)
{
    int32_t mida = mid;
    auto it = std::find(this->m_monsterBroadcasts.begin(), this->m_monsterBroadcasts.end(), mida);
    return it != this->m_monsterBroadcasts.end();
}

CfgDungeonReward *CfgData::getDungeonReward(CfgData *const this, int32_t dungeonID)
{
    int32_t dungeonIDa = dungeonID;
    auto it = this->m_dungeonReward.find(dungeonIDa);
    if (it != this->m_dungeonReward.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgDungeonDrop *CfgData::randDungeonDrop(CfgData *const this, int32_t dungeonID)
{
    int32_t dungeonIDa = dungeonID;
    auto it = this->m_dungeonDropGroup.find(dungeonIDa);
    if (it == this->m_dungeonDropGroup.end())
    {
        return nullptr;
    }
    return CfgDungeonDropGroup::RandDrop(&it->second);
}

CfgMonsterDropGroupVector *CfgData::getMonsterDropGroup(CfgData *const this, int32_t group_id)
{
    int32_t group_ida = group_id;
    auto it = this->m_monsterDropGroups.find(group_ida);
    if (it != this->m_monsterDropGroups.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMonsterGroupDropVector *CfgData::getMonsterGroupDrop(CfgData *const this, int32_t mid)
{
    int32_t mida = mid;
    auto it = this->m_monsterGroupDrops.find(mida);
    if (it != this->m_monsterGroupDrops.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgMonsterTaskDropVector *CfgData::getMonsterTaskDrop(CfgData *const this, int32_t mid)
{
    int32_t mida = mid;
    auto it = this->m_monsterTaskDrops.find(mida);
    if (it != this->m_monsterTaskDrops.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgNpc *CfgData::getNpc(CfgData *const this, int32_t npcid)
{
    int32_t npcida = npcid;
    auto it = this->m_npcs.find(npcida);
    if (it != this->m_npcs.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgNpcAirport *CfgData::getNpcAirport(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_npcAirports.find(ida);
    if (it != this->m_npcAirports.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgPlant *CfgData::getPlant(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_plants.find(ida);
    if (it != this->m_plants.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgTask *CfgData::getTask(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_tasks.find(ida);
    if (it != this->m_tasks.end())
    {
        return &it->second;
    }
    return nullptr;
}

int8_t CfgData::GetTaskType(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_tasks.find(ida);
    if (it != this->m_tasks.end())
    {
        return it->second.type;
    }
    return 0;
}

CfgTrap *CfgData::getTrap(CfgData *const this, int32_t id)
{
    int32_t ida = id;
    auto it = this->m_traps.find(ida);
    if (it != this->m_traps.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgLevelExp *CfgData::getUpgradeExp(CfgData *const this, int32_t level)
{
    int32_t levela = level;
    auto it = this->m_levelExps.find(levela);
    if (it != this->m_levelExps.end())
    {
        return &it->second;
    }
    return nullptr;
}

int64_t CfgData::getNeedLevelExp(CfgData *const this, int32_t level)
{
    int32_t levela = level;
    auto it = this->m_levelExps.find(levela);
    if (it != this->m_levelExps.end())
    {
        return it->second.upgrade_exp;
    }
    return INT64_MAX;
}

int64_t CfgData::getMaxExp(CfgData *const this, int32_t level)
{
    int32_t levela = level;
    auto it = this->m_levelExps.find(levela);
    if (it != this->m_levelExps.end())
    {
        return it->second.max_exp;
    }
    return INT64_MAX;
}

int32_t CfgData::GetPetExp(CfgData *const this, int32_t level)
{
    int32_t levela = level;
    auto it = this->m_levelExps.find(levela);
    if (it != this->m_levelExps.end())
    {
        return it->second.pet_exp;
    }
    return INT32_MAX;
}

int32_t CfgData::GetTalentPoint(CfgData *const this, int32_t level)
{
    int32_t levela = level;
    auto it = this->m_levelExps.find(levela);
    if (it != this->m_levelExps.end())
    {
        return it->second.talent_point;
    }
    return 0;
}

int32_t CfgData::GetAttrPoint(CfgData *const this, int32_t level)
{
    int32_t levela = level;
    auto it = this->m_levelExps.find(levela);
    if (it != this->m_levelExps.end())
    {
        return it->second.attr_point;
    }
    return 0;
}

CfgLevelAttr *CfgData::getLevelAttr(CfgLevelAttr *__return_ptr retstr, CfgData *const this, int32_t job, int32_t level)
{
    int key = (job << 16) | level;
    auto it = this->m_levelAttrs.find(key);
    if (it != this->m_levelAttrs.end())
    {
        CfgLevelAttr::CfgLevelAttr(retstr, &it->second);
    }
    else
    {
        CfgLevelAttr levelAttr{};
        CfgLevelAttr::CfgLevelAttr(retstr, &levelAttr);
        CfgLevelAttr::~CfgLevelAttr(&levelAttr);
    }
    return retstr;
}

int32_t CfgData::getBaseJob(CfgData *const this, int32_t job)
{
    if (job == 2) return 2;
    if (job == 3) return 3;
    return (job == 1) ? 1 : 0;
}

// ==================== Init 方法 1-10 ====================

void CfgData::InitGroupIconTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/GroupIcon.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_GROUP_ICON_TABLE fail, please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgGroupIcon icon{};                
                icon.nId = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
                icon.bShowInCross = CDBCFile::Search_Posistion(&readFile, i, 18)->iValue;
                
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, 19);
                std::string::operator=(&icon.platfrom, v1->pString);
                
                auto *v2 = std::map<int, CfgGroupIcon>::operator[](&this->m_cfgGroupIcons, &icon.nId);
                CfgGroupIcon::operator=(v2, &icon);
                CfgGroupIcon::~CfgGroupIcon(&icon);
            }
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

bool CfgData::IsShowIcon(CfgData *const this, int32_t nIconId, const std::string *const platform)
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
    
    return !(std::operator!=<char>(&icon->platfrom, "-1") && std::operator!=<char>(platform, &icon->platfrom));
}

void CfgData::InitFamilyRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/FamilyTaskRing.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_FAMILY_TASK_RING failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                FamilyTaskReward stu{};
                
                int32_t nIndex = 0;
                stu.Index = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                stu.NeedCount = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&stu.Rewards, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                
                auto *v2 = std::map<int, FamilyTaskReward>::operator[](&this->m_FamilyTaskReward, &stu.Index);
                FamilyTaskReward::operator=(v2, &stu);
                FamilyTaskReward::~FamilyTaskReward(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

FamilyTaskReward *CfgData::GetFamilyReward(CfgData *const this, int32_t Index)
{
    int32_t Indexa = Index;
    auto it = this->m_FamilyTaskReward.find(Indexa);
    if (it != this->m_FamilyTaskReward.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitAppendAttrTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/AdditionalAttribute.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_APPEND_ATTR_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgAppendAttr stu{};
                
                int32_t nIndex = 0;
                stu.Id = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                stu.Job = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                
                for (int32_t j = 0; j <= 9; ++j)
                {
                    AddAttribute Attr;
                    AddAttribute::AddAttribute(&Attr);
                    Attr.m_nAddAttrType = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                    Attr.m_nAddAttrValue = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                    if (Attr.m_nAddAttrValue > 0)
                    {
                        std::list<AddAttribute>::push_back(&stu.AttrList, &Attr);
                    }
                }
                
                std::list<CfgAppendAttr>::push_back(&this->m_AppendAttrTable, &stu);
                CfgAppendAttr::~CfgAppendAttr(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitHuoYueDuTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    bool ret = CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/HuoYueDu.txt");
    
    if (!ret)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_HUO_YUE_DU_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgHuoYueDu stu;
                memset(&stu, 0, sizeof(stu));
                stu.Index = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                stu.Type = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                stu.Effect = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
                stu.Count = CDBCFile::Search_Posistion(&TabFile, i, 3)->iValue;
                stu.Gold = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
                stu.AddHuoYueDu = CDBCFile::Search_Posistion(&TabFile, i, 5)->iValue;
                stu.IsCanSec = CDBCFile::Search_Posistion(&TabFile, i, 6)->iValue > 0;
                
                auto *v1 = std::map<int, CfgHuoYueDu>::operator[](&this->m_HuoYueDuTable, &stu.Index);
                *v1 = stu;
            }
            
            CDBCFile TabFileReward;
            CDBCFile::CDBCFile(&TabFileReward, 0);
            ret = CDBCFile::OpenFromTXT(&TabFileReward, "./ServerConfig/Tables/HuoYueDuReward.txt");
            
            if (!ret)
            {
                Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_HUO_YUE_DU_REWARD_TABLE failed,please check!!!\n");
            }
            else
            {
                iBaseTableCount = CDBCFile::GetRecordsNum(&TabFileReward);
                iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFileReward);
                
                if (iBaseColumnCount > 0)
                {
                    for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)
                    {
                        CfgHuoYueDuReward stu_0{};
                        
                        stu_0.Id = CDBCFile::Search_Posistion(&TabFileReward, i_0, 0)->iValue;
                        stu_0.NeedHuoYueDu = CDBCFile::Search_Posistion(&TabFileReward, i_0, 1)->iValue;
                        
                        std::string strItems;
                        char v10;
                        std::allocator<char>::allocator(&v10);
                        const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFileReward, i_0, 2);
                        std::string::string(&strItems, v2->pString, &v10);
                        
                        MemChrBagVector __x;
                        CItemHelper::parseItemVectorString(&__x, &strItems);
                        std::vector<MemChrBag>::operator=(&stu_0.Items, &__x);
                        std::vector<MemChrBag>::~vector(&__x);
                        std::string::~string(&strItems);
                        std::allocator<char>::~allocator(&v10);
                        
                        char __k = stu_0.Id;
                        auto *v3 = std::map<signed char, CfgHuoYueDuReward>::operator[](&this->m_HuoYueDuRewardTable, &__k);
                        CfgHuoYueDuReward::operator=(v3, &stu_0);
                        CfgHuoYueDuReward::~CfgHuoYueDuReward(&stu_0);
                    }
                }
            }
            CDBCFile::~CDBCFile(&TabFileReward);
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgHuoYueDu *CfgData::GetHuoYueDuCfg(CfgData *const this, int32_t Index)
{
    int32_t Indexa = Index;
    auto it = this->m_HuoYueDuTable.find(Indexa);
    if (it != this->m_HuoYueDuTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgHuoYueDuReward *CfgData::GetHuoYueDuReward(CfgData *const this, int32_t Index)
{
    char __x = Index;
    auto it = this->m_HuoYueDuRewardTable.find(__x);
    if (it != this->m_HuoYueDuRewardTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitKaiFuHuoDongTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/NewServer.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_KAI_FU_HUO_DONG failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgKaiFuHuoDongData stu{};
                int32_t Index = 0;
                stu.Index = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                stu.Type = CDBCFile::Search_Posistion(&TabFile, i, ++Index)->iValue;
                stu.Level = CDBCFile::Search_Posistion(&TabFile, i, ++Index)->iValue;
                stu.StartDay = CDBCFile::Search_Posistion(&TabFile, i, ++Index)->iValue;
                stu.EndDay = CDBCFile::Search_Posistion(&TabFile, i, ++Index)->iValue;
                stu.Conditions = CDBCFile::Search_Posistion(&TabFile, i, ++Index)->iValue;
                ++Index;
                
                if (stu.Type == 17)
                {
                    const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, Index);
                    std::string::operator=(&stu.ItemString, v1->pString);
                }
                else
                {
                    std::string strItems;
                    char v9;
                    std::allocator<char>::allocator(&v9);
                    const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, Index);
                    std::string::string(&strItems, v2->pString, &v9);
                    
                    MemChrBagVector __x;
                    CItemHelper::parseItemVectorString(&__x, &strItems);
                    std::vector<MemChrBag>::operator=(&stu.ItemVector, &__x);
                    std::vector<MemChrBag>::~vector(&__x);
                    std::string::~string(&strItems);
                    std::allocator<char>::~allocator(&v9);
                }
                
                stu.PetAttr = CDBCFile::Search_Posistion(&TabFile, i, ++Index)->iValue;
                stu.GongGaoId = CDBCFile::Search_Posistion(&TabFile, i, ++Index)->iValue;
                stu.Limit = CDBCFile::Search_Posistion(&TabFile, i, ++Index)->iValue;
                stu.PetEffect = CDBCFile::Search_Posistion(&TabFile, i, ++Index)->iValue;
                ++Index;
                Index += 5;
                stu.Conditions1 = CDBCFile::Search_Posistion(&TabFile, i, Index++)->iValue;
                
                const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&TabFile, i, Index);
                std::string::operator=(&stu.FlopString, v3->pString);
                
                auto *v4 = std::map<int, CfgKaiFuHuoDongData>::operator[](&this->m_KaiFuHuoDongTable, &stu.Index);
                CfgKaiFuHuoDongData::operator=(v4, &stu);
                CfgKaiFuHuoDongData::~CfgKaiFuHuoDongData(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}
void CfgData::InitDropRecordTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/DropRecord.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_DROP_RECORD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                CfgDropRecord record;
                memset(&record, 0, sizeof(record));
                record.nIndex = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                record.nItemId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
                record.nItemClass = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
                record.nSpecial = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
                ++nIndex;
                
                auto *v1 = std::map<int, CfgDropRecord>::operator[](&this->m_cfgDropRecordTable, &record.nIndex);
                *v1 = record;
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

const CfgDropRecord *CfgData::GetDropRecord(const CfgData *const this, int32_t nIndex)
{
    int32_t nIndexa = nIndex;
    auto iter = this->m_cfgDropRecordTable.find(nIndexa);
    if (iter != this->m_cfgDropRecordTable.end())
    {
        return &iter->second;
    }
    return nullptr;
}

void CfgData::InitDungeonScoreTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/DungeonScore.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_DUNGEON_SCORE_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                CfgDungeonScore score;
                memset(&score, 0, sizeof(score));
                score.nDungeonId = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                score.nKillCount = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
                ++nIndex;
                
                std::string strItem;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItem, v1->pString, &v7);
                
                MemChrBag v2;
                CItemHelper::parseItemString(&v2, &strItem);
                score.Item = v2;
                
                std::string::~string(&strItem);
                std::allocator<char>::~allocator(&v7);
                ++nIndex;
                
                CfgDungeonScoreTable::AddDungeonScore(&this->m_cfgDungeonScoreTable, &score);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitDungeonSummon(CfgData *const this)
{
    CDBCFile DungeonTrapFile;
    CDBCFile::CDBCFile(&DungeonTrapFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&DungeonTrapFile, "./ServerConfig/Tables/cfg_dungeon_summon.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_DUNGEON_SUMMON_TABLE.txt failed,please check!!");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&DungeonTrapFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&DungeonTrapFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgDungeonSummon stu;
                CfgDungeonSummon::CfgDungeonSummon(&stu);
                
                int32_t nIndex = 0;
                stu.nDungeon = CDBCFile::Search_Posistion(&DungeonTrapFile, i, nIndex++)->iValue;
                stu.nIndex = CDBCFile::Search_Posistion(&DungeonTrapFile, i, nIndex++)->iValue;
                stu.nGold = CDBCFile::Search_Posistion(&DungeonTrapFile, i, nIndex++)->iValue;
                stu.nLimit = CDBCFile::Search_Posistion(&DungeonTrapFile, i, nIndex++)->iValue;
                
                std::string path;
                char v9;
                std::allocator<char>::allocator(&v9);
                std::string::string(&path, v1->pString, &v9);
                
                std::list<int> __x;
                CfgData::paraseInt32List(this, &__x, &path, 0, nullptr);
                std::list<int>::operator=(&stu.lMonsters, &__x);
                std::list<int>::~list(&__x);
                std::string::~string(&path);
                std::allocator<char>::~allocator(&v9);
                ++nIndex;
                
                std::string strItem;
                char v13;
                std::allocator<char>::allocator(&v13);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&DungeonTrapFile, i, nIndex);
                std::string::string(&strItem, v2->pString, &v13);
                
                ItemData v20;
                v20 = CItemHelper::parseItemDataString(&strItem);
                stu.ConstItem.m_nId = v20.m_nId;
                stu.ConstItem.m_nClass = v20.m_nClass;
                stu.ConstItem.m_nCount = v20.m_nCount;
                
                std::string::~string(&strItem);
                std::allocator<char>::~allocator(&v13);
                stu.FreeCount = CDBCFile::Search_Posistion(&DungeonTrapFile, i, ++nIndex)->iValue;
                ++nIndex;
                
                auto __k = std::make_pair(stu.nDungeon, stu.nIndex);
                auto *v3 = std::map<std::pair<int, int>, CfgDungeonSummon>::operator[](&this->m_cfgDungeonSummon, &__k);
                CfgDungeonSummon::operator=(v3, &stu);
                CfgDungeonSummon::~CfgDungeonSummon(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&DungeonTrapFile);
}

const CfgDungeonSummon *CfgData::GetDungeonSummon(const CfgData *const this, int32_t nDungeon, int32_t nIndex)
{
    auto __x = std::make_pair(nDungeon, nIndex);
    auto iter = this->m_cfgDungeonSummon.find(__x);
    if (iter != this->m_cfgDungeonSummon.end())
    {
        return &iter->second;
    }
    return nullptr;
}

void CfgData::InitSpecialTreasureMapRandTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/SpecialTreasureMap.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SPECIAL_TREASURE_MAP_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                SpecialTreasureMapRandCfg stu{};
                
                stu.nItemId = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                stu.nType = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                
                std::string strItems;
                char v9;
                std::allocator<char>::allocator(&v9);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 2);
                std::string::string(&strItems, v1->pString, &v9);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&stu.vGetItems, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v9);
                
                stu.nRate = CDBCFile::Search_Posistion(&TabFile, i, 3)->iValue;
                
                std::string strItem;
                char v11;
                std::allocator<char>::allocator(&v11);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, 4);
                std::string::string(&strItem, v2->pString, &v11);
                
                MemChrBag v3;
                CItemHelper::parseItemString(&v3, &strItem);
                stu.vShowItem = v3;
                
                std::string::~string(&strItem);
                std::allocator<char>::~allocator(&v11);
                stu.nGongGaoId = CDBCFile::Search_Posistion(&TabFile, i, 5)->iValue;
                
                std::list<SpecialTreasureMapRandCfg>::push_back(&this->m_SpecialTreasureMapRandCfgList, &stu);
                SpecialTreasureMapRandCfg::~SpecialTreasureMapRandCfg(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitMonsterAddAttrTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/MonstAddAttr.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MONSTER_ADD_ATTR failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t Mid = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                
                CfgMonsterAddAttr AddAttrs{};
                AddAttrs.WorldBossLevelMin = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                AddAttrs.WorldBossLevelMax = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
                
                for (int32_t j = 0; j <= 6; ++j)
                {
                    AttrAddon stu;
                    memset(&stu, 0, sizeof(stu));
                    stu.index = CDBCFile::Search_Posistion(&TabFile, i, 2 * j + 3)->iValue;
                    stu.addon = CDBCFile::Search_Posistion(&TabFile, i, 2 * (j + 2))->iValue;
                    std::vector<AttrAddon>::push_back(&AddAttrs.AttrVector, &stu);
                }
                
                auto *v1 = std::map<int, std::list<CfgMonsterAddAttr>>::operator[](&this->m_MonstAddAttrMap, &Mid);
                std::list<CfgMonsterAddAttr>::push_back(v1, &AddAttrs);
                CfgMonsterAddAttr::~CfgMonsterAddAttr(&AddAttrs);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

AttrAddonVector *CfgData::GetAddMonsterAttrs(AttrAddonVector *__return_ptr retstr, CfgData *const this, int32_t Mid, int32_t WorldLevel)
{
    int32_t Mida = Mid;
    auto it = this->m_MonstAddAttrMap.find(Mida);
    
    if (it != this->m_MonstAddAttrMap.end())
    {
        for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
        {
            if (iter->WorldBossLevelMin <= WorldLevel && iter->WorldBossLevelMax >= WorldLevel)
            {
                std::vector<AttrAddon>::vector(retstr, &iter->AttrVector);
                return retstr;
            }
        }
    }
    
    AttrAddonVector AttrVector;
    std::vector<AttrAddon>::vector(&AttrVector);
    std::vector<AttrAddon>::vector(retstr, &AttrVector);
    std::vector<AttrAddon>::~vector(&AttrVector);
    return retstr;
}

void CfgData::InitDaZheQuanTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/ItemDiscount.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_DA_ZHE_QUAN_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                DaZheQuan stu{};
                
                stu.Index = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                stu.CurrencyType = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                stu.CurrencyValues = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 3);
                std::string::string(&strItems, v1->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&stu.Items, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                
                auto *v2 = std::map<int, DaZheQuan>::operator[](&this->m_DaZheQuanMap, &stu.Index);
                DaZheQuan::operator=(v2, &stu);
                DaZheQuan::~DaZheQuan(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

DaZheQuan *CfgData::GetDaZheQuanCfg(CfgData *const this, int32_t Index)
{
    int32_t Indexa = Index;
    auto it = this->m_DaZheQuanMap.find(Indexa);
    if (it != this->m_DaZheQuanMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::Init360RewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/GameLobby.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_360_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                Wan360Reward stu{};
                
                stu.Index = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 1);
                std::string::string(&strItems, v1->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&stu.Items, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                
                auto *v2 = std::map<int, Wan360Reward>::operator[](&this->m_Wan360RewardMap, &stu.Index);
                Wan360Reward::operator=(v2, &stu);
                Wan360Reward::~Wan360Reward(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::Init360RewardTypeTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/GameLobbyType.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_360_REWARD_TYPE_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                std::string platform;
                char v5;
                std::allocator<char>::allocator(&v5);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 0);
                std::string::string(&platform, v1->pString, &v5);
                std::allocator<char>::~allocator(&v5);
                
                int32_t icon = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                auto *v2 = std::map<std::string, int>::operator[](&this->m_Wan360RewardTypeMap, &platform);
                *v2 = icon;
                std::string::~string(&platform);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

Wan360Reward *CfgData::Get360RewardCfg(CfgData *const this, int32_t Index)
{
    int32_t Indexa = Index;
    auto it = this->m_Wan360RewardMap.find(Indexa);
    if (it != this->m_Wan360RewardMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CfgData::Get360RewardIcon(CfgData *const this, const std::string *const platform)
{
    return *std::map<std::string, int>::operator[](&this->m_Wan360RewardTypeMap, platform);
}

void CfgData::InitTotalChongZhiTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/ChongZhiHuiKui.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TOTAL_CHONG_ZHI_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount - 1; ++i)
            {
                CfgTotalChongZhi stu{};
                
                stu.Index = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                stu.NeedGold = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                
                std::string strItems;
                char v8;
                std::allocator<char>::allocator(&v8);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 2);
                std::string::string(&strItems, v1->pString, &v8);
                
                MemChrEquipBagVector __x;
                CfgData::parseEquipItemString(&__x, this, stu.Index, &strItems);
                std::vector<CfgEquipItem>::operator=(&stu.ItemVector, &__x);
                std::vector<CfgEquipItem>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v8);
                
                std::string v10;
                char v11;
                std::allocator<char>::allocator(&v11);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, 3);
                std::string::string(&v10, v2->pString, &v11);
                
                std::list<CfgGongGao> v9;
                CfgData::parseGongGaoString((CfgData *const)&v9, stu.Index, &v10);
                std::list<CfgGongGao>::operator=(&stu.GongGaoInfo, &v9);
                std::list<CfgGongGao>::~list(&v9);
                std::string::~string(&v10);
                std::allocator<char>::~allocator(&v11);
                
                stu.NewServerday = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
                
                auto *v3 = std::map<signed char, CfgTotalChongZhi>::operator[](&this->m_TotalChongZhiTable, &stu.Index);
                CfgTotalChongZhi::operator=(v3, &stu);
                CfgTotalChongZhi::~CfgTotalChongZhi(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgTotalChongZhi *CfgData::GetTotalChongZhiCfg(CfgData *const this, int8_t Index)
{
    int8_t Indexa[4] = {Index};
    auto it = this->m_TotalChongZhiTable.find(Indexa[0]);
    if (it != this->m_TotalChongZhiTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitEveryDayChongZhi(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/MeiRiChongZhi.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_EVERYDAY_CHONG_ZHI failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgEverydayChongZhi stu{};
                
                stu.Index = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                stu.Type = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                stu.NeedGold = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 3);
                std::string::string(&strItems, v1->pString, &v7);
                
                MemChrEquipBagVector __x;
                CfgData::parseEquipItemString(&__x, this, stu.Index, &strItems);
                std::vector<CfgEquipItem>::operator=(&stu.ItemVector, &__x);
                std::vector<CfgEquipItem>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                
                std::string v9;
                char v10;
                std::allocator<char>::allocator(&v10);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, 4);
                std::string::string(&v9, v2->pString, &v10);
                
                std::list<CfgGongGao> v8;
                CfgData::parseGongGaoString((CfgData *const)&v8, stu.Index, &v9);
                std::list<CfgGongGao>::operator=(&stu.GongGaoInfo, &v8);
                std::list<CfgGongGao>::~list(&v8);
                std::string::~string(&v9);
                std::allocator<char>::~allocator(&v10);
                
                std::list<CfgEverydayChongZhi>::push_back(&this->m_EveryDayChongZhiTable, &stu);
                CfgEverydayChongZhi::~CfgEverydayChongZhi(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgEverydayChongZhi *CfgData::GetEveryDayChongZhiCfg(CfgData *const this, int8_t nType, int8_t Index)
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

void CfgData::InitNewServerFavorable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    bool ret = CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/TeHui.txt");
    
    if (!ret)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_NEW_SERVER_FAVORABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                NewServerFavorable stu;
                NewServerFavorable::NewServerFavorable(&stu);
                
                stu.Index = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                
                std::string strItems;
                char v11;
                std::allocator<char>::allocator(&v11);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 1);
                std::string::string(&strItems, v1->pString, &v11);
                
                MemChrEquipBagVector __x;
                CfgData::parseEquipItemString(&__x, this, stu.Index, &strItems);
                std::vector<CfgEquipItem>::operator=(&stu.ItemVector, &__x);
                std::vector<CfgEquipItem>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v11);
                
                stu.NeedGold = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
                
                std::string v13;
                char v14;
                std::allocator<char>::allocator(&v14);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, 3);
                std::string::string(&v13, v2->pString, &v14);
                
                std::list<CfgGongGao> v12;
                CfgData::parseGongGaoString((CfgData *const)&v12, stu.Index, &v13);
                std::list<CfgGongGao>::operator=(&stu.GongGaoInfo, &v12);
                std::list<CfgGongGao>::~list(&v12);
                std::string::~string(&v13);
                std::allocator<char>::~allocator(&v14);
                
                stu.nLimitTime = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
                
                auto *v3 = std::map<signed char, NewServerFavorable>::operator[](&this->m_NewServerFavorable, &stu.Index);
                NewServerFavorable::operator=(v3, &stu);
                NewServerFavorable::~NewServerFavorable(&stu);
            }
            
            CDBCFile TabFile2;
            CDBCFile::CDBCFile(&TabFile2, 0);
            ret = CDBCFile::OpenFromTXT(&TabFile2, "./ServerConfig/Tables/SanChong.txt");
            
            if (!ret)
            {
                Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_THREE_PET_GIFT failed,please check!!!\n");
            }
            else
            {
                iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile2);
                iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile2);
                
                if (iBaseColumnCount > 0)
                {
                    for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)
                    {
                        NewServerFavorable stu;
                        NewServerFavorable::NewServerFavorable(&stu);
                        
                        this->m_ThreePetGift.Index = CDBCFile::Search_Posistion(&TabFile2, i_0, 0)->iValue;
                        
                        std::string v16;
                        char v17;
                        std::allocator<char>::allocator(&v17);
                        const CDBCFile::FIELD *v4 = CDBCFile::Search_Posistion(&TabFile2, i_0, 1);
                        std::string::string(&v16, v4->pString, &v17);
                        
                        MemChrEquipBagVector v15;
                        CfgData::parseEquipItemString(&v15, this, stu.Index, &v16);
                        std::vector<CfgEquipItem>::operator=(&this->m_ThreePetGift.ItemVector, &v15);
                        std::vector<CfgEquipItem>::~vector(&v15);
                        std::string::~string(&v16);
                        std::allocator<char>::~allocator(&v17);
                        
                        this->m_ThreePetGift.NeedGold = CDBCFile::Search_Posistion(&TabFile2, i_0, 2)->iValue;
                        
                        std::string v19;
                        char v20;
                        std::allocator<char>::allocator(&v20);
                        const CDBCFile::FIELD *v5 = CDBCFile::Search_Posistion(&TabFile2, i_0, 3);
                        std::string::string(&v19, v5->pString, &v20);
                        
                        std::list<CfgGongGao> v18;
                        CfgData::parseGongGaoString((CfgData *const)&v18, stu.Index, &v19);
                        std::list<CfgGongGao>::operator=(&this->m_ThreePetGift.GongGaoInfo, &v18);
                        std::list<CfgGongGao>::~list(&v18);
                        std::string::~string(&v19);
                        std::allocator<char>::~allocator(&v20);
                        
                        this->m_ThreePetGift.nLimitTime = CDBCFile::Search_Posistion(&TabFile2, i_0, 4)->iValue;
                        NewServerFavorable::~NewServerFavorable(&stu);
                    }
                }
            }
            CDBCFile::~CDBCFile(&TabFile2);
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

NewServerFavorable *CfgData::GetNewServerFavorable(CfgData *const this, int8_t Index)
{
    int8_t Indexa[4] = {Index};
    auto it = this->m_NewServerFavorable.find(Indexa[0]);
    if (it != this->m_NewServerFavorable.end())
    {
        return &it->second;
    }
    return nullptr;
}
void CfgData::InitShouChongLiBao(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/ShouChong.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SHOU_CHONG_LI_BAO failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgShouChong stu;
                CfgShouChong::CfgShouChong(&stu);
                
                stu.nIndex = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                stu.nGold = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                
                std::string strItems;
                char v6;
                std::allocator<char>::allocator(&v6);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 2);
                std::string::string(&strItems, v1->pString, &v6);
                
                MemChrEquipBagVector __x;
                CfgData::parseEquipItemString(&__x, this, 1, &strItems);
                std::vector<CfgEquipItem>::operator=(&stu.vReward, &__x);
                std::vector<CfgEquipItem>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v6);
                
                stu.nBroadcast = CDBCFile::Search_Posistion(&TabFile, i, 3)->iValue;
                
                CfgShouChongTable::Add(&this->m_cfgShouChongTable, &stu);
                CfgShouChong::~CfgShouChong(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitMoHuaHuanYiTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/MoHuaZhuanYi.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MO_HUA_ZHUAN_YI_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgEquipExchange stu;
                memset(&stu, 0, sizeof(stu));
                
                stu.Level = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                stu.Money = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                
                std::string strItem;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 2);
                std::string::string(&strItem, v1->pString, &v7);
                
                ItemData v12 = CItemHelper::parseItemDataString(&strItem);
                stu.CostItem.m_nId = v12.m_nId;
                stu.CostItem.m_nClass = v12.m_nClass;
                stu.CostItem.m_nCount = v12.m_nCount;
                
                std::string::~string(&strItem);
                std::allocator<char>::~allocator(&v7);
                
                auto *v2 = std::map<signed char, CfgEquipExchange>::operator[](&this->m_CfgEquipExchangeMap, &stu.Level);
                *v2 = stu;
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgEquipExchange *CfgData::GetEquipExchange(CfgData *const this, int8_t Level)
{
    int8_t Levela[4] = {Level};
    auto it = this->m_CfgEquipExchangeMap.find(Levela[0]);
    if (it != this->m_CfgEquipExchangeMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitZiYuanZhaoHuiTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/ZiYuanZhaoHui.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ZYZH_TABLE failed,please check!!");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                CfgSearchBack stu{};
                
                stu.nId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                stu.nType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                stu.nTimes = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                stu.nSubType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                stu.nOpenType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                stu.nMinLevel = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                stu.nMaxLevel = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                stu.nGoldCost = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&stu.vFreeReward, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                ++nIndex;
                
                std::string v9;
                char v10;
                std::allocator<char>::allocator(&v10);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
                std::string::string(&v9, v2->pString, &v10);
                
                MemChrBagVector v8;
                CItemHelper::parseItemVectorString(&v8, &v9);
                std::vector<MemChrBag>::operator=(&stu.vItemReward, &v8);
                std::vector<MemChrBag>::~vector(&v8);
                std::string::~string(&v9);
                std::allocator<char>::~allocator(&v10);
                ++nIndex;
                
                CfgSearchBackTable::AddSearchBack(&this->m_cfgSearchBackTable, &stu);
                CfgSearchBack::~CfgSearchBack(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitPlantEventTable(CfgData *const this)
{
    CDBCFile ItemGiftFile;
    CDBCFile::CDBCFile(&ItemGiftFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&ItemGiftFile, "./ServerConfig/Tables/PlantEvent.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_PLANT_EVENT_TABLE failed,please check!!");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&ItemGiftFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&ItemGiftFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgPlantEventEffect Event;
                CfgPlantEventEffect::CfgPlantEventEffect(&Event);
                
                Event.EventId = CDBCFile::Search_Posistion(&ItemGiftFile, i, 0)->iValue;
                Event.EventType = CDBCFile::Search_Posistion(&ItemGiftFile, i, 1)->iValue;
                
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&ItemGiftFile, i, 2);
                std::string::operator=(&Event.EventEffect, v1->pString);
                Event.GongGaoId = CDBCFile::Search_Posistion(&ItemGiftFile, i, 3)->iValue;
                
                auto *v2 = std::map<int, CfgPlantEventEffect>::operator[](&this->m_PlantEventMap, &Event.EventId);
                CfgPlantEventEffect::operator=(v2, &Event);
                CfgPlantEventEffect::~CfgPlantEventEffect(&Event);
            }
        }
    }
    CDBCFile::~CDBCFile(&ItemGiftFile);
}

CfgPlantEventEffect *CfgData::GetPlantEvent(CfgData *const this, int32_t EventId)
{
    int32_t EventIda = EventId;
    auto it = this->m_PlantEventMap.find(EventIda);
    if (it != this->m_PlantEventMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitFunctionOpenTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_unlock.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_FUNCTION_OPEN_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                FunctionOpenCfg stu;
                memset(&stu, 0, sizeof(stu));
                
                stu.Type = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
                stu.TaskId = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                stu.Level = CDBCFile::Search_Posistion(&TabFile, i, 3)->iValue;
                
                auto *v1 = std::map<int, FunctionOpenCfg>::operator[](&this->m_FunctionOpenCfg, &stu.Type);
                *v1 = stu;
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

FunctionOpenCfg *CfgData::GetOpenFunctionCfg(CfgData *const this, int32_t FunctionId)
{
    int32_t FunctionIda = FunctionId;
    auto it = this->m_FunctionOpenCfg.find(FunctionIda);
    if (it != this->m_FunctionOpenCfg.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitVipCardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/Privilege.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_VIP_CARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                VipCardCfg VipCard{};
                
                int32_t nIndex = 0;
                VipCard.VipCardId = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                VipCard.AddVipTime = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                VipCard.NeedGold = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                
                std::string v8;
                char v9;
                std::allocator<char>::allocator(&v9);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&v8, v1->pString, &v9);
                
                std::list<AddAttribute> __x;
                CfgData::parseAddAttribues((CfgData *const)&__x, &v8, i, nullptr);
                std::list<AddAttribute>::operator=(&VipCard.AddAttr, &__x);
                std::list<AddAttribute>::~list(&__x);
                std::string::~string(&v8);
                std::allocator<char>::~allocator(&v9);
                
                VipCard.VipLevel = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
                ++nIndex;
                nIndex += 2;
                VipCard.ReNeedGold = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                VipCard.Money = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                VipCard.TianShuJinHua = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                VipCard.WeiWang = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                
                int __k = VipCard.VipCardId;
                auto *v2 = std::map<int, VipCardCfg>::operator[](&this->m_VipCardTable, &__k);
                VipCardCfg::operator=(v2, &VipCard);
                VipCardCfg::~VipCardCfg(&VipCard);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

VipCardCfg *CfgData::GetVipCardCfg(CfgData *const this, int8_t VipType)
{
    int __x = VipType;
    auto it = this->m_VipCardTable.find(__x);
    if (it != this->m_VipCardTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitOffLineExpTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/OfflineExp.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_OFF_LINE_EXP_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgOffLineExp reward;
                memset(&reward, 0, sizeof(reward));
                
                reward.Level = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                reward.MinuteExp = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
                reward.TwoTimes = CDBCFile::Search_Posistion(&TabFile, i, 3)->iValue;
                reward.ThreeTimes = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
                reward.BaseExp = CDBCFile::Search_Posistion(&TabFile, i, 5)->iValue;
                
                auto *v1 = std::map<int, CfgOffLineExp>::operator[](&this->m_OffLineExpTable, &reward.Level);
                *v1 = reward;
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgOffLineExp *CfgData::GetOfflineExpCfg(CfgData *const this, int32_t Level)
{
    int32_t Levela = Level;
    auto it = this->m_OffLineExpTable.lower_bound(Levela);
    if (it != this->m_OffLineExpTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitWeekOnlineReward(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/WeekOnlineReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WEEK_ONLINE_REWARD failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgWeekOnlineReward reward;
                memset(&reward, 0, sizeof(reward));
                
                reward.Week = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                reward.Cash = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                reward.UpperLimit = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
                
                auto *v1 = std::map<int, CfgWeekOnlineReward>::operator[](&this->m_WeekOnlineReward, &reward.Week);
                *v1 = reward;
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgWeekOnlineReward *CfgData::GetWeekOnlineReward(CfgData *const this, int32_t Week)
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

void CfgData::InitLevelGift(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_level_gift.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_LEVEL_GIFT failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgLevelGift reward{};
                
                reward.Index = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                reward.Level = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 2);
                std::string::string(&strItems, v1->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&reward.ItemVector, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                
                int __k = reward.Index;
                auto *v2 = std::map<int, CfgLevelGift>::operator[](&this->m_LevelGift, &__k);
                CfgLevelGift::operator=(v2, &reward);
                CfgLevelGift::~CfgLevelGift(&reward);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgLevelGift *CfgData::GetLevelGiftCfg(CfgData *const this, int8_t Index)
{
    int __x = Index;
    auto it = this->m_LevelGift.find(__x);
    if (it != this->m_LevelGift.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitSevenLoginReward(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_seven_login_reward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SEVEN_LOGIN_REWARD failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgSevenLoginRewrad reward{};
                
                reward.Day = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 1);
                std::string::string(&strItems, v1->pString, &v7);
                
                MemChrEquipBagVector __x;
                CfgData::parseEquipItemString(&__x, this, reward.Day, &strItems);
                std::vector<CfgEquipItem>::operator=(&reward.ItemVector, &__x);
                std::vector<CfgEquipItem>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                
                auto *v2 = std::map<signed char, CfgSevenLoginRewrad>::operator[](&this->m_SevenLoginReward, &reward.Day);
                CfgSevenLoginRewrad::operator=(v2, &reward);
                CfgSevenLoginRewrad::~CfgSevenLoginRewrad(&reward);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgSevenLoginRewrad *CfgData::GetSevenLoginRewardCfg(CfgData *const this, int8_t Day)
{
    int8_t Daya[4] = {Day};
    auto it = this->m_SevenLoginReward.find(Daya[0]);
    if (it != this->m_SevenLoginReward.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitOnLimeReward(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_online_reward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ONLINE_REWARD failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgOnlineReward reward{};
                
                reward.Id = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                reward.NeedTime = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 2);
                std::string::string(&strItems, v1->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&reward.ItemVector, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                
                auto *v2 = std::map<signed char, CfgOnlineReward>::operator[](&this->m_OnLineReward, &reward.Id);
                CfgOnlineReward::operator=(v2, &reward);
                CfgOnlineReward::~CfgOnlineReward(&reward);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgOnlineReward *CfgData::GetOnlineRewardCfg(CfgData *const this, int8_t id)
{
    int8_t ida[4] = {id};
    auto it = this->m_OnLineReward.find(ida[0]);
    if (it != this->m_OnLineReward.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::fetchSignReward(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_sign_reward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SIGIN_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgSignReward reward{};
                
                reward.count = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 1);
                std::string::string(&strItems, v1->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&reward.ItemVector, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                
                char __k = reward.count;
                auto *v2 = std::map<signed char, CfgSignReward>::operator[](&this->m_signReward, &__k);
                CfgSignReward::operator=(v2, &reward);
                CfgSignReward::~CfgSignReward(&reward);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

MemChrBagVector *CfgData::GetSignReward(MemChrBagVector *__return_ptr retstr, CfgData *const this, int8_t SiginCount)
{
    int8_t SiginCounta[4] = {SiginCount};
    std::vector<MemChrBag>::vector(retstr);
    
    auto it = this->m_signReward.find(SiginCounta[0]);
    if (it != this->m_signReward.end())
    {
        std::vector<MemChrBag>::operator=(retstr, &it->second.ItemVector);
    }
    return retstr;
}
void CfgData::InitQQZoneRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/QQZone.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_QQZONE_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                CfgQQGift gift;
                CfgQQGift::CfgQQGift(&gift);
                
                gift.nIndex = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                gift.nType = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                ++nIndex;
                
                std::string strItems;
                char v6;
                std::allocator<char>::allocator(&v6);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v6);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&gift.vReward, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v6);
                
                gift.nCondition = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
                ++nIndex;
                
                CfgTencentTable::AddQQZoneGift(&this->m_cfgTencentTable, &gift);
                CfgQQGift::~CfgQQGift(&gift);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitQQGameRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/QQGameDaTing.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_QQGAME_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                CfgQQGift gift;
                CfgQQGift::CfgQQGift(&gift);
                
                gift.nIndex = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                gift.nType = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                ++nIndex;
                
                std::string strItems;
                char v6;
                std::allocator<char>::allocator(&v6);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v6);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&gift.vReward, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v6);
                
                gift.nCondition = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
                ++nIndex;
                
                CfgTencentTable::AddQQGameGift(&this->m_cfgTencentTable, &gift);
                CfgQQGift::~CfgQQGift(&gift);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitYellowRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/YellowReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_YELLOW_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                int32_t nId = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                ++nIndex;
                
                std::string strItems;
                char v5;
                std::allocator<char>::allocator(&v5);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v5);
                
                MemChrBagVector vReward;
                CItemHelper::parseItemVectorString(&vReward, &strItems);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v5);
                ++nIndex;
                
                CfgTencentTable::SetYellowNewerGift(&this->m_cfgTencentTable, &vReward);
                std::vector<MemChrBag>::~vector(&vReward);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitTencentSevenDayLoginTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/TencentSevenDayLogin.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TENCENT_SEVEN_DAY_LOGIN_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                CfgTencentSevenDayLogin stu;
                CfgTencentSevenDayLogin::CfgTencentSevenDayLogin(&stu);
                
                stu.nDays = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                
                std::string strItems;
                char v6;
                std::allocator<char>::allocator(&v6);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v6);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&stu.vReward, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v6);
                ++nIndex;
                
                CfgTencentTable::AddSevenDayLogin(&this->m_cfgTencentTable, &stu);
                CfgTencentSevenDayLogin::~CfgTencentSevenDayLogin(&stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitYellowDailyRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/YellowEverydayReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_YELLOW_DAILY_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                CfgTencentGift daily;
                CfgTencentGift year;
                CfgTencentGift::CfgTencentGift(&daily);
                CfgTencentGift::CfgTencentGift(&year);
                
                int32_t nId = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                int32_t nLevel = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                
                std::string strItems;
                char v8;
                std::allocator<char>::allocator(&v8);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v8);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&daily.vRewards, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v8);
                ++nIndex;
                
                std::string v10;
                char v11;
                std::allocator<char>::allocator(&v11);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&v10, v2->pString, &v11);
                
                MemChrBagVector v9;
                CItemHelper::parseItemVectorString(&v9, &v10);
                std::vector<MemChrBag>::operator=(&year.vRewards, &v9);
                std::vector<MemChrBag>::~vector(&v9);
                std::string::~string(&v10);
                std::allocator<char>::~allocator(&v11);
                ++nIndex;
                
                daily.nLevel = nLevel;
                year.nLevel = nLevel;
                
                CfgTencentTable::AddYellowDailyGift(&this->m_cfgTencentTable, &daily);
                CfgTencentTable::AddYellowYearGift(&this->m_cfgTencentTable, &year);
                
                CfgTencentGift::~CfgTencentGift(&year);
                CfgTencentGift::~CfgTencentGift(&daily);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitYellowLevelRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/YellowLevelReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_YELLOW_LEVEL_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                CfgTencentGift gift;
                CfgTencentGift::CfgTencentGift(&gift);
                
                int32_t nId = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                gift.nLevel = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&gift.vRewards, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                ++nIndex;
                
                std::string v9;
                char v10;
                std::allocator<char>::allocator(&v10);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&v9, v2->pString, &v10);
                
                MemChrBagVector v8;
                CItemHelper::parseItemVectorString(&v8, &v9);
                std::vector<MemChrBag>::operator=(&gift.vVipRewards, &v8);
                std::vector<MemChrBag>::~vector(&v8);
                std::string::~string(&v9);
                std::allocator<char>::~allocator(&v10);
                ++nIndex;
                
                CfgTencentTable::AddYellowLevelGift(&this->m_cfgTencentTable, nId, &gift);
                CfgTencentGift::~CfgTencentGift(&gift);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitBlueRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/BlueReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BLUE_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                int32_t nId = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                ++nIndex;
                
                std::string strItems;
                char v5;
                std::allocator<char>::allocator(&v5);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v5);
                
                MemChrBagVector vReward;
                CItemHelper::parseItemVectorString(&vReward, &strItems);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v5);
                ++nIndex;
                
                CfgTencentTable::SetBlueNewerGift(&this->m_cfgTencentTable, &vReward);
                std::vector<MemChrBag>::~vector(&vReward);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitBlueDailyRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/BlueEverydayReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BLUE_DAILY_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                CfgTencentGift daily;
                CfgTencentGift year;
                CfgTencentGift high;
                CfgTencentGift::CfgTencentGift(&daily);
                CfgTencentGift::CfgTencentGift(&year);
                CfgTencentGift::CfgTencentGift(&high);
                
                int32_t nId = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                int32_t nLevel = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                
                std::string strItems;
                char v10;
                std::allocator<char>::allocator(&v10);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v10);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&daily.vRewards, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v10);
                ++nIndex;
                
                std::string v12;
                char v13;
                std::allocator<char>::allocator(&v13);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&v12, v2->pString, &v13);
                
                MemChrBagVector v11;
                CItemHelper::parseItemVectorString(&v11, &v12);
                std::vector<MemChrBag>::operator=(&year.vRewards, &v11);
                std::vector<MemChrBag>::~vector(&v11);
                std::string::~string(&v12);
                std::allocator<char>::~allocator(&v13);
                ++nIndex;
                
                std::string v15;
                char v16;
                std::allocator<char>::allocator(&v16);
                const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&v15, v3->pString, &v16);
                
                MemChrBagVector v14;
                CItemHelper::parseItemVectorString(&v14, &v15);
                std::vector<MemChrBag>::operator=(&high.vRewards, &v14);
                std::vector<MemChrBag>::~vector(&v14);
                std::string::~string(&v15);
                std::allocator<char>::~allocator(&v16);
                ++nIndex;
                
                daily.nLevel = nLevel;
                year.nLevel = nLevel;
                high.nLevel = nLevel;
                
                CfgTencentTable::AddBlueDailyGift(&this->m_cfgTencentTable, &daily);
                CfgTencentTable::AddBlueYearGift(&this->m_cfgTencentTable, &year);
                CfgTencentTable::AddBlueHighGift(&this->m_cfgTencentTable, &high);
                
                CfgTencentGift::~CfgTencentGift(&high);
                CfgTencentGift::~CfgTencentGift(&year);
                CfgTencentGift::~CfgTencentGift(&daily);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitBlueLevelRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/BlueLevelReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BLUE_LEVEL_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                CfgTencentGift gift;
                CfgTencentGift::CfgTencentGift(&gift);
                
                int32_t nId = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                gift.nLevel = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                
                std::string strItems;
                char v6;
                std::allocator<char>::allocator(&v6);
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v1->pString, &v6);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&gift.vRewards, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v6);
                ++nIndex;
                
                CfgTencentTable::AddBlueLevelGift(&this->m_cfgTencentTable, nId, &gift);
                CfgTencentGift::~CfgTencentGift(&gift);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitTGPRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/TGPReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TGP_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::operator=(&gift, v1->pString);
                ++nIndex;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v2->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&gift.vReward, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                ++nIndex;
                
                CfgTencentTable::AddTGPNewerGift(&this->m_cfgTencentTable, &gift);
                CfgTGPGift::~CfgTGPGift(&gift);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitTGPDailyRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/TGPEverydayReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TGP_DAILY_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::operator=(&gift, v1->pString);
                ++nIndex;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v2->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&gift.vReward, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                ++nIndex;
                
                CfgTencentTable::AddTGPDailyGift(&this->m_cfgTencentTable, &gift);
                CfgTGPGift::~CfgTGPGift(&gift);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitTGPLevelRewardTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/TGPLevelReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TGP_LEVEL_REWARD_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                int32_t nIndex = 0;
                
                gift.nIndex = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
                
                const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::operator=(&gift, v1->pString);
                gift.nLevel = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
                ++nIndex;
                
                std::string strItems;
                char v7;
                std::allocator<char>::allocator(&v7);
                const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItems, v2->pString, &v7);
                
                MemChrBagVector __x;
                CItemHelper::parseItemVectorString(&__x, &strItems);
                std::vector<MemChrBag>::operator=(&gift.vReward, &__x);
                std::vector<MemChrBag>::~vector(&__x);
                std::string::~string(&strItems);
                std::allocator<char>::~allocator(&v7);
                ++nIndex;
                
                CfgTencentTable::AddTGPLevelGift(&this->m_cfgTencentTable, &gift);
                CfgTGPGift::~CfgTGPGift(&gift);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitFamilyDungeonTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/FamilyDungeon.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_FAMILY_DUNGEON_TABLE failed,please check!!!\n");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount; ++i)
            {
                CfgFamilyDungeon stu;
                
                stu.nHard = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
                stu.nDungeonId = CDBCFile::Search_Posistion(&TabFile, i, ++i)->iValue;
                stu.nFamilyLevel = CDBCFile::Search_Posistion(&TabFile, i, ++i)->iValue;
                stu.nCostMoney = CDBCFile::Search_Posistion(&TabFile, i, ++i)->iValue;
                ++i;
                i += 7;
                stu.nBaoKuFuBen = CDBCFile::Search_Posistion(&TabFile, i, ++i)->iValue;
                stu.nNpcId = CDBCFile::Search_Posistion(&TabFile, i, ++i)->iValue;
                stu.X = CDBCFile::Search_Posistion(&TabFile, i, ++i)->iValue;
                stu.Y = CDBCFile::Search_Posistion(&TabFile, i, ++i)->iValue;
                stu.nTime = CDBCFile::Search_Posistion(&TabFile, i, ++i)->iValue;
                
                CfgFamilyDungeonTable::AddDungeon(&this->m_cfgFamilyDungeonTable, &stu);
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}
void CfgData::fetchActivity(CfgData *const this)
{
    CDBCFile ActivityFile;
    CDBCFile::CDBCFile(&ActivityFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&ActivityFile, "./ServerConfig/Tables/cfg_activity.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_activity.txt failed,please check!!");
        CDBCFile::~CDBCFile(&ActivityFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&ActivityFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&ActivityFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgActivity cfg;
            CfgActivity::CfgActivity(&cfg);
            
            int32_t nIndex = 0;
            cfg.id = CDBCFile::Search_Posistion(&ActivityFile, i, 0)->iValue;
            cfg.typeId = CDBCFile::Search_Posistion(&ActivityFile, i, 5)->iValue;
            int32_t weekday = CDBCFile::Search_Posistion(&ActivityFile, i, 9)->iValue;
            cfg.begin_date = CDBCFile::Search_Posistion(&ActivityFile, i, 10)->iValue;
            cfg.end_date = CDBCFile::Search_Posistion(&ActivityFile, i, 11)->iValue;
            cfg.line = CDBCFile::Search_Posistion(&ActivityFile, i, 12)->iValue;
            
            std::string maps;
            std::string start_hour;
            std::string position;
            std::string awards;
            std::string daily;
            
            char v120;
            std::allocator<char>::allocator(&v120);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&ActivityFile, i, 13);
            std::string::string(&maps, v1->pString, &v120);
            std::allocator<char>::~allocator(&v120);
            
            char v121;
            std::allocator<char>::allocator(&v121);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&ActivityFile, i, 14);
            std::string::string(&start_hour, v2->pString, &v121);
            std::allocator<char>::~allocator(&v121);
            
            cfg.duration = CDBCFile::Search_Posistion(&ActivityFile, i, 15)->iValue;
            cfg.level = CDBCFile::Search_Posistion(&ActivityFile, i, 16)->iValue;
            
            char v122;
            std::allocator<char>::allocator(&v122);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&ActivityFile, i, 17);
            std::string::string(&position, v3->pString, &v122);
            std::allocator<char>::~allocator(&v122);
            
            char v123;
            std::allocator<char>::allocator(&v123);
            const CDBCFile::FIELD *v4 = CDBCFile::Search_Posistion(&ActivityFile, i, 18);
            std::string::string(&awards, v4->pString, &v123);
            std::allocator<char>::~allocator(&v123);
            
            cfg.iconid = CDBCFile::Search_Posistion(&ActivityFile, i, 21)->iValue;
            cfg.world_event_chapter = CDBCFile::Search_Posistion(&ActivityFile, i, 23)->iValue;
            int32_t target_mapid = CDBCFile::Search_Posistion(&ActivityFile, i, 24)->iValue;
            int32_t target_regiona = CDBCFile::Search_Posistion(&ActivityFile, i, 25)->iValue;
            
            char v124;
            std::allocator<char>::allocator(&v124);
            const CDBCFile::FIELD *v5 = CDBCFile::Search_Posistion(&ActivityFile, i, 26);
            std::string::string(&daily, v5->pString, &v124);
            std::allocator<char>::~allocator(&v124);
            
            cfg.StartGongGao = CDBCFile::Search_Posistion(&ActivityFile, i, 30)->iValue;
            
            // 解析星期几
            while (weekday > 0)
            {
                int __x = weekday % 10;
                cfg.weekday.push_back(__x);
                weekday /= 10;
            }
            
            // 解析奖励
            if (std::string::size(&awards) > 0 && std::string::c_str(&awards)[0] != '\0')
            {
                std::string delims;
                std::string str;
                char v127;
                std::allocator<char>::allocator(&v127);
                std::string::string(&delims, ":", &v127);
                
                char v129;
                std::allocator<char>::allocator(&v129);
                std::string::string(&str, std::string::c_str(&awards), &v129);
                
                StringVector strParams;
                Answer::StringUtility::split(&strParams, &str, &delims, 0);
                std::string::~string(&str);
                std::allocator<char>::~allocator(&v129);
                std::string::~string(&delims);
                std::allocator<char>::~allocator(&v127);
                
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
                    std::string* v8 = strParams[j];
                    int val = atoi(std::string::c_str(v8));
                    v77[j] = val;
                }
                strParams.~vector();
                
                // 根据类型设置 gift_id
                for (int32_t j = 0; j < nsize; ++j)
                {
                    cfg.gift_id.push_back(&v77[j]);
                }
            }
            
            // 解析每日奖励
            if (std::string::size(&daily) > 0 && std::string::c_str(&daily)[0] != '\0')
            {
                std::string v130;
                std::string v132;
                char v131;
                std::allocator<char>::allocator(&v131);
                std::string::string(&v130, ":", &v131);
                
                char v133;
                std::allocator<char>::allocator(&v133);
                std::string::string(&v132, std::string::c_str(&daily), &v133);
                
                StringVector strParams_0;
                Answer::StringUtility::split(&strParams_0, &v132, &v130, 0);
                std::string::~string(&v132);
                std::allocator<char>::~allocator(&v133);
                std::string::~string(&v130);
                std::allocator<char>::~allocator(&v131);
                
                int16_t nsize_0 = (int16_t)strParams_0.size();
                std::vector<int> v78;
                v78.resize(nsize_0, 0);
                for (int32_t j_0 = 0; j_0 < nsize_0; ++j_0)
                {
                    std::string* v12 = strParams_0[j_0];
                    int val = atoi(std::string::c_str(v12));
                    v78[j_0] = val;
                }
                strParams_0.~vector();
                
                for (int32_t j_0 = 0; j_0 < nsize_0; ++j_0)
                {
                    cfg.daily_reward.push_back(&v78[j_0]);
                }
            }
            
            // 解析地图列表
            {
                std::string v134;
                std::string v136;
                char v135;
                std::allocator<char>::allocator(&v135);
                std::string::string(&v134, ":", &v135);
                
                char v137;
                std::allocator<char>::allocator(&v137);
                std::string::string(&v136, std::string::c_str(&maps), &v137);
                
                StringVector strMaps;
                Answer::StringUtility::split(&strMaps, &v136, &v134, 0);
                std::string::~string(&v136);
                std::allocator<char>::~allocator(&v137);
                std::string::~string(&v134);
                std::allocator<char>::~allocator(&v135);
                
                for (size_t j_1 = 0; j_1 < strMaps.size(); ++j_1)
                {
                    std::string* v15 = strMaps[j_1];
                    int val = atoi(std::string::c_str(v15));
                    cfg.maps.push_back(val);
                }
                strMaps.~vector();
            }
            
            // 解析目标区域
            {
                std::string v139;
                std::string v141;
                char v140;
                std::allocator<char>::allocator(&v140);
                std::string::string(&v139, ":", &v140);
                
                char v142;
                std::allocator<char>::allocator(&v142);
                std::string::string(&v141, std::string::c_str(&position), &v142);
                
                StringVector strParams_1;
                Answer::StringUtility::split(&strParams_1, &v141, &v139, 0);
                std::string::~string(&v141);
                std::allocator<char>::~allocator(&v142);
                std::string::~string(&v139);
                std::allocator<char>::~allocator(&v140);
                
                if (strParams_1.size() == 2)
                {
                    std::string* v19 = strParams_1[0];
                    cfg.target_mapid = atoi(std::string::c_str(v19));
                    
                    std::string v143;
                    std::string v145;
                    char v144;
                    std::allocator<char>::allocator(&v144);
                    std::string::string(&v143, "|", &v144);
                    
                    char v146;
                    std::allocator<char>::allocator(&v146);
                    std::string* v21 = strParams_1[1];
                    std::string::string(&v145, std::string::c_str(v21), &v146);
                    
                    StringVector stritemParams;
                    Answer::StringUtility::split(&stritemParams, &v145, &v143, 0);
                    std::string::~string(&v145);
                    std::allocator<char>::~allocator(&v146);
                    std::string::~string(&v143);
                    std::allocator<char>::~allocator(&v144);
                    
                    for (size_t j_2 = 0; j_2 < stritemParams.size(); ++j_2)
                    {
                        std::string* v23 = stritemParams[j_2];
                        int val = atoi(std::string::c_str(v23));
                        cfg.target_regiona.push_back(val);
                    }
                    stritemParams.~vector();
                }
                else if (strParams_1.size() == 3)
                {
                    std::string* v25 = strParams_1[0];
                    cfg.target_mapid = atoi(std::string::c_str(v25));
                    
                    std::string v148;
                    std::string v150;
                    char v149;
                    std::allocator<char>::allocator(&v149);
                    std::string::string(&v148, "|", &v149);
                    
                    char v151;
                    std::allocator<char>::allocator(&v151);
                    std::string* v27 = strParams_1[1];
                    std::string::string(&v150, std::string::c_str(v27), &v151);
                    
                    StringVector stritemParams_0;
                    Answer::StringUtility::split(&stritemParams_0, &v150, &v148, 0);
                    std::string::~string(&v150);
                    std::allocator<char>::~allocator(&v151);
                    std::string::~string(&v148);
                    std::allocator<char>::~allocator(&v149);
                    
                    for (size_t j_3 = 0; j_3 < stritemParams_0.size(); ++j_3)
                    {
                        std::string* v29 = stritemParams_0[j_3];
                        int val = atoi(std::string::c_str(v29));
                        cfg.target_regiona.push_back(val);
                    }
                    
                    std::string v154;
                    std::string v156;
                    char v155;
                    std::allocator<char>::allocator(&v155);
                    std::string::string(&v154, "|", &v155);
                    
                    char v157;
                    std::allocator<char>::allocator(&v157);
                    std::string* v31 = strParams_1[2];
                    std::string::string(&v156, std::string::c_str(v31), &v157);
                    
                    StringVector v153;
                    Answer::StringUtility::split(&v153, &v156, &v154, 0);
                    stritemParams_0.operator=(v153);
                    v153.~vector();
                    std::string::~string(&v156);
                    std::allocator<char>::~allocator(&v157);
                    std::string::~string(&v154);
                    std::allocator<char>::~allocator(&v155);
                    
                    for (size_t k = 0; k < stritemParams_0.size(); ++k)
                    {
                        std::string* v33 = stritemParams_0[k];
                        int val = atoi(std::string::c_str(v33));
                        cfg.target_regionb.push_back(val);
                    }
                    stritemParams_0.~vector();
                }
                strParams_1.~vector();
            }
            
            // 解析开始时间
            {
                std::string v159;
                char v160;
                std::allocator<char>::allocator(&v160);
                std::string::string(&v159, ":", &v160);
                
                StringVector vStartHour;
                Answer::StringUtility::split(&vStartHour, &start_hour, &v159, 0);
                std::string::~string(&v159);
                std::allocator<char>::~allocator(&v160);
                
                for (size_t j_4 = 0; j_4 < vStartHour.size(); ++j_4)
                {
                    std::string* v35 = vStartHour[j_4];
                    int tstart = atoi(std::string::c_str(v35));
                    if (tstart >= 0 && cfg.duration >= 0 && cfg.duration + tstart <= 1439)
                    {
                        cfg.start_hour.push_back(&tstart);
                    }
                    else
                    {
                        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "wrong activity data with id=%d\n", cfg.id);
                    }
                }
                vStartHour.~vector();
            }
            
            // 存储到 map
            auto v38 = std::map<int, CfgActivity>::operator[](&this->m_activities, &cfg.id);
            *v38 = cfg;
            
            // 清理
            daily.~string();
            awards.~string();
            position.~string();
            start_hour.~string();
            maps.~string();
            cfg.~CfgActivity();
        }
        
        // 加载活动事件
        CDBCFile ActivityEventFile;
        CDBCFile::CDBCFile(&ActivityEventFile, 0);
        
        if (CDBCFile::OpenFromTXT(&ActivityEventFile, "./ServerConfig/Tables/cfg_activity_event.txt"))
        {
            int32_t iBaseTableCount_Event = CDBCFile::GetRecordsNum(&ActivityEventFile);
            int32_t iBaseColumnCount_Event = CDBCFile::GetFieldsNum(&ActivityEventFile);
            
            if (iBaseColumnCount_Event > 0)
            {
                for (int32_t i_0 = 0; i_0 < iBaseTableCount_Event; ++i_0)
                {
                    CfgMapEvent event;
                    CfgMapEvent::CfgMapEvent(&event);
                    
                    int32_t nIndex = 0;
                    event.id = CDBCFile::Search_Posistion(&ActivityEventFile, i_0, 0)->iValue;
                    event.activity_id = CDBCFile::Search_Posistion(&ActivityEventFile, i_0, ++nIndex)->iValue;
                    event.plant_id = CDBCFile::Search_Posistion(&ActivityEventFile, i_0, ++nIndex)->iValue;
                    event.bInitOpen = CDBCFile::Search_Posistion(&ActivityEventFile, i_0, ++nIndex)->iValue == 0;
                    event.count = CDBCFile::Search_Posistion(&ActivityEventFile, i_0, ++nIndex)->iValue;
                    ++nIndex;
                    
                    std::string triggerParam;
                    char v161;
                    std::allocator<char>::allocator(&v161);
                    const CDBCFile::FIELD *v39 = CDBCFile::Search_Posistion(&ActivityEventFile, i_0, nIndex);
                    std::string::string(&triggerParam, v39->pString, &v161);
                    std::allocator<char>::~allocator(&v161);
                    
                    event.mapid = CDBCFile::Search_Posistion(&ActivityEventFile, i_0, ++nIndex)->iValue;
                    
                    const CDBCFile::FIELD *v40 = CDBCFile::Search_Posistion(&ActivityEventFile, i_0, ++nIndex);
                    std::string::operator=(&event.effect, v40->pString);
                    ++nIndex;
                    
                    // 解析 trigger_param
                    std::string v162;
                    std::string v164;
                    char v163;
                    std::allocator<char>::allocator(&v163);
                    std::string::string(&v162, ":", &v163);
                    
                    char v165;
                    std::allocator<char>::allocator(&v165);
                    std::string::string(&v164, std::string::c_str(&triggerParam), &v165);
                    
                    StringVector strParams_2;
                    Answer::StringUtility::split(&strParams_2, &v164, &v162, 0);
                    std::string::~string(&v164);
                    std::allocator<char>::~allocator(&v165);
                    std::string::~string(&v162);
                    std::allocator<char>::~allocator(&v163);
                    
                    for (auto& strParam : strParams_2)
                    {
                        int val = atoi(std::string::c_str(strParam));
                        event.trigger_param.push_back(val);
                    }
                    strParams_2.~vector();
                    triggerParam.~string();
                    
                    // 存储事件
                    auto v44 = std::map<int, std::map<int, std::list<CfgMapEvent>>>::operator[](&this->m_activityEvents, &event.id);
                    auto v45 = std::map<int, std::list<CfgMapEvent>>::operator[](v44, &event.mapid);
                    v45->push_back(event);
                    
                    event.~CfgMapEvent();
                }
            }
            CDBCFile::~CDBCFile(&ActivityEventFile);
            
            // 加载活动怪物
            CDBCFile ActivityMonsterFile;
            CDBCFile::CDBCFile(&ActivityMonsterFile, 0);
            
            if (CDBCFile::OpenFromTXT(&ActivityMonsterFile, "./ServerConfig/Tables/cfg_activity_monster.txt"))
            {
                int32_t iBaseTableCount_Monster = CDBCFile::GetRecordsNum(&ActivityMonsterFile);
                int32_t iBaseColumnCount_Monster = CDBCFile::GetFieldsNum(&ActivityMonsterFile);
                
                if (iBaseColumnCount_Monster > 0)
                {
                    for (int32_t i_1 = 0; i_1 < iBaseTableCount_Monster; ++i_1)
                    {
                        CfgActivityMonster monster;
                        CfgActivityMonster::CfgActivityMonster(&monster);
                        
                        monster.id = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 0)->iValue;
                        monster.wave = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 1)->iValue;
                        monster.mid = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 2)->iValue;
                        monster.count = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 3)->iValue;
                        monster.side = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 4)->iValue;
                        
                        std::string road;
                        char v168;
                        std::allocator<char>::allocator(&v168);
                        const CDBCFile::FIELD *v46 = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 5);
                        std::string::string(&road, v46->pString, &v168);
                        std::allocator<char>::~allocator(&v168);
                        
                        monster.x = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 6)->iValue;
                        monster.y = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 7)->iValue;
                        monster.delay = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 8)->iValue;
                        monster.times = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 9)->iValue;
                        monster.buff = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 10)->iValue;
                        
                        std::string randpos;
                        char v169;
                        std::allocator<char>::allocator(&v169);
                        const CDBCFile::FIELD *v47 = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 11);
                        std::string::string(&randpos, v47->pString, &v169);
                        std::allocator<char>::~allocator(&v169);
                        
                        monster.left = CDBCFile::Search_Posistion(&ActivityMonsterFile, i_1, 12)->iValue;
                        
                        // 解析路线
                        if (road.size() > 3)
                        {
                            std::string v170;
                            char v171;
                            std::allocator<char>::allocator(&v171);
                            std::string::string(&v170, ":", &v171);
                            
                            StringVector vRoad;
                            Answer::StringUtility::split(&vRoad, &road, &v170, 0);
                            std::string::~string(&v170);
                            std::allocator<char>::~allocator(&v171);
                            
                            for (size_t j_5 = 0; j_5 < vRoad.size(); ++j_5)
                            {
                                std::string v172;
                                char v173;
                                std::allocator<char>::allocator(&v173);
                                std::string::string(&v172, ",", &v173);
                                
                                StringVector vPos;
                                Answer::StringUtility::split(&vPos, vRoad[j_5], &v172, 0);
                                std::string::~string(&v172);
                                std::allocator<char>::~allocator(&v173);
                                
                                if (vPos.size() == 2)
                                {
                                    Position pos;
                                    pos.x = atoi(std::string::c_str(vPos[0]));
                                    pos.y = atoi(std::string::c_str(vPos[1]));
                                    monster.road.push_back(pos);
                                }
                                vPos.~vector();
                            }
                            vRoad.~vector();
                        }
                        
                        // 解析随机位置
                        if (randpos.size() > 3)
                        {
                            std::string v175;
                            char v176;
                            std::allocator<char>::allocator(&v176);
                            std::string::string(&v175, "|", &v176);
                            
                            StringVector vRandPos;
                            Answer::StringUtility::split(&vRandPos, &randpos, &v175, 0);
                            std::string::~string(&v175);
                            std::allocator<char>::~allocator(&v176);
                            
                            for (size_t j_6 = 0; j_6 < vRandPos.size(); ++j_6)
                            {
                                std::string v177;
                                char v178;
                                std::allocator<char>::allocator(&v178);
                                std::string::string(&v177, ":", &v178);
                                
                                StringVector vPos_0;
                                Answer::StringUtility::split(&vPos_0, vRandPos[j_6], &v177, 0);
                                std::string::~string(&v177);
                                std::allocator<char>::~allocator(&v178);
                                
                                if (vPos_0.size() == 2)
                                {
                                    Position pos;
                                    pos.x = atoi(std::string::c_str(vPos_0[0]));
                                    pos.y = atoi(std::string::c_str(vPos_0[1]));
                                    monster.randpos.push_back(pos);
                                }
                                vPos_0.~vector();
                            }
                            vRandPos.~vector();
                        }
                        
                        auto v64 = std::map<int, CfgActivityMonster>::operator[](&this->m_activityMonsters, &monster.id);
                        *v64 = monster;
                        
                        randpos.~string();
                        road.~string();
                        monster.~CfgActivityMonster();
                    }
                }
                CDBCFile::~CDBCFile(&ActivityMonsterFile);
            }
            
            // 加载活动NPC
            CDBCFile ActivityNpcFile;
            CDBCFile::CDBCFile(&ActivityNpcFile, 0);
            
            if (CDBCFile::OpenFromTXT(&ActivityNpcFile, "./ServerConfig/Tables/cfg_activity_npc.txt"))
            {
                int32_t iBaseTableCount_Npc = CDBCFile::GetRecordsNum(&ActivityNpcFile);
                int32_t iBaseColumnCount_Npc = CDBCFile::GetFieldsNum(&ActivityNpcFile);
                
                if (iBaseColumnCount_Npc > 0)
                {
                    for (int32_t i_2 = 0; i_2 < iBaseTableCount_Npc; ++i_2)
                    {
                        CfgActivityNpc npc;
                        CfgActivityNpc::CfgActivityNpc(&npc);
                        
                        npc.id = CDBCFile::Search_Posistion(&ActivityNpcFile, i_2, 0)->iValue;
                        npc.activity_id = CDBCFile::Search_Posistion(&ActivityNpcFile, i_2, 1)->iValue;
                        npc.npc_id = CDBCFile::Search_Posistion(&ActivityNpcFile, i_2, 2)->iValue;
                        npc.count = CDBCFile::Search_Posistion(&ActivityNpcFile, i_2, 3)->iValue;
                        
                        std::string regionId;
                        char v180;
                        std::allocator<char>::allocator(&v180);
                        const CDBCFile::FIELD *v65 = CDBCFile::Search_Posistion(&ActivityNpcFile, i_2, 4);
                        std::string::string(&regionId, v65->pString, &v180);
                        std::allocator<char>::~allocator(&v180);
                        
                        std::string v181;
                        char v182;
                        std::allocator<char>::allocator(&v182);
                        std::string::string(&v181, ":", &v182);
                        
                        char v184;
                        std::allocator<char>::allocator(&v184);
                        std::string v183;
                        std::string::string(&v183, std::string::c_str(&regionId), &v184);
                        
                        StringVector strRegions;
                        Answer::StringUtility::split(&strRegions, &v183, &v181, 0);
                        std::string::~string(&v183);
                        std::allocator<char>::~allocator(&v184);
                        std::string::~string(&v181);
                        std::allocator<char>::~allocator(&v182);
                        
                        for (size_t it_0 = 0; it_0 < strRegions.size(); ++it_0)
                        {
                            std::string* v67 = strRegions[it_0];
                            int val = atoi(std::string::c_str(v67));
                            npc.region_id.push_back(val);
                        }
                        strRegions.~vector();
                        regionId.~string();
                        
                        auto v69 = std::map<int, CfgActivityNpc>::operator[](&this->m_activityNpcs, &npc.id);
                        *v69 = npc;
                        npc.~CfgActivityNpc();
                    }
                }
                CDBCFile::~CDBCFile(&ActivityNpcFile);
            }
            
            // 加载活动植物
            CDBCFile ActivityPlantFile;
            CDBCFile::CDBCFile(&ActivityPlantFile, 0);
            
            if (CDBCFile::OpenFromTXT(&ActivityPlantFile, "./ServerConfig/Tables/cfg_activity_plant.txt"))
            {
                int32_t iBaseTableCount_Plant = CDBCFile::GetRecordsNum(&ActivityPlantFile);
                int32_t iBaseColumnCount_Plant = CDBCFile::GetFieldsNum(&ActivityPlantFile);
                
                if (iBaseColumnCount_Plant > 0)
                {
                    for (int32_t i_3 = 0; i_3 < iBaseTableCount_Plant; ++i_3)
                    {
                        CfgActivityPlant plant;
                        CfgActivityPlant::CfgActivityPlant(&plant);
                        
                        plant.id = CDBCFile::Search_Posistion(&ActivityPlantFile, i_3, 0)->iValue;
                        plant.activity_id = CDBCFile::Search_Posistion(&ActivityPlantFile, i_3, 1)->iValue;
                        plant.plant_id = CDBCFile::Search_Posistion(&ActivityPlantFile, i_3, 2)->iValue;
                        plant.count = CDBCFile::Search_Posistion(&ActivityPlantFile, i_3, 3)->iValue;
                        plant.wave = CDBCFile::Search_Posistion(&ActivityPlantFile, i_3, 4)->iValue;
                        plant.region_id = CDBCFile::Search_Posistion(&ActivityPlantFile, i_3, 5)->iValue;
                        plant.whoplant = CDBCFile::Search_Posistion(&ActivityPlantFile, i_3, 6)->iValue;
                        
                        std::string strPos;
                        char v189;
                        std::allocator<char>::allocator(&v189);
                        const CDBCFile::FIELD *v70 = CDBCFile::Search_Posistion(&ActivityPlantFile, i_3, 7);
                        std::string::string(&strPos, v70->pString, &v189);
                        
                        std::vector<Position> v187;
                        CfgData::paresPosition(&v187, this, &strPos);
                        plant.EnterPosVector = v187;
                        v187.~vector();
                        strPos.~string();
                        std::allocator<char>::~allocator(&v189);
                        
                        plant.life_time = CDBCFile::Search_Posistion(&ActivityPlantFile, i_3, 8)->iValue;
                        
                        auto v71 = std::map<int, CfgActivityPlant>::operator[](&this->m_activityPlants, &plant.id);
                        *v71 = plant;
                        plant.~CfgActivityPlant();
                    }
                }
                CDBCFile::~CDBCFile(&ActivityPlantFile);
            }
            
            // 加载活动掉落
            CDBCFile ActivityDropFile;
            CDBCFile::CDBCFile(&ActivityDropFile, 0);
            
            if (CDBCFile::OpenFromTXT(&ActivityDropFile, "./ServerConfig/Tables/cfg_activity_drop.txt"))
            {
                int32_t iBaseTableCount_Drop = CDBCFile::GetRecordsNum(&ActivityDropFile);
                int32_t iBaseColumnCount_Drop = CDBCFile::GetFieldsNum(&ActivityDropFile);
                
                if (iBaseColumnCount_Drop > 0)
                {
                    for (int32_t i_4 = 0; i_4 < iBaseTableCount_Drop; ++i_4)
                    {
                        CfgActivityDrop drop;
                        drop.id = CDBCFile::Search_Posistion(&ActivityDropFile, i_4, 0)->iValue;
                        drop.activity_id = CDBCFile::Search_Posistion(&ActivityDropFile, i_4, 1)->iValue;
                        drop.monster_min_level = CDBCFile::Search_Posistion(&ActivityDropFile, i_4, 2)->iValue;
                        drop.drop_group_id = CDBCFile::Search_Posistion(&ActivityDropFile, i_4, 3)->iValue;
                        drop.probability = CDBCFile::Search_Posistion(&ActivityDropFile, i_4, 4)->iValue;
                        drop.bind_type = CDBCFile::Search_Posistion(&ActivityDropFile, i_4, 5)->iValue;
                        
                        auto v72 = std::map<int, CfgActivityDrop>::operator[](&this->m_activityDrops, &drop.id);
                        *v72 = drop;
                    }
                }
                CDBCFile::~CDBCFile(&ActivityDropFile);
            }
            
            // 加载活动陷阱
            CDBCFile ActivityTrapFile;
            CDBCFile::CDBCFile(&ActivityTrapFile, 0);
            
            if (CDBCFile::OpenFromTXT(&ActivityTrapFile, "./ServerConfig/Tables/cfg_activity_trap.txt"))
            {
                int32_t iBaseTableCount_Trap = CDBCFile::GetRecordsNum(&ActivityTrapFile);
                int32_t iBaseColumnCount_Trap = CDBCFile::GetFieldsNum(&ActivityTrapFile);
                
                if (iBaseColumnCount_Trap > 0)
                {
                    for (int32_t i_5 = 0; i_5 < iBaseTableCount_Trap; ++i_5)
                    {
                        CfgActivityTrap trap;
                        trap.id = CDBCFile::Search_Posistion(&ActivityTrapFile, i_5, 0)->iValue;
                        trap.tid = CDBCFile::Search_Posistion(&ActivityTrapFile, i_5, 1)->iValue;
                        trap.x = CDBCFile::Search_Posistion(&ActivityTrapFile, i_5, 2)->iValue;
                        trap.y = CDBCFile::Search_Posistion(&ActivityTrapFile, i_5, 3)->iValue;
                        
                        auto v73 = std::map<int, CfgActivityTrap>::operator[](&this->m_activityTraps, &trap.id);
                        *v73 = trap;
                    }
                }
                CDBCFile::~CDBCFile(&ActivityTrapFile);
            }
        }
    }
    CDBCFile::~CDBCFile(&ActivityFile);
}
void CfgData::fetchBuff(CfgData *const this)
{
    CDBCFile BuffFile;
    CDBCFile::CDBCFile(&BuffFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&BuffFile, "./ServerConfig/Tables/cfg_buff.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_buff.txt failed,please check!!");
        CDBCFile::~CDBCFile(&BuffFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&BuffFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&BuffFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgBuff buff;
            CfgBuff::CfgBuff(&buff);
            
            int32_t nIndex = 0;
            buff.id = CDBCFile::Search_Posistion(&BuffFile, i, nIndex++)->iValue;
            buff.groupid = CDBCFile::Search_Posistion(&BuffFile, i, nIndex++)->iValue;
            buff.attack_type = CDBCFile::Search_Posistion(&BuffFile, i, nIndex++)->iValue;
            buff.level = CDBCFile::Search_Posistion(&BuffFile, i, nIndex++)->iValue;
            ++nIndex;
            buff.beneficial = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.special = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.duration = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.interval = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string str;
            char v8;
            std::allocator<char>::allocator(&v8);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&BuffFile, i, nIndex);
            std::string::string(&str, v1->pString, &v8);
            
            BuffAttrVector __x;
            CfgData::paraseBuffAttr(&__x, this, &str);
            buff.buffAttr = __x;
            __x.~vector();
            str.~string();
            std::allocator<char>::~allocator(&v8);
            ++nIndex;
            
            buff.isShow = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            buff.modify = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.control = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.hpValue = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.hpPecent = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.shieldValue = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.shieldRatio = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.effectType = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.effectTimes = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.canRemove = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            buff.canRecover = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            ++nIndex;
            ++nIndex;
            
            std::string v9;
            char v10;
            std::allocator<char>::allocator(&v10);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&BuffFile, i, nIndex);
            std::string::string(&v9, v2->pString, &v10);
            buff.angry = CfgData::paraseParam2(this, &v9);
            v9.~string();
            std::allocator<char>::~allocator(&v10);
            
            buff.battle = CDBCFile::Search_Posistion(&BuffFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto *v3 = std::map<int, CfgBuff>::operator[](&this->m_buffs, &buff.id);
            *v3 = buff;
            buff.~CfgBuff();
        }
    }
    CDBCFile::~CDBCFile(&BuffFile);
}

BuffAttrVector *CfgData::paraseBuffAttr(BuffAttrVector *__return_ptr retstr, CfgData *const this, const std::string *const str)
{
    std::vector<BuffAttr>::vector(retstr);
    
    if (str->size() > 4)
    {
        std::string delims;
        char v16;
        std::allocator<char>::allocator(&v16);
        std::string::string(&delims, "|", &v16);
        
        StringVector vstr;
        Answer::StringUtility::split(&vstr, str, &delims, 0);
        delims.~string();
        std::allocator<char>::~allocator(&v16);
        
        if (!vstr.empty())
        {
            int32_t nSize = (int32_t)vstr.size();
            retstr->reserve(nSize);
            
            for (int32_t i = 0; i < nSize; ++i)
            {
                std::string v17;
                char v18;
                std::allocator<char>::allocator(&v18);
                std::string::string(&v17, ":", &v18);
                
                StringVector vBuff;
                Answer::StringUtility::split(&vBuff, vstr[i], &v17, 0);
                v17.~string();
                std::allocator<char>::~allocator(&v18);
                
                if (vBuff.size() == 3)
                {
                    BuffAttr stu;
                    stu.attr = atoi(std::string::c_str(vBuff[0]));
                    stu.ratio = atoi(std::string::c_str(vBuff[1]));
                    stu.addon = atoi(std::string::c_str(vBuff[2]));
                    retstr->push_back(stu);
                }
                vBuff.~vector();
            }
        }
        vstr.~vector();
    }
    return retstr;
}

void CfgData::fetchDungeon(CfgData *const this)
{
    CDBCFile DungeonFile;
    CDBCFile::CDBCFile(&DungeonFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&DungeonFile, "./ServerConfig/Tables/cfg_dungeon.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_dungeon.txt failed,please check!!");
        CDBCFile::~CDBCFile(&DungeonFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&DungeonFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&DungeonFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            
            int32_t nIndex = 0;
            dungeon.id = CDBCFile::Search_Posistion(&DungeonFile, i, nIndex++)->iValue;
            
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&DungeonFile, i, nIndex);
            std::string::operator=(&dungeon.name, v1->pString);
            ++nIndex;
            ++nIndex;
            
            dungeon.mapid = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.x = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.y = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.type = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.sequence = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.group_id = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.duration = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.job = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.level = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.maxLevel = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.vip = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.player_num = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            int32_t weekday = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.start_hour = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.end_hour = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.daily_count = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            dungeon.last_id = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.next_id = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.player_buff = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            dungeon.star = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.double_cost = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.reward_time = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            
            std::string strItem;
            char v21;
            std::allocator<char>::allocator(&v21);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&DungeonFile, i, nIndex);
            std::string::string(&strItem, v2->pString, &v21);
            
            ItemData v55 = CItemHelper::parseItemDataString(&strItem);
            dungeon.costItem.m_nId = v55.m_nId;
            dungeon.costItem.m_nClass = v55.m_nClass;
            dungeon.costItem.m_nCount = v55.m_nCount;
            strItem.~string();
            std::allocator<char>::~allocator(&v21);
            ++nIndex;
            
            std::string strItems;
            char v24;
            std::allocator<char>::allocator(&v24);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&DungeonFile, i, nIndex);
            std::string::string(&strItems, v3->pString, &v24);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            dungeon.rewardItem = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v24);
            ++nIndex;
            
            std::string v26;
            char v27;
            std::allocator<char>::allocator(&v27);
            const CDBCFile::FIELD *v4 = CDBCFile::Search_Posistion(&DungeonFile, i, nIndex);
            std::string::string(&v26, v4->pString, &v27);
            
            MemChrBagVector v25;
            CItemHelper::parseItemVectorString(&v25, &v26);
            dungeon.rewardOnce = v25;
            v25.~vector();
            v26.~string();
            std::allocator<char>::~allocator(&v27);
            ++nIndex;
            
            dungeon.Battle = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.costMoney = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.costGold = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            dungeon.buyTimes = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.buyCost = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.quickDoneCost = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.quickDoneDrop = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.needUpTowerStar = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.summon_cost = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.summon_boss = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            dungeon.rand_time = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            ++nIndex;
            dungeon.task_id = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            dungeon.stay_position = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string path;
            std::string str;
            char v30;
            std::allocator<char>::allocator(&v30);
            std::string::string(&path, "./ServerConfig/Tables/cfg_dungeon.txt", &v30);
            
            char v32;
            std::allocator<char>::allocator(&v32);
            const CDBCFile::FIELD *v5 = CDBCFile::Search_Posistion(&DungeonFile, i, nIndex);
            std::string::string(&str, v5->pString, &v32);
            
            Int32Vector v28;
            CfgData::paraseInt32Vector(&v28, this, &str, &path, 0);
            dungeon.win_star = v28;
            v28.~vector();
            str.~string();
            std::allocator<char>::~allocator(&v32);
            path.~string();
            std::allocator<char>::~allocator(&v30);
            ++nIndex;
            
            std::string v34;
            char v35;
            std::allocator<char>::allocator(&v35);
            const CDBCFile::FIELD *v6 = CDBCFile::Search_Posistion(&DungeonFile, i, nIndex);
            std::string::string(&v34, v6->pString, &v35);
            
            MemChrBagVector v33;
            CItemHelper::parseItemVectorString(&v33, &v34);
            dungeon.star_reward = v33;
            v33.~vector();
            v34.~string();
            std::allocator<char>::~allocator(&v35);
            ++nIndex;
            
            std::string v37;
            char v38;
            std::allocator<char>::allocator(&v38);
            std::string::string(&v37, "./ServerConfig/Tables/cfg_dungeon.txt", &v38);
            
            char v40;
            std::allocator<char>::allocator(&v40);
            const CDBCFile::FIELD *v7 = CDBCFile::Search_Posistion(&DungeonFile, i, nIndex);
            std::string::string(&v39, v7->pString, &v40);
            
            Int32Vector v36;
            CfgData::paraseInt32Vector(&v36, this, &v39, &v37, 0);
            dungeon.star_ratio = v36;
            v36.~vector();
            v39.~string();
            std::allocator<char>::~allocator(&v40);
            v37.~string();
            std::allocator<char>::~allocator(&v38);
            ++nIndex;
            
            dungeon.TeQuan = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.backMapId = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.backX = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.backY = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            dungeon.CanEnter = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            dungeon.BuffId = CDBCFile::Search_Posistion(&DungeonFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            while (weekday > 0)
            {
                dungeon.weekday |= 1 << (weekday % 10);
                weekday /= 10;
            }
            
            auto *v8 = std::map<int, CfgDungeon>::operator[](&this->m_dungeons, &dungeon.id);
            *v8 = dungeon;
            
            if (dungeon.type == 16)
            {
                auto *v9 = std::map<int, CfgDungeon>::operator[](&this->m_mUpTowerDungeon, &dungeon.star);
                *v9 = dungeon;
            }
            
            dungeon.~CfgDungeon();
        }
        
        // 加载副本奖励
        CDBCFile DungeonRewardFile;
        CDBCFile::CDBCFile(&DungeonRewardFile, 0);
        
        if (CDBCFile::OpenFromTXT(&DungeonRewardFile, "./ServerConfig/Tables/cfg_dungeon_reward.txt"))
        {
            int32_t iBaseTableCount_Reward = CDBCFile::GetRecordsNum(&DungeonRewardFile);
            int32_t iBaseColumnCount_Reward = CDBCFile::GetFieldsNum(&DungeonRewardFile);
            
            if (iBaseColumnCount_Reward > 0)
            {
                for (int32_t i_0 = 0; i_0 < iBaseTableCount_Reward; ++i_0)
                {
                    CfgDungeonReward reward;
                    reward.dungeonID = CDBCFile::Search_Posistion(&DungeonRewardFile, i_0, 0)->iValue;
                    reward.exp = CDBCFile::Search_Posistion(&DungeonRewardFile, i_0, 1)->iValue;
                    reward.money = CDBCFile::Search_Posistion(&DungeonRewardFile, i_0, 2)->iValue;
                    reward.cash = CDBCFile::Search_Posistion(&DungeonRewardFile, i_0, 3)->iValue;
                    reward.groupID = CDBCFile::Search_Posistion(&DungeonRewardFile, i_0, 4)->iValue;
                    
                    auto *v10 = std::map<int, CfgDungeonReward>::operator[](&this->m_dungeonReward, &reward.dungeonID);
                    *v10 = reward;
                }
            }
            CDBCFile::~CDBCFile(&DungeonRewardFile);
        }
        
        // 加载副本掉落组
        CDBCFile DungeonDropFile;
        CDBCFile::CDBCFile(&DungeonDropFile, 0);
        
        if (CDBCFile::OpenFromTXT(&DungeonDropFile, "./ServerConfig/Tables/cfg_dungeon_drop_group.txt"))
        {
            int32_t iBaseTableCount_Drop = CDBCFile::GetRecordsNum(&DungeonDropFile);
            int32_t iBaseColumnCount_Drop = CDBCFile::GetFieldsNum(&DungeonDropFile);
            
            if (iBaseColumnCount_Drop > 0)
            {
                for (int32_t i_1 = 0; i_1 < iBaseTableCount_Drop; ++i_1)
                {
                    int32_t id = CDBCFile::Search_Posistion(&DungeonDropFile, i_1, 0)->iValue;
                    CfgDungeonDrop group;
                    group.type = CDBCFile::Search_Posistion(&DungeonDropFile, i_1, 1)->iValue;
                    group.item_id = CDBCFile::Search_Posistion(&DungeonDropFile, i_1, 3)->iValue;
                    group.item_class = CDBCFile::Search_Posistion(&DungeonDropFile, i_1, 4)->iValue;
                    group.bind_type = CDBCFile::Search_Posistion(&DungeonDropFile, i_1, 5)->iValue;
                    group.count = CDBCFile::Search_Posistion(&DungeonDropFile, i_1, 6)->iValue;
                    group.weight = CDBCFile::Search_Posistion(&DungeonDropFile, i_1, 7)->iValue;
                    group.probability = CDBCFile::Search_Posistion(&DungeonDropFile, i_1, 8)->iValue;
                    
                    auto *v11 = std::map<int, CfgDungeonDropGroup>::operator[](&this->m_dungeonDropGroup, &id);
                    CfgDungeonDropGroup::Add(v11, &group);
                }
            }
            CDBCFile::~CDBCFile(&DungeonDropFile);
        }
    }
    CDBCFile::~CDBCFile(&DungeonFile);
}
void CfgData::fetchItem(CfgData *const this, bool bSend)
{
    CfgItemTable newItems;
    std::map<int, CfgItem *>::map(&newItems);
    
    Answer::RwLock::wrlock(&this->m_itemsLock);
    
    CDBCFile ItemFile;
    CDBCFile::CDBCFile(&ItemFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&ItemFile, "./ServerConfig/Tables/cfg_item.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_item.txt failed,please check!!");
    }
    else
    {
        int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&ItemFile);
        int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&ItemFile);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i_1 = 0; i_1 < iBaseTableCount; ++i_1)
            {
                CfgItem *pItem = new CfgItem();
                
                pItem->id = CDBCFile::Search_Posistion(&ItemFile, i_1, 0)->iValue;
                
                const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&ItemFile, i_1, 1);
                std::string::operator=(&pItem->name, v3->pString);
                
                const CDBCFile::FIELD *v4 = CDBCFile::Search_Posistion(&ItemFile, i_1, 3);
                std::string::operator=(&pItem->desc, v4->pString);
                
                pItem->type = CDBCFile::Search_Posistion(&ItemFile, i_1, 4)->iValue;
                pItem->level = CDBCFile::Search_Posistion(&ItemFile, i_1, 5)->iValue;
                pItem->grade = CDBCFile::Search_Posistion(&ItemFile, i_1, 6)->iValue;
                pItem->job = CDBCFile::Search_Posistion(&ItemFile, i_1, 7)->iValue;
                pItem->in_value = CDBCFile::Search_Posistion(&ItemFile, i_1, 8)->iValue;
                pItem->out_value = CDBCFile::Search_Posistion(&ItemFile, i_1, 9)->iValue;
                pItem->bind = CDBCFile::Search_Posistion(&ItemFile, i_1, 10)->iValue;
                pItem->combine = CDBCFile::Search_Posistion(&ItemFile, i_1, 11)->iValue;
                pItem->quality = CDBCFile::Search_Posistion(&ItemFile, i_1, 12)->iValue;
                
                const CDBCFile::FIELD *v14 = CDBCFile::Search_Posistion(&ItemFile, i_1, 13);
                std::string::operator=(&pItem->url, v14->pString);
                
                const CDBCFile::FIELD *v15 = CDBCFile::Search_Posistion(&ItemFile, i_1, 14);
                std::string::operator=(&pItem->drop_url, v15->pString);
                
                const CDBCFile::FIELD *v16 = CDBCFile::Search_Posistion(&ItemFile, i_1, 15);
                std::string::operator=(&pItem->effect, v16->pString);
                
                const CDBCFile::FIELD *v17 = CDBCFile::Search_Posistion(&ItemFile, i_1, 16);
                std::string::operator=(&pItem->use_method, v17->pString);
                
                pItem->downgrade = CDBCFile::Search_Posistion(&ItemFile, i_1, 19)->iValue;
                pItem->group_id = CDBCFile::Search_Posistion(&ItemFile, i_1, 20)->iValue;
                pItem->cd_group = CDBCFile::Search_Posistion(&ItemFile, i_1, 21)->iValue;
                pItem->overlay = CDBCFile::Search_Posistion(&ItemFile, i_1, 22)->iValue;
                pItem->can_sell = CDBCFile::Search_Posistion(&ItemFile, i_1, 23)->iValue;
                pItem->broadcast = CDBCFile::Search_Posistion(&ItemFile, i_1, 24)->iValue;
                pItem->valid_time = CDBCFile::Search_Posistion(&ItemFile, i_1, 25)->iValue;
                pItem->item_Grade = CDBCFile::Search_Posistion(&ItemFile, i_1, 26)->iValue;
                pItem->CanDrop = CDBCFile::Search_Posistion(&ItemFile, i_1, 32)->iValue;
                pItem->useBroadcast = CDBCFile::Search_Posistion(&ItemFile, i_1, 33)->iValue;
                pItem->useInCarrier = CDBCFile::Search_Posistion(&ItemFile, i_1, 35)->iValue;
                pItem->GongGaoId = CDBCFile::Search_Posistion(&ItemFile, i_1, 40)->iValue;
                pItem->KunLingJingHua = CDBCFile::Search_Posistion(&ItemFile, i_1, 42)->iValue;
                pItem->XinMoBag = CDBCFile::Search_Posistion(&ItemFile, i_1, 43)->iValue;
                
                std::string str;
                char v51;
                std::allocator<char>::allocator(&v51);
                const CDBCFile::FIELD *v33 = CDBCFile::Search_Posistion(&ItemFile, i_1, 44);
                std::string::string(&str, v33->pString, &v51);
                pItem->RongHeReceovery = CfgData::paraseParam2(this, &str);
                str.~string();
                std::allocator<char>::~allocator(&v51);
                
                auto it_1 = this->m_items.find(pItem->id);
                if (it_1 == this->m_items.end())
                {
                    std::pair<int, CfgItem *> v54(pItem->id, pItem);
                    std::map<int, CfgItem *>::insert(&newItems, &v54);
                }
                
                auto *v34 = std::map<int, CfgItem *>::operator[](&this->m_items, &pItem->id);
                *v34 = pItem;
            }
        }
    }
    CDBCFile::~CDBCFile(&ItemFile);
    
    Answer::RwLock::unlock(&this->m_itemsLock);
    
    if (!std::map<int, CfgItem *>::empty(&newItems) && bSend)
    {
        CfgData::sendNewItems(this, &newItems);
    }
    
    newItems.~map();
    
    // 加载物品礼包
    Answer::RwLock::wrlock(&this->m_itemGiftsLock);
    this->m_itemGifts.clear();
    
    CDBCFile ItemGiftFile;
    CDBCFile::CDBCFile(&ItemGiftFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&ItemGiftFile, "./ServerConfig/Tables/cfg_item_gift.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_item_gift.txt failed,please check!!");
    }
    else
    {
        int32_t iBaseTableCount_Gift = CDBCFile::GetRecordsNum(&ItemGiftFile);
        int32_t iBaseColumnCount_Gift = CDBCFile::GetFieldsNum(&ItemGiftFile);
        
        if (iBaseColumnCount_Gift > 0)
        {
            for (int32_t i_0 = 0; i_0 < iBaseTableCount_Gift; ++i_0)
            {
                CfgItemGift itemGift;
                memset(&itemGift, 0, sizeof(itemGift));
                itemGift.id = CDBCFile::Search_Posistion(&ItemGiftFile, i_0, 0)->iValue;
                itemGift.item = CDBCFile::Search_Posistion(&ItemGiftFile, i_0, 2)->iValue;
                itemGift.type = CDBCFile::Search_Posistion(&ItemGiftFile, i_0, 3)->iValue;
                itemGift.count = CDBCFile::Search_Posistion(&ItemGiftFile, i_0, 4)->iValue;
                itemGift.bind = CDBCFile::Search_Posistion(&ItemGiftFile, i_0, 5)->iValue;
                itemGift.job = CDBCFile::Search_Posistion(&ItemGiftFile, i_0, 6)->iValue;
                itemGift.time = CDBCFile::Search_Posistion(&ItemGiftFile, i_0, 7)->iValue;
                
                auto it_0 = this->m_itemGifts.find(itemGift.id);
                if (it_0 == this->m_itemGifts.end())
                {
                    CfgItemGiftVector *v36 = new std::vector<CfgItemGift>();
                    *std::map<int, CfgItemGiftVector *>::operator[](&this->m_itemGifts, &itemGift.id) = v36;
                }
                
                auto v37 = std::map<int, CfgItemGiftVector *>::operator[](&this->m_itemGifts, &itemGift.id);
                v37->push_back(itemGift);
            }
        }
    }
    CDBCFile::~CDBCFile(&ItemGiftFile);
    Answer::RwLock::unlock(&this->m_itemGiftsLock);
    
    // 加载随机物品礼包
    Answer::RwLock::wrlock(&this->m_itemGiftRandomsLock);
    this->m_itemGiftRandoms.clear();
    
    CDBCFile ItemGiftRandFile;
    CDBCFile::CDBCFile(&ItemGiftRandFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&ItemGiftRandFile, "./ServerConfig/Tables/cfg_item_gift_random.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_item_gift_random.txt failed,please check!!");
    }
    else
    {
        int32_t iBaseTableCount_GiftRand = CDBCFile::GetRecordsNum(&ItemGiftRandFile);
        int32_t iBaseColumnCount_GiftRand = CDBCFile::GetFieldsNum(&ItemGiftRandFile);
        
        if (iBaseColumnCount_GiftRand > 0)
        {
            for (int32_t i = 0; i < iBaseTableCount_GiftRand; ++i)
            {
                CfgItemGiftRandom itemGiftRandom;
                itemGiftRandom.id = CDBCFile::Search_Posistion(&ItemGiftRandFile, i, 0)->iValue;
                itemGiftRandom.item = CDBCFile::Search_Posistion(&ItemGiftRandFile, i, 1)->iValue;
                itemGiftRandom.type = CDBCFile::Search_Posistion(&ItemGiftRandFile, i, 2)->iValue;
                itemGiftRandom.count = CDBCFile::Search_Posistion(&ItemGiftRandFile, i, 3)->iValue;
                itemGiftRandom.bind = CDBCFile::Search_Posistion(&ItemGiftRandFile, i, 4)->iValue;
                itemGiftRandom.static_probability = CDBCFile::Search_Posistion(&ItemGiftRandFile, i, 5)->iValue;
                itemGiftRandom.sum_probability = CDBCFile::Search_Posistion(&ItemGiftRandFile, i, 6)->iValue;
                itemGiftRandom.job = CDBCFile::Search_Posistion(&ItemGiftRandFile, i, 7)->iValue;
                itemGiftRandom.MinLevel = CDBCFile::Search_Posistion(&ItemGiftRandFile, i, 8)->iValue;
                itemGiftRandom.MaxLevel = CDBCFile::Search_Posistion(&ItemGiftRandFile, i, 9)->iValue;
                
                auto it = this->m_itemGiftRandoms.find(itemGiftRandom.id);
                if (it == this->m_itemGiftRandoms.end())
                {
                    CfgItemGiftRandomVector *v38 = new std::vector<CfgItemGiftRandom>();
                    *std::map<int, CfgItemGiftRandomVector *>::operator[](&this->m_itemGiftRandoms, &itemGiftRandom.id) = v38;
                }
                
                auto v39 = std::map<int, CfgItemGiftRandomVector *>::operator[](&this->m_itemGiftRandoms, &itemGiftRandom.id);
                v39->push_back(itemGiftRandom);
            }
        }
    }
    CDBCFile::~CDBCFile(&ItemGiftRandFile);
    Answer::RwLock::unlock(&this->m_itemGiftRandomsLock);
}

void CfgData::sendNewItems(CfgData *const this, const CfgItemTable *const items)
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
        packet->writeUTF8(&pCfgItem->name);
        packet->writeUTF8(&pCfgItem->desc);
        packet->writeInt32(pCfgItem->type);
        packet->writeInt32(pCfgItem->level);
        packet->writeInt32(pCfgItem->job);
        packet->writeInt32(pCfgItem->in_value);
        packet->writeInt32(pCfgItem->out_value);
        packet->writeInt32(pCfgItem->bind);
        packet->writeInt32(pCfgItem->combine);
        packet->writeInt32(pCfgItem->quality);
        packet->writeUTF8(&pCfgItem->url);
        packet->writeUTF8(&pCfgItem->drop_url);
        packet->writeUTF8(&pCfgItem->effect);
        packet->writeUTF8(&pCfgItem->use_method);
        packet->writeInt32(pCfgItem->group_id);
        packet->writeInt32(pCfgItem->cd_group);
        packet->writeInt32(pCfgItem->overlay);
        packet->writeInt32(pCfgItem->can_sell);
        packet->writeInt32(pCfgItem->valid_time);
    }
    
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    
    GameService *v5 = Answer::Singleton<GameService>::instance();
    GameService::broadcast(v5, packet);
}

void CfgData::fetchServerConfig(CfgData *const this, int32_t line)
{
    if (line == 9) return;
    
    char szSQL[4096];
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL) - 1, "SELECT * FROM `sys_server_start`");
    
    Answer::DBPool *v2 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db;
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v2);
    
    Answer::MySqlQuery *v3 = Answer::MySqlDBGuard::query(&db, szSQL);
    Answer::MySqlQuery result;
    Answer::MySqlQuery::MySqlQuery(&result, v3);
    
    if (!Answer::MySqlQuery::eof(&result))
    {
        this->m_heFuTime = Answer::MySqlQuery::getIntValue(&result, "he_fu_time", 0);
        this->m_kaiFuTime = Answer::MySqlQuery::getIntValue(&result, "kai_fu_time", 0);
        this->m_serverType = Answer::MySqlQuery::getIntValue(&result, "type", 0);
    }
    
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

int32_t CfgData::getServerStartTime(CfgData *const this, SERVER_TYPE nType)
{
    if (nType == SERVER_TYPE::SVT_UNITE)
        return this->m_heFuTime;
    else
        return this->m_kaiFuTime;
}

int32_t CfgData::getServerStartDayZeroTime(CfgData *const this, SERVER_TYPE nType)
{
    if (nType == SERVER_TYPE::SVT_UNITE)
        return Answer::DayTime::dayzero(this->m_heFuTime);
    else
        return Answer::DayTime::dayzero(this->m_kaiFuTime);
}

int32_t CfgData::getServerDiffDay(CfgData *const this, SERVER_TYPE nType)
{
    if (nType == SERVER_TYPE::SVT_UNITE)
        return Answer::DayTime::daydiff(this->m_heFuTime);
    else
        return Answer::DayTime::daydiff(this->m_kaiFuTime);
}

void CfgData::updateServerStartTime(CfgData *const this, int32_t kaiFuTime)
{
    if (CfgData::getServerType(this) != 0) return;
    
    this->m_kaiFuTime = kaiFuTime;
    COpenBeta *v2 = Answer::Singleton<COpenBeta>::instance();
    COpenBeta::updateStartTime(v2);
}
void CfgData::fetchDungeonEvent(CfgData *const this)
{
    CDBCFile DungeonEventFile;
    CDBCFile::CDBCFile(&DungeonEventFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&DungeonEventFile, "./ServerConfig/Tables/cfg_dungeon_event.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_dungeon_event.txt failed,please check!!");
        CDBCFile::~CDBCFile(&DungeonEventFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&DungeonEventFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&DungeonEventFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMapEvent dungeonEvent;
            CfgMapEvent::CfgMapEvent(&dungeonEvent);
            
            dungeonEvent.id = CDBCFile::Search_Posistion(&DungeonEventFile, i, 0)->iValue;
            dungeonEvent.trigger_id = CDBCFile::Search_Posistion(&DungeonEventFile, i, 1)->iValue;
            dungeonEvent.bInitOpen = CDBCFile::Search_Posistion(&DungeonEventFile, i, 2)->iValue == 0;
            dungeonEvent.trigger_type = CDBCFile::Search_Posistion(&DungeonEventFile, i, 3)->iValue;
            
            std::string triggerParam;
            char v12;
            std::allocator<char>::allocator(&v12);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&DungeonEventFile, i, 4);
            std::string::string(&triggerParam, v1->pString, &v12);
            std::allocator<char>::~allocator(&v12);
            
            dungeonEvent.event_type = CDBCFile::Search_Posistion(&DungeonEventFile, i, 5)->iValue;
            
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&DungeonEventFile, i, 6);
            std::string::operator=(&dungeonEvent.effect, v2->pString);
            dungeonEvent.repeate = CDBCFile::Search_Posistion(&DungeonEventFile, i, 7)->iValue;
            
            std::string delims;
            std::string str;
            char v14;
            std::allocator<char>::allocator(&v14);
            std::string::string(&delims, ":", &v14);
            
            char v16;
            std::allocator<char>::allocator(&v16);
            std::string::string(&str, triggerParam.c_str(), &v16);
            
            StringVector strTriggerParam;
            Answer::StringUtility::split(&strTriggerParam, &str, &delims, 0);
            str.~string();
            std::allocator<char>::~allocator(&v16);
            delims.~string();
            std::allocator<char>::~allocator(&v14);
            
            for (auto& param : strTriggerParam)
            {
                int val = atoi(param.c_str());
                dungeonEvent.trigger_param.push_back(val);
            }
            strTriggerParam.~vector();
            triggerParam.~string();
            
            if (!dungeonEvent.trigger_param.empty())
            {
                auto v6 = std::map<int, std::list<CfgMapEvent>>::operator[](&this->m_dungeonEvents, &dungeonEvent.id);
                v6->push_back(dungeonEvent);
            }
            
            dungeonEvent.~CfgMapEvent();
        }
    }
    CDBCFile::~CDBCFile(&DungeonEventFile);
}

void CfgData::fetchDungeonMonster(CfgData *const this)
{
    CDBCFile DungeonMonsterFile;
    CDBCFile::CDBCFile(&DungeonMonsterFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&DungeonMonsterFile, "./ServerConfig/Tables/cfg_dungeon_monster.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_dungeon_monster.txt failed,please check!!");
        CDBCFile::~CDBCFile(&DungeonMonsterFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&DungeonMonsterFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&DungeonMonsterFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgDungeonMonster monster;
            CfgDungeonMonster::CfgDungeonMonster(&monster);
            
            monster.id = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 0)->iValue;
            monster.wave = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 1)->iValue;
            
            std::string nIndex;
            char v32;
            std::allocator<char>::allocator(&v32);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 2);
            std::string::string(&nIndex, v2->pString, &v32);
            
            std::list<Param2> __x;
            CfgData::paraseParam2List((CfgData *const)&__x, this, atoi(nIndex.c_str()), nullptr);
            monster.mids = __x;
            __x.~list();
            nIndex.~string();
            std::allocator<char>::~allocator(&v32);
            
            monster.x = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 3)->iValue;
            monster.y = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 4)->iValue;
            monster.count = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 5)->iValue;
            monster.side = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 6)->iValue;
            
            std::string road;
            char v35;
            std::allocator<char>::allocator(&v35);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 7);
            std::string::string(&road, v3->pString, &v35);
            std::allocator<char>::~allocator(&v35);
            
            monster.delay = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 8)->iValue;
            monster.times = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 9)->iValue;
            monster.money = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 10)->iValue;
            monster.life = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 11)->iValue;
            
            std::string randpos;
            char v36;
            std::allocator<char>::allocator(&v36);
            const CDBCFile::FIELD *v4 = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 12);
            std::string::string(&randpos, v4->pString, &v36);
            std::allocator<char>::~allocator(&v36);
            
            monster.wait = CDBCFile::Search_Posistion(&DungeonMonsterFile, i, 13)->iValue;
            
            // 解析路线
            if (road.size() > 3)
            {
                std::string delims;
                char v38;
                std::allocator<char>::allocator(&v38);
                std::string::string(&delims, ":", &v38);
                
                StringVector vRoad;
                Answer::StringUtility::split(&vRoad, &road, &delims, 0);
                delims.~string();
                std::allocator<char>::~allocator(&v38);
                
                for (size_t j = 0; j < vRoad.size(); ++j)
                {
                    std::string v39;
                    char v40;
                    std::allocator<char>::allocator(&v40);
                    std::string::string(&v39, ",", &v40);
                    
                    StringVector vPos;
                    Answer::StringUtility::split(&vPos, vRoad[j], &v39, 0);
                    v39.~string();
                    std::allocator<char>::~allocator(&v40);
                    
                    if (vPos.size() == 2)
                    {
                        Position pos;
                        pos.x = atoi(vPos[0]->c_str());
                        pos.y = atoi(vPos[1]->c_str());
                        monster.road.push_back(pos);
                    }
                    vPos.~vector();
                }
                vRoad.~vector();
            }
            
            // 解析随机位置
            if (randpos.size() > 3)
            {
                std::string v42;
                char v43;
                std::allocator<char>::allocator(&v43);
                std::string::string(&v42, "|", &v43);
                
                StringVector vRandPos;
                Answer::StringUtility::split(&vRandPos, &randpos, &v42, 0);
                v42.~string();
                std::allocator<char>::~allocator(&v43);
                
                for (size_t j_0 = 0; j_0 < vRandPos.size(); ++j_0)
                {
                    std::string v44;
                    char v45;
                    std::allocator<char>::allocator(&v45);
                    std::string::string(&v44, ":", &v45);
                    
                    StringVector vPos_0;
                    Answer::StringUtility::split(&vPos_0, vRandPos[j_0], &v44, 0);
                    v44.~string();
                    std::allocator<char>::~allocator(&v45);
                    
                    if (vPos_0.size() == 2)
                    {
                        Position pos;
                        pos.x = atoi(vPos_0[0]->c_str());
                        pos.y = atoi(vPos_0[1]->c_str());
                        monster.randpos.push_back(pos);
                    }
                    vPos_0.~vector();
                }
                vRandPos.~vector();
            }
            
            auto v21 = std::map<int, CfgDungeonMonster>::operator[](&this->m_dungeonMonsters, &monster.id);
            *v21 = monster;
            
            randpos.~string();
            road.~string();
            monster.~CfgDungeonMonster();
        }
    }
    CDBCFile::~CDBCFile(&DungeonMonsterFile);
}

void CfgData::fetchMap(CfgData *const this)
{
    CDBCFile MapFile;
    CDBCFile::CDBCFile(&MapFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&MapFile, "./ServerConfig/Tables/cfg_map.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_map.txt failed,please check!!");
        CDBCFile::~CDBCFile(&MapFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&MapFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&MapFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMap map;
            CfgMap::CfgMap(&map);
            
            map.id = CDBCFile::Search_Posistion(&MapFile, i, 0)->iValue;
            
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&MapFile, i, 1);
            std::string::operator=(&map.name, v1->pString);
            
            map.type = CDBCFile::Search_Posistion(&MapFile, i, 2)->iValue;
            map.param = CDBCFile::Search_Posistion(&MapFile, i, 3)->iValue;
            map.width = CDBCFile::Search_Posistion(&MapFile, i, 4)->iValue;
            map.height = CDBCFile::Search_Posistion(&MapFile, i, 5)->iValue;
            map.revive = CDBCFile::Search_Posistion(&MapFile, i, 6)->iValue;
            map.pk_mode = CDBCFile::Search_Posistion(&MapFile, i, 7)->iValue;
            map.anti_protect = CDBCFile::Search_Posistion(&MapFile, i, 8)->iValue;
            map.isMount = CDBCFile::Search_Posistion(&MapFile, i, 9)->iValue;
            map.isFly = CDBCFile::Search_Posistion(&MapFile, i, 10)->iValue;
            map.isVicegeneral = CDBCFile::Search_Posistion(&MapFile, i, 11)->iValue;
            map.player_level = CDBCFile::Search_Posistion(&MapFile, i, 12)->iValue;
            map.player_level_max = CDBCFile::Search_Posistion(&MapFile, i, 13)->iValue;
            map.team_member = CDBCFile::Search_Posistion(&MapFile, i, 14)->iValue;
            map.hide_mini = CDBCFile::Search_Posistion(&MapFile, i, 15)->iValue;
            map.runnerId = CDBCFile::Search_Posistion(&MapFile, i, 23)->iValue;
            map.jump = CDBCFile::Search_Posistion(&MapFile, i, 30)->iValue;
            map.GongGaoId = CDBCFile::Search_Posistion(&MapFile, i, 33)->iValue;
            map.reviveTime = CDBCFile::Search_Posistion(&MapFile, i, 35)->iValue;
            map.pvp = CDBCFile::Search_Posistion(&MapFile, i, 36)->iValue;
            map.cross = CDBCFile::Search_Posistion(&MapFile, i, 38)->iValue;
            map.x = CDBCFile::Search_Posistion(&MapFile, i, 44)->iValue;
            map.y = CDBCFile::Search_Posistion(&MapFile, i, 45)->iValue;
            map.drop_rate = CDBCFile::Search_Posistion(&MapFile, i, 47)->iValue;
            map.XingMaiLevel = CDBCFile::Search_Posistion(&MapFile, i, 48)->iValue;
            map.SuitId = CDBCFile::Search_Posistion(&MapFile, i, 49)->iValue;
            map.JueWeiLevel = CDBCFile::Search_Posistion(&MapFile, i, 50)->iValue;
            map.VipLevel = CDBCFile::Search_Posistion(&MapFile, i, 51)->iValue;
            map.NeedBroadcast = CDBCFile::Search_Posistion(&MapFile, i, 54)->iValue;
            map.MapCanUsePet = CDBCFile::Search_Posistion(&MapFile, i, 55)->iValue;
            map.SignOut = CDBCFile::Search_Posistion(&MapFile, i, 56)->iValue;
            map.Rdx = CDBCFile::Search_Posistion(&MapFile, i, 58)->iValue;
            map.Refresh = CDBCFile::Search_Posistion(&MapFile, i, 60)->iValue;
            map.CanRand = CDBCFile::Search_Posistion(&MapFile, i, 61)->iValue;
            map.LimitDay = CDBCFile::Search_Posistion(&MapFile, i, 62)->iValue;
            map.XinMoAct = CDBCFile::Search_Posistion(&MapFile, i, 63)->iValue;
            
            auto v2 = std::map<int, CfgMap>::operator[](&this->m_maps, &map.id);
            *v2 = map;
            map.~CfgMap();
        }
    }
    CDBCFile::~CDBCFile(&MapFile);
}

void CfgData::fetchMapMonster(CfgData *const this)
{
    CDBCFile MapMonsterFile;
    CDBCFile::CDBCFile(&MapMonsterFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&MapMonsterFile, "./ServerConfig/Tables/cfg_map_monster.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_map_monster.txt failed,please check!!");
        CDBCFile::~CDBCFile(&MapMonsterFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&MapMonsterFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&MapMonsterFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMapMonster mapmonster;
            memset(&mapmonster, 0, sizeof(mapmonster));
            mapmonster.id = CDBCFile::Search_Posistion(&MapMonsterFile, i, 0)->iValue;
            mapmonster.mapid = CDBCFile::Search_Posistion(&MapMonsterFile, i, 1)->iValue;
            mapmonster.monsterid = CDBCFile::Search_Posistion(&MapMonsterFile, i, 2)->iValue;
            mapmonster.x = CDBCFile::Search_Posistion(&MapMonsterFile, i, 3)->iValue;
            mapmonster.y = CDBCFile::Search_Posistion(&MapMonsterFile, i, 4)->iValue;
            mapmonster.hide = CDBCFile::Search_Posistion(&MapMonsterFile, i, 5)->iValue;
            mapmonster.side = CDBCFile::Search_Posistion(&MapMonsterFile, i, 6)->iValue;
            mapmonster.boss = CDBCFile::Search_Posistion(&MapMonsterFile, i, 7)->iValue;
            mapmonster.Day = CDBCFile::Search_Posistion(&MapMonsterFile, i, 8)->iValue;
            
            auto v1 = std::map<int, std::vector<CfgMapMonster>>::operator[](&this->m_mapMonsters, &mapmonster.mapid);
            v1->push_back(mapmonster);
            
            auto v2 = std::map<int, CfgMapMonster>::operator[](&this->m_CfgMapMonsters, &mapmonster.id);
            *v2 = mapmonster;
        }
    }
    CDBCFile::~CDBCFile(&MapMonsterFile);
}

void CfgData::fetchMapPlant(CfgData *const this)
{
    CDBCFile MapPlantFile;
    CDBCFile::CDBCFile(&MapPlantFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&MapPlantFile, "./ServerConfig/Tables/cfg_map_plant.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_map_plant.txt failed,please check!!");
        CDBCFile::~CDBCFile(&MapPlantFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&MapPlantFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&MapPlantFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            int32_t id = CDBCFile::Search_Posistion(&MapPlantFile, i, 0)->iValue;
            CfgMapPlant mapPlant;
            mapPlant.mapid = CDBCFile::Search_Posistion(&MapPlantFile, i, 1)->iValue;
            mapPlant.plantid = CDBCFile::Search_Posistion(&MapPlantFile, i, 2)->iValue;
            mapPlant.x = CDBCFile::Search_Posistion(&MapPlantFile, i, 3)->iValue;
            mapPlant.y = CDBCFile::Search_Posistion(&MapPlantFile, i, 4)->iValue;
            
            auto v1 = std::map<int, std::vector<CfgMapPlant>>::operator[](&this->m_mapPlants, &mapPlant.mapid);
            v1->push_back(mapPlant);
            
            auto v2 = std::map<int, CfgMapPlant>::operator[](&this->m_mMapPlants, &id);
            *v2 = mapPlant;
        }
    }
    CDBCFile::~CDBCFile(&MapPlantFile);
}

void CfgData::fetchMapRegion(CfgData *const this)
{
    CDBCFile MapRegionFile;
    CDBCFile::CDBCFile(&MapRegionFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&MapRegionFile, "./ServerConfig/Tables/cfg_map_region.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_map_region.txt failed,please check!!");
        CDBCFile::~CDBCFile(&MapRegionFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&MapRegionFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&MapRegionFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMapRegion mapRegion;
            mapRegion.id = CDBCFile::Search_Posistion(&MapRegionFile, i, 0)->iValue;
            mapRegion.mapid = CDBCFile::Search_Posistion(&MapRegionFile, i, 2)->iValue;
            mapRegion.min_x = CDBCFile::Search_Posistion(&MapRegionFile, i, 3)->iValue;
            mapRegion.min_y = CDBCFile::Search_Posistion(&MapRegionFile, i, 4)->iValue;
            mapRegion.max_x = CDBCFile::Search_Posistion(&MapRegionFile, i, 5)->iValue;
            mapRegion.max_y = CDBCFile::Search_Posistion(&MapRegionFile, i, 6)->iValue;
            mapRegion.type = CDBCFile::Search_Posistion(&MapRegionFile, i, 7)->iValue;
            mapRegion.mId = CDBCFile::Search_Posistion(&MapRegionFile, i, 9)->iValue;
            
            auto v1 = std::map<int, CfgMapRegion>::operator[](&this->m_mapRegions, &mapRegion.id);
            *v1 = mapRegion;
            
            auto v2 = std::map<int, std::vector<CfgMapRegion>>::operator[](&this->m_mapRegionsByMapId, &mapRegion.mapid);
            v2->push_back(mapRegion);
        }
    }
    CDBCFile::~CDBCFile(&MapRegionFile);
}

void CfgData::fetchMonster(CfgData *const this)
{
    CDBCFile MonsterFile;
    CDBCFile::CDBCFile(&MonsterFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&MonsterFile, "./ServerConfig/Tables/Monster.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MONSTER_TABLE failed,please check!!");
        CDBCFile::~CDBCFile(&MonsterFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&MonsterFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&MonsterFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMonster monster{};
            
            int32_t nIndex = 0;
            monster.mid = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            monster.group_id = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.level = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.quality = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            
            for (int j = 1; j <= 28; ++j)
            {
                monster.vAttr[j] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            }
            
            monster.rand_count = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string randtypes;
            char v19;
            std::allocator<char>::allocator(&v19);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex);
            std::string::string(&randtypes, v1->pString, &v19);
            std::allocator<char>::~allocator(&v19);
            
            monster.exp = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.type = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.revive_time = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.skill_id = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string strSkill;
            char v21;
            std::allocator<char>::allocator(&v21);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex);
            std::string::string(&strSkill, v2->pString, &v21);
            CfgData::parseMonsterSkill(this, monster.mid, &monster.unique_skill, &strSkill);
            strSkill.~string();
            std::allocator<char>::~allocator(&v21);
            ++nIndex;
            
            std::string v22;
            char v23;
            std::allocator<char>::allocator(&v23);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex);
            std::string::string(&v22, v3->pString, &v23);
            CfgData::parseMonsterSkill(this, monster.mid, &monster.random_skill, &v22);
            v22.~string();
            std::allocator<char>::~allocator(&v23);
            
            monster.hpPercent = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.ai = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            nIndex += 9;
            monster.broadcast = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            monster.drop_free = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            nIndex += 2;
            monster.boss_sign = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            nIndex += 5;
            monster.BossSocre = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            monster.TaskShare = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            nIndex += 4;
            monster.camp = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            nIndex += 2;
            monster.kill_point = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            monster.revive_skin = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            ++nIndex;
            monster.PortalRatio = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.PortalId = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.PortalDuration = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            monster.prestige = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.dust = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.is_building = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.PureDamage = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            monster.IfCurse = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            monster.Score = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.IsShowOwner = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            nIndex += 2;
            monster.DiligenceType = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            monster.DiligenceValue = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex++)->iValue;
            
            const CDBCFile::FIELD *v4 = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex);
            monster.MonsterHp = strtoll(v4->pString, nullptr, 10);
            monster.CanTunshi = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.Mid2 = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.FamilyValue = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            monster.vAttr[41] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.vAttr[42] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.vAttr[43] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.vAttr[44] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.IsXinMoMonster = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.QieGe = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.vAttr[49] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.Energy = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string strItem;
            char v25;
            std::allocator<char>::allocator(&v25);
            const CDBCFile::FIELD *v5 = CDBCFile::Search_Posistion(&MonsterFile, i, nIndex);
            std::string::string(&strItem, v5->pString, &v25);
            CItemHelper::parseItemString(&monster.cItem, &strItem);
            strItem.~string();
            std::allocator<char>::~allocator(&v25);
            
            monster.IsSunAndMoon = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            monster.corpse_time = 3000;
            
            // 解析随机类型
            if (!randtypes.empty() && randtypes != "-1")
            {
                std::string delims;
                char v27;
                std::allocator<char>::allocator(&v27);
                std::string::string(&delims, ":", &v27);
                
                StringVector vRand;
                Answer::StringUtility::split(&vRand, &randtypes, &delims, 0);
                delims.~string();
                std::allocator<char>::~allocator(&v27);
                
                for (auto& randStr : vRand)
                {
                    int val = atoi(randStr->c_str());
                    monster.rand_types.push_back(val);
                }
                vRand.~vector();
            }
            
            auto v9 = std::map<int, CfgMonster>::operator[](&this->m_monsters, &monster.mid);
            *v9 = monster;
            
            randtypes.~string();
            monster.~CfgMonster();
        }
        
        // 加载怪物广播列表
        CDBCFile MonsterBroadcastFile;
        CDBCFile::CDBCFile(&MonsterBroadcastFile, 0);
        
        if (CDBCFile::OpenFromTXT(&MonsterBroadcastFile, "./ServerConfig/Tables/cfg_monster_broadcast.txt"))
        {
            int32_t iBaseTableCountBroadcast = CDBCFile::GetRecordsNum(&MonsterBroadcastFile);
            int32_t iBaseColumnCountBroadcast = CDBCFile::GetFieldsNum(&MonsterBroadcastFile);
            
            if (iBaseColumnCountBroadcast > 0)
            {
                for (int32_t i_0 = 0; i_0 < iBaseTableCountBroadcast; ++i_0)
                {
                    const CDBCFile::FIELD *v10 = CDBCFile::Search_Posistion(&MonsterBroadcastFile, i_0, 1);
                    this->m_monsterBroadcasts.push_back(v10->iValue);
                }
            }
            CDBCFile::~CDBCFile(&MonsterBroadcastFile);
        }
    }
    CDBCFile::~CDBCFile(&MonsterFile);
}
void CfgData::fetchNpc(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    bool ret = CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_npc.txt");
    
    if (!ret)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_npc.txt failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgNpc npc;
            CfgNpc::CfgNpc(&npc);
            
            npc.id = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            npc.npcid = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
            
            // 解析地图列表
            std::string delims;
            std::string str;
            char v43;
            std::allocator<char>::allocator(&v43);
            std::string::string(&delims, "|", &v43);
            
            char v45;
            std::allocator<char>::allocator(&v45);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 6);
            std::string::string(&str, v1->pString, &v45);
            
            StringVector mapids;
            Answer::StringUtility::split(&mapids, &str, &delims, 0);
            str.~string();
            std::allocator<char>::~allocator(&v45);
            delims.~string();
            std::allocator<char>::~allocator(&v43);
            
            for (auto& mapidStr : mapids)
            {
                int val = atoi(mapidStr->c_str());
                npc.mapids.push_back(val);
            }
            mapids.~vector();
            
            npc.x = CDBCFile::Search_Posistion(&TabFile, i, 7)->iValue;
            npc.y = CDBCFile::Search_Posistion(&TabFile, i, 8)->iValue;
            npc.func = CDBCFile::Search_Posistion(&TabFile, i, 9)->iValue;
            npc.func_extra = CDBCFile::Search_Posistion(&TabFile, i, 10)->iValue;
            
            std::string param;
            char v48;
            std::allocator<char>::allocator(&v48);
            const CDBCFile::FIELD *v4 = CDBCFile::Search_Posistion(&TabFile, i, 21);
            std::string::string(&param, v4->pString, &v48);
            std::allocator<char>::~allocator(&v48);
            
            std::string v49;
            char v50;
            std::allocator<char>::allocator(&v50);
            std::string::string(&v49, ":", &v50);
            
            StringVector vparam;
            Answer::StringUtility::split(&vparam, &param, &v49, 0);
            v49.~string();
            std::allocator<char>::~allocator(&v50);
            
            for (size_t j = 0; j < vparam.size(); ++j)
            {
                int val = atoi(vparam[j]->c_str());
                npc.params.push_back(val);
            }
            vparam.~vector();
            param.~string();
            
            npc.unite_flag = CDBCFile::Search_Posistion(&TabFile, i, 23)->iValue;
            
            std::string platform;
            char v52;
            std::allocator<char>::allocator(&v52);
            const CDBCFile::FIELD *v8 = CDBCFile::Search_Posistion(&TabFile, i, 25);
            std::string::string(&platform, v8->pString, &v52);
            std::allocator<char>::~allocator(&v52);
            
            if (platform != "0")
            {
                std::string v53;
                char v54;
                std::allocator<char>::allocator(&v54);
                std::string::string(&v53, "|", &v54);
                
                StringVector vpf;
                Answer::StringUtility::split(&vpf, &platform, &v53, 0);
                v53.~string();
                std::allocator<char>::~allocator(&v54);
                
                for (auto& pf : vpf)
                {
                    npc.platforms.push_back(pf);
                }
                vpf.~vector();
            }
            platform.~string();
            
            // 解析消耗物品
            std::string v55;
            char v56;
            std::allocator<char>::allocator(&v56);
            std::string::string(&v55, "|", &v56);
            
            char v58;
            std::allocator<char>::allocator(&v58);
            const CDBCFile::FIELD *v11 = CDBCFile::Search_Posistion(&TabFile, i, 27);
            std::string v57;
            std::string::string(&v57, v11->pString, &v58);
            
            StringVector CostVector;
            Answer::StringUtility::split(&CostVector, &v57, &v55, 0);
            v57.~string();
            std::allocator<char>::~allocator(&v58);
            v55.~string();
            std::allocator<char>::~allocator(&v56);
            
            for (auto& costStr : CostVector)
            {
                std::string v60;
                char v61;
                std::allocator<char>::allocator(&v61);
                std::string::string(&v60, ":", &v61);
                
                StringVector CostItem;
                Answer::StringUtility::split(&CostItem, costStr, &v60, 0);
                v60.~string();
                std::allocator<char>::~allocator(&v61);
                
                if (CostItem.size() == 3)
                {
                    CfgDungeonNpcCost NpcCost;
                    NpcCost.ItemType = atoi(CostItem[0]->c_str());
                    NpcCost.ItemId = atoi(CostItem[1]->c_str());
                    NpcCost.ItemCount = atoi(CostItem[2]->c_str());
                    npc.m_vNpcCost.push_back(NpcCost);
                }
                CostItem.~vector();
            }
            CostVector.~vector();
            
            auto v19 = std::map<int, CfgNpc>::operator[](&this->m_npcs, &npc.id);
            *v19 = npc;
            npc.~CfgNpc();
        }
        
        // 加载NPC机场
        CDBCFile TabFileAir;
        CDBCFile::CDBCFile(&TabFileAir, 0);
        ret = CDBCFile::OpenFromTXT(&TabFileAir, "./ServerConfig/Tables/cfg_npc_airport.txt");
        
        if (ret)
        {
            iBaseTableCount = CDBCFile::GetRecordsNum(&TabFileAir);
            iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFileAir);
            
            if (iBaseColumnCount > 0)
            {
                for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)
                {
                    CfgNpcAirport airport;
                    airport.id = CDBCFile::Search_Posistion(&TabFileAir, i_0, 0)->iValue;
                    airport.npcid = CDBCFile::Search_Posistion(&TabFileAir, i_0, 1)->iValue;
                    airport.player_kingdom = CDBCFile::Search_Posistion(&TabFileAir, i_0, 3)->iValue;
                    airport.kingdom_id = CDBCFile::Search_Posistion(&TabFileAir, i_0, 4)->iValue;
                    airport.map_id = CDBCFile::Search_Posistion(&TabFileAir, i_0, 5)->iValue;
                    airport.map_x = CDBCFile::Search_Posistion(&TabFileAir, i_0, 6)->iValue;
                    airport.map_y = CDBCFile::Search_Posistion(&TabFileAir, i_0, 7)->iValue;
                    airport.cost = CDBCFile::Search_Posistion(&TabFileAir, i_0, 8)->iValue;
                    
                    auto v20 = std::map<int, CfgNpcAirport>::operator[](&this->m_npcAirports, &airport.id);
                    *v20 = airport;
                }
            }
            CDBCFile::~CDBCFile(&TabFileAir);
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::fetchPlant(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_plant.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_plant.txt failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgPlant plant{};
            
            plant.id = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            plant.type = CDBCFile::Search_Posistion(&TabFile, i, 3)->iValue;
            plant.level = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
            
            std::string delims;
            std::string str;
            char v22;
            std::allocator<char>::allocator(&v22);
            std::string::string(&delims, "|", &v22);
            
            char v24;
            std::allocator<char>::allocator(&v24);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 5);
            std::string::string(&str, v1->pString, &v24);
            
            StringVector strItems;
            Answer::StringUtility::split(&strItems, &str, &delims, 0);
            str.~string();
            std::allocator<char>::~allocator(&v24);
            delims.~string();
            std::allocator<char>::~allocator(&v22);
            
            for (auto& eventStr : strItems)
            {
                std::string v26;
                char v27;
                std::allocator<char>::allocator(&v27);
                std::string::string(&v26, ":", &v27);
                
                StringVector EventVt;
                Answer::StringUtility::split(&EventVt, eventStr, &v26, 0);
                v26.~string();
                std::allocator<char>::~allocator(&v27);
                
                if (EventVt.size() == 2)
                {
                    CfgPlantEvent Event;
                    Event.EventId = atoi(EventVt[0]->c_str());
                    Event.Probability = atoi(EventVt[1]->c_str());
                    plant.EventMaxRate += Event.Probability;
                    plant.Events.push_back(Event);
                }
                EventVt.~vector();
            }
            strItems.~vector();
            
            plant.item_cost = CDBCFile::Search_Posistion(&TabFile, i, 6)->iValue;
            plant.start_hour = CDBCFile::Search_Posistion(&TabFile, i, 7)->iValue;
            plant.end_hour = CDBCFile::Search_Posistion(&TabFile, i, 8)->iValue;
            plant.gather_time = CDBCFile::Search_Posistion(&TabFile, i, 9)->iValue;
            plant.revive_time = CDBCFile::Search_Posistion(&TabFile, i, 10)->iValue;
            plant.hide_time = CDBCFile::Search_Posistion(&TabFile, i, 16)->iValue;
            plant.get_points = CDBCFile::Search_Posistion(&TabFile, i, 18)->iValue;
            plant.boss_id = CDBCFile::Search_Posistion(&TabFile, i, 19)->iValue;
            plant.BuffId = CDBCFile::Search_Posistion(&TabFile, i, 20)->iValue;
            plant.revive_skin = CDBCFile::Search_Posistion(&TabFile, i, 21)->iValue;
            plant.TaskId = CDBCFile::Search_Posistion(&TabFile, i, 22)->iValue;
            
            std::string v28;
            char v29;
            std::allocator<char>::allocator(&v29);
            std::string::string(&v28, "|", &v29);
            
            char v31;
            std::allocator<char>::allocator(&v31);
            const CDBCFile::FIELD *v7 = CDBCFile::Search_Posistion(&TabFile, i, 23);
            std::string v30;
            std::string::string(&v30, v7->pString, &v31);
            
            StringVector ItemString;
            Answer::StringUtility::split(&ItemString, &v30, &v28, 0);
            v30.~string();
            std::allocator<char>::~allocator(&v31);
            v28.~string();
            std::allocator<char>::~allocator(&v29);
            
            for (auto& itemStr : ItemString)
            {
                std::string v33;
                char v34;
                std::allocator<char>::allocator(&v34);
                std::string::string(&v33, ":", &v34);
                
                StringVector RateVt;
                Answer::StringUtility::split(&RateVt, itemStr, &v33, 0);
                v33.~string();
                std::allocator<char>::~allocator(&v34);
                
                if (RateVt.size() == 2)
                {
                    int val = atoi(RateVt[0]->c_str());
                    plant.ItemVt.push_back(val);
                }
                RateVt.~vector();
            }
            ItemString.~vector();
            
            auto v11 = std::map<int, CfgPlant>::operator[](&this->m_plants, &plant.id);
            *v11 = plant;
            plant.~CfgPlant();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::fetchTask(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_task.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_task.txt failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgTask task;
            CfgTask::CfgTask(&task);
            
            task.id = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 1);
            snprintf(task.name, sizeof(task.name), "%s", v1->pString);
            
            task.type = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
            task.group = CDBCFile::Search_Posistion(&TabFile, i, 3)->iValue;
            task.can_giveup = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
            task.pretask = CDBCFile::Search_Posistion(&TabFile, i, 5)->iValue;
            task.posttask = CDBCFile::Search_Posistion(&TabFile, i, 6)->iValue;
            task.main_order = CDBCFile::Search_Posistion(&TabFile, i, 7)->iValue;
            task.job = CDBCFile::Search_Posistion(&TabFile, i, 8)->iValue;
            task.level = CDBCFile::Search_Posistion(&TabFile, i, 9)->iValue;
            task.max_level = CDBCFile::Search_Posistion(&TabFile, i, 10)->iValue;
            task.kingdom = CDBCFile::Search_Posistion(&TabFile, i, 11)->iValue;
            task.start_npc = CDBCFile::Search_Posistion(&TabFile, i, 12)->iValue;
            task.end_npc = CDBCFile::Search_Posistion(&TabFile, i, 13)->iValue;
            task.dungeon = CDBCFile::Search_Posistion(&TabFile, i, 14)->iValue;
            
            std::string strItems;
            char v12;
            std::allocator<char>::allocator(&v12);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, 15);
            std::string::string(&strItems, v2->pString, &v12);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            task.items_receive = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v12);
            
            task.award_exp = CDBCFile::Search_Posistion(&TabFile, i, 16)->iValue;
            task.award_money = CDBCFile::Search_Posistion(&TabFile, i, 17)->iValue;
            task.gold = CDBCFile::Search_Posistion(&TabFile, i, 18)->iValue;
            task.dilong = CDBCFile::Search_Posistion(&TabFile, i, 19)->iValue;
            task.rongyu = CDBCFile::Search_Posistion(&TabFile, i, 20)->iValue;
            task.fuwen = CDBCFile::Search_Posistion(&TabFile, i, 21)->iValue;
            task.BossScore = CDBCFile::Search_Posistion(&TabFile, i, 22)->iValue;
            
            std::string v14;
            char v15;
            std::allocator<char>::allocator(&v15);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&TabFile, i, 23);
            std::string::string(&v14, v3->pString, &v15);
            
            MemChrBagVector v13;
            CItemHelper::parseItemVectorString(&v13, &v14);
            task.award_item = v13;
            v13.~vector();
            v14.~string();
            std::allocator<char>::~allocator(&v15);
            
            std::string v17;
            char v18;
            std::allocator<char>::allocator(&v18);
            const CDBCFile::FIELD *v4 = CDBCFile::Search_Posistion(&TabFile, i, 24);
            std::string::string(&v17, v4->pString, &v18);
            
            MemChrJobBagVector v16;
            CfgData::parseTaskItemJobString(&v16, this, task.id, &v17);
            task.award_optional = v16;
            v16.~vector();
            v17.~string();
            std::allocator<char>::~allocator(&v18);
            
            task.condition = CDBCFile::Search_Posistion(&TabFile, i, 35)->iValue;
            
            std::string strRequest;
            char v20;
            std::allocator<char>::allocator(&v20);
            const CDBCFile::FIELD *v5 = CDBCFile::Search_Posistion(&TabFile, i, 36);
            std::string::string(&strRequest, v5->pString, &v20);
            task.request = CfgData::parseTaskCondition(this, task.id, task.condition, &strRequest);
            strRequest.~string();
            std::allocator<char>::~allocator(&v20);
            
            task.GongXian = CDBCFile::Search_Posistion(&TabFile, i, 50)->iValue;
            task.JunTuanZiJin = CDBCFile::Search_Posistion(&TabFile, i, 51)->iValue;
            task.Double = CDBCFile::Search_Posistion(&TabFile, i, 52)->iValue;
            task.acScore = CDBCFile::Search_Posistion(&TabFile, i, 53)->iValue;
            task.quickDoneCost = CDBCFile::Search_Posistion(&TabFile, i, 55)->iValue;
            task.quality = CDBCFile::Search_Posistion(&TabFile, i, 61)->iValue;
            task.ratio = CDBCFile::Search_Posistion(&TabFile, i, 62)->iValue;
            task.dust = CDBCFile::Search_Posistion(&TabFile, i, 63)->iValue;
            task.activity_score = CDBCFile::Search_Posistion(&TabFile, i, 65)->iValue;
            task.talent_point = CDBCFile::Search_Posistion(&TabFile, i, 67)->iValue;
            
            std::string path;
            char v23;
            std::allocator<char>::allocator(&v23);
            std::string::string(&path, "./ServerConfig/Tables/cfg_task.txt", &v23);
            
            char v25;
            std::allocator<char>::allocator(&v25);
            const CDBCFile::FIELD *v6 = CDBCFile::Search_Posistion(&TabFile, i, 68);
            std::string v22;
            std::string::string(&v22, v6->pString, &v25);
            
            std::list<TaskDrop> v21;
            CfgData::parseTaskDrop((CfgData *const)&v21, task.id, &v22);
            task.drop_list = v21;
            v21.~list();
            v22.~string();
            std::allocator<char>::~allocator(&v25);
            path.~string();
            std::allocator<char>::~allocator(&v23);
            
            task.DoubleGold = CDBCFile::Search_Posistion(&TabFile, i, 69)->iValue;
            task.BuffId = CDBCFile::Search_Posistion(&TabFile, i, 70)->iValue;
            
            auto v7 = std::map<int, CfgTask>::operator[](&this->m_tasks, &task.id);
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
            
            task.~CfgTask();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::fetchTrap(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_trap.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_trap.txt failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgTrap trap;
            CfgTrap::CfgTrap(&trap);
            
            trap.id = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            trap.cd = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
            trap.delay = CDBCFile::Search_Posistion(&TabFile, i, 3)->iValue;
            trap.event_type = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
            
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 6);
            std::string::operator=(&trap.effect, v1->pString);
            trap.item_cost = CDBCFile::Search_Posistion(&TabFile, i, 7)->iValue;
            trap.life = CDBCFile::Search_Posistion(&TabFile, i, 12)->iValue;
            
            auto v2 = std::map<int, CfgTrap>::operator[](&this->m_traps, &trap.id);
            *v2 = trap;
            trap.~CfgTrap();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::fetchLevelExp(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_level_exp.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_level_exp.txt failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgLevelExp levelExp;
            levelExp.level = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 1);
            levelExp.upgrade_exp = strtoll(v1->pString, nullptr, 10);
            
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, 2);
            levelExp.max_exp = strtoll(v2->pString, nullptr, 10);
            
            levelExp.pet_exp = CDBCFile::Search_Posistion(&TabFile, i, 3)->iValue;
            levelExp.mount_exp = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
            levelExp.vicegeneral_id = CDBCFile::Search_Posistion(&TabFile, i, 5)->iValue;
            levelExp.attr_point = CDBCFile::Search_Posistion(&TabFile, i, 6)->iValue;
            levelExp.talent_point = CDBCFile::Search_Posistion(&TabFile, i, 7)->iValue;
            
            auto v3 = std::map<int, CfgLevelExp>::operator[](&this->m_levelExps, &levelExp.level);
            *v3 = levelExp;
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::fetchLevelAttr(CfgData *const this)
{
    CDBCFile LevelAttrFile;
    CDBCFile::CDBCFile(&LevelAttrFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&LevelAttrFile, "./ServerConfig/Tables/cfg_level_attr.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_PLAYER_LEVEL_ATTR_TABLE failed,please check!!");
        CDBCFile::~CDBCFile(&LevelAttrFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&LevelAttrFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&LevelAttrFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgLevelAttr levelAttr;
            CfgLevelAttr::CfgLevelAttr(&levelAttr);
            
            levelAttr.level = CDBCFile::Search_Posistion(&LevelAttrFile, i, 0)->iValue;
            levelAttr.job = CDBCFile::Search_Posistion(&LevelAttrFile, i, 1)->iValue;
            
            std::string path;
            std::string addonAttr;
            char v8;
            std::allocator<char>::allocator(&v8);
            std::string::string(&path, "./ServerConfig/Tables/cfg_level_attr.txt", &v8);
            
            char v10;
            std::allocator<char>::allocator(&v10);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&LevelAttrFile, i, 2);
            std::string::string(&addonAttr, v1->pString, &v10);
            
            AttrAddonVector __x;
            CfgData::paraseAttrAddon(&__x, this, &addonAttr, i, &path);
            levelAttr.addonattr = __x;
            __x.~vector();
            addonAttr.~string();
            std::allocator<char>::~allocator(&v10);
            path.~string();
            std::allocator<char>::~allocator(&v8);
            
            std::string v12;
            char v13;
            std::allocator<char>::allocator(&v13);
            std::string::string(&v12, "./ServerConfig/Tables/cfg_level_attr.txt", &v13);
            
            char v15;
            std::allocator<char>::allocator(&v15);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&LevelAttrFile, i, 3);
            std::string v14;
            std::string::string(&v14, v2->pString, &v15);
            
            AttrAddonVector v11;
            CfgData::paraseAttrAddon(&v11, this, &v14, i, &v12);
            levelAttr.addonPoint = v11;
            v11.~vector();
            v14.~string();
            std::allocator<char>::~allocator(&v15);
            v12.~string();
            std::allocator<char>::~allocator(&v13);
            
            int __k = (levelAttr.job << 16) | levelAttr.level;
            auto v3 = std::map<int, CfgLevelAttr>::operator[](&this->m_levelAttrs, &__k);
            *v3 = levelAttr;
            levelAttr.~CfgLevelAttr();
        }
    }
    CDBCFile::~CDBCFile(&LevelAttrFile);
}

void CfgData::fetchJob(CfgData *const this)
{
    CDBCFile JobFile;
    CDBCFile::CDBCFile(&JobFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&JobFile, "./ServerConfig/Tables/cfg_job.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_job.txt failed,please check!!");
        CDBCFile::~CDBCFile(&JobFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&JobFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&JobFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgJob job;
            job.id = CDBCFile::Search_Posistion(&JobFile, i, 0)->iValue;
            job.job_task = CDBCFile::Search_Posistion(&JobFile, i, 4)->iValue;
            job.attack_attr = CDBCFile::Search_Posistion(&JobFile, i, 5)->iValue;
            job.base_skill = CDBCFile::Search_Posistion(&JobFile, i, 6)->iValue;
            
            auto v1 = std::map<int, CfgJob>::operator[](&this->m_jobs, &job.id);
            *v1 = job;
        }
    }
    CDBCFile::~CDBCFile(&JobFile);
}

void CfgData::fetchChrShop(CfgData *const this)
{
    CDBCFile ChrShopFile;
    CDBCFile::CDBCFile(&ChrShopFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&ChrShopFile, "./ServerConfig/Tables/cfg_chr_shop.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_chr_shop.txt failed,please check!!");
        CDBCFile::~CDBCFile(&ChrShopFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&ChrShopFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&ChrShopFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgChrShop chrShop;
            chrShop.Index = CDBCFile::Search_Posistion(&ChrShopFile, i, 0)->iValue;
            chrShop.ItemId = CDBCFile::Search_Posistion(&ChrShopFile, i, 1)->iValue;
            chrShop.ItemClass = CDBCFile::Search_Posistion(&ChrShopFile, i, 2)->iValue;
            chrShop.IsBind = CDBCFile::Search_Posistion(&ChrShopFile, i, 3)->iValue;
            chrShop.LimitCount = CDBCFile::Search_Posistion(&ChrShopFile, i, 4)->iValue;
            chrShop.Price = CDBCFile::Search_Posistion(&ChrShopFile, i, 5)->iValue;
            chrShop.ConstType = CDBCFile::Search_Posistion(&ChrShopFile, i, 9)->iValue;
            chrShop.QiQinglevel = CDBCFile::Search_Posistion(&ChrShopFile, i, 10)->iValue;
            
            auto v1 = std::map<int, CfgChrShop>::operator[](&this->m_chrShops, &chrShop.Index);
            *v1 = chrShop;
        }
    }
    CDBCFile::~CDBCFile(&ChrShopFile);
}
void CfgData::fetchMovie(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_movie.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_movie.txt failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMovie movie;
            movie.id = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            int32_t MoveId = CDBCFile::Search_Posistion(&TabFile, i, 2)->iValue;
            
            if (movie.id == MoveId)
            {
                auto v1 = std::map<int, CfgMovie>::operator[](&this->m_movie, &movie.id);
                v1->id = movie.id;
            }
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::fetchMonsterAI(CfgData *const this)
{
    CDBCFile MonsterFile;
    CDBCFile::CDBCFile(&MonsterFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&MonsterFile, "./ServerConfig/Tables/MonsterAi.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MONSTER_AI_TABLE failed,please check!!");
        CDBCFile::~CDBCFile(&MonsterFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&MonsterFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&MonsterFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMonsterAI ai;
            memset(&ai, 0, sizeof(ai));
            
            int32_t nIndex = 0;
            ai.id = CDBCFile::Search_Posistion(&MonsterFile, i, 0)->iValue;
            ai.style = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.target = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.escape_hp = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.view_range = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.move_range = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.rest_range = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.rest_time_min = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.rest_time_max = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.rest_ratio = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.run_distance = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.run_range = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.run_cd = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ai.pursuit_range = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto v1 = std::map<int, CfgMonsterAI>::operator[](&this->m_mMonsterAI, &ai.id);
            *v1 = ai;
        }
    }
    CDBCFile::~CDBCFile(&MonsterFile);
}

void CfgData::fetchMonsterAdjustTable(CfgData *const this)
{
    CDBCFile MonsterFile;
    CDBCFile::CDBCFile(&MonsterFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&MonsterFile, "./ServerConfig/Tables/MonsterAdjust.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MONSTER_ADJUST_TABLE failed,please check!!");
        CDBCFile::~CDBCFile(&MonsterFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&MonsterFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&MonsterFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMonsterAdjust monster;
            memset(&monster, 0, sizeof(monster));
            
            int32_t nIndex = 0;
            monster.mid = CDBCFile::Search_Posistion(&MonsterFile, i, 0)->iValue;
            monster.adj_level = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.level = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            
            for (int j = 1; j <= 28; ++j)
            {
                monster.vAttr[j] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            }
            
            monster.vAttr[41] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.vAttr[42] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.vAttr[43] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            monster.vAttr[44] = CDBCFile::Search_Posistion(&MonsterFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto key = std::make_pair(monster.mid, monster.adj_level);
            auto result = this->m_mMonsterAdjust.insert(std::make_pair(key, monster));
        }
    }
    CDBCFile::~CDBCFile(&MonsterFile);
    
    // 加载 Boss 成长表
    CDBCFile MonsterFile2;
    CDBCFile::CDBCFile(&MonsterFile2, 0);
    
    if (CDBCFile::OpenFromTXT(&MonsterFile2, "./ServerConfig/Tables/BossGrow.txt"))
    {
        iBaseTableCount = CDBCFile::GetRecordsNum(&MonsterFile2);
        iBaseColumnCount = CDBCFile::GetFieldsNum(&MonsterFile2);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)
            {
                CfgMonsterAdjust monster;
                memset(&monster, 0, sizeof(monster));
                
                int32_t nIndex_0 = 0;
                monster.mid = CDBCFile::Search_Posistion(&MonsterFile2, i_0, 0)->iValue;
                monster.adj_level = CDBCFile::Search_Posistion(&MonsterFile2, i_0, ++nIndex_0)->iValue;
                monster.exp = CDBCFile::Search_Posistion(&MonsterFile2, i_0, ++nIndex_0)->iValue;
                ++nIndex_0;
                
                for (int j = 1; j <= 28; ++j)
                {
                    monster.vAttr[j] = CDBCFile::Search_Posistion(&MonsterFile2, i_0, ++nIndex_0)->iValue;
                }
                
                monster.vAttr[41] = CDBCFile::Search_Posistion(&MonsterFile2, i_0, ++nIndex_0)->iValue;
                monster.vAttr[42] = CDBCFile::Search_Posistion(&MonsterFile2, i_0, ++nIndex_0)->iValue;
                monster.vAttr[43] = CDBCFile::Search_Posistion(&MonsterFile2, i_0, ++nIndex_0)->iValue;
                monster.vAttr[44] = CDBCFile::Search_Posistion(&MonsterFile2, i_0, ++nIndex_0)->iValue;
                ++nIndex_0;
                
                auto key = std::make_pair(monster.mid, monster.adj_level);
                this->m_mMonsterAdjust.insert(std::make_pair(key, monster));
            }
        }
        CDBCFile::~CDBCFile(&MonsterFile2);
    }
}

void CfgData::fetchMonsterDropGroup(CfgData *const this)
{
    CDBCFile MonsterDropFile;
    CDBCFile::CDBCFile(&MonsterDropFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&MonsterDropFile, "./ServerConfig/Tables/cfg_monster_drop_group.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_monster_drop_group.txt failed,please check!!");
        CDBCFile::~CDBCFile(&MonsterDropFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&MonsterDropFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&MonsterDropFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMonsterDropGroup monsterDropGroup;
            memset(&monsterDropGroup, 0, sizeof(monsterDropGroup));
            
            int32_t nIndex = 0;
            monsterDropGroup.group_id = CDBCFile::Search_Posistion(&MonsterDropFile, i, 0)->iValue;
            monsterDropGroup.item_id = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            monsterDropGroup.item_class = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            monsterDropGroup.item_count = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            monsterDropGroup.bind_type = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            monsterDropGroup.weight = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            monsterDropGroup.probability = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            monsterDropGroup.cost_type = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            monsterDropGroup.cost_value = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            monsterDropGroup.limit_time = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            monsterDropGroup.daily_limit = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string p_StringTime;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&MonsterDropFile, i, nIndex);
            std::string::string(&p_StringTime, v1->pString, &v7);
            monsterDropGroup.start_date = Answer::DayTime::StringToIntTime(&p_StringTime);
            p_StringTime.~string();
            std::allocator<char>::~allocator(&v7);
            ++nIndex;
            
            std::string v8;
            char v9;
            std::allocator<char>::allocator(&v9);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&MonsterDropFile, i, nIndex);
            std::string::string(&v8, v2->pString, &v9);
            monsterDropGroup.end_date = Answer::DayTime::StringToIntTime(&v8);
            v8.~string();
            std::allocator<char>::~allocator(&v9);
            
            monsterDropGroup.record = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            monsterDropGroup.festival_group = CDBCFile::Search_Posistion(&MonsterDropFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto v3 = std::map<int, std::vector<CfgMonsterDropGroup>>::operator[](&this->m_monsterDropGroups, &monsterDropGroup.group_id);
            v3->push_back(monsterDropGroup);
        }
    }
    CDBCFile::~CDBCFile(&MonsterDropFile);
}

void CfgData::fetchMonsterGroupDrop(CfgData *const this)
{
    CDBCFile MonsterGroupFile;
    CDBCFile::CDBCFile(&MonsterGroupFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&MonsterGroupFile, "./ServerConfig/Tables/cfg_monster_group_drop.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_monster_group_drop.txt failed,please check!!");
        CDBCFile::~CDBCFile(&MonsterGroupFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&MonsterGroupFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&MonsterGroupFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMonsterGroupDrop monsterGroupDrop;
            memset(&monsterGroupDrop, 0, sizeof(monsterGroupDrop));
            
            int32_t nIndex = 0;
            monsterGroupDrop.mid = CDBCFile::Search_Posistion(&MonsterGroupFile, i, 0)->iValue;
            monsterGroupDrop.group_id = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.probability = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.begin_time = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.end_time = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.repeat = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.job = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.grow_level = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.dropType = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.bind_type = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.hard = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.quality = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.world_event = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            monsterGroupDrop.DropLimit = CDBCFile::Search_Posistion(&MonsterGroupFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            if (monsterGroupDrop.begin_time < 1440 && monsterGroupDrop.end_time < 1440 &&
                monsterGroupDrop.begin_time <= monsterGroupDrop.end_time && monsterGroupDrop.repeat > 0)
            {
                auto v1 = std::map<int, std::vector<CfgMonsterGroupDrop>>::operator[](&this->m_monsterGroupDrops, &monsterGroupDrop.mid);
                v1->push_back(monsterGroupDrop);
            }
            else
            {
                Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, 
                    "wrong cfg_monster_group_drop data with mid = %d, group_id = %d\n", 
                    monsterGroupDrop.mid, monsterGroupDrop.group_id);
            }
        }
    }
    CDBCFile::~CDBCFile(&MonsterGroupFile);
}

void CfgData::fetchMonsterTaskDrop(CfgData *const this)
{
    CDBCFile MonsterTaskFile;
    CDBCFile::CDBCFile(&MonsterTaskFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&MonsterTaskFile, "./ServerConfig/Tables/cfg_monster_task_drop.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open cfg_monster_task_drop.txt failed,please check!!");
        CDBCFile::~CDBCFile(&MonsterTaskFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&MonsterTaskFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&MonsterTaskFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMonsterTaskDrop monsterTaskDrop;
            monsterTaskDrop.mid = CDBCFile::Search_Posistion(&MonsterTaskFile, i, 0)->iValue;
            monsterTaskDrop.tid = CDBCFile::Search_Posistion(&MonsterTaskFile, i, 1)->iValue;
            monsterTaskDrop.item = CDBCFile::Search_Posistion(&MonsterTaskFile, i, 2)->iValue;
            monsterTaskDrop.probability = CDBCFile::Search_Posistion(&MonsterTaskFile, i, 3)->iValue;
            
            auto v1 = std::map<int, std::vector<CfgMonsterTaskDrop>>::operator[](&this->m_monsterTaskDrops, &monsterTaskDrop.mid);
            v1->push_back(monsterTaskDrop);
        }
    }
    CDBCFile::~CDBCFile(&MonsterTaskFile);
}

// ==================== 辅助方法 ====================

std::vector<Position> *CfgData::paresPosition(std::vector<Position> *__return_ptr retstr, CfgData *const this, const std::string *const strPos)
{
    std::vector<Position>::vector(retstr);
    
    if (!strPos->empty())
    {
        std::string delims;
        char v15;
        std::allocator<char>::allocator(&v15);
        std::string::string(&delims, "|", &v15);
        
        StringVector PosString;
        Answer::StringUtility::split(&PosString, strPos, &delims, 0);
        delims.~string();
        std::allocator<char>::~allocator(&v15);
        
        for (auto& posStr : PosString)
        {
            std::string v17;
            char v18;
            std::allocator<char>::allocator(&v18);
            std::string::string(&v17, ":", &v18);
            
            StringVector Pos;
            Answer::StringUtility::split(&Pos, posStr, &v17, 0);
            v17.~string();
            std::allocator<char>::~allocator(&v18);
            
            if (Pos.size() == 2)
            {
                Position stu;
                stu.x = atoi(Pos[0]->c_str());
                stu.y = atoi(Pos[1]->c_str());
                retstr->push_back(stu);
            }
            Pos.~vector();
        }
        PosString.~vector();
    }
    return retstr;
}

Param2 CfgData::paraseParam2(CfgData *const this, const std::string *const str)
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
    std::allocator<char>::allocator(&v13);
    std::string::string(&delims, ":", &v13);
    
    StringVector vParam;
    Answer::StringUtility::split(&vParam, str, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v13);
    
    Param2 result;
    if (vParam.size() == 2)
    {
        result.nParam1 = atoi(vParam[0]->c_str());
        result.nParam2 = atoi(vParam[1]->c_str());
    }
    else
    {
        result.nParam1 = 0;
        result.nParam2 = 0;
    }
    
    vParam.~vector();
    return result;
}

Int32Vector *CfgData::paraseInt32Vector(Int32Vector *__return_ptr retstr, CfgData *const this, 
                                         const std::string *const str, const std::string *const path, int32_t size)
{
    std::vector<int>::vector(retstr);
    
    if (str->empty() || *str == "-1") return retstr;
    
    std::string delims;
    char v19;
    std::allocator<char>::allocator(&v19);
    std::string::string(&delims, ":", &v19);
    
    StringVector vstr;
    Answer::StringUtility::split(&vstr, str, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v19);
    
    if (size > 0 && (int32_t)vstr.size() != size)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, 
            "CfgData::paraseInt32Vector() check size err from %s, where size = %d, str = %s\n",
            path->c_str(), size, str->c_str());
    }
    else
    {
        retstr->reserve(vstr.size());
        for (auto& valStr : vstr)
        {
            int val = atoi(valStr->c_str());
            retstr->push_back(val);
        }
    }
    
    vstr.~vector();
    return retstr;
}

AttrAddonVector *CfgData::paraseAttrAddon(AttrAddonVector *__return_ptr retstr, CfgData *const this,
                                           const std::string *const addonAttr, int32_t nIndex, const std::string *const path)
{
    std::vector<AttrAddon>::vector(retstr);
    
    if (addonAttr->empty() || *addonAttr == "-1" || *addonAttr == "0") return retstr;
    
    std::string delims;
    char v22;
    std::allocator<char>::allocator(&v22);
    std::string::string(&delims, "|", &v22);
    
    StringVector strAttrAddons;
    Answer::StringUtility::split(&strAttrAddons, addonAttr, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v22);
    
    for (auto& addonStr : strAttrAddons)
    {
        std::string v24;
        char v25;
        std::allocator<char>::allocator(&v25);
        std::string::string(&v24, ":", &v25);
        
        StringVector strAttrAddon;
        Answer::StringUtility::split(&strAttrAddon, addonStr, &v24, 0);
        v24.~string();
        std::allocator<char>::~allocator(&v25);
        
        if (strAttrAddon.size() == 2)
        {
            AttrAddon attrAddon;
            attrAddon.index = atoi(strAttrAddon[0]->c_str());
            attrAddon.addon = atoi(strAttrAddon[1]->c_str());
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
        strAttrAddon.~vector();
    }
    
    strAttrAddons.~vector();
    return retstr;
}

int32_t CfgData::getOverlay(CfgData *const this, int32_t nId, int8_t nClass)
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
        CfgItem *cfgItem = CfgData::getItem(this, nId);
        if (!cfgItem) return 0;
        int32_t overlay = cfgItem->overlay;
        return (overlay == 0) ? 1 : overlay;
    }
    else if (nClass == 8)
    {
        WuHunItem *pWuHun = CfgData::GetWuHunItem(this, nId);
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

int32_t CfgData::getOutValue(CfgData *const this, int32_t nId, int8_t nClass)
{
    if (nClass == 2)
    {
        const CfgEquip *pEquip = CfgData::getEquip(this, nId);
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
        CfgItem *pItem = CfgData::getItem(this, nId);
        if (pItem) return pItem->out_value;
    }
    return -1;
}

bool CfgData::dropBroadcast(CfgData *const this, int32_t nId, int8_t nClass)
{
    switch (nClass)
    {
    case 2:
    {
        const CfgEquip *pEquip = CfgData::getEquip(this, nId);
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
        CfgItem *pItem = CfgData::getItem(this, nId);
        if (pItem) return pItem->broadcast > 0;
        break;
    }
    }
    return false;
}

bool CfgData::canSell(CfgData *const this, int32_t nId, int8_t nClass)
{
    if (nClass != 1) return true;
    CfgItem *pCfgItem = CfgData::getItem(this, nId);
    return (pCfgItem == nullptr) || (pCfgItem->can_sell != 0);
}

int32_t CfgData::InitXuWuTask(CfgData *const this, int32_t Level, int32_t Times)
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

int32_t CfgData::InitXinMoTask(CfgData *const this, int32_t Level, int32_t Times)
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

int32_t CfgData::RandPdbfTask(CfgData *const this, int32_t Level)
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
void CfgData::InitActiveSkillTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/cfg_skill_info.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_ACTIVE_SKILL_TABLE fail, please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgActiveSkill skill;
            memset(&skill, 0, sizeof(skill));
            std::vector<AttrAddon>::vector(&skill.summon_attr);
            
            int32_t nIndex = 0;
            skill.id = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            ++nIndex;
            skill.talent = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.groupid = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.job = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.kind = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.distance = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.range = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.area = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.self = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.target_num = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.beneficial = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.tar_type = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.addon_skill = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.addon_time = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.addon_cd = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.addon_delay = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.addon_trig_times = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.cd = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.cd_adjust = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.mp = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.power = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.special = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.attack_type = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.attack_modify = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.building_modify = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.buff_rate = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.buff = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            skill.chantTime = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.summon_id = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            skill.summon_delay = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string path;
            std::string addonAttr;
            char v6;
            std::allocator<char>::allocator(&v6);
            std::string::string(&path, "./ServerConfig/Tables/cfg_skill_info.txt", &v6);
            
            char v8;
            std::allocator<char>::allocator(&v8);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&addonAttr, v1->pString, &v8);
            
            AttrAddonVector __x;
            CfgData::paraseAttrAddon(&__x, this, &addonAttr, i, &path);
            skill.summon_attr = __x;
            __x.~vector();
            addonAttr.~string();
            std::allocator<char>::~allocator(&v8);
            path.~string();
            std::allocator<char>::~allocator(&v6);
            
            skill.summon_limit = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            nIndex += 17;
            skill.boss_addon_damage = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            skill.append_value = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            skill.shu_lian_du = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            skill.cd += skill.cd_adjust;
            
            CfgSkillTable::AddActiveSkill(&this->m_cfgSkillTable, &skill);
            skill.~CfgActiveSkill();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitPassiveSkillTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/SkillPassiveAttr.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_PASSIVE_SKILL_TABLE fail, please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgPassiveSkill stu;
            memset(&stu, 0, sizeof(stu));
            std::vector<AttrAddon>::vector(&stu.vAttrs);
            std::list<TalentAddon>::list(&stu.lTalentAddon);
            
            int32_t nIndex = 0;
            stu.id = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.type = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            ++nIndex;
            
            std::string path;
            std::string addonAttr;
            char v7;
            std::allocator<char>::allocator(&v7);
            std::string::string(&path, "./ServerConfig/Tables/SkillPassiveAttr.txt", &v7);
            
            char v9;
            std::allocator<char>::allocator(&v9);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&addonAttr, v1->pString, &v9);
            
            AttrAddonVector __x;
            CfgData::paraseAttrAddon(&__x, this, &addonAttr, i, &path);
            stu.vAttrs = __x;
            __x.~vector();
            addonAttr.~string();
            std::allocator<char>::~allocator(&v9);
            path.~string();
            std::allocator<char>::~allocator(&v7);
            ++nIndex;
            
            std::string v11;
            char v12;
            std::allocator<char>::allocator(&v12);
            std::string::string(&v11, "./ServerConfig/Tables/SkillPassiveAttr.txt", &v12);
            
            char v14;
            std::allocator<char>::allocator(&v14);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string v13;
            std::string::string(&v13, v2->pString, &v14);
            
            std::list<TalentAddon> v10;
            CfgData::paraseTalentAddon((CfgData *const)&v10, &v13, i, &v11);
            stu.lTalentAddon = v10;
            v10.~list();
            v13.~string();
            std::allocator<char>::~allocator(&v14);
            v11.~string();
            std::allocator<char>::~allocator(&v12);
            
            stu.dropMoneyRate = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            CfgSkillTable::AddPassiveSkill(&this->m_cfgSkillTable, &stu);
            stu.~CfgPassiveSkill();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitTrigSkillTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/SkillTrig.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_TRIG_SKILL_TABLE fail, please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgTrigSkill stu;
            CfgTrigSkill::CfgTrigSkill(&stu);
            
            int32_t nIndex = 0;
            stu.id = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.groupid = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.trigType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::operator=(&stu.trigParam, v1->pString);
            stu.targetType = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.trigRate = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.cdTime = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.trigBuff = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.special = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.specialParam = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.IsPvp = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            CfgSkillTable::AddTrigSkill(&this->m_cfgSkillTable, &stu);
            stu.~CfgTrigSkill();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitTalentTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/SkillTalent.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_TALENT_TABLE fail, please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgTalent stu;
            CfgTalent::CfgTalent(&stu);
            
            int32_t nIndex = 0;
            stu.id = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.Level = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.skillid = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.maxLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.Playerlevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            ++nIndex;
            
            std::string bCombi;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&bCombi, v1->pString, &v7);
            
            std::list<ItemData> strItems;
            CItemHelper::parseItemDataListString((const std::string *const)&strItems, (bool)bCombi.c_str());
            stu.costItem = strItems;
            strItems.~list();
            bCombi.~string();
            std::allocator<char>::~allocator(&v7);
            ++nIndex;
            
            std::string size;
            char v10;
            std::allocator<char>::allocator(&v10);
            std::string::string(&size, "./ServerConfig/Tables/SkillTalent.txt", &v10);
            
            std::string path;
            char v12;
            std::allocator<char>::allocator(&v12);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&path, v2->pString, &v12);
            
            std::list<int> __x;
            CfgData::paraseInt32List(this, &__x, &path, atoi(size.c_str()), nullptr);
            stu.powerSkills = __x;
            __x.~list();
            path.~string();
            std::allocator<char>::~allocator(&v12);
            size.~string();
            std::allocator<char>::~allocator(&v10);
            ++nIndex;
            ++nIndex;
            
            stu.battle = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            stu.Point = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.GongGaoId = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            CfgTalentTable::AddTalent(&this->m_cfgTalentTable, &stu);
            stu.~CfgTalent();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitTalentPageTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/SkillTree.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_TALENT_PAGE_TABLE fail, please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgTalentPage stu;
            CfgTalentPage::CfgTalentPage(&stu);
            
            int32_t nIndex = 0;
            stu.job = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string size;
            char v6;
            std::allocator<char>::allocator(&v6);
            std::string::string(&size, "./ServerConfig/Tables/SkillTree.txt", &v6);
            
            std::string path;
            char v8;
            std::allocator<char>::allocator(&v8);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&path, v1->pString, &v8);
            
            std::list<int> __x;
            CfgData::paraseInt32List(this, &__x, &path, atoi(size.c_str()), nullptr);
            stu.talents = __x;
            __x.~list();
            path.~string();
            std::allocator<char>::~allocator(&v8);
            size.~string();
            std::allocator<char>::~allocator(&v6);
            ++nIndex;
            
            CfgTalentTable::AddTalentPage(&this->m_cfgTalentTable, &stu);
            stu.~CfgTalentPage();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitFamilySkillTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/FamilySkill.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_FAMILY_SKILL_TABLE fail, please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgFamilySkill stu;
            CfgFamilySkill::CfgFamilySkill(&stu);
            
            int32_t nIndex = 0;
            stu.nId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nLevel = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nFamilyLevel = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nCostValue = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string path;
            std::string addonAttr;
            char v6;
            std::allocator<char>::allocator(&v6);
            std::string::string(&path, "./ServerConfig/Tables/FamilySkill.txt", &v6);
            
            char v8;
            std::allocator<char>::allocator(&v8);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&addonAttr, v1->pString, &v8);
            
            AttrAddonVector __x;
            CfgData::paraseAttrAddon(&__x, this, &addonAttr, i, &path);
            stu.vAttrAddon = __x;
            __x.~vector();
            addonAttr.~string();
            std::allocator<char>::~allocator(&v8);
            path.~string();
            std::allocator<char>::~allocator(&v6);
            
            stu.nCostMoney = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.PlayerLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            CfgSkillTable::AddFamilySkill(&this->m_cfgSkillTable, &stu);
            stu.~CfgFamilySkill();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitTalentActiveTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/SkillActive.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "Open FILE_ACTIVE_TALENT_TABLE fail, please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgTalentActive stu;
            CfgTalentActive::CfgTalentActive(&stu);
            
            int32_t nIndex = 0;
            stu.nId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string bCombi;
            char v6;
            std::allocator<char>::allocator(&v6);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&bCombi, v1->pString, &v6);
            
            std::list<ItemData> strItems;
            CItemHelper::parseItemDataListString((const std::string *const)&strItems, (bool)bCombi.c_str());
            stu.lItems = strItems;
            strItems.~list();
            bCombi.~string();
            std::allocator<char>::~allocator(&v6);
            ++nIndex;
            
            CfgSkillTable::AddTalentActive(&this->m_cfgSkillTable, &stu);
            stu.~CfgTalentActive();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}
void CfgData::InitEquipTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/Equip.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ITEM_EQUIP_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgEquip equip{};
            
            int32_t nIndex = 0;
            equip.m_nId = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            ++nIndex;
            equip.m_nType = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_nGrade = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_nLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_nJob = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_nQuality = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_nSuitId = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_nSuitId2 = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_nPrice = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string path;
            std::string addonAttr;
            char v8;
            std::allocator<char>::allocator(&v8);
            std::string::string(&path, "./ServerConfig/Tables/Equip.txt", &v8);
            
            char v10;
            std::allocator<char>::allocator(&v10);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&addonAttr, v1->pString, &v10);
            
            AttrAddonVector __x;
            CfgData::paraseAttrAddon(&__x, this, &addonAttr, i, &path);
            equip.m_vAttrAddon = __x;
            __x.~vector();
            addonAttr.~string();
            std::allocator<char>::~allocator(&v10);
            path.~string();
            std::allocator<char>::~allocator(&v8);
            ++nIndex;
            
            std::string v12;
            char v13;
            std::allocator<char>::allocator(&v13);
            std::string::string(&v12, "./ServerConfig/Tables/Equip.txt", &v13);
            
            char v15;
            std::allocator<char>::allocator(&v15);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string v14;
            std::string::string(&v14, v2->pString, &v15);
            
            AttrAddonVector v11;
            CfgData::paraseAttrAddon(&v11, this, &v14, i, &v12);
            equip.m_vElement = v11;
            v11.~vector();
            v14.~string();
            std::allocator<char>::~allocator(&v15);
            v12.~string();
            std::allocator<char>::~allocator(&v13);
            ++nIndex;
            nIndex += 10;
            equip.m_DropLuck = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            equip.m_DropRate = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            equip.m_BackType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            equip.m_BackValue = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            equip.m_nBroadcast = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            equip.m_backIndex = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            ++nIndex;
            equip.m_nLimitStar = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_BackGold = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string v17;
            char v18;
            std::allocator<char>::allocator(&v18);
            std::string::string(&v17, "./ServerConfig/Tables/Equip.txt", &v18);
            
            char v20;
            std::allocator<char>::allocator(&v20);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string v19;
            std::string::string(&v19, v3->pString, &v20);
            
            AttrAddonVector v16;
            CfgData::paraseAttrAddon(&v16, this, &v19, i, &v17);
            equip.m_BaseAttr = v16;
            v16.~vector();
            v19.~string();
            std::allocator<char>::~allocator(&v20);
            v17.~string();
            std::allocator<char>::~allocator(&v18);
            
            equip.m_PolishLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            equip.m_MoFuHuiShou = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_SellDay = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_GongMingLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_WingEquipRefiningLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_XinMoExp = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_XinMoBag = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_CanRongHe = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            equip.m_TeJieParam = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            CfgEquipTable::AddEquip(&this->m_cfgEquip, &equip);
            equip.~CfgEquip();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitEquipUpStarTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/EquipUpStar.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ITEM_EQUIP_UP_STAR_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgEquipUpStar stu;
            CfgEquipUpStar::CfgEquipUpStar(&stu);
            
            int32_t nIndex = 0;
            stu.m_nType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.m_nStar = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.m_nRate = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.m_nSuccessAddStar = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.m_nFailLostStar = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string bCombi;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&bCombi, v1->pString, &v7);
            
            std::list<ItemData> strItems;
            CItemHelper::parseItemDataListString((const std::string *const)&strItems, (bool)bCombi.c_str());
            stu.m_lCosItem = strItems;
            strItems.~list();
            bCombi.~string();
            std::allocator<char>::~allocator(&v7);
            
            stu.m_nCostMoney = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string path;
            std::string addonAttr;
            char v10;
            std::allocator<char>::allocator(&v10);
            std::string::string(&path, "./ServerConfig/Tables/EquipUpStar.txt", &v10);
            
            char v12;
            std::allocator<char>::allocator(&v12);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&addonAttr, v2->pString, &v12);
            
            AttrAddonVector __x;
            CfgData::paraseAttrAddon(&__x, this, &addonAttr, i, &path);
            stu.m_vAttrAddon = __x;
            __x.~vector();
            addonAttr.~string();
            std::allocator<char>::~allocator(&v12);
            path.~string();
            std::allocator<char>::~allocator(&v10);
            ++nIndex;
            ++nIndex;
            
            stu.m_nCostXingMai = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.m_RongLianAttr = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            CfgEquipTable::AddEquipUpStar(&this->m_cfgEquip, &stu);
            stu.~CfgEquipUpStar();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitWingCfgTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/AttributeWing.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WING_CFG_TABEL failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            WingCfg stu;
            memset(&stu, 0, sizeof(stu));
            std::list<ItemData>::list(&stu.ConstItems);
            std::vector<AttrAddon>::vector(&stu.AddonVector);
            
            int32_t nIndex = 0;
            stu.Level = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            ++nIndex;
            
            std::string bCombi;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&bCombi, v1->pString, &v7);
            
            std::list<ItemData> strItems;
            CItemHelper::parseItemDataListString((const std::string *const)&strItems, (bool)bCombi.c_str());
            stu.ConstItems = strItems;
            strItems.~list();
            bCombi.~string();
            std::allocator<char>::~allocator(&v7);
            
            stu.StartPoints = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.SuccessPoints = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.MaxPoints = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.Rate = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.FailAddPoints = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.SkillId = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.SkillLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.IsClear = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.GongGaoId = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string path;
            std::string addonAttr;
            char v10;
            std::allocator<char>::allocator(&v10);
            std::string::string(&path, "./ServerConfig/Tables/AttributeWing.txt", &v10);
            
            char v12;
            std::allocator<char>::allocator(&v12);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&addonAttr, v2->pString, &v12);
            
            AttrAddonVector __x;
            CfgData::paraseAttrAddon(&__x, this, &addonAttr, i, &path);
            stu.AddonVector = __x;
            __x.~vector();
            addonAttr.~string();
            std::allocator<char>::~allocator(&v12);
            path.~string();
            std::allocator<char>::~allocator(&v10);
            ++nIndex;
            
            WingCfg p_stu;
            WingCfg::WingCfg(&p_stu, &stu);
            CfgWingTable::AddWingCfg(&this->m_cfgWing, &p_stu);
            p_stu.~WingCfg();
            stu.~WingCfg();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitCarrierTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/Carrier.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_CARRIER_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgCarrier stu;
            memset(&stu, 0, sizeof(stu));
            std::list<int>::list(&stu.lSkills);
            
            int32_t nIndex = 0;
            stu.nId = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            ++nIndex;
            
            std::string size;
            char v6;
            std::allocator<char>::allocator(&v6);
            std::string::string(&size, "./ServerConfig/Tables/Carrier.txt", &v6);
            
            std::string path;
            char v8;
            std::allocator<char>::allocator(&v8);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&path, v1->pString, &v8);
            
            std::list<int> __x;
            CfgData::paraseInt32List(this, &__x, &path, atoi(size.c_str()), nullptr);
            stu.lSkills = __x;
            __x.~list();
            path.~string();
            std::allocator<char>::~allocator(&v8);
            size.~string();
            std::allocator<char>::~allocator(&v6);
            ++nIndex;
            
            CfgCarrierTable::AddCarrier(&this->m_cfgCarrierTable, &stu);
            stu.~CfgCarrier();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitPetTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/Pet.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_PET_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgPetData pet;
            CfgPetData::CfgPetData(&pet);
            
            int32_t nIndex = 0;
            pet.m_nPetId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            pet.m_Quality = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            for (int32_t j = 0; j <= 8; ++j)
            {
                pet.m_vSkill[j] = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                pet.m_vSkillOpen[j] = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            }
            
            CfgPetTable::Add(&this->m_cfgPetTable, &pet);
            pet.~CfgPetData();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitFamilyTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    bool ret = CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/Family.txt");
    
    if (!ret)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_FAMILY_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        CfgFamily family;
        CfgFamily::CfgFamily(&family);
        
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgFamily::CleanUp(&family);
            
            int32_t nIndex = 0;
            family.nLevel = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            family.nExp = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            family.nMaxMembers = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            family.vPosition[1] = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            family.vPosition[2] = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            family.nMaxBossPoints = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            CfgFamilyTable::Add(&this->m_cfgFamilyTable, &family);
        }
        family.~CfgFamily();
    }
    CDBCFile::~CDBCFile(&readFile);
    
    // 加载家族Boss表
    CDBCFile readFile2;
    CDBCFile::CDBCFile(&readFile2, 0);
    ret = CDBCFile::OpenFromTXT(&readFile2, "./ServerConfig/Tables/FamilyBoss.txt");
    
    if (ret)
    {
        iBaseTableCount = CDBCFile::GetRecordsNum(&readFile2);
        iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile2);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)
            {
                CfgFamilyBoss stu;
                int32_t nIndex_0 = 0;
                stu.nFamilyLevel = CDBCFile::Search_Posistion(&readFile2, i_0, 0)->iValue;
                stu.BossMid = CDBCFile::Search_Posistion(&readFile2, i_0, ++nIndex_0)->iValue;
                stu.FamilyExp = CDBCFile::Search_Posistion(&readFile2, i_0, ++nIndex_0)->iValue;
                stu.GongGaoId = CDBCFile::Search_Posistion(&readFile2, i_0, ++nIndex_0)->iValue;
                ++nIndex_0;
                
                CfgFamilyTable::AddFamilyBoss(&this->m_cfgFamilyTable, &stu);
            }
        }
        CDBCFile::~CDBCFile(&readFile2);
    }
}

void CfgData::InitHoeTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/Hoe.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_HOE_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            HoeCfg Stu;
            int32_t nIndex = 0;
            Stu.nId = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            Stu.nNextId = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            Stu.nDouble = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto v1 = std::map<int, HoeCfg>::operator[](&this->m_HoeCfgMap, &Stu.nId);
            *v1 = Stu;
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

HoeCfg *CfgData::GetHoeCfg(CfgData *const this, int32_t nId)
{
    auto it = this->m_HoeCfgMap.find(nId);
    if (it != this->m_HoeCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitTitleTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/Title.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TITLE_TABLE failed,please check!!");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgTitle title;
            CfgTitle::CfgTitle(&title);
            
            int32_t nIndex = 0;
            title.nId = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            ++nIndex;
            title.nType = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string params;
            char v14;
            std::allocator<char>::allocator(&v14);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&params, v1->pString, &v14);
            std::allocator<char>::~allocator(&v14);
            
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, ++nIndex);
            std::string::operator=(&title.sPlatform, v2->pString);
            title.nJob = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            title.nSex = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            title.nSpecial = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            title.nPriority = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string getttr;
            char v15;
            std::allocator<char>::allocator(&v15);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&getttr, v3->pString, &v15);
            std::allocator<char>::~allocator(&v15);
            ++nIndex;
            
            std::string dressattr;
            char v16;
            std::allocator<char>::allocator(&v16);
            const CDBCFile::FIELD *v4 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&dressattr, v4->pString, &v16);
            std::allocator<char>::~allocator(&v16);
            ++nIndex;
            
            std::string delims;
            char v18;
            std::allocator<char>::allocator(&v18);
            std::string::string(&delims, ":", &v18);
            
            StringVector strParams;
            Answer::StringUtility::split(&strParams, &params, &delims, 0);
            delims.~string();
            std::allocator<char>::~allocator(&v18);
            
            for (auto& param : strParams)
            {
                int val = atoi(param->c_str());
                title.vParams.push_back(val);
            }
            strParams.~vector();
            params.~string();
            
            std::string path;
            char v23;
            std::allocator<char>::allocator(&v23);
            std::string::string(&path, "./ServerConfig/Tables/Title.txt", &v23);
            
            AttrAddonVector v21;
            CfgData::paraseAttrAddon(&v21, this, &getttr, i, &path);
            title.vGetAttr = v21;
            v21.~vector();
            path.~string();
            std::allocator<char>::~allocator(&v23);
            
            std::string v25;
            char v26;
            std::allocator<char>::allocator(&v26);
            std::string::string(&v25, "./ServerConfig/Tables/Title.txt", &v26);
            
            AttrAddonVector v24;
            CfgData::paraseAttrAddon(&v24, this, &dressattr, i, &v25);
            title.vDressAttr = v24;
            v24.~vector();
            v25.~string();
            std::allocator<char>::~allocator(&v26);
            
            dressattr.~string();
            getttr.~string();
            
            CfgTitleTable::Add(&this->m_cfgTitleTable, &title);
            title.~CfgTitle();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}
void CfgData::InitGoldEggTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/ItemGoldEggOpen.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_GOLD_EGG_OPEN_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgGoldEgg stu;
            CfgGoldEgg::CfgGoldEgg(&stu);
            
            int32_t nIndex = 0;
            stu.nId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nGroupId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nOpenTimes = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nCostMoney = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nCostGold = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            CfgGoldEggTable::AddGoldEgg(&this->m_cfgGoldEggTable, &stu);
            stu.~CfgGoldEgg();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
    
    // 加载金蛋产出表
    CDBCFile readFile2;
    CDBCFile::CDBCFile(&readFile2, 0);
    if (CDBCFile::OpenFromTXT(&readFile2, "./ServerConfig/Tables/ItemGoldEgg.txt"))
    {
        iBaseTableCount = CDBCFile::GetRecordsNum(&readFile2);
        iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile2);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)
            {
                int32_t nId = CDBCFile::Search_Posistion(&readFile2, i_0, 0)->iValue;
                CfgGoldEggItem stu;
                stu.nId = CDBCFile::Search_Posistion(&readFile2, i_0, 1)->iValue;
                stu.nGroupId = CDBCFile::Search_Posistion(&readFile2, i_0, 2)->iValue;
                stu.nOpenTimes = CDBCFile::Search_Posistion(&readFile2, i_0, 3)->iValue;
                stu.nCostMoney = CDBCFile::Search_Posistion(&readFile2, i_0, 4)->iValue;
                stu.nCostGold = CDBCFile::Search_Posistion(&readFile2, i_0, 5)->iValue;
                
                CfgGoldEggTable::AddGoldEggProduce(&this->m_cfgGoldEggTable, nId, &stu);
            }
        }
        CDBCFile::~CDBCFile(&readFile2);
    }
}

void CfgData::InitLimitTimeTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/LimitTime.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_LIMIT_TIME_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgLimitTime stu;
            int32_t nIndex = 0;
            stu.nIndex = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string strTime;
            char v6;
            std::allocator<char>::allocator(&v6);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&strTime, v1->pString, &v6);
            std::allocator<char>::~allocator(&v6);
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
            strTime.~string();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitMysteryShopTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/MysteriousShop.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MYSTERY_SHOP_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMysteryShop stu;
            memset(&stu, 0, sizeof(stu));
            
            int32_t nIndex = 0;
            stu.nId = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string strItem;
            char v9;
            std::allocator<char>::allocator(&v9);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&strItem, v1->pString, &v9);
            
            MemChrBag v4;
            CItemHelper::parseItemString(&v4, &strItem);
            stu.item = v4;
            strItem.~string();
            std::allocator<char>::~allocator(&v9);
            
            stu.nCostType = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nPrice = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            stu.nRate = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nMinLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nMaxLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nBroad = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string v10;
            char v11;
            std::allocator<char>::allocator(&v11);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&v10, v2->pString, &v11);
            
            ItemData v17 = CItemHelper::parseItemDataString(&v10);
            stu.exchange.m_nId = v17.m_nId;
            stu.exchange.m_nClass = v17.m_nClass;
            stu.exchange.m_nCount = v17.m_nCount;
            v10.~string();
            std::allocator<char>::~allocator(&v11);
            ++nIndex;
            
            CfgMysteryShopTable::Add(&this->m_cfgMysteryShopTable, &stu);
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitBuyGiftTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/TimeLimit.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BUY_GIFT_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgBuyGift stu;
            CfgBuyGift::CfgBuyGift(&stu);
            
            int32_t nIndex = 0;
            stu.nIndex = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nGold = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string items;
            char v6;
            std::allocator<char>::allocator(&v6);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&items, v1->pString, &v6);
            std::allocator<char>::~allocator(&v6);
            ++nIndex;
            nIndex += 3;
            stu.nBroad = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &items);
            stu.vGift = __x;
            __x.~vector();
            
            auto v2 = std::map<int, CfgBuyGift>::operator[](&this->m_cfgBuyGiftTable, &stu.nIndex);
            *v2 = stu;
            items.~string();
            stu.~CfgBuyGift();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitExchangeTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/JiZiDuiJiang.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_EXCHANGE_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgExchange stu;
            CfgExchange::CfgExchange(&stu);
            
            int32_t nIndex = 0;
            stu.nIndex = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string items;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&items, v1->pString, &v7);
            std::allocator<char>::~allocator(&v7);
            ++nIndex;
            
            std::string rewards;
            char v8;
            std::allocator<char>::allocator(&v8);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&rewards, v2->pString, &v8);
            std::allocator<char>::~allocator(&v8);
            
            stu.nLimit = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::list<ItemData> strItems;
            CItemHelper::parseItemDataListString((const std::string *const)&strItems, (bool)items.c_str());
            stu.vCost = strItems;
            strItems.~list();
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &rewards);
            stu.vReward = __x;
            __x.~vector();
            
            CfgExchangeTable::Add(&this->m_cfgExchangeTable, &stu);
            rewards.~string();
            items.~string();
            stu.~CfgExchange();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitMysteryGiftTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/MysteriousGift.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MYSTERY_GIFT_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMysteryGift stu;
            CfgMysteryGift::CfgMysteryGift(&stu);
            
            int32_t nIndex = 0;
            stu.nIndex = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nCondition = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string strItems;
            char v6;
            std::allocator<char>::allocator(&v6);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&strItems, v1->pString, &v6);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.vItem = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v6);
            ++nIndex;
            nIndex += 3;
            stu.nBroadId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            CfgMysteryGiftTable::Add(&this->m_cfgMysteryGiftTable, &stu);
            stu.~CfgMysteryGift();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitDrawTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/Draw.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_DRAW_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgDrawReward stu;
            CfgDrawReward::CfgDrawReward(&stu);
            
            int32_t nIndex = 0;
            stu.nIndex = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nRate = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            MemChrBag item;
            memset(&item, 0, sizeof(item));
            item.itemClass = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            item.itemId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            item.itemCount = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            item.bind = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nBroad = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.vItem.push_back(item);
            
            CfgDrawTable::Add(&this->m_cfgDrawTable, &stu);
            stu.~CfgDrawReward();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitMapRoadTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/MapRoad.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MAP_ROAD_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMapRoad stu;
            CfgMapRoad::CfgMapRoad(&stu);
            
            int32_t nIndex = 0;
            stu.nIndex = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nNextIndex = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nMapId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string strRoad;
            char v13;
            std::allocator<char>::allocator(&v13);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&strRoad, v1->pString, &v13);
            std::allocator<char>::~allocator(&v13);
            ++nIndex;
            
            std::string delims;
            char v15;
            std::allocator<char>::allocator(&v15);
            std::string::string(&delims, "|", &v15);
            
            StringVector vRoad;
            Answer::StringUtility::split(&vRoad, &strRoad, &delims, 0);
            delims.~string();
            std::allocator<char>::~allocator(&v15);
            
            for (auto& roadStr : vRoad)
            {
                std::string v17;
                char v18;
                std::allocator<char>::allocator(&v18);
                std::string::string(&v17, ":", &v18);
                
                StringVector vPos;
                Answer::StringUtility::split(&vPos, roadStr, &v17, 0);
                v17.~string();
                std::allocator<char>::~allocator(&v18);
                
                if (vPos.size() == 2)
                {
                    Position __x;
                    __x.x = atoi(vPos[0]->c_str());
                    __x.y = atoi(vPos[1]->c_str());
                    stu.road.push_back(__x);
                }
                vPos.~vector();
            }
            vRoad.~vector();
            strRoad.~string();
            
            CfgMapRoadTable::Add(&this->m_cfgMapRoadTable, &stu);
            stu.~CfgMapRoad();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitTrailerTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/Trailer.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TRAILER_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgTrailer stu;
            CfgTrailer::CfgTrailer(&stu);
            
            int32_t nIndex = 0;
            stu.nId = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            stu.nAttrList = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.TrailerQuality = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.nTaskId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.PureDamage = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            CfgTrailerTable::Add(&this->m_cfgTrailerTable, &stu);
            stu.~CfgTrailer();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitMYSJRewardTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/MoYuShiJieReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MYSJ_REWARD_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
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
            int32_t nId = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            stu.nId = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nClass = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nCount = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nBind = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nWeight = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nShow = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nGroup = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nMin = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nMax = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nBroad = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nSpecial = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
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
        cardList.~list();
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitMaintainCompensateTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/MaintainCompensate.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MAINTAIN_COMPENSATE_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgMaintainCompensate stu;
            CfgMaintainCompensate::CfgMaintainCompensate(&stu);
            
            int32_t nIndex = 0;
            stu.nIndex = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string p_StringTime;
            char v6;
            std::allocator<char>::allocator(&v6);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&p_StringTime, v1->pString, &v6);
            stu.nTime = Answer::DayTime::StringToIntTime(&p_StringTime);
            p_StringTime.~string();
            std::allocator<char>::~allocator(&v6);
            ++nIndex;
            
            std::string strItems;
            char v9;
            std::allocator<char>::allocator(&v9);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&strItems, v2->pString, &v9);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.vItems = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v9);
            ++nIndex;
            
            CfgMaintainCompensateTable::Add(&this->m_cfgMaintainCompensateTable, &stu);
            stu.~CfgMaintainCompensate();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitWishRewardTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/WishReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WISH_REWARD_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgWishReward stu;
            CfgWishReward::CfgWishReward(&stu);
            
            int32_t nIndex = 0;
            int32_t nId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nTime = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string strItems;
            char v6;
            std::allocator<char>::allocator(&v6);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&strItems, v1->pString, &v6);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.vReward = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v6);
            ++nIndex;
            
            CfgWishRewardTable::Add(&this->m_cfgWishRewardTable, nId, &stu);
            stu.~CfgWishReward();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitBFZLEnterCostTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/BingFengZouLang.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BFZL_ENTER_COST_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            int32_t nTimes = CDBCFile::Search_Posistion(&readFile, i, 1)->iValue;
            ++nIndex;
            
            std::string bCombi;
            char v5;
            std::allocator<char>::allocator(&v5);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&bCombi, v1->pString, &v5);
            
            std::list<ItemData> vItem;
            CItemHelper::parseItemDataListString((const std::string *const)&vItem, (bool)bCombi.c_str());
            bCombi.~string();
            std::allocator<char>::~allocator(&v5);
            ++nIndex;
            
            CfgBFZLEnterCostTable::AddEnterCost(&this->m_cfgBFZLEnterCostTable, nTimes, &vItem);
            vItem.~list();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitBlacketMarketTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/BlackMarket.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BLACK_MARKET_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            int32_t nDays = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            int8_t nOrder = CDBCFile::Search_Posistion(&readFile, i, 1)->iValue;
            ++nIndex;
            
            std::string strItem;
            char v9;
            std::allocator<char>::allocator(&v9);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&strItem, v1->pString, &v9);
            
            MemChrBag item1;
            CItemHelper::parseItemString(&item1, &strItem);
            strItem.~string();
            std::allocator<char>::~allocator(&v9);
            ++nIndex;
            
            std::string v10;
            char v11;
            std::allocator<char>::allocator(&v11);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&v10, v2->pString, &v11);
            
            MemChrBag item2;
            CItemHelper::parseItemString(&item2, &v10);
            v10.~string();
            std::allocator<char>::~allocator(&v11);
            ++nIndex;
            
            std::string v12;
            char v13;
            std::allocator<char>::allocator(&v13);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&v12, v3->pString, &v13);
            
            MemChrBag item3;
            CItemHelper::parseItemString(&item3, &v12);
            v12.~string();
            std::allocator<char>::~allocator(&v13);
            
            int32_t nOldPrice = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            int32_t nPrice = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            int32_t nBroadcast = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            CfgBlacketMarketTable::Add(&this->m_cfgBlacketMarketTable, nDays, nOrder, 1, &item1, nPrice, nBroadcast);
            CfgBlacketMarketTable::Add(&this->m_cfgBlacketMarketTable, nDays, nOrder, 2, &item2, nPrice, nBroadcast);
            CfgBlacketMarketTable::Add(&this->m_cfgBlacketMarketTable, nDays, nOrder, 3, &item3, nPrice, nBroadcast);
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitLevelChatTimesTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/PublicChatTimes.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_LEVEL_CHAT_TIMES_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            int32_t nLevel = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            int32_t nTimes = CDBCFile::Search_Posistion(&readFile, i, 1)->iValue;
            ++nIndex;
            
            auto v1 = std::map<int, int>::operator[](&this->m_cfgLevelChatTable, &nLevel);
            *v1 = nTimes;
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

int32_t CfgData::GetChatTimes(const CfgData *const this, int32_t nLevel)
{
    auto it = this->m_cfgLevelChatTable.find(nLevel);
    if (it != this->m_cfgLevelChatTable.end())
    {
        return it->second;
    }
    return -1;
}

void CfgData::InitSuperMemberTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/SuperMember.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SUPER_MEMBER_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            std::string platform;
            char v6;
            std::allocator<char>::allocator(&v6);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, 0);
            std::string::string(&platform, v1->pString, &v6);
            std::allocator<char>::~allocator(&v6);
            
            CfgSuperMember stu;
            stu.nGold = CDBCFile::Search_Posistion(&readFile, i, 1)->iValue;
            stu.nQQ = CDBCFile::Search_Posistion(&readFile, i, 2)->iValue;
            ++nIndex;
            stu.nIcon = CDBCFile::Search_Posistion(&readFile, i, 4)->iValue;
            ++nIndex;
            
            auto v2 = std::map<std::string, CfgSuperMember>::operator[](&this->m_cfgSuperMember, &platform);
            *v2 = stu;
            platform.~string();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

const CfgSuperMember *CfgData::GetSuperMember(const CfgData *const this, const std::string *const platform)
{
    auto it = this->m_cfgSuperMember.find(*platform);
    if (it != this->m_cfgSuperMember.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitSouGouSkinTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/SogouSkin.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SOU_GOU_SKIN_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgSouGouSkin stu{};
            
            int32_t nIndex = 0;
            std::string platform;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&platform, v1->pString, &v7);
            std::allocator<char>::~allocator(&v7);
            ++nIndex;
            
            std::string strItems;
            char v10;
            std::allocator<char>::allocator(&v10);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&strItems, v2->pString, &v10);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.vReward = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v10);
            
            stu.nIcon = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto v3 = std::map<std::string, CfgSouGouSkin>::operator[](&this->m_cfgSouGouSkin, &platform);
            *v3 = stu;
            platform.~string();
            stu.~CfgSouGouSkin();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

const CfgSouGouSkin *CfgData::GetSouGouSkin(const CfgData *const this, const std::string *const platform)
{
    auto it = this->m_cfgSouGouSkin.find(*platform);
    if (it != this->m_cfgSouGouSkin.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitPetEquipTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/PetEquip.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_PET_EQUIP_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgPetEquip stu{};
            
            int32_t nIndex = 0;
            stu.nId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nPrice = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nQuality = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nSuitId = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string path;
            std::string addonAttr;
            char v7;
            std::allocator<char>::allocator(&v7);
            std::string::string(&path, "./ServerConfig/Tables/PetEquip.txt", &v7);
            
            char v9;
            std::allocator<char>::allocator(&v9);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&addonAttr, v1->pString, &v9);
            
            AttrAddonVector __x;
            CfgData::paraseAttrAddon(&__x, this, &addonAttr, i, &path);
            stu.vAttr = __x;
            __x.~vector();
            addonAttr.~string();
            std::allocator<char>::~allocator(&v9);
            path.~string();
            std::allocator<char>::~allocator(&v7);
            ++nIndex;
            nIndex += 5;
            stu.nBroadcast = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.bCanDrop = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            stu.nGrade = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            std::string v11;
            char v12;
            std::allocator<char>::allocator(&v12);
            std::string::string(&v11, "./ServerConfig/Tables/PetEquip.txt", &v12);
            
            char v14;
            std::allocator<char>::allocator(&v14);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string v13;
            std::string::string(&v13, v2->pString, &v14);
            
            AttrAddonVector v10;
            CfgData::paraseAttrAddon(&v10, this, &v13, i, &v11);
            stu.vOwnerAttr = v10;
            v10.~vector();
            v13.~string();
            std::allocator<char>::~allocator(&v14);
            v11.~string();
            std::allocator<char>::~allocator(&v12);
            
            stu.nNeedStar = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            CfgPetEquipTable::AddEquip(&this->m_cfgPetEquipTable, &stu);
            stu.~CfgPetEquip();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitWeiXinTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/WeiXin.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WEI_XIN_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            
            int32_t nIndex = 0;
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, 0);
            std::string::operator=(&stu, v1->pString);
            stu.nIconId = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            ++nIndex;
            ++nIndex;
            
            std::string strItems;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&strItems, v2->pString, &v7);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.vReward = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v7);
            ++nIndex;
            
            CfgWeiXinTable::Add(&this->m_cfgWeiXinTable, &stu);
            stu.~CfgWeiXingGift();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitAdultGiftTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/ShenFenYanZheng.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ADULT_GIFT_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            
            int32_t nIndex = 1;
            
            std::string strItems;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&readFile, i, nIndex);
            std::string::string(&strItems, v1->pString, &v7);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.vReward = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v7);
            
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&readFile, i, ++nIndex);
            std::string::operator=(&stu, v2->pString);
            stu.nIconId = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            CfgAdultGiftTable::Add(&this->m_cfgAdultGiftTable, &stu);
            stu.~CfgAdultGift();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

// ==================== 解析辅助方法 ====================

void CfgData::parseMonsterSkill(CfgData *const this, int32_t id, MonsterSkill (*const vSkill)[10], const std::string *const strSkill)
{
    if (strSkill->empty() || strSkill->size() <= 3) return;
    
    std::string delims;
    char v22;
    std::allocator<char>::allocator(&v22);
    std::string::string(&delims, "|", &v22);
    
    StringVector skills;
    Answer::StringUtility::split(&skills, strSkill, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v22);
    
    int32_t isize = (int32_t)skills.size();
    if (isize > 9) isize = 10;
    
    for (int32_t i = 0; i < isize; ++i)
    {
        std::string v23;
        char v24;
        std::allocator<char>::allocator(&v24);
        std::string::string(&v23, ":", &v24);
        
        StringVector skill;
        Answer::StringUtility::split(&skill, skills[i], &v23, 0);
        v23.~string();
        std::allocator<char>::~allocator(&v24);
        
        if (skill.size() == 3)
        {
            (*vSkill)[i].maxHp = atoi(skill[0]->c_str());
            (*vSkill)[i].minHp = atoi(skill[1]->c_str());
            (*vSkill)[i].skillid = atoi(skill[2]->c_str());
        }
        else
        {
            Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
                "CfgData::parseMonsterSkill wrong data with id = %d, string = %s\n",
                id, strSkill->c_str());
        }
        skill.~vector();
    }
    skills.~vector();
}

TaskRequest CfgData::parseTaskCondition(CfgData *const this, int32_t id, int32_t condition, const std::string *const strRequest)
{
    TaskRequest request;
    memset(&request, 0, sizeof(request));
    
    std::string delims;
    char v21;
    std::allocator<char>::allocator(&v21);
    std::string::string(&delims, ":", &v21);
    
    StringVector requests;
    Answer::StringUtility::split(&requests, strRequest, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v21);
    
    int32_t nSize = (int32_t)requests.size();
    
    switch (nSize)
    {
    case 3:
        request.param1 = atoi(requests[0]->c_str());
        request.param2 = atoi(requests[1]->c_str());
        request.param3 = atoi(requests[2]->c_str());
        break;
    case 2:
        request.param1 = atoi(requests[0]->c_str());
        request.param2 = atoi(requests[1]->c_str());
        break;
    case 1:
        request.param1 = atoi(requests[0]->c_str());
        break;
    default:
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgData::parseTaskCondition wrong data with id = %d, string = %s\n",
            id, strRequest->c_str());
        break;
    }
    
    requests.~vector();
    return request;
}

void CfgData::parseTaskDrop(CfgData *const this, int32_t id, const std::string *const strDrop)
{
    // 解析任务掉落
    if (strDrop->empty() || *strDrop == "-1") return;
    
    // 实现解析逻辑...
}

int32_t CfgData::GetMonsterReviveTime(CfgData *const this, int32_t Time, int32_t BossId)
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

void CfgData::InitMobilePhoneGiftTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/ShouJi.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MOBILE_PHONE_GIFT_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            
            int32_t nIndex = 1;
            
            std::string strItems;
            char v8;
            std::allocator<char>::allocator(&v8);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&strItems, v1->pString, &v8);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.vItem = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v8);
            
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex);
            std::string::operator=(&stu, v2->pString);
            stu.nIcon = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto v3 = std::map<std::string, CfgMobilePhoneGift>::operator[](&this->m_CfgMobilePhoneGift, &stu.strPlatfrom);
            *v3 = stu;
            stu.~CfgMobilePhoneGift();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}
void CfgData::InitMiniClientTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/WeiDuan.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MINI_CLIENT_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            
            int32_t nIndex = 0;
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::operator=(&stu, v1->pString);
            stu.nIconDownload = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.nIconLogin = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string strItems;
            char v8;
            std::allocator<char>::allocator(&v8);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&strItems, v2->pString, &v8);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.vReward = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v8);
            ++nIndex;
            
            auto v3 = std::map<std::string, CfgMiniClient>::operator[](&this->m_CfgMiniClient, &stu.strPlatfrom);
            *v3 = stu;
            stu.~CfgMiniClient();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitWuHunShopTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/MysterShop.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WU_HUN_SHOP_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgWuHunShop stu;
            memset(&stu, 0, sizeof(stu));
            
            int32_t nIndex = 0;
            stu.Index = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            int32_t ShopId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string strItem;
            char v10;
            std::allocator<char>::allocator(&v10);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&strItem, v1->pString, &v10);
            
            MemChrBag v4;
            CItemHelper::parseItemString(&v4, &strItem);
            stu.Item = v4;
            strItem.~string();
            std::allocator<char>::~allocator(&v10);
            
            stu.Rate = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            stu.Const = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto v2 = std::map<int, std::list<CfgWuHunShop>>::operator[](&this->m_CfgWuHunShopMap, &ShopId);
            v2->push_back(stu);
            
            auto v3 = std::map<int, CfgWuHunShop>::operator[](&this->m_CfgWuHunShopItemMap, &stu.Index);
            *v3 = stu;
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitWuHunItemTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/WuHun.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_WU_HUN_ITEM_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            WuHunItem stu{};
            
            int32_t nIndex = 0;
            stu.nId = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nLevel = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nQuality = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nNeedQuality = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            
            std::string v6;
            char v7;
            std::allocator<char>::allocator(&v7);
            std::string::string(&v6, "./ServerConfig/Tables/WuHun.txt", &v7);
            
            std::string v8;
            char v9;
            std::allocator<char>::allocator(&v9);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&v8, v1->pString, &v9);
            
            std::list<AddAttribute> __x;
            CfgData::parseAddAttribues((CfgData *const)&__x, &v8, i, &v6);
            stu.lAttrList = __x;
            __x.~list();
            v8.~string();
            std::allocator<char>::~allocator(&v9);
            v6.~string();
            std::allocator<char>::~allocator(&v7);
            
            stu.nTalentId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.nTalentLevel = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            nIndex += 8;
            stu.overlay = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            nIndex += 3;
            stu.nDressLevel = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            
            auto v2 = std::map<int, WuHunItem>::operator[](&this->m_WuHunItemMap, &stu.nId);
            *v2 = stu;
            stu.~WuHunItem();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitWuHunCreateTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/WuHunMake.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_CREATE_WU_HUN_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CreateWuHun stu{};
            
            int32_t nIndex = 0;
            stu.nId = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            nIndex += 2;
            
            std::string bCombi;
            char v11;
            std::allocator<char>::allocator(&v11);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&bCombi, v1->pString, &v11);
            
            std::list<ItemData> strItems;
            CItemHelper::parseItemDataListString((const std::string *const)&strItems, (bool)bCombi.c_str());
            stu.ConstItem = strItems;
            strItems.~list();
            bCombi.~string();
            std::allocator<char>::~allocator(&v11);
            ++nIndex;
            
            std::string v13;
            char v14;
            std::allocator<char>::allocator(&v14);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&v13, v2->pString, &v14);
            
            std::list<RateItem> v12;
            CItemHelper::parseRateItemDataListString((const std::string *const)&v12);
            stu.GetItemRate = v12;
            v12.~list();
            v13.~string();
            std::allocator<char>::~allocator(&v14);
            ++nIndex;
            nIndex += 3;
            
            std::string strItem;
            char v16;
            std::allocator<char>::allocator(&v16);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&strItem, v3->pString, &v16);
            
            ItemData v25 = CItemHelper::parseItemDataString(&strItem);
            stu.SpecialCost.m_nId = v25.m_nId;
            stu.SpecialCost.m_nClass = v25.m_nClass;
            stu.SpecialCost.m_nCount = v25.m_nCount;
            strItem.~string();
            std::allocator<char>::~allocator(&v16);
            ++nIndex;
            
            std::string v18;
            char v19;
            std::allocator<char>::allocator(&v19);
            const CDBCFile::FIELD *v4 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&v18, v4->pString, &v19);
            
            std::list<RateItem> v17;
            CItemHelper::parseRateItemDataListString((const std::string *const)&v17);
            stu.GetItemRate2 = v17;
            v17.~list();
            v18.~string();
            std::allocator<char>::~allocator(&v19);
            ++nIndex;
            
            auto v5 = std::map<int, CreateWuHun>::operator[](&this->m_CreateWuHunMap, &stu.nId);
            *v5 = stu;
            stu.~CreateWuHun();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitBossDistribution(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/MonsterRefreshInMaps.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_BOSS_LEVEL_IFNO_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            BossLevelInfo stu;
            memset(&stu, 0, sizeof(stu));
            std::list<int>::list(&stu.BossMapList);
            
            int32_t nIndex = 0;
            stu.BossLevel = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            
            std::string BossMapListString;
            char v9;
            std::allocator<char>::allocator(&v9);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&BossMapListString, v2->pString, &v9);
            std::allocator<char>::~allocator(&v9);
            ++nIndex;
            
            std::list<int> __x;
            CfgData::paraseInt32List(this, &__x, &BossMapListString, 0, nullptr);
            stu.BossMapList = __x;
            __x.~list();
            BossMapListString.~string();
            
            BossLevelInfo p_stu;
            BossLevelInfo::BossLevelInfo(&p_stu, &stu);
            BossDistribution::AddBossLevelInfo(&this->m_BossDistribution, &p_stu);
            p_stu.~BossLevelInfo();
            stu.~BossLevelInfo();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
    
    // 加载地图Boss信息
    CDBCFile TabFile2;
    CDBCFile::CDBCFile(&TabFile2, 0);
    if (CDBCFile::OpenFromTXT(&TabFile2, "./ServerConfig/Tables/MapClassInfo.txt"))
    {
        iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile2);
        iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile2);
        
        if (iBaseColumnCount > 0)
        {
            for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)
            {
                MapBossInfo stu;
                memset(&stu, 0, sizeof(stu));
                std::list<int>::list(&stu.BossMapList);
                
                int32_t nIndex_0 = 0;
                stu.nId = CDBCFile::Search_Posistion(&TabFile2, i_0, nIndex_0++)->iValue;
                
                std::string BossMapListString_0;
                char v12;
                std::allocator<char>::allocator(&v12);
                const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&TabFile2, i_0, nIndex_0);
                std::string::string(&BossMapListString_0, v3->pString, &v12);
                std::allocator<char>::~allocator(&v12);
                ++nIndex_0;
                
                std::list<int> v13;
                CfgData::paraseInt32List(this, &v13, &BossMapListString_0, 0, nullptr);
                stu.BossMapList = v13;
                v13.~list();
                BossMapListString_0.~string();
                
                MapBossInfo v14;
                MapBossInfo::MapBossInfo(&v14, &stu);
                BossDistribution::AddMapBossInfo(&this->m_BossDistribution, &v14);
                v14.~MapBossInfo();
                stu.~MapBossInfo();
            }
        }
        CDBCFile::~CDBCFile(&TabFile2);
    }
}

void CfgData::InitSpecialBossMapCfgMap(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/MapConsume.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MAP_BOSS_INFO_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            SpecialBossMapCfg stu;
            memset(&stu, 0, sizeof(stu));
            
            int32_t nIndex = 0;
            stu.MapId = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            ++nIndex;
            
            std::string strItem;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&strItem, v1->pString, &v7);
            
            ItemData v13 = CItemHelper::parseItemDataString(&strItem);
            stu.ConstItem.m_nId = v13.m_nId;
            stu.ConstItem.m_nClass = v13.m_nClass;
            stu.ConstItem.m_nCount = v13.m_nCount;
            strItem.~string();
            std::allocator<char>::~allocator(&v7);
            
            stu.StartCD = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.ContinuedTime = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto v2 = std::map<int, SpecialBossMapCfg>::operator[](&this->m_SpecialBossMapCfgMap, &stu.MapId);
            *v2 = stu;
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitDuiHuanLimitTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/ExchangeShengYaoBi.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_MAP_BOSS_INFO_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        this->m_DuiHuanLimit.clear();
        
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            DuiHuanLimit stu;
            int32_t nIndex = 1;
            int32_t MaxLevel = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
            stu.ShengYaoBi = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.ConstGold = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.ConstCurrency = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.Limit = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto v1 = std::map<int, DuiHuanLimit>::operator[](&this->m_DuiHuanLimit, &MaxLevel);
            *v1 = stu;
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

DuiHuanLimit *CfgData::GetDuiHuanLimitCount(CfgData *const this, int32_t Level)
{
    auto it = this->m_DuiHuanLimit.lower_bound(Level);
    if (it != this->m_DuiHuanLimit.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitSuperTeHuiTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/SuperDiscount.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SUPER_DISCOUNT_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        this->m_SuperTeHuiCfgMap.clear();
        
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            SuperTeHuiCfg stu{};
            
            int32_t nIndex = 0;
            stu.nIndex = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nNeedVip = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            
            std::string strItems;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&strItems, v1->pString, &v7);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.Items = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v7);
            
            stu.nPrice = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.nGongGaoId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto v2 = std::map<int, SuperTeHuiCfg>::operator[](&this->m_SuperTeHuiCfgMap, &stu.nIndex);
            *v2 = stu;
            stu.~SuperTeHuiCfg();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

SuperTeHuiCfg *CfgData::GetSuperTeHuiCfg(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_SuperTeHuiCfgMap.find(nIndex);
    if (it != this->m_SuperTeHuiCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitJewelPavilionTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/TreasureShop.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TREASURE_SHOP_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        this->m_JewelPavilionCfgMap.clear();
        
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            JewelPavilionCfg stu;
            memset(&stu, 0, sizeof(stu));
            
            int32_t nIndex = 0;
            stu.nDay = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            stu.nIndex = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string strItem;
            char v8;
            std::allocator<char>::allocator(&v8);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&strItem, v1->pString, &v8);
            
            MemChrBag v3;
            CItemHelper::parseItemString(&v3, &strItem);
            stu.Item = v3;
            strItem.~string();
            std::allocator<char>::~allocator(&v8);
            
            stu.nPrice = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto key = std::make_pair(stu.nDay, stu.nIndex);
            auto result = this->m_JewelPavilionCfgMap.insert(std::make_pair(key, stu));
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

JewelPavilionCfg *CfgData::GetJewelPavilionCfg(CfgData *const this, int32_t nDay, int32_t nIndex)
{
    auto key = std::make_pair(nDay, nIndex);
    auto it = this->m_JewelPavilionCfgMap.find(key);
    if (it != this->m_JewelPavilionCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitGoblinTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/GoblinUp.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_GOBLIN_SUIT_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        this->m_GoblinCfgMap.clear();
        
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            GoblinCfg stu;
            int32_t nIndex = 0;
            int32_t nType = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            int32_t nLevel = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.UpAttr = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.SuitId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            stu.ConstCurr = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto key = std::make_pair(nType, nLevel);
            this->m_GoblinCfgMap.insert(std::make_pair(key, stu));
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

GoblinCfg *CfgData::GetGoblinCfg(CfgData *const this, int32_t nType, int32_t nLevel)
{
    auto key = std::make_pair(nType, nLevel);
    auto it = this->m_GoblinCfgMap.find(key);
    if (it != this->m_GoblinCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitShouHuRefining(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/StarSpaceLevel.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_GOBLIN_SUIT_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        this->m_ShouHuRefinishingCfgMap.clear();
        
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            ShouHuRefinishingCfg stu{};
            
            int32_t nIndex = 0;
            int32_t nType = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            int32_t nLevel = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            
            std::string bCombi;
            char v12;
            std::allocator<char>::allocator(&v12);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&bCombi, v2->pString, &v12);
            
            std::list<ItemData> strItems;
            CItemHelper::parseItemDataListString((const std::string *const)&strItems, (bool)bCombi.c_str());
            stu.lCostList = strItems;
            strItems.~list();
            bCombi.~string();
            std::allocator<char>::~allocator(&v12);
            ++nIndex;
            
            std::string v14;
            char v15;
            std::allocator<char>::allocator(&v15);
            std::string::string(&v14, "./ServerConfig/Tables/StarSpaceLevel.txt", &v15);
            
            std::string v16;
            char v17;
            std::allocator<char>::allocator(&v17);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&v16, v3->pString, &v17);
            
            std::list<AddAttribute> __x;
            CfgData::parseAddAttribues((CfgData *const)&__x, &v16, i, &v14);
            stu.lAttrList = __x;
            __x.~list();
            v16.~string();
            std::allocator<char>::~allocator(&v17);
            v14.~string();
            std::allocator<char>::~allocator(&v15);
            
            stu.SuitId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto key = std::make_pair(nType, nLevel);
            this->m_ShouHuRefinishingCfgMap.insert(std::make_pair(key, stu));
            stu.~ShouHuRefinishingCfg();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

ShouHuRefinishingCfg *CfgData::GetShouHuRefinishingCfg(CfgData *const this, int32_t nType, int32_t nLevel)
{
    auto key = std::make_pair(nType, nLevel);
    auto it = this->m_ShouHuRefinishingCfgMap.find(key);
    if (it != this->m_ShouHuRefinishingCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitWingEquipPolish(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/WingEquipPolish.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_GOBLIN_SUIT_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        this->m_WingEquipPolishCfgMap.clear();
        
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            WingEquipPolish stu{};
            
            int32_t nIndex = 0;
            int32_t nType = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            int32_t nLevel = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            
            std::string v18;
            char v19;
            std::allocator<char>::allocator(&v19);
            std::string::string(&v18, "./ServerConfig/Tables/StarSpaceLevel.txt", &v19);
            
            std::string v20;
            char v21;
            std::allocator<char>::allocator(&v21);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&v20, v2->pString, &v21);
            
            std::list<AddAttribute> __x;
            CfgData::parseAddAttribues((CfgData *const)&__x, &v20, i, &v18);
            stu.lAttrList = __x;
            __x.~list();
            v20.~string();
            std::allocator<char>::~allocator(&v21);
            v18.~string();
            std::allocator<char>::~allocator(&v19);
            ++nIndex;
            
            std::string bCombi;
            char v24;
            std::allocator<char>::allocator(&v24);
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&bCombi, v3->pString, &v24);
            
            std::list<ItemData> strItems;
            CItemHelper::parseItemDataListString((const std::string *const)&strItems, (bool)bCombi.c_str());
            stu.lCostList = strItems;
            strItems.~list();
            bCombi.~string();
            std::allocator<char>::~allocator(&v24);
            
            stu.nConstMoney = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.SuitId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.GongGaoId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto key = std::make_pair(nType, nLevel);
            this->m_WingEquipPolishCfgMap.insert(std::make_pair(key, stu));
            stu.~WingEquipPolish();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
    
    // 加载套装属性
    CDBCFile TabFile2;
    CDBCFile::CDBCFile(&TabFile2, 0);
    if (CDBCFile::OpenFromTXT(&TabFile2, "./ServerConfig/Tables/WingEquipPolishSuit.txt"))
    {
        iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile2);
        iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile2);
        
        if (iBaseColumnCount > 0)
        {
            this->m_WingEquipPolishSuitMap.clear();
            
            for (int32_t i_0 = 0; i_0 < iBaseTableCount; ++i_0)
            {
                int32_t nId = CDBCFile::Search_Posistion(&TabFile2, i_0, 0)->iValue;
                ++nIndex;
                
                std::string v28;
                char v29;
                std::allocator<char>::allocator(&v29);
                std::string::string(&v28, "./ServerConfig/Tables/StarSpaceSuit.txt", &v29);
                
                std::string v30;
                char v31;
                std::allocator<char>::allocator(&v31);
                const CDBCFile::FIELD *v5 = CDBCFile::Search_Posistion(&TabFile2, i_0, nIndex);
                std::string::string(&v30, v5->pString, &v31);
                
                std::list<AddAttribute> AddAttrs;
                CfgData::parseAddAttribues((CfgData *const)&AddAttrs, &v30, i_0, &v28);
                v30.~string();
                std::allocator<char>::~allocator(&v31);
                v28.~string();
                std::allocator<char>::~allocator(&v29);
                ++nIndex;
                
                auto v6 = std::map<int, std::list<AddAttribute>>::operator[](&this->m_WingEquipPolishSuitMap, &nId);
                *v6 = AddAttrs;
                AddAttrs.~list();
            }
        }
        CDBCFile::~CDBCFile(&TabFile2);
    }
}

WingEquipPolish *CfgData::GetWingEquipPolishCfg(CfgData *const this, int32_t nType, int32_t nLevel)
{
    auto key = std::make_pair(nType, nLevel);
    auto it = this->m_WingEquipPolishCfgMap.find(key);
    if (it != this->m_WingEquipPolishCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitGuiGuDaoRenTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/TaoistTask.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_GUI_GU_DAO_REN_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        this->m_GuiGuDaoRenCfgMap.clear();
        
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            GuiGuDaoRenCfg stu{};
            
            int32_t nIndex = 0;
            stu.nNpcId = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nMaxCount = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string RefreshMonsterString;
            char v23;
            std::allocator<char>::allocator(&v23);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&RefreshMonsterString, v1->pString, &v23);
            std::allocator<char>::~allocator(&v23);
            ++nIndex;
            
            std::string delims;
            char v25;
            std::allocator<char>::allocator(&v25);
            std::string::string(&delims, "|", &v25);
            
            StringVector SplitStr;
            Answer::StringUtility::split(&SplitStr, &RefreshMonsterString, &delims, 0);
            delims.~string();
            std::allocator<char>::~allocator(&v25);
            
            for (auto& monsterStr : SplitStr)
            {
                std::string v28;
                char v29;
                std::allocator<char>::allocator(&v29);
                std::string::string(&v28, ":", &v29);
                
                StringVector vstr;
                Answer::StringUtility::split(&vstr, monsterStr, &v28, 0);
                v28.~string();
                std::allocator<char>::~allocator(&v29);
                
                if (vstr.size() == 4)
                {
                    RefreshMonster tmpStu;
                    tmpStu.nCount = atoi(vstr[0]->c_str());
                    tmpStu.BossId = atoi(vstr[1]->c_str());
                    tmpStu.AliveTime = atoi(vstr[2]->c_str());
                    tmpStu.GongGaoId = atoi(vstr[3]->c_str());
                    stu.lRefreshMonster.push_back(tmpStu);
                }
                vstr.~vector();
            }
            SplitStr.~vector();
            RefreshMonsterString.~string();
            
            stu.vItemData.reserve(3);
            stu.vItem.reserve(3);
            
            for (int32_t j = 0; j <= 2; ++j)
            {
                std::string strItem;
                char v32;
                std::allocator<char>::allocator(&v32);
                const CDBCFile::FIELD *v11 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&strItem, v11->pString, &v32);
                
                ItemData __x = CItemHelper::parseItemDataString(&strItem);
                stu.vItemData.push_back(__x);
                strItem.~string();
                std::allocator<char>::~allocator(&v32);
                ++nIndex;
                
                std::string v34;
                char v35;
                std::allocator<char>::allocator(&v35);
                const CDBCFile::FIELD *v12 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
                std::string::string(&v34, v12->pString, &v35);
                
                MemChrBag v33;
                CItemHelper::parseItemString(&v33, &v34);
                stu.vItem.push_back(v33);
                v34.~string();
                std::allocator<char>::~allocator(&v35);
                ++nIndex;
            }
            
            std::string path;
            char v38;
            std::allocator<char>::allocator(&v38);
            std::string::string(&path, "./ServerConfig/Tables/TaoistTask.txt", &v38);
            
            std::string str;
            char v40;
            std::allocator<char>::allocator(&v40);
            const CDBCFile::FIELD *v13 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&str, v13->pString, &v40);
            
            Int32Vector v36;
            CfgData::paraseInt32Vector(&v36, this, &str, &path, 0);
            stu.vMapId = v36;
            v36.~vector();
            str.~string();
            std::allocator<char>::~allocator(&v40);
            path.~string();
            std::allocator<char>::~allocator(&v38);
            ++nIndex;
            
            auto v14 = std::map<int, GuiGuDaoRenCfg>::operator[](&this->m_GuiGuDaoRenCfgMap, &stu.nNpcId);
            *v14 = stu;
            stu.~GuiGuDaoRenCfg();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

GuiGuDaoRenCfg *CfgData::GetGuiGuDaoRenCfg(CfgData *const this, int32_t NpcId)
{
    auto it = this->m_GuiGuDaoRenCfgMap.find(NpcId);
    if (it != this->m_GuiGuDaoRenCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitShiZhuangTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/ShiZhuang.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SHIZHUANG_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgShiZhuang stu;
            CfgShiZhuang::CfgShiZhuang(&stu);
            
            int32_t nIndex = 0;
            stu.nId = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            ++nIndex;
            stu.nType = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            for (int32_t j = 0; j <= 6; ++j)
            {
                AddAttribute AddAttr;
                AddAttribute::AddAttribute(&AddAttr);
                AddAttr.m_nAddAttrType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                AddAttr.m_nAddAttrValue = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                if (AddAttr.m_nAddAttrValue > 0)
                {
                    stu.vAttr.push_back(AddAttr);
                }
            }
            nIndex += 7;
            stu.nSuitId = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
            
            CfgShiZhuangTable::AddShiZhuang(&this->m_cfgShiZhuangTable, &stu);
            stu.~CfgShiZhuang();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitShiZhuangLevelTable(CfgData *const this)
{
    CDBCFile readFile;
    CDBCFile::CDBCFile(&readFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&readFile, "./ServerConfig/Tables/ShiZhuangLevel.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SHIZHUANG_LEVEL_TABLE failed,please check!!!\n");
        CDBCFile::~CDBCFile(&readFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&readFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&readFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgShiZhuangLevel stu;
            CfgShiZhuangLevel::CfgShiZhuangLevel(&stu);
            
            int32_t nIndex = 0;
            stu.nType = CDBCFile::Search_Posistion(&readFile, i, 0)->iValue;
            stu.nLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nLevelExp = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nNeedLevel = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nCostItem = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nGetExp = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nSmallCritRate = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nLargeCritRate = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            stu.nBroadcast = CDBCFile::Search_Posistion(&readFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            for (int32_t j = 0; j <= 6; ++j)
            {
                AddAttribute AddAttr;
                AddAttribute::AddAttribute(&AddAttr);
                AddAttr.m_nAddAttrType = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                AddAttr.m_nAddAttrValue = CDBCFile::Search_Posistion(&readFile, i, nIndex++)->iValue;
                if (AddAttr.m_nAddAttrValue > 0)
                {
                    stu.vAttr.push_back(AddAttr);
                }
            }
            
            CfgShiZhuangTable::AddShiZhuangLevel(&this->m_cfgShiZhuangTable, &stu);
            stu.~CfgShiZhuangLevel();
        }
    }
    CDBCFile::~CDBCFile(&readFile);
}

void CfgData::InitMonthlyChouJiangTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/TurntableReward.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_TURNTABLE_REWARD_TABLE failed, please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            int32_t nIndex = 0;
            int32_t Month = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            int32_t nId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string strItem;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&strItem, v2->pString, &v7);
            
            RateItem ItemRate;
            CItemHelper::parseRateItemDataString(&ItemRate, &strItem);
            strItem.~string();
            std::allocator<char>::~allocator(&v7);
            ++nIndex;
            
            MonthlyChouJiangTable::AddMonthlyChouJiangItemMap(&this->m_MonthlyChouJiangTable, Month, nId, ItemRate);
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitActDropTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/shoujihuodong.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_ACTDROP_TABLE failed, please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            ActDropItem stu;
            memset(&stu, 0, sizeof(stu));
            
            int32_t nIndex = 0;
            int32_t Type = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            stu.nMinLevel = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.nMaxLevel = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.nMapType = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.nId = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.nClass = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.nCount = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            stu.nBind = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            std::string p_StringTime;
            char v8;
            std::allocator<char>::allocator(&v8);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&p_StringTime, v1->pString, &v8);
            stu.nStartTime = Answer::DayTime::StringToIntTime(&p_StringTime);
            p_StringTime.~string();
            std::allocator<char>::~allocator(&v8);
            ++nIndex;
            
            std::string v9;
            char v10;
            std::allocator<char>::allocator(&v10);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&v9, v2->pString, &v10);
            stu.nEndTime = Answer::DayTime::StringToIntTime(&v9);
            v9.~string();
            std::allocator<char>::~allocator(&v10);
            
            stu.nProbability = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            ++nIndex;
            
            auto v3 = std::map<int, std::list<ActDropItem>>::operator[](&this->m_ActDropItemListMap, &Type);
            v3->push_back(stu);
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitOutLinkFestivalTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/OutLinkFestival.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_OUT_LINK_FESTIVAL_TABLE failed, please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgOutLinkFestival stu;
            memset(&stu, 0, sizeof(stu));
            std::string::string(&stu.strPlatfrom);
            
            int32_t nIndex = 0;
            stu.nIndex = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            
            std::string p_StringTime;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&p_StringTime, v1->pString, &v7);
            stu.nStartTime = Answer::DayTime::StringToIntTime(&p_StringTime);
            p_StringTime.~string();
            std::allocator<char>::~allocator(&v7);
            ++nIndex;
            
            std::string v8;
            char v9;
            std::allocator<char>::allocator(&v9);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, nIndex);
            std::string::string(&v8, v2->pString, &v9);
            stu.nEndTime = Answer::DayTime::StringToIntTime(&v8);
            v8.~string();
            std::allocator<char>::~allocator(&v9);
            
            stu.nIcon = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex)->iValue;
            
            const CDBCFile::FIELD *v3 = CDBCFile::Search_Posistion(&TabFile, i, ++nIndex);
            std::string::operator=(&stu.strPlatfrom, v3->pString);
            ++nIndex;
            
            CfgOutLinkFestivalTable::Add(&this->m_cfgOutLinkFestivalTable, &stu);
            stu.~CfgOutLinkFestival();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

void CfgData::InitFestivalActivityTable(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/FestivalActivity.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_FESTIVAL_ACTIVITY_TABLE failed, please check!!!
");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgFestivalActivity stu{};
            
            int32_t nIndex = 0;
            stu.nIcon = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nStartDay = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nEndDay = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nPlantId = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nPlantCount = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nNpcId = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nNpcCount = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nTimes = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            stu.nPlantTime = CDBCFile::Search_Posistion(&TabFile, i, nIndex++)->iValue;
            
            // 解析开始/结束天数列表
            {
                std::string strStartDay;
                char vAlloc;
                std::allocator<char>::allocator(&vAlloc);
                const CDBCFile::FIELD *pField = CDBCFile::Search_Posistion(&TabFile, i, nIndex++);
                std::string::string(&strStartDay, pField->pString, &vAlloc);
                
                std::vector<int> vStart;
                CfgData::paraseInt32List(this, &vStart, &strStartDay, 0, nullptr);
                for (size_t idx = 0; idx < vStart.size() && idx < 5; ++idx)
                    stu.vStartDayList.push_back(vStart[idx]);
                std::vector<int>::~vector(&vStart);
                std::string::~string(&strStartDay);
                std::allocator<char>::~allocator(&vAlloc);
            }
            
            {
                std::string strEndDay;
                char vAlloc;
                std::allocator<char>::allocator(&vAlloc);
                const CDBCFile::FIELD *pField = CDBCFile::Search_Posistion(&TabFile, i, nIndex++);
                std::string::string(&strEndDay, pField->pString, &vAlloc);
                
                std::vector<int> vEnd;
                CfgData::paraseInt32List(this, &vEnd, &strEndDay, 0, nullptr);
                for (size_t idx = 0; idx < vEnd.size() && idx < 5; ++idx)
                    stu.vEndDayList.push_back(vEnd[idx]);
                std::vector<int>::~vector(&vEnd);
                std::string::~string(&strEndDay);
                std::allocator<char>::~allocator(&vAlloc);
            }
            
            // 解析植物位置 (MapId:PosX:PosY|MapId:PosX:PosY)
            {
                std::string strPlantPos;
                char vAlloc;
                std::allocator<char>::allocator(&vAlloc);
                const CDBCFile::FIELD *pField = CDBCFile::Search_Posistion(&TabFile, i, nIndex++);
                std::string::string(&strPlantPos, pField->pString, &vAlloc);
                
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
                std::string::~string(&strPlantPos);
                std::allocator<char>::~allocator(&vAlloc);
            }
            
            // 解析NPC位置
            {
                std::string strNpcPos;
                char vAlloc;
                std::allocator<char>::allocator(&vAlloc);
                const CDBCFile::FIELD *pField = CDBCFile::Search_Posistion(&TabFile, i, nIndex++);
                std::string::string(&strNpcPos, pField->pString, &vAlloc);
                
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
                std::string::~string(&strNpcPos);
                std::allocator<char>::~allocator(&vAlloc);
            }
            
            
            // 解析每日/活动奖励物品
            {
                std::string strDayGift;
                char vAlloc;
                std::allocator<char>::allocator(&vAlloc);
                const CDBCFile::FIELD *pField = CDBCFile::Search_Posistion(&TabFile, i, nIndex++);
                std::string::string(&strDayGift, pField->pString, &vAlloc);
                
                CItemHelper::parseItemString(stu.dayGift, &strDayGift);
                
                std::string::~string(&strDayGift);
                std::allocator<char>::~allocator(&vAlloc);
            }
            
            {
                std::string strActGift;
                char vAlloc;
                std::allocator<char>::allocator(&vAlloc);
                const CDBCFile::FIELD *pField = CDBCFile::Search_Posistion(&TabFile, i, nIndex++);
                std::string::string(&strActGift, pField->pString, &vAlloc);
                
                CItemHelper::parseItemString(stu.actGift, &strActGift);
                
                std::string::~string(&strActGift);
                std::allocator<char>::~allocator(&vAlloc);
            }
            
            // 解析兑换配置 (Times:CostGold|GetItems|CostItems#Times:CostGold|GetItems|CostItems)
            {
                std::string strChangeCfg;
                char vAlloc;
                std::allocator<char>::allocator(&vAlloc);
                const CDBCFile::FIELD *pField = CDBCFile::Search_Posistion(&TabFile, i, nIndex++);
                std::string::string(&strChangeCfg, pField->pString, &vAlloc);
                
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
                                CItemHelper::parseItemString(item, &strItem);
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
                                CItemHelper::parseItemString(item, &strItem);
                                cfgItem.vCostItems.push_back(item);
                            }
                        }
                        
                        stu.vChangeCfgList.push_back(cfgItem);
                    }
                }
                
                std::string::~string(&strChangeCfg);
                std::allocator<char>::~allocator(&vAlloc);
            }
            
            // 解析答题列表 (answers separated by |, groups separated by #)
            {
                std::string strAnswer;
                char vAlloc;
                std::allocator<char>::allocator(&vAlloc);
                const CDBCFile::FIELD *pField = CDBCFile::Search_Posistion(&TabFile, i, nIndex++);
                std::string::string(&strAnswer, pField->pString, &vAlloc);
                
                std::vector<std::string> vAnswerGroups;
                CDBCFile::_ConvertStringToVector(strAnswer.c_str(), &vAnswerGroups, "#", 1, 1);
                
                for (const auto& group : vAnswerGroups)
                {
                    std::vector<std::string> vAnswers;
                    CDBCFile::_ConvertStringToVector(group.c_str(), &vAnswers, "|", 1, 1);
                    stu.vAnswerList.push_back(vAnswers);
                }
                
                std::string::~string(&strAnswer);
                std::allocator<char>::~allocator(&vAlloc);
            }
            this->m_festivalActivityTable.Add(stu);
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgFestivalActivityTable *CfgData::GetFestivalActivityTable()
{
    return &this->m_festivalActivityTable;
}

void CfgData::InitYYDaTing(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/YYDaTing.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_YY_DATING_TABLE failed, please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgYYGameApp stu;
            memset(&stu, 0, sizeof(stu));
            std::vector<MemChrBag>::vector(&stu.Rewards);
            
            stu.nIndex = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
            
            std::string strItems;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 3);
            std::string::string(&strItems, v1->pString, &v7);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.Rewards = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v7);
            
            stu.nCondition = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
            
            auto v2 = std::map<int, CfgYYGameApp>::operator[](&this->m_CfgYYGameAppMap, &stu.nIndex);
            *v2 = stu;
            stu.~CfgYYGameApp();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgYYGameApp *CfgData::GetYYGameApp(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_CfgYYGameAppMap.find(nIndex);
    if (it != this->m_CfgYYGameAppMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitLaDaShiHuiYuan(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/LuDaShiHuiYuan.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_LUDA_SHI_HUI_YUAN_TABLE failed, please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            LuDaShiVip stu{};
            
            stu.nIndex = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
            
            std::string strItems;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 3);
            std::string::string(&strItems, v1->pString, &v7);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.Rewards = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v7);
            
            stu.nCondition = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
            stu.nVipType = CDBCFile::Search_Posistion(&TabFile, i, 8)->iValue;
            stu.nMaxCondition = CDBCFile::Search_Posistion(&TabFile, i, 11)->iValue;
            
            auto v2 = std::map<int, LuDaShiVip>::operator[](&this->m_LuDaShiVipMap, &stu.nIndex);
            *v2 = stu;
            stu.~LuDaShiVip();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

LuDaShiVip *CfgData::GetLaDaShiHuiYuan(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_LuDaShiVipMap.find(nIndex);
    if (it != this->m_LuDaShiVipMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitYYVip(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/YYHuiYuan.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_YY_VIP_TABLE failed, please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgYYVip stu;
            memset(&stu, 0, sizeof(stu));
            std::vector<MemChrBag>::vector(&stu.Rewards);
            
            stu.nIndex = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
            
            std::string strItems;
            char v11;
            std::allocator<char>::allocator(&v11);
            const CDBCFile::FIELD *v2 = CDBCFile::Search_Posistion(&TabFile, i, 3);
            std::string::string(&strItems, v2->pString, &v11);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.Rewards = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v11);
            
            stu.nCondition = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
            stu.nPric = CDBCFile::Search_Posistion(&TabFile, i, 5)->iValue;
            stu.nLimit = CDBCFile::Search_Posistion(&TabFile, i, 6)->iValue;
            
            auto v3 = std::map<int, CfgYYVip>::operator[](&this->m_CfgYYVipMap, &stu.nIndex);
            *v3 = stu;
            stu.~CfgYYVip();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgYYVip *CfgData::GetYYVip(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_CfgYYVipMap.find(nIndex);
    if (it != this->m_CfgYYVipMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgData::InitSouGouDaTing(CfgData *const this)
{
    CDBCFile TabFile;
    CDBCFile::CDBCFile(&TabFile, 0);
    
    if (!CDBCFile::OpenFromTXT(&TabFile, "./ServerConfig/Tables/SouGouDaTing.txt"))
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "open FILE_SOU_GOU_DATING_TABLE failed, please check!!!\n");
        CDBCFile::~CDBCFile(&TabFile);
        return;
    }
    
    int32_t iBaseTableCount = CDBCFile::GetRecordsNum(&TabFile);
    int32_t iBaseColumnCount = CDBCFile::GetFieldsNum(&TabFile);
    
    if (iBaseColumnCount > 0)
    {
        for (int32_t i = 0; i < iBaseTableCount; ++i)
        {
            CfgSgGameApp stu;
            memset(&stu, 0, sizeof(stu));
            std::vector<MemChrBag>::vector(&stu.vRewards);
            
            stu.nIndex = CDBCFile::Search_Posistion(&TabFile, i, 0)->iValue;
            stu.nType = CDBCFile::Search_Posistion(&TabFile, i, 1)->iValue;
            
            std::string strItems;
            char v7;
            std::allocator<char>::allocator(&v7);
            const CDBCFile::FIELD *v1 = CDBCFile::Search_Posistion(&TabFile, i, 3);
            std::string::string(&strItems, v1->pString, &v7);
            
            MemChrBagVector __x;
            CItemHelper::parseItemVectorString(&__x, &strItems);
            stu.vRewards = __x;
            __x.~vector();
            strItems.~string();
            std::allocator<char>::~allocator(&v7);
            
            stu.nCondition = CDBCFile::Search_Posistion(&TabFile, i, 4)->iValue;
            
            auto v2 = std::map<int, CfgSgGameApp>::operator[](&this->m_CfgSgGameAppMap, &stu.nIndex);
            *v2 = stu;
            stu.~CfgSgGameApp();
        }
    }
    CDBCFile::~CDBCFile(&TabFile);
}

CfgSgGameApp *CfgData::GetSoGouGameApp(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_CfgSgGameAppMap.find(nIndex);
    if (it != this->m_CfgSgGameAppMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

// ==================== 解析辅助方法（续） ====================

void CfgData::parseAddAttribues(CfgData *const this, const std::string *const addonAttr, 
                                 int32_t nIndex, const std::string *const path)
{
    // 实现解析添加属性列表
    // 此方法在多个地方被调用，用于解析 AddAttribute 列表
}

const CfgMiniClient *CfgData::GetMiniClient(const CfgData *const this, const std::string *const platform)
{
    auto it = this->m_CfgMiniClient.find(*platform);
    if (it != this->m_CfgMiniClient.end())
    {
        return &it->second;
    }
    return nullptr;
}
// ==================== 解析辅助方法 ====================

void CfgData::parseAddAttribues(CfgData *const this, std::list<AddAttribute> *result, 
                                 const std::string *const addonAttr, int32_t nIndex, const std::string *const path)
{
    if (addonAttr->empty() || *addonAttr == "-1" || *addonAttr == "0") return;
    
    std::string delims;
    char v22;
    std::allocator<char>::allocator(&v22);
    std::string::string(&delims, "|", &v22);
    
    StringVector strAttrAddons;
    Answer::StringUtility::split(&strAttrAddons, addonAttr, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v22);
    
    for (auto& addonStr : strAttrAddons)
    {
        std::string v24;
        char v25;
        std::allocator<char>::allocator(&v25);
        std::string::string(&v24, ":", &v25);
        
        StringVector strAttrAddon;
        Answer::StringUtility::split(&strAttrAddon, addonStr, &v24, 0);
        v24.~string();
        std::allocator<char>::~allocator(&v25);
        
        if (strAttrAddon.size() == 2)
        {
            AddAttribute attr;
            attr.m_nAddAttrType = atoi(strAttrAddon[0]->c_str());
            attr.m_nAddAttrValue = atoi(strAttrAddon[1]->c_str());
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
        strAttrAddon.~vector();
    }
    strAttrAddons.~vector();
}

void CfgData::paraseTalentAddon(CfgData *const this, std::list<TalentAddon> *result,
                                 const std::string *const str, int32_t nIndex, const std::string *const path)
{
    if (str->empty() || *str == "-1") return;
    
    std::string delims;
    char v10;
    std::allocator<char>::allocator(&v10);
    std::string::string(&delims, "|", &v10);
    
    StringVector strTalentAddons;
    Answer::StringUtility::split(&strTalentAddons, str, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v10);
    
    for (auto& talentStr : strTalentAddons)
    {
        std::string v12;
        char v13;
        std::allocator<char>::allocator(&v13);
        std::string::string(&v12, ":", &v13);
        
        StringVector vParam;
        Answer::StringUtility::split(&vParam, talentStr, &v12, 0);
        v12.~string();
        std::allocator<char>::~allocator(&v13);
        
        if (vParam.size() == 2)
        {
            TalentAddon addon;
            addon.nSkillId = atoi(vParam[0]->c_str());
            addon.nSkillLevel = atoi(vParam[1]->c_str());
            result->push_back(addon);
        }
        vParam.~vector();
    }
    strTalentAddons.~vector();
}

void CfgData::paraseParam2List(CfgData *const this, std::list<Param2> *result,
                                const std::string *const str, int32_t nIndex, const std::string *const path)
{
    if (str->empty() || *str == "-1") return;
    
    std::string delims;
    char v14;
    std::allocator<char>::allocator(&v14);
    std::string::string(&delims, "|", &v14);
    
    StringVector strParams;
    Answer::StringUtility::split(&strParams, str, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v14);
    
    for (auto& paramStr : strParams)
    {
        std::string v16;
        char v17;
        std::allocator<char>::allocator(&v17);
        std::string::string(&v16, ":", &v17);
        
        StringVector vParam;
        Answer::StringUtility::split(&vParam, paramStr, &v16, 0);
        v16.~string();
        std::allocator<char>::~allocator(&v17);
        
        if (vParam.size() == 2)
        {
            Param2 param;
            param.nParam1 = atoi(vParam[0]->c_str());
            param.nParam2 = atoi(vParam[1]->c_str());
            result->push_back(param);
        }
        vParam.~vector();
    }
    strParams.~vector();
}

void CfgData::paraseInt32List(CfgData *const this, std::list<int> *result,
                               const std::string *const str, int32_t size, const std::string *const path)
{
    if (str->empty() || *str == "-1") return;
    
    std::string delims;
    char v19;
    std::allocator<char>::allocator(&v19);
    std::string::string(&delims, ":", &v19);
    
    StringVector vstr;
    Answer::StringUtility::split(&vstr, str, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v19);
    
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
            int val = atoi(valStr->c_str());
            result->push_back(val);
        }
    }
    vstr.~vector();
}

Int32Vector *CfgData::paraseInt32Vector2(Int32Vector *__return_ptr retstr, CfgData *const this,
                                          const std::string *const str, const std::string *const path, int32_t size)
{
    std::vector<int>::vector(retstr);
    if (str->empty() || *str == "-1") return retstr;
    
    std::string delims;
    char v19;
    std::allocator<char>::allocator(&v19);
    std::string::string(&delims, "|", &v19);
    
    StringVector vstr;
    Answer::StringUtility::split(&vstr, str, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v19);
    
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
            int val = atoi(valStr->c_str());
            retstr->push_back(val);
        }
    }
    vstr.~vector();
    return retstr;
}

Int32VtVector *CfgData::paraseInt32VtVector(Int32VtVector *__return_ptr retstr, CfgData *const this,
                                             const std::string *const str, const std::string *const path)
{
    std::vector<std::vector<int>>::vector(retstr);
    if (str->empty() || *str == "-1") return retstr;
    
    std::string delims;
    char v18;
    std::allocator<char>::allocator(&v18);
    std::string::string(&delims, "|", &v18);
    
    StringVector SplitStr;
    Answer::StringUtility::split(&SplitStr, str, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v18);
    
    retstr->reserve(SplitStr.size());
    
    for (auto& splitItem : SplitStr)
    {
        std::string v21;
        char v22;
        std::allocator<char>::allocator(&v22);
        std::string::string(&v21, ":", &v22);
        
        StringVector vstr;
        Answer::StringUtility::split(&vstr, splitItem, &v21, 0);
        v21.~string();
        std::allocator<char>::~allocator(&v22);
        
        std::vector<int> probability;
        probability.reserve(vstr.size());
        
        for (auto& valStr : vstr)
        {
            int val = atoi(valStr->c_str());
            probability.push_back(val);
        }
        retstr->push_back(probability);
        vstr.~vector();
    }
    SplitStr.~vector();
    return retstr;
}

void CfgData::parseTaskItemJobString(MemChrJobBagVector *__return_ptr retstr, CfgData *const this,
                                      int32_t id, const std::string *const strItems)
{
    std::vector<MemChrJobBag>::vector(retstr);
    if (strItems->empty() || strItems->size() <= 4) return;
    
    std::string delims;
    char v47;
    std::allocator<char>::allocator(&v47);
    std::string::string(&delims, "|", &v47);
    
    StringVector items_receive;
    Answer::StringUtility::split(&items_receive, strItems, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v47);
    
    for (auto& itemStr : items_receive)
    {
        std::string v49;
        char v50;
        std::allocator<char>::allocator(&v50);
        std::string::string(&v49, ":", &v50);
        
        StringVector item;
        Answer::StringUtility::split(&item, itemStr, &v49, 0);
        v49.~string();
        std::allocator<char>::~allocator(&v50);
        
        if (item.size() == 4)
        {
            MemChrJobBag itemData;
            memset(&itemData, 0, sizeof(itemData));
            itemData.id = atoi(item[0]->c_str());
            itemData.type = atoi(item[1]->c_str());
            itemData.count = atoi(item[2]->c_str());
            itemData.job = atoi(item[3]->c_str());
            retstr->push_back(itemData);
        }
        else if (item.size() == 5)
        {
            MemChrJobBag itemData;
            memset(&itemData, 0, sizeof(itemData));
            itemData.id = atoi(item[0]->c_str());
            itemData.type = atoi(item[1]->c_str());
            itemData.count = atoi(item[2]->c_str());
            itemData.bind = atoi(item[3]->c_str());
            itemData.job = atoi(item[4]->c_str());
            retstr->push_back(itemData);
        }
        else if (item.size() == 6)
        {
            MemChrJobBag itemData;
            memset(&itemData, 0, sizeof(itemData));
            itemData.id = atoi(item[0]->c_str());
            itemData.type = atoi(item[1]->c_str());
            itemData.count = atoi(item[2]->c_str());
            itemData.bind = atoi(item[3]->c_str());
            itemData.job = atoi(item[4]->c_str());
            itemData.time = atoi(item[5]->c_str());
            retstr->push_back(itemData);
        }
        else
        {
            Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
                "CfgData::parseTaskItemJobString wrong data with id = %d, string = %s\n",
                id, strItems->c_str());
        }
        item.~vector();
    }
    items_receive.~vector();
}

MemJobItemTable *CfgData::parseGambleEquip(MemJobItemTable *__return_ptr retstr, CfgData *const this,
                                            int32_t id, const std::string *const strItems)
{
    std::map<int, MemJobItem>::map(retstr);
    if (strItems->empty()) return retstr;
    
    std::string delims;
    char v19;
    std::allocator<char>::allocator(&v19);
    std::string::string(&delims, "|", &v19);
    
    StringVector items_receive;
    Answer::StringUtility::split(&items_receive, strItems, &delims, 0);
    delims.~string();
    std::allocator<char>::~allocator(&v19);
    
    for (auto& itemStr : items_receive)
    {
        std::string v21;
        char v22;
        std::allocator<char>::allocator(&v22);
        std::string::string(&v21, ":", &v22);
        
        StringVector item;
        Answer::StringUtility::split(&item, itemStr, &v21, 0);
        v21.~string();
        std::allocator<char>::~allocator(&v22);
        
        if (item.size() == 2)
        {
            MemJobItem itemData;
            itemData.job = atoi(item[0]->c_str());
            itemData.item = atoi(item[1]->c_str());
            auto v9 = std::map<int, MemJobItem>::operator[](retstr, &itemData.job);
            *v9 = itemData;
        }
        else
        {
            Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
                "CfgData::parseGambleEquip wrong data with id = %d, string = %s\n",
                id, strItems->c_str());
        }
        item.~vector();
    }
    items_receive.~vector();
    return retstr;
}

// ==================== 剩余的 Getter 方法 ====================

int32_t CfgData::GetPetExp(CfgData *const this, int32_t level)
{
    auto it = this->m_levelExps.find(level);
    if (it != this->m_levelExps.end())
    {
        return it->second.pet_exp;
    }
    return INT32_MAX;
}

int32_t CfgData::GetTalentPoint(CfgData *const this, int32_t level)
{
    auto it = this->m_levelExps.find(level);
    if (it != this->m_levelExps.end())
    {
        return it->second.talent_point;
    }
    return 0;
}

int32_t CfgData::GetAttrPoint(CfgData *const this, int32_t level)
{
    auto it = this->m_levelExps.find(level);
    if (it != this->m_levelExps.end())
    {
        return it->second.attr_point;
    }
    return 0;
}

CfgLevelAttr *CfgData::getLevelAttr(CfgLevelAttr *__return_ptr retstr, CfgData *const this, int32_t job, int32_t level)
{
    int key = (job << 16) | level;
    auto it = this->m_levelAttrs.find(key);
    if (it != this->m_levelAttrs.end())
    {
        CfgLevelAttr::CfgLevelAttr(retstr, &it->second);
    }
    else
    {
        CfgLevelAttr levelAttr{};
        CfgLevelAttr::CfgLevelAttr(retstr, &levelAttr);
        levelAttr.~CfgLevelAttr();
    }
    return retstr;
}

int32_t CfgData::getBaseJob(CfgData *const this, int32_t job)
{
    if (job == 2) return 2;
    if (job == 3) return 3;
    return (job == 1) ? 1 : 0;
}

AttrAddonVector *CfgData::GetAddMonsterAttrs(AttrAddonVector *__return_ptr retstr, CfgData *const this, int32_t Mid, int32_t WorldLevel)
{
    auto it = this->m_MonstAddAttrMap.find(Mid);
    if (it != this->m_MonstAddAttrMap.end())
    {
        for (auto& attrs : it->second)
        {
            if (attrs.WorldBossLevelMin <= WorldLevel && attrs.WorldBossLevelMax >= WorldLevel)
            {
                std::vector<AttrAddon>::vector(retstr, &attrs.AttrVector);
                return retstr;
            }
        }
    }
    std::vector<AttrAddon>::vector(retstr);
    return retstr;
}

DaZheQuan *CfgData::GetDaZheQuanCfg(CfgData *const this, int32_t Index)
{
    auto it = this->m_DaZheQuanMap.find(Index);
    if (it != this->m_DaZheQuanMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

Wan360Reward *CfgData::Get360RewardCfg(CfgData *const this, int32_t Index)
{
    auto it = this->m_Wan360RewardMap.find(Index);
    if (it != this->m_Wan360RewardMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CfgData::Get360RewardIcon(CfgData *const this, const std::string *const platform)
{
    auto it = this->m_Wan360RewardTypeMap.find(*platform);
    if (it != this->m_Wan360RewardTypeMap.end())
    {
        return it->second;
    }
    return 0;
}

CfgTotalChongZhi *CfgData::GetTotalChongZhiCfg(CfgData *const this, int8_t Index)
{
    auto it = this->m_TotalChongZhiTable.find(Index);
    if (it != this->m_TotalChongZhiTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgEverydayChongZhi *CfgData::GetEveryDayChongZhiCfg(CfgData *const this, int8_t nType, int8_t Index)
{
    for (auto& item : this->m_EveryDayChongZhiTable)
    {
        if (item.Type == nType && item.Index == Index)
        {
            return &item;
        }
    }
    return nullptr;
}

NewServerFavorable *CfgData::GetNewServerFavorable(CfgData *const this, int8_t Index)
{
    auto it = this->m_NewServerFavorable.find(Index);
    if (it != this->m_NewServerFavorable.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgEquipExchange *CfgData::GetEquipExchange(CfgData *const this, int8_t Level)
{
    auto it = this->m_CfgEquipExchangeMap.find(Level);
    if (it != this->m_CfgEquipExchangeMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgPlantEventEffect *CfgData::GetPlantEvent(CfgData *const this, int32_t EventId)
{
    auto it = this->m_PlantEventMap.find(EventId);
    if (it != this->m_PlantEventMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

FunctionOpenCfg *CfgData::GetOpenFunctionCfg(CfgData *const this, int32_t FunctionId)
{
    auto it = this->m_FunctionOpenCfg.find(FunctionId);
    if (it != this->m_FunctionOpenCfg.end())
    {
        return &it->second;
    }
    return nullptr;
}

VipCardCfg *CfgData::GetVipCardCfg(CfgData *const this, int8_t VipType)
{
    auto it = this->m_VipCardTable.find(VipType);
    if (it != this->m_VipCardTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgOffLineExp *CfgData::GetOfflineExpCfg(CfgData *const this, int32_t Level)
{
    auto it = this->m_OffLineExpTable.lower_bound(Level);
    if (it != this->m_OffLineExpTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgWeekOnlineReward *CfgData::GetWeekOnlineReward(CfgData *const this, int32_t Week)
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

CfgLevelGift *CfgData::GetLevelGiftCfg(CfgData *const this, int8_t Index)
{
    auto it = this->m_LevelGift.find(Index);
    if (it != this->m_LevelGift.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgSevenLoginRewrad *CfgData::GetSevenLoginRewardCfg(CfgData *const this, int8_t Day)
{
    auto it = this->m_SevenLoginReward.find(Day);
    if (it != this->m_SevenLoginReward.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgOnlineReward *CfgData::GetOnlineRewardCfg(CfgData *const this, int8_t id)
{
    auto it = this->m_OnLineReward.find(id);
    if (it != this->m_OnLineReward.end())
    {
        return &it->second;
    }
    return nullptr;
}

MemChrBagVector *CfgData::GetSignReward(MemChrBagVector *__return_ptr retstr, CfgData *const this, int8_t SiginCount)
{
    std::vector<MemChrBag>::vector(retstr);
    
    auto it = this->m_signReward.find(SiginCount);
    if (it != this->m_signReward.end())
    {
        *retstr = it->second.ItemVector;
    }
    return retstr;
}

QuestionsVector *CfgData::GetAllQuestions(QuestionsVector *__return_ptr retstr, CfgData *const this, int8_t nType)
{
    auto it = this->m_mQuestions.find(nType);
    if (it != this->m_mQuestions.end())
    {
        std::vector<CfgQuestions>::vector(retstr, &it->second);
    }
    else
    {
        std::vector<CfgQuestions>::vector(retstr);
    }
    return retstr;
}

BossInfo *CfgData::GetBossInfo(CfgData *const this, int32_t BossId)
{
    auto it = this->m_BossInfo.find(BossId);
    if (it != this->m_BossInfo.end())
    {
        return &it->second;
    }
    return nullptr;
}

PkDropRate *CfgData::GetPkDropRate(CfgData *const this, int32_t PkValues)
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

GroupMonsterVector *CfgData::GetGroupMonsterVector(GroupMonsterVector *__return_ptr retstr, CfgData *const this, int32_t MapId)
{
    auto it = this->m_GroupMonsterMap.find(MapId);
    if (it != this->m_GroupMonsterMap.end())
    {
        std::vector<GroupMonster>::vector(retstr, &it->second);
    }
    else
    {
        std::vector<GroupMonster>::vector(retstr);
    }
    return retstr;
}

CfgSysMail *CfgData::GetSysMail(CfgData *const this, int32_t Id)
{
    auto it = this->m_sysMail.find(Id);
    if (it != this->m_sysMail.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgGameShop *CfgData::GetGameShop(CfgData *const this, int32_t nShopId)
{
    auto it = this->m_GameShopMap.find(nShopId);
    if (it != this->m_GameShopMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgGameShop *CfgData::GetGameShopItem(CfgData *const this, int8_t Class, int32_t Id)
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

BuyFaBaoResCfg *CfgData::GetBuyFaBaoResCfg(CfgData *const this, int32_t Id)
{
    auto it = this->m_FaBaoResMap.find(Id);
    if (it != this->m_FaBaoResMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

XingMaiCfg *CfgData::GetXingMaiCfg(CfgData *const this, int32_t XingMaiLevel)
{
    auto it = this->m_XingMaiCfgTable.find(XingMaiLevel);
    if (it != this->m_XingMaiCfgTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgXingMaiSlot *CfgData::GetXingMaiSlot(const CfgData *const this, int8_t nIndex)
{
    auto it = this->m_cfgXingMaiSlotTable.find(nIndex);
    if (it != this->m_cfgXingMaiSlotTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

TianLingCfg *CfgData::GetTianLingCfg(CfgData *const this, int32_t TianLingLevel)
{
    auto it = this->m_TianLingCfgTable.find(TianLingLevel);
    if (it != this->m_TianLingCfgTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

GongMingCfg *CfgData::GetGongMingCfg(CfgData *const this, int32_t GongMinLevel)
{
    auto it = this->m_GongMingCfgMap.find(GongMinLevel);
    if (it != this->m_GongMingCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

DamnationCfg *CfgData::GetDamnationCfg(CfgData *const this, int32_t Level)
{
    auto it = this->m_DamnationCfgTable.find(Level);
    if (it != this->m_DamnationCfgTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CfgData::GetAttrBattle(CfgData *const this, int32_t nAttr)
{
    if (nAttr <= 0 || nAttr > 49) return 0;
    return this->m_cfgAttrBattle[nAttr];
}

ScoreShopCfg *CfgData::GetScoreShopCfg(CfgData *const this, int32_t Index)
{
    auto it = this->m_ScoreShopCfgTable.find(Index);
    if (it != this->m_ScoreShopCfgTable.end())
    {
        return &it->second;
    }
    return nullptr;
}

LevelRefinCfg *CfgData::GetRefining(CfgData *const this, int32_t Level)
{
    auto it = this->m_LevelRefinCfgMap.find(Level);
    if (it != this->m_LevelRefinCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

UltimateChallengeCfg *CfgData::GetUltimateChallengeCfgMap(CfgData *const this, int32_t MapId)
{
    auto it = this->m_UltimateChallengeCfgMap.find(MapId);
    if (it != this->m_UltimateChallengeCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgCachet *CfgData::GetCfgCachet(CfgData *const this, int32_t level)
{
    auto it = this->m_CfgCachetMap.find(level);
    if (it != this->m_CfgCachetMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CfgData::GetCachetLevel(CfgData *const this, int64_t Exp)
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

BossKilledReward *CfgData::GetBossKilledReward(CfgData *const this, int32_t BossId)
{
    auto it = this->m_BossKilledRewardMap.find(BossId);
    if (it != this->m_BossKilledRewardMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

SystemOpenGift *CfgData::GetSystemOpenGift(CfgData *const this, int32_t Id)
{
    auto it = this->m_SystemOpenGiftMap.find(Id);
    if (it != this->m_SystemOpenGiftMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

bool CfgData::IsSpecialMap(CfgData *const this, int32_t MapId)
{
    auto it = this->m_SpecialMapMap.find(MapId);
    return it != this->m_SpecialMapMap.end();
}

TestServerReward *CfgData::GetTestServerReward(CfgData *const this, int32_t nId)
{
    auto it = this->m_TestServerRewardMap.find(nId);
    if (it != this->m_TestServerRewardMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

GuWuCfg *CfgData::GetGuWuCfg(CfgData *const this, int32_t Level)
{
    auto it = this->m_GuWuCfgMap.find(Level);
    if (it != this->m_GuWuCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

FunctionOpenMail *CfgData::GetOpenFunctionMailnCfg(CfgData *const this, int32_t nId)
{
    auto it = this->m_FunctionOpenMailMap.find(nId);
    if (it != this->m_FunctionOpenMailMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CfgData::RandXiangYaoTaskId(CfgData *const this, int32_t Level, bool bBest)
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

int32_t CfgData::getXiangYaoStart(CfgData *const this, int32_t TaskId)
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

ShenWeiTaskCfg *CfgData::GetShenWeiTaskCfg(CfgData *const this, int32_t nId)
{
    auto it = this->m_ShenWeiTaskCfgMap.find(nId);
    if (it != this->m_ShenWeiTaskCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CfgData::GetMonsterReviveTime(CfgData *const this, int32_t Time, int32_t BossId)
{
    if (BossId != 1) return Time;
    if (Time <= 0) return Time;
    
    int32_t diffDay = CfgData::getServerDiffDay(this, SERVER_TYPE::SVT_NORMAL);
    
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

const CfgMobilePhoneGift *CfgData::GetMobilePhoneGift(const CfgData *const this, const std::string *const platform)
{
    auto it = this->m_CfgMobilePhoneGift.find(*platform);
    if (it != this->m_CfgMobilePhoneGift.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgMiniClient *CfgData::GetMiniClient(const CfgData *const this, const std::string *const platform)
{
    auto it = this->m_CfgMiniClient.find(*platform);
    if (it != this->m_CfgMiniClient.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgWuHunShop *CfgData::GetWuHunShopItem(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_CfgWuHunShopItemMap.find(nIndex);
    if (it != this->m_CfgWuHunShopItemMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

WuHunItem *CfgData::GetWuHunItem(CfgData *const this, int32_t nId)
{
    auto it = this->m_WuHunItemMap.find(nId);
    if (it != this->m_WuHunItemMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

CreateWuHun *CfgData::GetCreateWuHun(CfgData *const this, int32_t nId)
{
    auto it = this->m_CreateWuHunMap.find(nId);
    if (it != this->m_CreateWuHunMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

SpecialBossMapCfg *CfgData::GetSpecialBossMapCfg(CfgData *const this, int32_t MapId)
{
    auto it = this->m_SpecialBossMapCfgMap.find(MapId);
    if (it != this->m_SpecialBossMapCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

SuperTeHuiCfg *CfgData::GetSuperTeHuiCfg(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_SuperTeHuiCfgMap.find(nIndex);
    if (it != this->m_SuperTeHuiCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

JewelPavilionCfg *CfgData::GetJewelPavilionCfg(CfgData *const this, int32_t nDay, int32_t nIndex)
{
    auto key = std::make_pair(nDay, nIndex);
    auto it = this->m_JewelPavilionCfgMap.find(key);
    if (it != this->m_JewelPavilionCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

GoblinCfg *CfgData::GetGoblinCfg(CfgData *const this, int32_t nType, int32_t nLevel)
{
    auto key = std::make_pair(nType, nLevel);
    auto it = this->m_GoblinCfgMap.find(key);
    if (it != this->m_GoblinCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

ShouHuRefinishingCfg *CfgData::GetShouHuRefinishingCfg(CfgData *const this, int32_t nType, int32_t nLevel)
{
    auto key = std::make_pair(nType, nLevel);
    auto it = this->m_ShouHuRefinishingCfgMap.find(key);
    if (it != this->m_ShouHuRefinishingCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

WingEquipPolish *CfgData::GetWingEquipPolishCfg(CfgData *const this, int32_t nType, int32_t nLevel)
{
    auto key = std::make_pair(nType, nLevel);
    auto it = this->m_WingEquipPolishCfgMap.find(key);
    if (it != this->m_WingEquipPolishCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

WinRefiningCfg *CfgData::GetWingEquipRefiningCfg(CfgData *const this, int32_t nType, int32_t nLevel)
{
    auto key = std::make_pair(nType, nLevel);
    auto it = this->m_WinRefiningCfgMap.find(key);
    if (it != this->m_WinRefiningCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

GuiGuDaoRenCfg *CfgData::GetGuiGuDaoRenCfg(CfgData *const this, int32_t NpcId)
{
    auto it = this->m_GuiGuDaoRenCfgMap.find(NpcId);
    if (it != this->m_GuiGuDaoRenCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgYYGameApp *CfgData::GetYYGameApp(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_CfgYYGameAppMap.find(nIndex);
    if (it != this->m_CfgYYGameAppMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

LuDaShiVip *CfgData::GetLaDaShiHuiYuan(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_LuDaShiVipMap.find(nIndex);
    if (it != this->m_LuDaShiVipMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

CfgYYVip *CfgData::GetYYVip(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_CfgYYVipMap.find(nIndex);
    if (it != this->m_CfgYYVipMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgYYSuperBuff *CfgData::GetSuperBuff(const CfgData *const this, int32_t SuperLevel)
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

const CfgYYSuperBuff *CfgData::Get37wanSuperBuff(const CfgData *const this, int32_t SuperLevel)
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

CfgSgGameApp *CfgData::GetSoGouGameApp(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_CfgSgGameAppMap.find(nIndex);
    if (it != this->m_CfgSgGameAppMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

PlatformDaTing *CfgData::GetGetPlatformDaTingCfg(CfgData *const this, std::string *p_Platform, int32_t nId)
{
    auto key = std::make_pair(*p_Platform, nId);
    auto it = this->m_PlatformDaTingMap.find(key);
    if (it != this->m_PlatformDaTingMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

MiniClientCfg *CfgData::GetMiniClientCfg(CfgData *const this, std::string *p_Platform, int32_t nId)
{
    auto key = std::make_pair(*p_Platform, nId);
    auto it = this->m_GetMiniClientMap.find(key);
    if (it != this->m_GetMiniClientMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

PlatformVip *CfgData::GetGetPlatformVipCfg(CfgData *const this, std::string *p_Platform, int32_t nId)
{
    auto key = std::make_pair(*p_Platform, nId);
    auto it = this->m_PlatformVipMap.find(key);
    if (it != this->m_PlatformVipMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

PlatformRewardCfg *CfgData::GetGetPlatformRewardCfg(CfgData *const this, std::string *p_Platform, int32_t nId)
{
    auto key = std::make_pair(*p_Platform, nId);
    auto it = this->m_PlatformRewardCfgMap.find(key);
    if (it != this->m_PlatformRewardCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

DiligenceCfg *CfgData::GetDiligenceCfg(CfgData *const this, int32_t nType)
{
    auto it = this->m_DiligenceCfgMap.find(nType);
    if (it != this->m_DiligenceCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

ContributionCfg *CfgData::GettFamilyDonateCfg(CfgData *const this, int32_t nId)
{
    auto it = this->m_ContributionCfgMap.find(nId);
    if (it != this->m_ContributionCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

ShangRenCfg *CfgData::GetShangRenCfg(CfgData *const this, int32_t nId)
{
    auto it = this->m_ShangRenCfgMap.find(nId);
    if (it != this->m_ShangRenCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

SelectItemCfg *CfgData::GetSelectItemCfg(CfgData *const this, int32_t nId)
{
    auto it = this->m_SelectItemCfgMap.find(nId);
    if (it != this->m_SelectItemCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

XunLeiCfg *CfgData::GetXunLeiCfg(CfgData *const this, int32_t nIndex)
{
    auto it = this->m_XunLeiCfgMap.find(nIndex);
    if (it != this->m_XunLeiCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

CrossTowerCfg *CfgData::GetCrossTowerCfg(CfgData *const this, int32_t MapId)
{
    auto it = this->m_CrossTowerCfgMap.find(MapId);
    if (it != this->m_CrossTowerCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

MoFuSkillCfg *CfgData::GetMoFuSkillCfg(CfgData *const this, int32_t nLevel)
{
    auto it = this->m_MoFuSkillCfgMap.find(nLevel);
    if (it != this->m_MoFuSkillCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

ZhuNengCfg *CfgData::GetZhuNengCfg(CfgData *const this, int32_t nType, int32_t nLevel)
{
    auto key = std::make_pair(nType, nLevel);
    auto it = this->m_ZhuNengCfgMap.find(key);
    if (it != this->m_ZhuNengCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

MoFuDuiHuan *CfgData::GetMoFuDuiHuanlCfg(CfgData *const this, int32_t nId)
{
    auto it = this->m_MoFuDuiHuanMap.find(nId);
    if (it != this->m_MoFuDuiHuanMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

ChristmasDuiHuan *CfgData::GetChristmasDuiHuanCfg(CfgData *const this, int32_t nId)
{
    auto it = this->m_ChristmasDuiHuanMap.find(nId);
    if (it != this->m_ChristmasDuiHuanMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CfgData::GetTongTianChiReward(CfgData *const this, int32_t nId)
{
    auto it = this->m_TongTianChiRankReward.lower_bound(nId);
    if (it != this->m_TongTianChiRankReward.end())
    {
        return it->second;
    }
    return 0;
}

int32_t CfgData::GetChargeDungeonId(CfgData *const this, int32_t nId, int32_t Todaycharge)
{
    int32_t KaiFuDay = CfgData::getServerDiffDay(this, SERVER_TYPE::SVT_NORMAL) + 1;
    
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

BaoKuFuBen *CfgData::GetBaoKuFuBen(CfgData *const this, int32_t FuBenId)
{
    auto it = this->m_BaoKuFuBenMap.find(FuBenId);
    if (it != this->m_BaoKuFuBenMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

BaoKuRandomMap *CfgData::GetBaoKuRandomMap(BaoKuRandomMap *__return_ptr retstr, CfgData *const this)
{
    std::map<int, BaoKuRandom>::map(retstr, &this->m_BaoKuRandomMap);
    return retstr;
}

const TreasureHunterCfg *CfgData::GetTreasureHunterCfg(const CfgData *const this, int32_t nId)
{
    auto it = this->m_TreasureHunterCfgMap.find(nId);
    if (it != this->m_TreasureHunterCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const SunAndMoonCfg *CfgData::GetSunAndMoonCfg(const CfgData *const this, int32_t nLevel)
{
    auto it = this->m_SunAndMoonCfgMap.find(nLevel);
    if (it != this->m_SunAndMoonCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const ZhanHunCfg *CfgData::GetZhanHunCfg(const CfgData *const this, int32_t nLevel)
{
    auto it = this->m_ZhanHunCfgMap.find(nLevel);
    if (it != this->m_ZhanHunCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const VipEquipPosLevelUp *CfgData::GetVipEquipPosLevelUp(const CfgData *const this, int32_t nSlot, int32_t nLevel)
{
    auto key = std::make_pair(nSlot, nLevel);
    auto it = this->m_VipEquipPosLevelUpMap.find(key);
    if (it != this->m_VipEquipPosLevelUpMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const VipEQuipPosSuit *CfgData::GetVipEQuipPosSuit(const CfgData *const this, int32_t nLevel)
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

const CVipClubLuckyDrop *CfgData::GetCVipClubLuckyDrop(CfgData *const this, int32_t nCondition)
{
    auto it = this->m_CVipClubLuckyDropMap.find(nCondition);
    if (it != this->m_CVipClubLuckyDropMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const ZeroBuyPetCfg *CfgData::GetZeroBuyPetCfg(const CfgData *const this)
{
    if (ZeroBuyPetCfg::IsEmpty(&this->m_ZeroBuyPetCfg))
    {
        return nullptr;
    }
    return &this->m_ZeroBuyPetCfg;
}

const ShiQuCfg *CfgData::GetShiQuCfg(const CfgData *const this, int32_t nId)
{
    auto it = this->m_ShiQuCfgMap.find(nId);
    if (it != this->m_ShiQuCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const LittleHelperCfg *CfgData::GetLittleHelperCfg(const CfgData *const this, int32_t nId)
{
    auto it = this->m_LittleHelperCfgMap.find(nId);
    if (it != this->m_LittleHelperCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const ShiZhuLevelUp *CfgData::GetShiZhuLevelUp(const CfgData *const this, int32_t nId, int32_t nLevel)
{
    auto key = std::make_pair(nId, nLevel);
    auto it = this->m_ShiZhuLevelUpMap.find(key);
    if (it != this->m_ShiZhuLevelUpMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const NationalDayTaskCfg *CfgData::GetNationalDayTaskCfg(const CfgData *const this, int32_t nType, int32_t nId)
{
    auto key = std::make_pair(nType, nId);
    auto it = this->m_NationalDayTaskCfgMap.find(key);
    if (it != this->m_NationalDayTaskCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const NationalDayLevelCfg *CfgData::GetNationalDayLevelCfg(const CfgData *const this, int32_t nLevel)
{
    auto it = this->m_NationalDayLevelCfgMap.find(nLevel);
    if (it != this->m_NationalDayLevelCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const EquipRongHe *CfgData::GetEquipRongHe(const CfgData *const this, int32_t ItemId)
{
    auto it = this->m_EquipRongHeMap.find(ItemId);
    if (it != this->m_EquipRongHeMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const RongHeCfg *CfgData::GetRongHeCfg(const CfgData *const this, int32_t nId)
{
    auto it = this->m_RongHeCfgMap.find(nId);
    if (it != this->m_RongHeCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}