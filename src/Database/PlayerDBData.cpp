#include "Database/PlayerDBData.h"
#include "Answer/DayTime.h"
#include "Answer/MySqlQuery.h"
#include "Answer/MySqlDBGuard.h"
#include "Answer/NetPacket.h"

PlayerDBData::PlayerDBData()
    : connid(0)
    , cgindex(0)
    , loadTime(0)
    , saveRefCount(0)
    , m_nJueWei(0)
{
}

PlayerDBData::~PlayerDBData()
{
}

void PlayerDBData::CleanUp()
{
    connid = 0;
    cgindex = 0;
    loadTime = 0;
    saveRefCount = 0;

    // Anonymous structs - zero out inline
    bzero(bagData.bagData, sizeof(bagData.bagData));
    bagData.m_ItemLimit.clear();

    gambleDepot.OpneCount = 0;
    gambleDepot.Password.clear();
    gambleDepot.SecondPassword.clear();
    bzero(gambleDepot.gambleDepot, sizeof(gambleDepot.gambleDepot));
    bzero(gambleDepot.Currency, sizeof(gambleDepot.Currency));

    m_CharPets.nStar = 0;
    m_CharPets.nHuanHua = 0;

    m_PortalDBData.m_PortalInfoList.clear();
    m_PortalDBData.nPortalId = 0;

    skillData.talents.clear();
    skillData.m_nPower = 0;

    m_CharWingDBData.m_Level = 0;
    m_CharWingDBData.m_Luck = 0;
    m_CharWingDBData.m_HuanHua = 0;

    m_WishDBData.lstWishs.clear();

    m_WorshipData.nTimes = 0;
    m_WorshipData.strCharList.clear();

    m_CFlopDraw.m_FlopDrawRecordMap.clear();

    m_CLittleHelper.m_ActLittleHelperInfoMap.clear();
    m_CLittleHelper.m_LittleHelperId = 0;

    taskCycleData.nFinishTimes = 0;
    taskCycleData.nTaskId = 0;
    taskCycleData.nState = 0;

    m_OperateLimit.m_mOperateLimit.clear();

    bzero(m_CurrencyData.vCurrency, sizeof(m_CurrencyData.vCurrency));

    bzero(equipData.vEquip, sizeof(equipData.vEquip));

    gemData.gemInfo.clear();
    gemData.posLevel.clear();
    gemData.Strengthen.clear();
    gemData.ShenYaoEquipPos.clear();

    bzero(&chr, sizeof(chr));

    m_MysteryShopDBData.lstShop.clear();

    m_FamilyData.nFamilyId = 0;
    m_FamilyData.nPosition = 0;
    m_FamilyData.nContribution = 0;
    m_FamilyData.nDailyContribution = 0;
    m_FamilyData.strSkills.clear();
    m_FamilyData.nMedLevel = 0;
    m_FamilyData.nMedRes = 0;
    m_FamilyData.nHoe = 0;
    m_FamilyData.EnterDungeionFamilyId = 0;
    m_FamilyData.EnterCount = 0;
    m_FamilyData.EnterTime = 0;

    m_ExchangeDBData.lstExchange.clear();

    // Typed instances - call their CleanUp
    sysUser.CleanUp();
    signInfo.CleanUp();
    taskData.CleanUp();
    actionData.CleanUp();
    autoFight.CleanUp();
    systemSetting.CleanUp();
    buffData.CleanUp();
    sysUserPreventWallow.CleanUp();
    attrData.CleanUp();
    m_FriendExpReward.CleanUp();
    m_MailDBData.CleanUp();
    m_ShangChengData.CleanUp();
    m_BossKilledReward.CleanUp();
    m_FaBaoData.CleanUp();
    m_CGoblinData.CleanUp();
    m_JueWeiData.CleanUp();
    m_ScoreShopData.CleanUp();
    m_TouZiData.CleanUp();
    m_HuoYueDuData.CleanUp();
    m_MysteryShopDBData.CleanUp();
    m_ExchangeDBData.CleanUp();
    m_WishDBData.CleanUp();
    m_VplanData.CleanUp();
    m_CharWingDBData.CleanUp();
    m_MagicBoxDBData.CleanUp();
    m_vipData.CleanUp();
    m_ChouJinagData.CleanUp();
    m_MoneyRewardTaskData.CleanUp();
    m_WuHunShopDBData.CleanUp();
    m_ShiZhuangData.CleanUp();
    m_CMonthlyChouJiangData.CleanUp();
    m_CMingGeData.CleanUp();
    m_CKunData.CleanUp();
    m_CFlopDraw.CleanUp();
    m_CSevenDayData.CleanUp();
    m_PortalDBData.CleanUp();
    m_XinMoDBData.CleanUp();
    m_CLittleHelper.CleanUp();
    m_NationalDayData.CleanUp();
    m_EquipRongHeData.CleanUp();
    m_CharPets.CleanUp();

    m_nJueWei = 0;
}

void PlayerDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    sysUser.SaveToSqlString(sqls, szSQL, nCid);
    signInfo.SaveToSqlString(sqls, szSQL, nCid);
    taskData.SaveToSqlString(sqls, szSQL, nCid);
    actionData.SaveToSqlString(sqls, szSQL, nCid);
    autoFight.SaveToSqlString(sqls, szSQL, nCid);
    systemSetting.SaveToSqlString(sqls, szSQL, nCid);
    buffData.SaveToSqlString(sqls, szSQL, nCid);
    sysUserPreventWallow.SaveToSqlString(sqls, szSQL, nCid);
    attrData.SaveToSqlString(sqls, szSQL, nCid);
    m_FriendExpReward.SaveToSqlString(sqls, szSQL, nCid);
    m_MailDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_ShangChengData.SaveToSqlString(sqls, szSQL, nCid);
    m_BossKilledReward.SaveToSqlString(sqls, szSQL, nCid);
    m_FaBaoData.SaveToSqlString(sqls, szSQL, nCid);
    m_CGoblinData.SaveToSqlString(sqls, szSQL, nCid);
    m_FamilyData.SaveToSqlString(sqls, szSQL, nCid);
    m_WorshipData.SaveToSqlString(sqls, szSQL, nCid);
    m_JueWeiData.SaveToSqlString(sqls, szSQL, nCid);
    m_ScoreShopData.SaveToSqlString(sqls, szSQL, nCid);
    m_TouZiData.SaveToSqlString(sqls, szSQL, nCid);
    m_HuoYueDuData.SaveToSqlString(sqls, szSQL, nCid);
    m_MysteryShopDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_ExchangeDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_WishDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_VplanData.SaveToSqlString(sqls, szSQL, nCid);
    m_CharWingDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_MagicBoxDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_vipData.SaveToSqlString(sqls, szSQL, nCid);
    m_ChouJinagData.SaveToSqlString(sqls, szSQL, nCid);
    m_MoneyRewardTaskData.SaveToSqlString(sqls, szSQL, nCid);
    m_WuHunShopDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_ShiZhuangData.SaveToSqlString(sqls, szSQL, nCid);
    m_CMonthlyChouJiangData.SaveToSqlString(sqls, szSQL, nCid);
    m_CMingGeData.SaveToSqlString(sqls, szSQL, nCid);
    m_CKunData.SaveToSqlString(sqls, szSQL, nCid);
    m_CFlopDraw.SaveToSqlString(sqls, szSQL, nCid);
    m_CSevenDayData.SaveToSqlString(sqls, szSQL, nCid);
    m_PortalDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_XinMoDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_CLittleHelper.SaveToSqlString(sqls, szSQL, nCid);
    m_NationalDayData.SaveToSqlString(sqls, szSQL, nCid);
    m_EquipRongHeData.SaveToSqlString(sqls, szSQL, nCid);
}

bool PlayerDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    loadTime = Answer::DayTime::now();

    sysUser.LoadFromDB(db, szSQL, nUid, nSid, 0);
    sysUserPreventWallow.LoadFromDB(db, szSQL, nUid, nSid, 0);
    signInfo.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    taskData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    actionData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    autoFight.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    systemSetting.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    buffData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    attrData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_FriendExpReward.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_MailDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_ShangChengData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_BossKilledReward.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_FaBaoData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CGoblinData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_FamilyData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_WorshipData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_JueWeiData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_ScoreShopData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_TouZiData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_HuoYueDuData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_MysteryShopDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_ExchangeDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_WishDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_VplanData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CharWingDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_MagicBoxDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_vipData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_ChouJinagData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_MoneyRewardTaskData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_WuHunShopDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_ShiZhuangData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CMonthlyChouJiangData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CMingGeData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CKunData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CFlopDraw.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CSevenDayData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_PortalDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_XinMoDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CLittleHelper.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_NationalDayData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_EquipRongHeData.LoadFromDB(db, szSQL, nUid, nSid, nCid);

    return true;
}

void PlayerDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;

    Answer::NetPacket::writeInt8(packet, connid);
    Answer::NetPacket::writeInt16(packet, cgindex);
    Answer::NetPacket::writeInt32(packet, loadTime);

    attrData.PackageData(packet);
    sysUser.PackageData(packet);
    sysUserPreventWallow.PackageData(packet);
    signInfo.PackageData(packet);
    taskData.PackageData(packet);
    actionData.PackageData(packet);
    autoFight.PackageData(packet);
    systemSetting.PackageData(packet);
    buffData.PackageData(packet);
    m_FriendExpReward.PackageData(packet);
    m_MailDBData.PackageData(packet);
    m_ShangChengData.PackageData(packet);
    m_BossKilledReward.PackageData(packet);
    m_FaBaoData.PackageData(packet);
    m_CGoblinData.PackageData(packet);
    m_FamilyData.PackageData(packet);
    m_WorshipData.PackageData(packet);
    m_JueWeiData.PackageData(packet);
    m_ScoreShopData.PackageData(packet);
    m_TouZiData.PackageData(packet);
    m_HuoYueDuData.PackageData(packet);
    m_MysteryShopDBData.PackageData(packet);
    m_ExchangeDBData.PackageData(packet);
    m_WishDBData.PackageData(packet);
    m_VplanData.PackageData(packet);
    m_CharWingDBData.PackageData(packet);
    m_MagicBoxDBData.PackageData(packet);
    m_vipData.PackageData(packet);
    m_ChouJinagData.PackageData(packet);
    m_MoneyRewardTaskData.PackageData(packet);
    m_WuHunShopDBData.PackageData(packet);
    m_ShiZhuangData.PackageData(packet);
    m_CMonthlyChouJiangData.PackageData(packet);
    m_CMingGeData.PackageData(packet);
    m_CKunData.PackageData(packet);
    m_CFlopDraw.PackageData(packet);
    m_CSevenDayData.PackageData(packet);
    m_PortalDBData.PackageData(packet);
    m_XinMoDBData.PackageData(packet);
    m_CLittleHelper.PackageData(packet);
    m_NationalDayData.PackageData(packet);
    m_EquipRongHeData.PackageData(packet);
}
