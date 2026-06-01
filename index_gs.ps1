# Direct implementation from gameserver.cc references
$gsPath = "D:\444\gameserver_src\gameserver.cc"

# Read the entire .cc file once (for fast access via line numbers)
Write-Object "Loading gameserver.cc line index..."
$gsByteLength = (Get-Item $gsPath).Length
$gsLines = Get-Content $gsPath
$gsCount = $gsLines.Count
Write-Object ("Loaded " + $gsCount + " lines")

# Store line ranges for each method of a class for quick access
# Pre-index: class -> method -> {startLine, endLine}
$classIndex = @{}

# Build index - scan all function declarations
for ($i = 0; $i -lt $gsCount; $i++) {
    $line = $gsLines[$i]
    if ($line -match "^\w+ __cdecl (\w+)::(\w+)\(") {
        $cn = $matches[1]
        $mn = $matches[2]
        # Find the end of this function
        $braceDepth = 0
        $inBody = $false
        $endIdx = $i
        for ($j = $i + 1; $j -lt $gsCount; $j++) {
            if (-not $inBody) {
                if ($gsLines[$j] -match '^\s*\{') { $inBody = $true }
                continue
            }
            if ($gsLines[$j] -match '^#{3,}') { $endIdx = $j - 1; break }
            if ($gsLines[$j] -match '^\w+ __cdecl \w+::') { $endIdx = $j - 1; break }
            if ($gsLines[$j] -match '^\w+ __fastcall \w+::') { $endIdx = $j - 1; break }
            
            foreach ($ch in $gsLines[$j].ToCharArray()) {
                if ($ch -eq '{') { $braceDepth++ }
                elseif ($ch -eq '}') { $braceDepth-- }
            }
            if ($braceDepth -le 0) { $endIdx = $j; break }
        }
        if (-not $classIndex.ContainsKey($cn)) { $classIndex[$cn] = @{} }
        $classIndex[$cn][$mn] = @{start=$i; end=$endIdx}
        $i = $endIdx  # Skip ahead
    }
}

Write-Object ("Indexed " + $classIndex.Count + " classes")

# Test: print CLibraryTable methods
if ($classIndex.ContainsKey("CLibraryTable")) {
    Write-Object ("CLibraryTable methods:")
    foreach ($mn in $classIndex["CLibraryTable"].Keys) {
        $info = $classIndex["CLibraryTable"][$mn]
        # Print raw body
        Write-Object ("--- $mn (lines $($info.start)-$($info.end)) ---")
        for ($j = $info.start; $j -le $info.end; $j++) {
            Write-Object ("  " + $gsLines[$j])
        }
    }
}
