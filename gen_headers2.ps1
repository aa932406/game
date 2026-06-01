function Get-ClassMethods {
    param([string]$cn, [string]$cpp)
    $lines = Get-Content $cpp
    $methods = @()
    foreach ($line in $lines) {
        if ($line -match '^\s*//') { continue }
        if ($line -match '^\s*#') { continue }
        if ($line -match '^\s*$') { continue }
        
        if ($line -match "^\s*(?:(virtual|static)\s+)?(.*?)\s*$cn::(\w+)\s*\(") {
            $prefix = $matches[1]
            $retType = $matches[2].Trim()
            $method = $matches[3]
            
            if ($method -eq $cn -or $method -eq "~$cn") { continue }
            
            $fullSig = $line.Trim()
            if ($prefix) { $fullSig = $fullSig -replace "^\s*$prefix\s+", "" }
            $fullSig = $fullSig -replace "^\s*$retType\s+", ""
            $fullSig = $fullSig -replace "^$cn::$method\s*\(\s*", ""
            
            $params = ""
            if ($fullSig -match '^(.+?)\)') { $params = $matches[1].Trim() }
            
            # Handle __return_ptr in both params and retType
            $params = $params -replace '\*__return_ptr (\w+)', '&$1'
            $retType = $retType -replace '\*__return_ptr (\w+)', '&$1'
            
            $methods += @{name=$method; retType=$retType; params=$params; prefix=$prefix}
        }
    }
    return $methods
}

$classes = @(
"CBlackMarket","CBossKilledReward","CChrDepot","CDaTingReward",
"CFriendExpReward","CFunctionOpen","CGoblin","CGongMing",
"CharLittlerHelper","ChouJiang","CHuoYueDu","CKunExt",
"CLevelRefining","CMingGeExt","CMoneyRewardTask","CMonthlyChouJiang",
"CNationalDayHd","CPlayerMail","CQiFu","CRongHe",
"CSevenDayTask","CShangCheng","CShenWei","CShiZhuang",
"CSpecialEquip","CSuperTeHui","CTestServerReward","CTianLing",
"CTouZi","CTrade","CTreasureMap","Curse",
"CVip","CVplan","CWan360","CWuHunShop",
"CXingMai","CXinMo","CYunYingHD","ScoreShop"
)

$overrides = @("OnCleanUp","OnUpdate","InitSystem","DispatchNetDatas",
    "OnDaySwitch","OnLoadFromDB","OnSaveToDB","GetInterestsProtocol")

foreach ($cn in $classes) {
    $cppPath = "D:\444\gameserver_src\src\Other\$cn.cpp"
    if (-not (Test-Path $cppPath)) { Write-Output ("SKIP no cpp: $cn"); continue }
    
    $methods = Get-ClassMethods $cn $cppPath
    if ($methods.Count -eq 0) { 
        Write-Output ("SKIP no methods: $cn"); continue 
    }
    
    $over = @(); $regular = @()
    foreach ($m in $methods) {
        if ($overrides -contains $m.name) { $over += $m }
        else { $regular += $m }
    }
    
    $hdr = "#ifndef _$($cn.ToUpper())_H_`r`n#define _$($cn.ToUpper())_H_`r`n`r`n"
    $hdr += '#include "Character/CExtSystemBase.h"' + "`r`n`r`n"
    $hdr += "class $cn : public CExtSystemBase`r`n{`r`npublic:`r`n"
    $hdr += "    $cn();`r`n    virtual ~$cn();`r`n`r`n"
    
    foreach ($m in $over) {
        $sig = "    $($m.retType) $($m.name)($($m.params)) override;"
        $sig = $sig -replace '(?<=[^\s])\s{2,}', ' '
        $sig = $sig -replace '\(\s+\)', '()'
        $hdr += "$sig`r`n"
    }
    
    foreach ($m in $regular) {
        $sig = "    $($m.retType) $($m.name)($($m.params));"
        $sig = $sig -replace '(?<=[^\s])\s{2,}', ' '
        $sig = $sig -replace '\(\s+\)', '()'
        $hdr += "$sig`r`n"
    }
    
    $hdr += "};`r`n`r`n#endif // _$($cn.ToUpper())_H_`r`n"
    Set-Content "D:\444\gameserver_src\include\Other\$cn.h" $hdr
    Write-Output ("OK $cn - " + $methods.Count + " methods")
}
