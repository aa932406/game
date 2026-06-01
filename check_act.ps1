$files = @('CCampWar','CCityWar','CDaTiHD','CFamilyLight','CFamilyWar','CHorseRacing','CKaiFuBoss','CMoLingRuQin','CPeerlessWar','CRefreshMonster','CRongLian','CrossTower','CSqiderQueen','CTianJiangBaoXiang','CXinMaiMap','CYaBiao','UltimateChallenge')
$ok = 0
foreach ($f in $files) {
    $hc = Get-Content "D:\444\gameserver_src\include\Other\$f.h" -Raw
    $cc = Get-Content "D:\444\gameserver_src\src\Other\$f.cpp" -Raw
    $issues = @()
    if ($hc -match 'void\*\*') { $issues += 'vptr' }
    if ($hc -match "$f \*") { $issues += 'this' }
    if ($cc -match '_vptr_CActivity') { $issues += 'cpp_vptr' }
    if ($hc -notmatch ': public CActivity') { $issues += 'no_base' }
    if ($hc -match '~\w+\(const') { $issues += 'bad_dtor' }
    if ($issues.Count -eq 0) { $ok++ } else { Write-Output ("BAD $f " + ($issues -join ',')) }
}
Write-Output ("OK $ok/17")
