$dir = "D:\444\gameserver_src\src\Other"
Get-ChildItem "$dir\*.cpp" | ForEach-Object {
    $cn = $_.BaseName
    $hdr = "D:\444\gameserver_src\include\Other\$cn.h"
    if ((Test-Path $hdr) -and (Get-Content $hdr -Raw) -match 'class \w+ : public CExtSystemBase') {
        $c = Get-Content $_.FullName -Raw
        if ($c -match "$cn \*const this") {
            Write-Output ("$cn has explicit this param")
        }
    }
}
Write-Output "Check complete"
