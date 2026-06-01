$dir = "D:\444\gameserver_src\include\Other"
Get-ChildItem "$dir\*.h" | Where-Object {
    (Get-Content $_.FullName -Raw) -match 'class \w+ : public CExtSystemBase'
} | ForEach-Object {
    $path = $_.FullName
    $c = Get-Content $path -Raw
    $newc = $c -replace '    ~(\w+)\(\);', '    virtual ~$1();'
    if ($c -ne $newc) {
        Set-Content $path $newc
        Write-Output ("Fixed: " + $_.BaseName)
    }
}
