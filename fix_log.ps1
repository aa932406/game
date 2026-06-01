$LogClasses = @(
    "skill_log","boss_log","level_up_log","main_task_log",
    "important_system_log","currency_log","bossjoin_log",
    "death_log","proc_log","item_log","online_user_24th_log","logout_log"
)

# 1 - BASE class
$hdrBase = "D:\444\gameserver_src\include\Other\platform_log_base.h"
$h = Get-Content $hdrBase -Raw
$h = $h -replace "void PackageData\(platform_log_base \* this, Answer", "virtual void PackageData(Answer"
$h = $h -replace "void UnPackageData\(platform_log_base \* this, Answer", "virtual void UnPackageData(Answer"
$h = $h -replace 'virtual void CleanUp\(\)', 'virtual void CleanUp()'
if ($h -notmatch 'CleanUp') { $h = $h -replace '    ~platform_log_base\(\);', "    ~platform_log_base();`r`n    virtual void CleanUp();" }
if ($h -match 'void\*\*') { $h = $h -replace '(?m)^.*void\*\*.*\r?\n','' }
Set-Content $hdrBase $h

$cppBase = "D:\444\gameserver_src\src\Other\platform_log_base.cpp"
$c = Get-Content $cppBase -Raw
$c = $c -replace "platform_log_base::platform_log_base\(platform_log_base \*const this\)", "platform_log_base::platform_log_base()"
$c = $c -replace "platform_log_base::PackageData\(platform_log_base \*const this, Answer", "void platform_log_base::PackageData(Answer"
$c = $c -replace "platform_log_base::UnPackageData\(platform_log_base \*const this, Answer", "void platform_log_base::UnPackageData(Answer"
$c = $c -replace "platform_log_base::CleanUp\(platform_log_base \*const this\)", "void platform_log_base::CleanUp()"
$c = $c -replace '(?m)^.*_vptr_platform_log_base.*\r?\n',''
$c = $c -replace "(?s)platform_log_base::~platform_log_base\(\)\r?\n\{.*?\r?\n\}\r?\n", ""
Set-Content $cppBase $c
Write-Output "BASE OK"

# 2 - DERIVED
foreach ($cn in $LogClasses) {
    $hdrPath = "D:\444\gameserver_src\include\Other\$cn.h"
    $cppPath = "D:\444\gameserver_src\src\Other\$cn.cpp"
    if (-not (Test-Path $hdrPath)) { Write-Output "NO $cn"; continue }

    $h = Get-Content $hdrPath -Raw
    $h = $h -replace '(#include <map>\r?\n)', "`$1#include `"Other/platform_log_base.h`"`r`n"
    $h = $h -replace "(?m)^class $cn$", "class $cn : public platform_log_base"
    $h = $h -replace "$cn \* this, ", ""
    $h = $h -replace "$cn \* this\)", ")"
    if ($h -match 'void\*\*') { $h = $h -replace '(?m)^.*void\*\*.*\r?\n','' }
    # Add override to overridden methods
    if ($h -match 'CleanUp') { $h = $h -replace '(?m)^    void CleanUp\(\)', '    void CleanUp() override' }
    $h = $h -replace '(?m)^    void (PackageData|UnPackageData)\(Answer::NetPacket \*(\w+)\);', '    void $1(Answer::NetPacket *$2) override;'
    # Remove void** _vptr from header
    if ($h -match 'void\*\*') { $h = $h -replace '(?m)^.*void\*\*.*\r?\n','' }
    Set-Content $hdrPath $h

    $c = Get-Content $cppPath -Raw
    $c = $c -replace "$cn::$cn\($cn \*const this\)", "$cn::$cn()"
    $c = $c -replace "$cn \*const this, ", ""
    $c = $c -replace "$cn \*const this\)", ")"
    $c = $c -replace '(?m)^.*_vptr_platform_log_base.*\r?\n',''
    # Remove all destructor stubs and add one
    $c = $c -replace "(?s)$cn::~$cn\(\)\r?\n\{.*?\r?\n\}", "$cn::~$cn()`r`n{`r`n    // TODO: impl`r`n}"
    # Fix base calls
    $c = $c -replace 'platform_log_base::CleanUp\(this\)', 'platform_log_base::CleanUp()'
    $c = $c -replace 'platform_log_base::PackageData\(this, ', 'platform_log_base::PackageData('
    $c = $c -replace 'platform_log_base::UnPackageData\(this, ', 'platform_log_base::UnPackageData('
    $c = $c -replace 'platform_log_base::platform_log_base\(this\)', ''
    $c = $c -replace 'platform_log_base::~platform_log_base\(this\)', ''
    Set-Content $cppPath $c
    Write-Output "OK $cn"
}
