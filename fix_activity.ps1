$ActivityClasses = @{
    "CCampWar" = @("reset","OnUpdate","onPlayerKilled","onMonsterDie","addKillReward","onPlayerRevive","onPlantGather","RefreshBuff","broadcastActivityResult","onBeginGather","addPlayer","removePlayer","addPlayerScore","refreshRankIndex","addRewards","onTimeEnd","onWarEnd","canEnter","packetActivityScore","sendPlayerScore","SendPlayerRankInfo","broadcastReady","broadcastStart","selectCamp","getContTitle","notifyAddScore")
    "CCityWar" = @()
    "CDaTiHD" = @()
    "CFamilyLight" = @()
    "CFamilyWar" = @()
    "CHorseRacing" = @()
    "CKaiFuBoss" = @()
    "CMoLingRuQin" = @()
    "CPeerlessWar" = @()
    "CRefreshMonster" = @("reset","CheckActivity","FlushMonsters","stopActivity","broadcastStart","FreshObjGongGao")
    "CRongLian" = @("broadcastStart","sendPlayerScore")
    "CrossTower" = @()
    "CSqiderQueen" = @("StopActivityBySqiderQueenDie","broadcastStart","broadcastTimeEnd","broadcastKillAllSqiderQueen")
    "CTianJiangBaoXiang" = @()
    "CXinMaiMap" = @()
    "CYaBiao" = @()
    "UltimateChallenge" = @()
}

foreach ($cn in $ActivityClasses.Keys) {
    $hdrPath = "D:\444\gameserver_src\include\Other\$cn.h"
    $cppPath = "D:\444\gameserver_src\src\Other\$cn.cpp"
    
    if (-not (Test-Path $hdrPath)) { Write-Output "SKIP $cn - no header"; continue }
    
    # --- FIX HEADER ---
    $h = Get-Content $hdrPath -Raw
    
    # Inherit from CActivity
    $h = $h -replace '(?m)^class \w+$', "class $cn : public CActivity"
    
    # Add CActivity include (after last standard include or after existing includes)
    $h = $h -replace '(#include <map>\r?\n)(?!.*CActivity)', "`$1#include `"Activity/CActivity.h`"`r`n"
    
    # Remove _vptr line
    $h = $h -replace '(?m)^\s+// 虚函数表指针.*\r?\n', ''
    $h = $h -replace '(?m)^\s+void\*\* _vptr;\r?\n', ''
    
    # Remove m_cfgActivity (from base class)
    $h = $h -replace '(?m)^\s+const CfgActivity\* m_cfgActivity;.*\r?\n', ''
    $h = $h -replace '(?m)^\s+// TODO: 确认配置类型\r?\n', ''
    
    # Fix constructor/destructor signatures (remove this param from decl)
    $h = $h -replace "$cn\($cn \*const this, ", "$cn("
    $h = $h -replace "$cn\(\)", "$cn(const CfgActivity* cfgActivity)"
    
    # Remove explicit this from method params
    $h = $h -replace "(?m)^    (\w+(?:<[^>]*>)?\*? )\w+\($cn \*const this, ", '    $1'
    $h = $h -replace "(?m)^    void (\w+)\($cn \*const this\)", '    void $1()'
    $h = $h -replace "(?m)^    (bool|int32_t|int64_t|int8_t|int) (\w+)\($cn \*const this\)", '    $1 $2()'
    $h = $h -replace "(?m)^    (bool|int32_t|int64_t) (\w+)\($cn \*const this,", '    $1 $2('
    
    # Remove unused forward declares
    $h = $h -replace '(?m)^class Map;\r?\n', ''
    $h = $h -replace '(?m)^class Unit;\r?\n', ''
    $h = $h -replace '(?m)^class Monster;\r?\n', ''
    $h = $h -replace '(?m)^class Npc;\r?\n', ''
    $h = $h -replace '(?m)^class CfgActivity;\r?\n', ''
    $h = $h -replace '(?m)^class CfgMonster;\r?\n', ''
    $h = $h -replace '(?m)^class CfgMap;\r?\n', ''
    $h = $h -replace '(?m)^class Position;\r?\n', ''
    $h = $h -replace '(?m)^class Answer_NetPacket;\r?\n', ''
    
    Set-Content $hdrPath $h
    Write-Output "Header fixed: $cn"
    
    # --- FIX CPP ---
    if (-not (Test-Path $cppPath)) { Write-Output "  SKIP cpp for $cn"; continue }
    $c = Get-Content $cppPath -Raw
    
    # Fix constructor
    $c = $c -replace "$cn::$cn\($cn \*const this, const CfgActivity \*const cfgActivity\)", "$cn::$cn(const CfgActivity* cfgActivity)"
    $c = $c -replace "(?s)($cn::$cn\(const CfgActivity\* cfgActivity\)\r?\n\{).*?(\r?\n\})", "`$1`r`n    : CActivity(cfgActivity)`r`n{`r`n    // TODO: impl`r`n`$2"
    
    # Fix destructor - remove _vptr and the third (deleting) destructor
    $c = $c -replace "(?s)$cn::~$cn\(\)\r?\n\{.*?this->_vptr_CActivity.*?\r?\n\}", "$cn::~$cn()`r`n{`r`n    // TODO: impl`r`n}"
    
    # Remove explicit this from method params
    $c = $c -replace "$cn \*const this, ", ""
    $c = $c -replace "$cn \*const this\)", ")"
    
    # Remove _vptr_CActivity lines
    $c = $c -replace '(?m)^.*this->_vptr_CActivity.*\r?\n', ''
    
    # Remove second (deleting) destructor
    $c = $c -replace "(?s)$cn::~$cn\(\)\r?\n\{`r?\n?\s*$cn::~$cn\(this\);`r?\n?\s*operator delete\(this\);`r?\n?\}\r?\n", ""
    
    Set-Content $cppPath $c
    Write-Output "  CPP fixed: $cn"
}
