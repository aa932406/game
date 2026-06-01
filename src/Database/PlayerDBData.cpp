#include "Database/PlayerDBData.h"

PlayerDBData::PlayerDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4970; */
    MemCharacterData::MemCharacterData(&this->chr);
    SysUserData::SysUserData(&this->sysUser);
    DailyActivityData::DailyActivityData(&this->signInfo);
    MemChrEquipData::MemChrEquipData(&this->equipData);
    MemChrGemData::MemChrGemData(&this->gemData);
    MemChrBagData::MemChrBagData(&this->bagData);
    MemChrSkillData::MemChrSkillData(&this->skillData);
    MemChrTaskData::MemChrTaskData(&this->taskData);
    MemChrTaskCycleData::MemChrTaskCycleData(&this->taskCycleData);
    MemChrActionData::MemChrActionData(&this->actionData);
    MemChrAutoFightData::MemChrAutoFightData(&this->autoFight);
    MemChrSystemSettingData::MemChrSystemSettingData(&this->systemSetting);
    MemChrBuffData::MemChrBuffData(&this->buffData);
    MemChrDepotData::MemChrDepotData(&this->gambleDepot);
    SysUserPreventWallowData::SysUserPreventWallowData(&this->sysUserPreventWallow);
    MemAttrData::MemAttrData(&this->attrData);
    CurrencyDBData::CurrencyDBData(&this->m_CurrencyData);
    OperateLimitDBData::OperateLimitDBData(&this->m_OperateLimit);
    PetDBData::PetDBData(&this->m_CharPets);
    FriendExpReward::FriendExpReward(&this->m_FriendExpReward);
    MailDBData::MailDBData(&this->m_MailDBData);
    ShangChengData::ShangChengData(&this->m_ShangChengData);
    BossKilledRewardData::BossKilledRewardData(&this->m_BossKilledReward);
    CFaBaoData::CFaBaoData(&this->m_FaBaoData);
    CGoblinData::CGoblinData(&this->m_CGoblinData);
    CharFamilyDBData::CharFamilyDBData(&this->m_FamilyData);
    WorshipDBData::WorshipDBData(&this->m_WorshipData);
    CJueWeiData::CJueWeiData(&this->m_JueWeiData);
    ScoreShopData::ScoreShopData(&this->m_ScoreShopData);
    TouZiData::TouZiData(&this->m_TouZiData);
    CHuoYueDuData::CHuoYueDuData(&this->m_HuoYueDuData);
    MysteryShopDBData::MysteryShopDBData(&this->m_MysteryShopDBData);
    ExchangeDBData::ExchangeDBData(&this->m_ExchangeDBData);
    CharWishDBData::CharWishDBData(&this->m_WishDBData);
    CVplanData::CVplanData(&this->m_VplanData);
    CharWingDBData::CharWingDBData(&this->m_CharWingDBData);
    MagicBoxDBData::MagicBoxDBData(&this->m_MagicBoxDBData);
    VipData::VipData(&this->m_vipData);
    ChouJiangData::ChouJiangData(&this->m_ChouJinagData);
    MoneyRewardTaskData::MoneyRewardTaskData(&this->m_MoneyRewardTaskData);
    WuHunShopDBData::WuHunShopDBData(&this->m_WuHunShopDBData);
    CShiZhuangData::CShiZhuangData(&this->m_ShiZhuangData);
    CMonthlyChouJiangData::CMonthlyChouJiangData(&this->m_CMonthlyChouJiangData);
    CMingGeData::CMingGeData(&this->m_CMingGeData);
    CKunData::CKunData(&this->m_CKunData);
    CFlopDraw::CFlopDraw(&this->m_CFlopDraw);
    CSevenDayData::CSevenDayData(&this->m_CSevenDayData);
    PortalDBData::PortalDBData(&this->m_PortalDBData);
    XinMoDBData::XinMoDBData(&this->m_XinMoDBData);
    CLittleHelperData::CLittleHelperData(&this->m_CLittleHelper);
    NationalDayData::NationalDayData(&this->m_NationalDayData);
    EquipRongHeData::EquipRongHeData(&this->m_EquipRongHeData);
    /* base init: PlayerDBData::CleanUp(this); */
}

PlayerDBData::~PlayerDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4970; */
    EquipRongHeData::~EquipRongHeData(&this->m_EquipRongHeData);
    NationalDayData::~NationalDayData(&this->m_NationalDayData);
    CLittleHelperData::~CLittleHelperData(&this->m_CLittleHelper);
    XinMoDBData::~XinMoDBData(&this->m_XinMoDBData);
    PortalDBData::~PortalDBData(&this->m_PortalDBData);
    CSevenDayData::~CSevenDayData(&this->m_CSevenDayData);
    CFlopDraw::~CFlopDraw(&this->m_CFlopDraw);
    CKunData::~CKunData(&this->m_CKunData);
    CMingGeData::~CMingGeData(&this->m_CMingGeData);
    CMonthlyChouJiangData::~CMonthlyChouJiangData(&this->m_CMonthlyChouJiangData);
    CShiZhuangData::~CShiZhuangData(&this->m_ShiZhuangData);
    WuHunShopDBData::~WuHunShopDBData(&this->m_WuHunShopDBData);
    MoneyRewardTaskData::~MoneyRewardTaskData(&this->m_MoneyRewardTaskData);
    ChouJiangData::~ChouJiangData(&this->m_ChouJinagData);
    VipData::~VipData(&this->m_vipData);
    MagicBoxDBData::~MagicBoxDBData(&this->m_MagicBoxDBData);
    CharWingDBData::~CharWingDBData(&this->m_CharWingDBData);
    CVplanData::~CVplanData(&this->m_VplanData);
    CharWishDBData::~CharWishDBData(&this->m_WishDBData);
    ExchangeDBData::~ExchangeDBData(&this->m_ExchangeDBData);
    MysteryShopDBData::~MysteryShopDBData(&this->m_MysteryShopDBData);
    CHuoYueDuData::~CHuoYueDuData(&this->m_HuoYueDuData);
    TouZiData::~TouZiData(&this->m_TouZiData);
    ScoreShopData::~ScoreShopData(&this->m_ScoreShopData);
    CJueWeiData::~CJueWeiData(&this->m_JueWeiData);
    WorshipDBData::~WorshipDBData(&this->m_WorshipData);
    CharFamilyDBData::~CharFamilyDBData(&this->m_FamilyData);
    CGoblinData::~CGoblinData(&this->m_CGoblinData);
    CFaBaoData::~CFaBaoData(&this->m_FaBaoData);
    BossKilledRewardData::~BossKilledRewardData(&this->m_BossKilledReward);
    ShangChengData::~ShangChengData(&this->m_ShangChengData);
    MailDBData::~MailDBData(&this->m_MailDBData);
    FriendExpReward::~FriendExpReward(&this->m_FriendExpReward);
    PetDBData::~PetDBData(&this->m_CharPets);
    OperateLimitDBData::~OperateLimitDBData(&this->m_OperateLimit);
    CurrencyDBData::~CurrencyDBData(&this->m_CurrencyData);
    MemAttrData::~MemAttrData(&this->attrData);
    SysUserPreventWallowData::~SysUserPreventWallowData(&this->sysUserPreventWallow);
    MemChrDepotData::~MemChrDepotData(&this->gambleDepot);
    MemChrBuffData::~MemChrBuffData(&this->buffData);
    MemChrSystemSettingData::~MemChrSystemSettingData(&this->systemSetting);
    MemChrAutoFightData::~MemChrAutoFightData(&this->autoFight);
    MemChrActionData::~MemChrActionData(&this->actionData);
    MemChrTaskCycleData::~MemChrTaskCycleData(&this->taskCycleData);
    MemChrTaskData::~MemChrTaskData(&this->taskData);
    MemChrSkillData::~MemChrSkillData(&this->skillData);
    MemChrBagData::~MemChrBagData(&this->bagData);
    MemChrGemData::~MemChrGemData(&this->gemData);
    MemChrEquipData::~MemChrEquipData(&this->equipData);
    DailyActivityData::~DailyActivityData(&this->signInfo);
    SysUserData::~SysUserData(&this->sysUser);
    MemCharacterData::~MemCharacterData(&this->chr);
}

void PlayerDBData::CleanUp()
{
    this->connid = 0;
    this->cgindex = 0;
    this->loadTime = 0;
    this->saveRefCount = 0;
    MemCharacterData::CleanUp(&this->chr);
    MemAttrData::CleanUp(&this->attrData);
    SysUserData::CleanUp(&this->sysUser);
    DailyActivityData::CleanUp(&this->signInfo);
    MemChrEquipData::CleanUp(&this->equipData);
    MemChrGemData::CleanUp(&this->gemData);
    MemChrBagData::CleanUp(&this->bagData);
    MemChrSkillData::CleanUp(&this->skillData);
    MemChrTaskData::CleanUp(&this->taskData);
    MemChrTaskCycleData::CleanUp(&this->taskCycleData);
    MemChrActionData::CleanUp(&this->actionData);
    MemChrAutoFightData::CleanUp(&this->autoFight);
    MemChrSystemSettingData::CleanUp(&this->systemSetting);
    MemChrBuffData::CleanUp(&this->buffData);
    MemChrDepotData::CleanUp(&this->gambleDepot);
    SysUserPreventWallowData::CleanUp(&this->sysUserPreventWallow);
    CurrencyDBData::CleanUp(&this->m_CurrencyData);
    OperateLimitDBData::CleanUp(&this->m_OperateLimit);
    PetDBData::CleanUp(&this->m_CharPets);
    FriendExpReward::CleanUp(&this->m_FriendExpReward);
    MailDBData::CleanUp(&this->m_MailDBData);
    ShangChengData::CleanUp(&this->m_ShangChengData);
    BossKilledRewardData::CleanUp(&this->m_BossKilledReward);
    CFaBaoData::CleanUp(&this->m_FaBaoData);
    CGoblinData::CleanUp(&this->m_CGoblinData);
    CharFamilyDBData::CleanUp(&this->m_FamilyData);
    WorshipDBData::CleanUp(&this->m_WorshipData);
    CJueWeiData::CleanUp(&this->m_JueWeiData);
    ScoreShopData::CleanUp(&this->m_ScoreShopData);
    TouZiData::CleanUp(&this->m_TouZiData);
    CHuoYueDuData::CleanUp(&this->m_HuoYueDuData);
    MysteryShopDBData::CleanUp(&this->m_MysteryShopDBData);
    ExchangeDBData::CleanUp(&this->m_ExchangeDBData);
    CharWishDBData::CleanUp(&this->m_WishDBData);
    CVplanData::CleanUp(&this->m_VplanData);
    CharWingDBData::CleanUp(&this->m_CharWingDBData);
    MagicBoxDBData::CleanUp(&this->m_MagicBoxDBData);
    VipData::CleanUp(&this->m_vipData);
    CShiZhuangData::CleanUp(&this->m_ShiZhuangData);
    CMonthlyChouJiangData::CleanUp(&this->m_CMonthlyChouJiangData);
    ChouJiangData::CleanUp(&this->m_ChouJinagData);
    MoneyRewardTaskData::CleanUp(&this->m_MoneyRewardTaskData);
    WuHunShopDBData::CleanUp(&this->m_WuHunShopDBData);
    CMingGeData::CleanUp(&this->m_CMingGeData);
    CKunData::CleanUp(&this->m_CKunData);
    CFlopDraw::CleanUp(&this->m_CFlopDraw);
    CSevenDayData::CleanUp(&this->m_CSevenDayData);
    PortalDBData::CleanUp(&this->m_PortalDBData);
    XinMoDBData::CleanUp(&this->m_XinMoDBData);
    CLittleHelperData::CleanUp(&this->m_CLittleHelper);
    NationalDayData::CleanUp(&this->m_NationalDayData);
    EquipRongHeData::CleanUp(&this->m_EquipRongHeData);
}

void PlayerDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    MemCharacterData::SaveToSqlString(&this->chr, sqls, szSQL, nCid);
    MemAttrData::SaveToSqlString(&this->attrData, sqls, szSQL, nCid);
    SysUserData::SaveToSqlString(&this->sysUser, sqls, szSQL, nCid);
    SysUserPreventWallowData::SaveToSqlString(&this->sysUserPreventWallow, sqls, szSQL, nCid);
    DailyActivityData::SaveToSqlString(&this->signInfo, sqls, szSQL, nCid);
    MemChrEquipData::SaveToSqlString(&this->equipData, sqls, szSQL, nCid);
    MemChrGemData::SaveToSqlString(&this->gemData, sqls, szSQL, nCid);
    MemChrBagData::SaveToSqlString(&this->bagData, sqls, szSQL, nCid);
    MemChrDepotData::SaveToSqlString(&this->gambleDepot, sqls, szSQL, nCid);
    MemChrSkillData::SaveToSqlString(&this->skillData, sqls, szSQL, nCid);
    MemChrTaskData::SaveToSqlString(&this->taskData, sqls, szSQL, nCid);
    MemChrTaskCycleData::SaveToSqlString(&this->taskCycleData, sqls, szSQL, nCid);
    MemChrActionData::SaveToSqlString(&this->actionData, sqls, szSQL, nCid);
    MemChrAutoFightData::SaveToSqlString(&this->autoFight, sqls, szSQL, nCid);
    MemChrSystemSettingData::SaveToSqlString(&this->systemSetting, sqls, szSQL, nCid);
    MemChrBuffData::SaveToSqlString(&this->buffData, sqls, szSQL, nCid);
    CurrencyDBData::SaveToSqlString(&this->m_CurrencyData, sqls, szSQL, nCid);
    OperateLimitDBData::SaveToSqlString(&this->m_OperateLimit, sqls, szSQL, nCid);
    PetDBData::SaveToSqlString(&this->m_CharPets, sqls, szSQL, nCid);
    FriendExpReward::SaveToSqlString(&this->m_FriendExpReward, sqls, szSQL, nCid);
    MailDBData::SaveToSqlString(&this->m_MailDBData, sqls, szSQL, nCid);
    ShangChengData::SaveToSqlString(&this->m_ShangChengData, sqls, szSQL, nCid);
    BossKilledRewardData::SaveToSqlString(&this->m_BossKilledReward, sqls, szSQL, nCid);
    CFaBaoData::SaveToSqlString(&this->m_FaBaoData, sqls, szSQL, nCid);
    CGoblinData::SaveToSqlString(&this->m_CGoblinData, sqls, szSQL, nCid);
    CharFamilyDBData::SaveToSqlString(&this->m_FamilyData, sqls, szSQL, nCid);
    WorshipDBData::SaveToSqlString(&this->m_WorshipData, sqls, szSQL, nCid);
    CJueWeiData::SaveToSqlString(&this->m_JueWeiData, sqls, szSQL, nCid);
    ScoreShopData::SaveToSqlString(&this->m_ScoreShopData, sqls, szSQL, nCid);
    TouZiData::SaveToSqlString(&this->m_TouZiData, sqls, szSQL, nCid);
    CHuoYueDuData::SaveToSqlString(&this->m_HuoYueDuData, sqls, szSQL, nCid);
    MysteryShopDBData::SaveToSqlString(&this->m_MysteryShopDBData, sqls, szSQL, nCid);
    ExchangeDBData::SaveToSqlString(&this->m_ExchangeDBData, sqls, szSQL, nCid);
    CharWishDBData::SaveToSqlString(&this->m_WishDBData, sqls, szSQL, nCid);
    CharWingDBData::SaveToSqlString(&this->m_CharWingDBData, sqls, szSQL, nCid);
    MagicBoxDBData::SaveToSqlString(&this->m_MagicBoxDBData, sqls, szSQL, nCid);
    VipData::SaveToSqlString(&this->m_vipData, sqls, szSQL, nCid);
    CShiZhuangData::SaveToSqlString(&this->m_ShiZhuangData, sqls, szSQL, nCid);
    CMonthlyChouJiangData::SaveToSqlString(&this->m_CMonthlyChouJiangData, sqls, szSQL, nCid);
    ChouJiangData::SaveToSqlString(&this->m_ChouJinagData, sqls, szSQL, nCid);
    MoneyRewardTaskData::SaveToSqlString(&this->m_MoneyRewardTaskData, sqls, szSQL, nCid);
    WuHunShopDBData::SaveToSqlString(&this->m_WuHunShopDBData, sqls, szSQL, nCid);
    CMingGeData::SaveToSqlString(&this->m_CMingGeData, sqls, szSQL, nCid);
    CKunData::SaveToSqlString(&this->m_CKunData, sqls, szSQL, nCid);
    CFlopDraw::SaveToSqlString(&this->m_CFlopDraw, sqls, szSQL, nCid);
    CSevenDayData::SaveToSqlString(&this->m_CSevenDayData, sqls, szSQL, nCid);
    PortalDBData::SaveToSqlString(&this->m_PortalDBData, sqls, szSQL, nCid);
    XinMoDBData::SaveToSqlString(&this->m_XinMoDBData, sqls, szSQL, nCid);
    CLittleHelperData::SaveToSqlString(&this->m_CLittleHelper, sqls, szSQL, nCid);
    NationalDayData::SaveToSqlString(&this->m_NationalDayData, sqls, szSQL, nCid);
    EquipRongHeData::SaveToSqlString(&this->m_EquipRongHeData, sqls, szSQL, nCid);
}

bool PlayerDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    CharId_t nCida; // [rsp+0h] [rbp-40h]

    this->loadTime = Answer::DayTime::now();
    if ( !MemCharacterData::LoadFromDB(&this->chr, db, szSQL, nUid, nSid, 0) )
    return 0;
    nCida = this->chr.data.cid;
    if ( SysUserData::LoadFromDB(&this->sysUser, db, szSQL, nUid, nSid, 0) )
    SysUserPreventWallowData::LoadFromDB(&this->sysUserPreventWallow, db, szSQL, nUid, nSid, 0);
    MemChrDepotData::LoadFromDB(&this->gambleDepot, db, szSQL, nUid, nSid, nCida);
    DailyActivityData::LoadFromDB(&this->signInfo, db, szSQL, nUid, nSid, nCida);
    MemChrEquipData::LoadFromDB(&this->equipData, db, szSQL, nUid, nSid, nCida);
    MemChrGemData::LoadFromDB(&this->gemData, db, szSQL, nUid, nSid, nCida);
    MemChrBagData::LoadFromDB(&this->bagData, db, szSQL, nUid, nSid, nCida);
    MemChrSkillData::LoadFromDB(&this->skillData, db, szSQL, nUid, nSid, nCida);
    MemChrTaskData::LoadFromDB(&this->taskData, db, szSQL, nUid, nSid, nCida);
    MemChrTaskCycleData::LoadFromDB(&this->taskCycleData, db, szSQL, nUid, nSid, nCida);
    MemChrActionData::LoadFromDB(&this->actionData, db, szSQL, nUid, nSid, nCida);
    MemChrAutoFightData::LoadFromDB(&this->autoFight, db, szSQL, nUid, nSid, nCida);
    MemChrSystemSettingData::LoadFromDB(&this->systemSetting, db, szSQL, nUid, nSid, nCida);
    MemChrBuffData::LoadFromDB(&this->buffData, db, szSQL, nUid, nSid, nCida);
    CurrencyDBData::LoadFromDB(&this->m_CurrencyData, db, szSQL, nUid, nSid, nCida);
    OperateLimitDBData::LoadFromDB(&this->m_OperateLimit, db, szSQL, nUid, nSid, nCida);
    PetDBData::LoadFromDB(&this->m_CharPets, db, szSQL, nUid, nSid, nCida);
    FriendExpReward::LoadFromDB(&this->m_FriendExpReward, db, szSQL, nUid, nSid, nCida);
    MailDBData::LoadFromDB(&this->m_MailDBData, db, szSQL, nUid, nSid, nCida);
    ShangChengData::LoadFromDB(&this->m_ShangChengData, db, szSQL, nUid, nSid, nCida);
    BossKilledRewardData::LoadFromDB(&this->m_BossKilledReward, db, szSQL, nUid, nSid, nCida);
    CFaBaoData::LoadFromDB(&this->m_FaBaoData, db, szSQL, nUid, nSid, nCida);
    CGoblinData::LoadFromDB(&this->m_CGoblinData, db, szSQL, nUid, nSid, nCida);
    CharFamilyDBData::LoadFromDB(&this->m_FamilyData, db, szSQL, nUid, nSid, nCida);
    WorshipDBData::LoadFromDB(&this->m_WorshipData, db, szSQL, nUid, nSid, nCida);
    CJueWeiData::LoadFromDB(&this->m_JueWeiData, db, szSQL, nUid, nSid, nCida);
    ScoreShopData::LoadFromDB(&this->m_ScoreShopData, db, szSQL, nUid, nSid, nCida);
    TouZiData::LoadFromDB(&this->m_TouZiData, db, szSQL, nUid, nSid, nCida);
    CHuoYueDuData::LoadFromDB(&this->m_HuoYueDuData, db, szSQL, nUid, nSid, nCida);
    MysteryShopDBData::LoadFromDB(&this->m_MysteryShopDBData, db, szSQL, nUid, nSid, nCida);
    ExchangeDBData::LoadFromDB(&this->m_ExchangeDBData, db, szSQL, nUid, nSid, nCida);
    CharWishDBData::LoadFromDB(&this->m_WishDBData, db, szSQL, nUid, nSid, nCida);
    CVplanData::LoadFromDB(&this->m_VplanData, db, szSQL, nUid, nSid, nCida);
    CharWingDBData::LoadFromDB(&this->m_CharWingDBData, db, szSQL, nUid, nSid, nCida);
    MagicBoxDBData::LoadFromDB(&this->m_MagicBoxDBData, db, szSQL, nUid, nSid, nCida);
    VipData::LoadFromDB(&this->m_vipData, db, szSQL, nUid, nSid, nCida);
    CShiZhuangData::LoadFromDB(&this->m_ShiZhuangData, db, szSQL, nUid, nSid, nCida);
    CMonthlyChouJiangData::LoadFromDB(&this->m_CMonthlyChouJiangData, db, szSQL, nUid, nSid, nCida);
    ChouJiangData::LoadFromDB(&this->m_ChouJinagData, db, szSQL, nUid, nSid, nCida);
    MoneyRewardTaskData::LoadFromDB(&this->m_MoneyRewardTaskData, db, szSQL, nUid, nSid, nCida);
    WuHunShopDBData::LoadFromDB(&this->m_WuHunShopDBData, db, szSQL, nUid, nSid, nCida);
    CMingGeData::LoadFromDB(&this->m_CMingGeData, db, szSQL, nUid, nSid, nCida);
    CKunData::LoadFromDB(&this->m_CKunData, db, szSQL, nUid, nSid, nCida);
    CFlopDraw::LoadFromDB(&this->m_CFlopDraw, db, szSQL, nUid, nSid, nCida);
    CSevenDayData::LoadFromDB(&this->m_CSevenDayData, db, szSQL, nUid, nSid, nCida);
    PortalDBData::LoadFromDB(&this->m_PortalDBData, db, szSQL, nUid, nSid, nCida);
    XinMoDBData::LoadFromDB(&this->m_XinMoDBData, db, szSQL, nUid, nSid, nCida);
    CLittleHelperData::LoadFromDB(&this->m_CLittleHelper, db, szSQL, nUid, nSid, nCida);
    NationalDayData::LoadFromDB(&this->m_NationalDayData, db, szSQL, nUid, nSid, nCida);
    EquipRongHeData::LoadFromDB(&this->m_EquipRongHeData, db, szSQL, nUid, nSid, nCida);
    return 1;
}

void PlayerDBData::PackageData(Answer::NetPacket *packet)
{
    Answer::NetPacket::writeInt8(packet, this->connid);
    Answer::NetPacket::writeInt16(packet, this->cgindex);
    Answer::NetPacket::writeInt32(packet, this->loadTime);
    MemCharacterData::PackageData(&this->chr, packet);
    MemAttrData::PackageData(&this->attrData, packet);
    MemChrDepotData::PackageData(&this->gambleDepot, packet);
    SysUserData::PackageData(&this->sysUser, packet);
    SysUserPreventWallowData::PackageData(&this->sysUserPreventWallow, packet);
    DailyActivityData::PackageData(&this->signInfo, packet);
    MemChrEquipData::PackageData(&this->equipData, packet);
    MemChrGemData::PackageData(&this->gemData, packet);
    MemChrBagData::PackageData(&this->bagData, packet);
    MemChrSkillData::PackageData(&this->skillData, packet);
    MemChrTaskData::PackageData(&this->taskData, packet);
    MemChrTaskCycleData::PackageData(&this->taskCycleData, packet);
    MemChrActionData::PackageData(&this->actionData, packet);
    MemChrAutoFightData::PackageData(&this->autoFight, packet);
    MemChrSystemSettingData::PackageData(&this->systemSetting, packet);
    MemChrBuffData::PackageData(&this->buffData, packet);
    CurrencyDBData::PackageData(&this->m_CurrencyData, packet);
    OperateLimitDBData::PackageData(&this->m_OperateLimit, packet);
    PetDBData::PackageData(&this->m_CharPets, packet);
    FriendExpReward::PackageData(&this->m_FriendExpReward, packet);
    MailDBData::PackageData(&this->m_MailDBData, packet);
    ShangChengData::PackageData(&this->m_ShangChengData, packet);
    BossKilledRewardData::PackageData(&this->m_BossKilledReward, packet);
    CFaBaoData::PackageData(&this->m_FaBaoData, packet);
    CGoblinData::PackageData(&this->m_CGoblinData, packet);
    CharFamilyDBData::PackageData(&this->m_FamilyData, packet);
    WorshipDBData::PackageData(&this->m_WorshipData, packet);
    CJueWeiData::PackageData(&this->m_JueWeiData, packet);
    ScoreShopData::PackageData(&this->m_ScoreShopData, packet);
    TouZiData::PackageData(&this->m_TouZiData, packet);
    CHuoYueDuData::PackageData(&this->m_HuoYueDuData, packet);
    MysteryShopDBData::PackageData(&this->m_MysteryShopDBData, packet);
    ExchangeDBData::PackageData(&this->m_ExchangeDBData, packet);
    CharWishDBData::PackageData(&this->m_WishDBData, packet);
    CVplanData::PackageData(&this->m_VplanData, packet);
    CharWingDBData::PackageData(&this->m_CharWingDBData, packet);
    MagicBoxDBData::PackageData(&this->m_MagicBoxDBData, packet);
    VipData::PackageData(&this->m_vipData, packet);
    CShiZhuangData::PackageData(&this->m_ShiZhuangData, packet);
    CMonthlyChouJiangData::PackageData(&this->m_CMonthlyChouJiangData, packet);
    ChouJiangData::PackageData(&this->m_ChouJinagData, packet);
    MoneyRewardTaskData::PackageData(&this->m_MoneyRewardTaskData, packet);
    WuHunShopDBData::PackageData(&this->m_WuHunShopDBData, packet);
    CMingGeData::PackageData(&this->m_CMingGeData, packet);
    CKunData::PackageData(&this->m_CKunData, packet);
    CFlopDraw::PackageData(&this->m_CFlopDraw, packet);
    CSevenDayData::PackageData(&this->m_CSevenDayData, packet);
    PortalDBData::PackageData(&this->m_PortalDBData, packet);
    XinMoDBData::PackageData(&this->m_XinMoDBData, packet);
    CLittleHelperData::PackageData(&this->m_CLittleHelper, packet);
    NationalDayData::PackageData(&this->m_NationalDayData, packet);
    EquipRongHeData::PackageData(&this->m_EquipRongHeData, packet);
}

（内容由AI生成，仅供参考）
