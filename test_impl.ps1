$cn = "BossDistribution"
$cpp = Get-Content "D:\444\gameserver_src\src\Other\$cn.cpp" -Raw
$hdr = Get-Content "D:\444\gameserver_src\include\Other\$cn.h" -Raw

Write-Output "=== Analyzing $cn ==="
Write-Output ""
Write-Output "Header declares these members/params:"
$hdr
