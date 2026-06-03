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

    m_CharPets.CleanUp();

    m_PortalDBData.CleanUp();
    skillData.talents.clear();
    skillData.m_nPower = 0;

    m_CharWingDBData.CleanUp();
    m_WishDBData.lstWishs.clear();
    m_WorshipData.CleanUp();
    m_CFlopDraw.m_FlopDrawRecordMap.clear();
    m_CLittleHelper.CleanUp();
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
    m_FamilyData.CleanUp();
    m_ExchangeDBData.lstExchange.clear();

    // Typed instances - call CleanUp if available
    sysUser.CleanUp();
    signInfo.CleanUp();
    attrData.CleanUp();
    actionData.CleanUp();
    autoFight.CleanUp();
    systemSetting.CleanUp();
    sysUserPreventWallow.CleanUp();
    depotData.CleanUp();
    m_FaBaoData.CleanUp();
    m_CGoblinData.CleanUp();
    m_VplanData.CleanUp();
    m_vipData.CleanUp();
    m_MoneyRewardTaskData.CleanUp();
    m_ShiZhuangData.CleanUp();
    m_CMonthlyChouJiangData.CleanUp();
    m_CMingGeData.CleanUp();
    m_CKunData.CleanUp();
    m_CSevenDayData.CleanUp();
    m_NationalDayData.CleanUp();
    m_EquipRongHeData.CleanUp();
    m_BossKilledReward.CleanUp();
    m_MagicBoxDBData.CleanUp();

    m_JueWeiData.nJueWei = 0;

    m_nJueWei = 0;
}

void PlayerDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    // Database types - have SaveToSqlString
    m_FamilyData.SaveToSqlString(sqls, szSQL, nCid);
    m_WorshipData.SaveToSqlString(sqls, szSQL, nCid);
    m_MysteryShopDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_ExchangeDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_WishDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_CharWingDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_MagicBoxDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_WuHunShopDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_PortalDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_XinMoDBData.SaveToSqlString(sqls, szSQL, nCid);
    m_CharPets.SaveToSqlString(sqls, szSQL, nCid);
    m_OperateLimit.SaveToSqlString(sqls, szSQL, nCid);
    m_CurrencyData.SaveToSqlString(sqls, szSQL, nCid);
    m_MailDBData.SaveToSqlString(sqls, szSQL, nCid);
}

bool PlayerDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    loadTime = Answer::DayTime::now();

    // Database types - have LoadFromDB
    m_FamilyData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_WorshipData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_MysteryShopDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_ExchangeDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_WishDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CharWingDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_MagicBoxDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_WuHunShopDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_PortalDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_XinMoDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CharPets.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_OperateLimit.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_CurrencyData.LoadFromDB(db, szSQL, nUid, nSid, nCid);
    m_MailDBData.LoadFromDB(db, szSQL, nUid, nSid, nCid);

    return true;
}

void PlayerDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;

    Answer::NetPacket::writeInt8(packet, connid);
    Answer::NetPacket::writeInt16(packet, cgindex);
    Answer::NetPacket::writeInt32(packet, loadTime);

    // All types have PackageData
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
    depotData.PackageData(packet);
}
