param([string]$className = "CLibraryTable")

$gsPath = "D:\444\gameserver_src\gameserver.cc"
$cppDir = "D:\444\gameserver_src\src\Other"

# Find all __cdecl methods for this class in gameserver.cc
$lines = Get-Content $gsPath
$classMethods = @{}
$i = 0
while ($i -lt $lines.Count) {
    if ($lines[$i] -match "^\w+ __cdecl $className\:\:(\w+)\(") {
        $mn = $matches[1]
        $sigLine = $i
        $bodyStart = $i + 1
        # Find {
        while ($bodyStart -lt $lines.Count -and $lines[$bodyStart] -notmatch '^\s*\{') { $bodyStart++ }
        if ($bodyStart -ge $lines.Count) { $i++; continue }
        $bodyStart++ # past {
        
        # Find matching }
        $depth = 1
        $bodyEnd = $bodyStart
        while ($bodyEnd -lt $lines.Count -and $depth -gt 0) {
            foreach ($ch in $lines[$bodyEnd].ToCharArray()) {
                if ($ch -eq '{') { $depth++ }
                elseif ($ch -eq '}') { $depth-- }
            }
            if ($depth -gt 0) { $bodyEnd++ }
        }
        
        # Store sig + body (excluding surrounding braces)
        $classMethods[$mn] = @{
            sig = $lines[$sigLine]
            body = @($lines[($bodyStart)..($bodyEnd-1)])
        }
        $i = $bodyEnd
    }
    $i++
}

Write-Output ("Found " + $classMethods.Count + " methods for $className")

# Convert and apply to .cpp
$cppPath = "$cppDir\$className.cpp"
if (-not (Test-Path $cppPath)) { Write-Output "No cpp"; exit }

$cppLines = Get-Content $cppPath
$newLines = @()
$i = 0
while ($i -lt $cppLines.Count) {
    $newLines += $cppLines[$i]
    
    if ($cppLines[$i] -match "^\w+(?:\s+\w+)*\s+$className::(\w+)\s*\(") {
        $mn = $matches[1]
        if ($mn -eq $className -or $mn -eq "~$className") { $i++; continue }
        
        # Read ahead to check for TODO
        $hasTodo = $false
        for ($j = $i + 1; $j -lt [Math]::Min($i + 10, $cppLines.Count); $j++) {
            if ($cppLines[$j] -match 'TODO') { $hasTodo = $true; break }
        }
        
        if ($hasTodo -and $classMethods.ContainsKey($mn)) {
            # Find opening brace in cpp
            $braceLine = -1
            for ($j = $i + 1; $j -lt $cppLines.Count; $j++) {
                if ($cppLines[$j].Trim() -eq '{') { $braceLine = $j; break }
            }
            if ($braceLine -lt 0) { $i++; continue }
            
            # Skip todo body to matching }
            $skipTo = $braceLine + 1
            $depth = 0
            $inBody = $false
            while ($skipTo -lt $cppLines.Count) {
                if (-not $inBody) {
                    if ($cppLines[$skipTo].Trim() -eq '{') { $inBody = $true; $skipTo++; continue }
                }
                foreach ($ch in $cppLines[$skipTo].ToCharArray()) {
                    if ($ch -eq '{') { $depth++ }
                    elseif ($ch -eq '}') { $depth-- }
                }
                if ($depth -le 0) { break }
                $skipTo++
            }
            
            # Now replace: keep {, insert converted body, keep }
            Write-Output ("  Converting $mn (" + $classMethods[$mn].body.Count + " lines)")
            
            # Convert body
            $convBody = @()
            foreach ($bline in $classMethods[$mn].body) {
                $l = $bline
                # Remove IDA var decls
                if ($l -match '^\s*\w+(?:<[^>]*>)?\s+\*?\w+(?:\[.*?\])?\s*;\s*(?://.*)?$') { continue }
                if ($l -match '^\s*_\w+\s+\w+(?:\[.*?\])?\s*;') { continue }
                if ($l -match '^\s*\w+\s*\*?\s*\w+\s*;\s*$') { continue }
                if ($l -match '\w+a\s*=\s*(?:this|\w+);') { continue }
                # Remove vtable
                if ($l -match '_vptr_') { continue }
                # Remove register/BYREF
                $l = $l -replace '\s*// \[[^\]]*\] BYREF', ''
                $l = $l -replace '\s*// \w+$', ''
                # Remove calling convention
                $l = $l -replace '__cdecl ', ''
                # this->x -> x
                $l = $l -replace 'this->', ''
                # container method calls
                $l = $l -replace '(?s)\bstd::(\w+)<[^>]*>::(\w+)\(&(\w+),\s*', '$3.$2('
                $l = $l -replace '(?s)\bstd::(\w+)<[^>]*>::(\w+)\(&(\w+)\)', '$3.$2()'
                $l = $l -replace '(?s)\bstd::(\w+)<[^>]*>::operator\[\]\(&(\w+),\s*&(\w+)\)', '$2[$3]'
                # *vN = value -> just keep value (loses temp var)
                $l = $l -replace '^\s+\*v\d+\s*=\s*', '    '
                # vN = value -> auto vN = value
                $l = $l -replace '^\s+v\d+\s*=\s*', '    auto v = '
                # Answer::NetPacket
                $l = $l -replace 'Answer::NetPacket::writeInt32\((\w+),\s*', '$1->writeInt32('
                $l = $l -replace 'Answer::NetPacket::writeInt64\((\w+),\s*', '$1->writeInt64('
                $l = $l -replace 'Answer::NetPacket::writeUTF8\((\w+),\s*', '$1->writeUTF8('
                $l = $l -replace 'Answer::NetPacket::writeInt8\((\w+),\s*', '$1->writeInt8('
                $l = $l -replace 'Answer::NetPacket::writeUInt8\((\w+),\s*', '$1->writeUInt8('
                $l = $l -replace 'Answer::NetPacket::writeBool\((\w+),\s*', '$1->writeBool('
                $l = $l -replace 'Answer::NetPacket::readInt32\((\w+)\)', '$1->readInt32()'
                $l = $l -replace 'Answer::NetPacket::readInt64\((\w+)\)', '$1->readInt64()'
                $l = $l -replace 'Answer::NetPacket::readUTF8\((\w+)\)', '$1->readUTF8()'
                $l = $l -replace 'Answer::NetPacket::readInt8\((\w+)\)', '$1->readInt8()'
                $l = $l -replace 'Answer::NetPacket::readUInt8\((\w+)\)', '$1->readUInt8()'
                $l = $l -replace 'Answer::NetPacket::readBool\((\w+)\)', '$1->readBool()'
                # Clean spaces
                $l = $l -replace '(?<=[^\s])\s{2,}', ' '
                if ([string]::IsNullOrWhiteSpace($l)) { continue }
                $convBody += $l
            }
            
            # Remove old lines between { and }
            while ($newLines[-1].Trim() -ne '{') {
                $newLines = $newLines[0..($newLines.Count-2)]
            }
            # Add converted body
            $newLines += $convBody
            $newLines += '}'
            
            # Skip to end of original body
            $i = $skipTo
            continue
        }
    }
    
    # Fix constructor initializer list
    if ($newLines[-1] -match "^\s*$className::$className\(") {
        # Look ahead for "{\n    : Base(" pattern
    }
    
    $i++
}

Set-Content $cppPath ($newLines -join "`r`n")
Write-Output ("Written: $cppPath")
