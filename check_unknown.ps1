$ClassNames = @(
    "Answer","BossDistribution","CEquipBack","CFestivalDoubleEleven",
    "CGuiGuDaoRen","CharTitle","ChouJiangTable","ChrTask",
    "CItemHelper","CKaiFuHuoDong","CKiaFuRecharge","CLibraryTable",
    "CMingGeTable","COpenBeta","COutLinkFestival","CUniteServer",
    "CVipClub","CWorldBoss","CycleTowerTable","CZongHeYunYingHD",
    "DanTianCfg","FaBaoTable","FamilyInfo","FamilyManager",
    "FestivalRank","KunLingTable","SevenTaskTable","TencentInfo",
    "Tile","TileManager","TreasureMapTabale","User",
    "VipTable","WorldBossInfo","XinMoTable"
)
$ok = 0; $bad = 0
foreach ($cn in $ClassNames) {
    $hdrPath = "D:\444\gameserver_src\include\Other\$cn.h"
    $h = Get-Content $hdrPath -Raw
    $issues = @()
    if ($h -match "$cn \* this") { $issues += 'this' }
    if ($h -match '\*__return_ptr') { $issues += 'return_ptr' }
    if ($h -match 'void\*\*') { $issues += 'vptr' }
    if ($issues.Count -eq 0) { $ok++ } else { $bad++; Write-Output ("BAD $cn " + ($issues -join ',')) }
}
Write-Output ("Headers: OK $ok BAD $bad")

$ok = 0; $bad = 0
foreach ($cn in $ClassNames) {
    $cppPath = "D:\444\gameserver_src\src\Other\$cn.cpp"
    if (-not (Test-Path $cppPath)) { continue }
    $c = Get-Content $cppPath -Raw
    $issues = @()
    if ($c -match "$cn \*const this") { $issues += 'this' }
    if ($c -match '\*__return_ptr') { $issues += 'return_ptr' }
    if ($c -match '_vptr_') { $issues += 'vptr' }
    if ($issues.Count -eq 0) { $ok++ } else { $bad++; Write-Output ("BAD CPP $cn " + ($issues -join ',')) }
}
Write-Output ("CPPs: OK $ok BAD $bad")
