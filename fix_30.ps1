$files = @(
"cfgEquipUpPhase","RefreshMonsterCfg","RongHeCfg","RongHeRecord",
"ScoreShopCfg","SelectItemCfg","SevenTouZi","ShangRenCfg",
"ShenYaoPosSuit","ShiZhuangSuitCfg","ShiZhuangSuitInfo",
"ShopGoods","Speed360Reward","SummonSkill","SuperTeHuiCfg",
"SystemOpenGift","TaskDrop","TestServerReward","TreasureHunterCfg",
"TreasureMapData","VipCardCfg","VipEQuipPosSuit","VplanLevelReward",
"Wan360Reward","WinRefiningCfg","WuHunItem","XinMoCfg",
"XinQingReward","XunLeiCfg","ZhanHunCfg"
)

foreach ($cn in $files) {
    $cppPath = "D:\444\gameserver_src\src\Other\$cn.cpp"
    if (-not (Test-Path $cppPath)) { continue }
    
    $c = Get-Content $cppPath -Raw
    $c = $c -replace "$cn \*const this, ", ""
    $c = $c -replace "$cn \*const this\)", ")"
    Set-Content $cppPath $c
    Write-Output ("Fixed $cn")
}
