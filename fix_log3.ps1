$files = @("skill_log","boss_log","level_up_log","main_task_log","important_system_log","currency_log","bossjoin_log","death_log","proc_log","item_log","online_user_24th_log","logout_log")
foreach ($f in $files) {
    $cppPath = "D:\444\gameserver_src\src\Other\$f.cpp"
    $c = Get-Content $cppPath -Raw
    # Remove ALL destructors (ClassName::~ClassName() { ... }
    do { $prev = $c; $c = $c -replace "(?s)$f::~$f\(\)\r?\n\{.*?\}", "" } while ($c -ne $prev)
    # Remove extra blank lines from dtor removal
    $c = $c -replace "\r?\n{3,}", "`r`n`r`n"
    # Add one destructor after constructor
    $c = $c -replace "(?s)($f::$f\(\)\r?\n\{.*?\r?\n\})", "`$1`r`n$f::~$f()`r`n{`r`n    // TODO: impl`r`n}`r`n"
    Set-Content $cppPath $c
    Write-Output "Fixed $f"
}
