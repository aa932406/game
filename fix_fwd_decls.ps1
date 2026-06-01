$dir = "D:\444\gameserver_src\include\Other"
$fixes = @{
    "IconStateList" = "class IconStateList;"
    "ShowIcon" = "struct ShowIcon;"
    "MemChrBag" = "struct MemChrBag;"
    "MemChrBagVector" = "class MemChrBagVector;"
    "Int32Vector" = "class Int32Vector;"
    "DieRecord" = "class DieRecord;"
}

Get-ChildItem "$dir\*.h" | Where-Object {
    (Get-Content $_.FullName -Raw) -match 'class \w+ : public CExtSystemBase'
} | ForEach-Object {
    $path = $_.FullName
    $c = Get-Content $path -Raw
    
    # Get the class name
    $className = $null
    if ($c -match 'class (\w+) : public CExtSystemBase') {
        $className = $Matches[1]
    }
    
    $neededTypes = @{}
    foreach ($type in $fixes.Keys) {
        # Check if the type is used in a function signature
        $pattern = "(?<!\w)$type[\*\s]"
        if ($c -match $pattern) {
            # Check if it's already declared
            $declPattern = "(class|struct) $type"
            if ($c -notmatch $declPattern) {
                $neededTypes[$type] = $fixes[$type]
            }
        }
    }
    
    # Check for MemChrBagVector (special pattern)
    if ($c -match 'MemChrBagVector') {
        $neededTypes["MemChrBagVector"] = $fixes["MemChrBagVector"]
    }
    if ($c -match 'Int32Vector') {
        $neededTypes["Int32Vector"] = $fixes["Int32Vector"]
    }
    if ($c -match 'DieRecord') {
        $neededTypes["DieRecord"] = $fixes["DieRecord"]
    }
    
    if ($neededTypes.Count -gt 0) {
        $insertPoint = $c.IndexOf("class $className : public CExtSystemBase")
        $insertLine = $c.LastIndexOf("`n", $insertPoint) + 1
        $before = $c.Substring(0, $insertLine)
        $after = $c.Substring($insertLine)
        
        $fwdDecls = ""
        foreach ($type in $neededTypes.Keys) {
            $fwdDecls += $neededTypes[$type] + "`r`n"
        }
        
        $newc = $before + $fwdDecls + $after
        Set-Content $path $newc
        Write-Output ("$($_.BaseName): added " + ($neededTypes.Keys -join ", "))
    }
}
