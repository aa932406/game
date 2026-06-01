$files = @("platform_log_base","skill_log","boss_log","level_up_log","main_task_log","important_system_log","currency_log","bossjoin_log","death_log","proc_log","item_log","online_user_24th_log","logout_log")
$ok = 0; $bad = 0
foreach ($f in $files) {
    $h = Get-Content "D:\444\gameserver_src\include\Other\$f.h" -Raw
    $c = Get-Content "D:\444\gameserver_src\src\Other\$f.cpp" -Raw
    $issues = @()
    if ($h -match "$f \* this") { $issues += 'h_this' }
    if ($h -match 'void\*\*') { $issues += 'h_vptr' }
    if ($f -ne "platform_log_base") {
        if ($h -notmatch ': public platform_log_base') { $issues += 'h_no_base' }
        if ($h -notmatch 'override') { $issues += 'h_no_override' }
    }
    if ($c -match "$f \*const this") { $issues += 'c_this' }
    if ($c -match '_vptr_platform_log_base') { $issues += 'c_vptr' }
    if ($issues.Count -eq 0) { $ok++ } else { Write-Output ("BAD $f " + ($issues -join ',')) }
}
Write-Output ("OK $ok/13 BAD $bad/13")
