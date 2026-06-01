$files = @("platform_log_base","important_system_log","online_user_24th_log")
foreach ($f in $files) {
    $c = Get-Content "D:\444\gameserver_src\src\Other\$f.cpp" -Raw
    # Remove ALL destructors
    $c = $c -replace "(?s)$f::~$f\(\)\r?\n\{.*?\r?\n\}", ""
    # Add one destructor back at start (before first method after ctor)
    $c = $c -replace "(?s)($f::$f\(\)\r?\n\{.*?\r?\n\}).*?($f::)", "`$1`r`n$f::~$f()`r`n{`r`n    // TODO: impl`r`n}`r`n`r`n`$2"
    Set-Content "D:\444\gameserver_src\src\Other\$f.cpp" $c
    Write-Output "Fixed $f"
}
