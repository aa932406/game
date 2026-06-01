$files = @('CCampWar','CCityWar','CDaTiHD','CFamilyLight','CFamilyWar','CHorseRacing','CKaiFuBoss','CMoLingRuQin','CPeerlessWar','CRefreshMonster','CRongLian','CrossTower','CSqiderQueen','CTianJiangBaoXiang','CXinMaiMap','CYaBiao','UltimateChallenge')
foreach ($cn in $files) {
    $hdrPath = "D:\444\gameserver_src\include\Other\$cn.h"
    $cppPath = "D:\444\gameserver_src\src\Other\$cn.cpp"
    
    # Fix header
    $h = Get-Content $hdrPath -Raw
    # Fix destructor that got parameter
    $h = $h -replace "~\w+\(const CfgActivity\* cfgActivity\)", "~$cn()"
    # Remove ALL "ClassName * this," patterns from method params
    $h = $h -replace "$cn \* this, ", ""
    $h = $h -replace "$cn \* this\)", ")"
    # Remove CCampWar *const this 
    $h = $h -replace "$cn \*const this, ", ""
    $h = $h -replace "$cn \*const this\)", ")"
    Set-Content $hdrPath $h
    
    # Fix cpp
    $c = Get-Content $cppPath -Raw
    $c = $c -replace '~\w+\(const CfgActivity\* cfgActivity\)', "~$cn()"
    $c = $c -replace "$cn \*const this, ", ""
    $c = $c -replace "$cn \*const this\)", ")"
    $c = $c -replace "$cn \* this, ", ""
    $c = $c -replace "$cn \* this\)", ")"
    Set-Content $cppPath $c
    
    Write-Output "Fixed: $cn"
}
