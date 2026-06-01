# Process all TODO files using gameserver.cc as the source of truth
$gameserver = "D:\444\gameserver_src\gameserver.cc"
$gsContent = Get-Content $gameserver -ReadCount 0

# Build line index for fast searching
Write-Output "Building line index..."
$gsLines = $gsContent
$gsCount = $gsLines.Count
Write-Output "gameserver.cc has $gsCount lines"

function Find-MethodInGameserver {
    param([string]$class, [string]$method)
    $target = "$class::$method("
    $target2 = "$class::$method("
    for ($i = 0; $i -lt $gsCount; $i++) {
        $line = $gsLines[$i]
        if ($line -match "__cdecl $class\:\:$method\(") {
            return $i
        }
    }
    return -1
}

function Extract-MethodBody {
    param([int]$startLine)
    $braceCount = 0
    $started = $false
    $body = @()
    for ($i = $startLine; $i -lt $gsCount; $i++) {
        $line = $gsLines[$i]
        if (-not $started) {
            if ($line -match '^\s*\{') { $started = $true }
            continue
        }
        # Check if it's the end of function (next ##### or next function)
        if ($line -match '^#{2,}') { break }
        if ($line -match '^\w+ __cdecl \w+::') { break }
        if ($line -match '^\w+ \w+\(\)$') { break }
        
        foreach ($ch in $line.ToCharArray()) {
            if ($ch -eq '{') { $braceCount++ }
            elseif ($ch -eq '}') { $braceCount-- }
        }
        
        $body += $line
        if ($braceCount -le 0) { break }
    }
    return $body
}

# Test with CLibraryTable
$class = "CLibraryTable"
$methods = @("AddCfgLibraryItem","AddCfgLibraryQuality","AddCfgLibraryCost","GetCfgLibraryQuality","GetCfgLibraryItem")
foreach ($m in $methods) {
    $idx = Find-MethodInGameserver $class $m
    if ($idx -ge 0) {
        Write-Output "$class::$m at line $idx"
        $body = Extract-MethodBody $idx
        $body | ForEach-Object { Write-Output "  $_" }
    }
}
