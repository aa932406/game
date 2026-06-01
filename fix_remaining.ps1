# Fix remaining 18 header+cpp files with "ClassName * this" patterns
$pairs = @(
    @{cn="CDropItemGroup"}, @{cn="CEquipManager"}, @{cn="cfgSpecialMonsterTable"}
    @{cn="CGMBackstate"}, @{cn="CHttpRequest"}, @{cn="CMapRunner"}
    @{cn="CPetManager"}, @{cn="CRankMirror"}, @{cn="CTerritory"}
    @{cn="ItemBuff"}, @{cn="MonsterDungeon"}, @{cn="NpcDungeon"}
    @{cn="PathFinder"}, @{cn="PlantDungeon"}, @{cn="SkillBuff"}
    @{cn="TradeData"}, @{cn="Trailer"}, @{cn="URLService"}
)

foreach ($p in $pairs) {
    $cn = $p.cn
    $hdrPath = "D:\444\gameserver_src\include\Other\$cn.h"
    $cppPath = "D:\444\gameserver_src\src\Other\$cn.cpp"
    
    if (Test-Path $hdrPath) {
        $h = Get-Content $hdrPath -Raw
        $h = $h -replace "$cn \* this, ", ""
        $h = $h -replace "$cn \* this\)", ")"
        $h = $h -replace "$cn \*this, ", ""
        $h = $h -replace "$cn \*this\)", ")"
        $h = $h -replace '\*__return_ptr (\w+)', '&$1'
        Set-Content $hdrPath $h
    }
    
    if (Test-Path $cppPath) {
        $c = Get-Content $cppPath -Raw
        $c = $c -replace "$cn \*const this, ", ""
        $c = $c -replace "$cn \*const this\)", ")"
        $c = $c -replace "$cn \* this, ", ""
        $c = $c -replace "$cn \* this\)", ")"
        $c = $c -replace '\*__return_ptr (\w+)', '&$1'
        Set-Content $cppPath $c
    }
    
    Write-Output ("Fixed $cn")
}
