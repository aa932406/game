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

foreach ($cn in $ClassNames) {
    $hdrPath = "D:\444\gameserver_src\include\Other\$cn.h"
    $cppPath = "D:\444\gameserver_src\src\Other\$cn.cpp"
    
    if (-not (Test-Path $hdrPath)) { Write-Output "NOHEADER $cn"; continue }
    
    $h = Get-Content $hdrPath -Raw
    $c = ""

    # Remove explicit this from header: "ClassName * this" or "ClassName *this" in method params
    $h = $h -replace "$cn \* this, ", ""
    $h = $h -replace "$cn \* this\)", ")"
    $h = $h -replace "$cn \*this, ", ""
    $h = $h -replace "$cn \*this\)", ")"
    
    # Remove __return_ptr: "Type *__return_ptr retstr" -> "Type &retstr" or just fix name
    $h = $h -replace '\*__return_ptr (\w+)', '&$1'
    
    # Remove base-class junk from header (in case any have lingering forward declares)
    $h = $h -replace '(?m)^class (Monster|CfgMonster|CfgMap|CfgActivity|CActivityMap|Answer_NetPacket|Position);\r?\n', ''

    Set-Content $hdrPath $h

    if (Test-Path $cppPath) {
        $c = Get-Content $cppPath -Raw
        
        # Remove explicit this from cpp: "ClassName *const this" or "ClassName * this" 
        $c = $c -replace "$cn \*const this, ", ""
        $c = $c -replace "$cn \*const this\)", ")"
        $c = $c -replace "$cn \* this, ", ""
        $c = $c -replace "$cn \* this\)", ")"
        $c = $c -replace "$cn \*const this\)\s*(\r?\n)", ")`$1"
        $c = $c -replace "$cn \*this, ", ""
        $c = $c -replace "$cn \*this\)", ")"
        
        # Fix constructors: ClassName::ClassName()  (remove *const this from between parens)
        $c = $c -replace "$cn::$cn\((?:\w+ \*const this|$cn \*const this)\)", "$cn::$cn()"
        
        # Fix __return_ptr in cpp
        $c = $c -replace '\*__return_ptr (\w+)', '&$1'
        
        # Remove _vptr lines (in case any exist)
        $c = $c -replace '(?m)^.*_vptr_.*\r?\n', ''
        
        Set-Content $cppPath $c
    }
    
    Write-Output "OK $cn"
}
