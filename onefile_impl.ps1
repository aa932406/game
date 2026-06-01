param([string]$className = "CLibraryTable")

$gsPath = "D:\444\gameserver_src\gameserver.cc"
$cppDir = "D:\444\gameserver_src\src\Other"
$hdrDir = "D:\444\gameserver_src\include\Other"

function Get-GsMethodBody($cn, $mn) {
    $match = Select-String -LiteralPath $gsPath -Pattern "__cdecl $cn\:\:$mn\(" -SimpleMatch:$false
    if (-not $match) { return $null }
    $startLine = $match[0].LineNumber
    $allLines = Get-Content $gsPath
    $body = @()
    $depth = 0
    $inBody = $false
    for ($i = $startLine; $i -lt $allLines.Count; $i++) {
        $line = $allLines[$i]
        if (-not $inBody) {
            if ($line.Trim() -eq '{') { $inBody = $true }
            continue
        }
        if ($line -match '^#{3,}') { break }
        if ($line -match '^\w+ __cdecl \w+::') { break }
        
        foreach ($ch in $line.ToCharArray()) {
            if ($ch -eq '{') { $depth++ }
            elseif ($ch -eq '}') { $depth-- }
        }
        $body += $line
        if ($depth -le 0) { break }
    }
    return $body
}

function Convert-Simple($lines) {
    $result = @()
    foreach ($line in $lines) {
        $l = $line
        
        # Skip empty or IDA var decl
        if ($l -match '^\s*$') { continue }
        if ($l -match '^\s*\w+(?:<[^>]*>)?\s+\*?\w+(?:\[.*?\])?\s*;\s*//') { continue }
        if ($l -match '^\s*_\w+\s+\w+(?:\[.*?\])?\s*;') { continue }
        if ($l -match '^\s*\w+\s*\*?\s*\w+\s*;\s*$') { continue }
        if ($l -match '^\s*\w+[a-z]\s*=\s*(?:this|\w+);\s*$') { continue }
        
        # this->x -> x
        $l = $l -replace 'this->', ''
        
        # Remove register/BYREF comments
        $l = $l -replace '\s*// \w+$', ''
        $l = $l -replace '\s*// \[[^\]]*\] BYREF', ''
        
        # Remove vtable assignment
        if ($l -match '_vptr_') { continue }
        
        # std::container::method(&var, args) -> var.method(args)
        $l = $l -replace '(?s)\bstd::(\w+)<[^>]*>::(\w+)\(&(\w+),\s*', '$3.$2('
        $l = $l -replace '(?s)\bstd::(\w+)<[^>]*>::(\w+)\(&(\w+)\)', '$3.$2()'
        $l = $l -replace '(?s)\bstd::(\w+)<[^>]*>::operator\[\]\(&(\w+),\s*&(\w+)\)', '$2[$3]'
        $l = $l -replace '(?s)\bstd::(\w+)<[^>]*>::operator=\(&(\w+),\s*&(\w+)\)', '$2 = $3'
        
        # std::string::operator=
        $l = $l -replace 'std::string::operator=\(&(\w+),\s*', '$1 = '
        
        # Answer::NetPacket 
        $l = $l -replace 'Answer::NetPacket::writeInt32\((\w+),\s*', '$1->writeInt32('
        $l = $l -replace 'Answer::NetPacket::writeInt64\((\w+),\s*', '$1->writeInt64('
        $l = $l -replace 'Answer::NetPacket::writeUTF8\((\w+),\s*', '$1->writeUTF8('
        $l = $l -replace 'Answer::NetPacket::writeInt8\((\w+),\s*', '$1->writeInt8('
        $l = $l -replace 'Answer::NetPacket::writeUInt8\((\w+),\s*', '$1->writeUInt8('
        $l = $l -replace 'Answer::NetPacket::writeBool\((\w+),\s*', '$1->writeBool('
        $l = $l -replace 'Answer::NetPacket::writeFloat\((\w+),\s*', '$1->writeFloat('
        $l = $l -replace 'Answer::NetPacket::readInt32\((\w+)\)', '$1->readInt32()'
        $l = $l -replace 'Answer::NetPacket::readInt64\((\w+)\)', '$1->readInt64()'
        $l = $l -replace 'Answer::NetPacket::readUTF8\((\w+)\)', '$1->readUTF8()'
        $l = $l -replace 'Answer::NetPacket::readInt8\((\w+)\)', '$1->readInt8()'
        $l = $l -replace 'Answer::NetPacket::readUInt8\((\w+)\)', '$1->readUInt8()'
        $l = $l -replace 'Answer::NetPacket::readBool\((\w+)\)', '$1->readBool()'
        
        # Handle *vN = value (deref assign)
        $l = $l -replace '^\s+\*v\d+\s*=\s*(.+)', '    $1'
        $l = $l -replace '^\s+v\d+\s*=\s*(.+)', '    auto v = $1'
        
        # operator delete
        if ($l -match 'operator delete') { continue }
        if ($l -match '::~') { continue }
        
        # byte_ constant -> ""
        $l = $l -replace 'byte_\w+', '""'
        
        # Clean up multi-spaces
        $l = $l -replace '(?<=[^\s])\s{2,}', ' '
        
        $result += $l
    }
    return $result
}

function Process-File($cn) {
    $cppPath = "$cppDir\$cn.cpp"
    if (-not (Test-Path $cppPath)) { Write-Output ("SKIP: no cpp for $cn"); return }
    
    Write-Output ("Processing $cn...")
    $lines = Get-Content $cppPath
    $newLines = @()
    
    $i = 0
    while ($i -lt $lines.Count) {
        $line = $lines[$i]
        
        # Check if this line starts a method definition
        if ($line -match "^\s*(?:\w+(?:\s+\w+)*)\s+$cn::(\w+)\s*\(") {
            $methodName = $matches[1]
            $retType = ($line -replace "^\s*((?:\w+(?:\s+\w+)*))\s+$cn.*", '$1').Trim()
            if ($retType -eq $line.Trim()) { $retType = "void" }  # constructor
            
            # Add method signature
            $newLines += $line
            
            # Read until we find the opening brace
            $braceFound = $false
            $nextIdx = $i + 1
            while ($nextIdx -lt $lines.Count) {
                $l = $lines[$nextIdx]
                if ($l.Trim() -eq '{') { 
                    $braceFound = $true
                    $nextIdx++
                    break
                }
                if ($l.Trim() -match ':\s+\w+\(') {
                    # Initializer list after opening brace - fix it
                    $newLines[-1] = $newLines[-1] + $l.Trim()
                } else {
                    $newLines += $l
                }
                $nextIdx++
            }
            
            if (-not $braceFound) { $newLines += $line; $i++; continue }
            
            # Look for TODO in the next few lines
            $isTodo = $false
            $checkEnd = [Math]::Min($nextIdx + 5, $lines.Count)
            for ($j = $nextIdx; $j -lt $checkEnd; $j++) {
                if ($lines[$j] -match 'TODO') { $isTodo = $true; break }
            }
            
            if ($isTodo -and $methodName -ne $cn -and $methodName -ne "~$cn") {
                # Try to get from gameserver.cc
                $gsBody = Get-GsMethodBody $cn $methodName
                if ($gsBody) {
                    $converted = Convert-Simple $gsBody
                    $newLines += '{'
                    $newLines += $converted
                    # Ensure closing brace
                    if ($converted.Count -gt 0 -and $converted[-1] -notmatch '^\s*\}') {
                        $newLines += '}'
                    }
                    # Skip original body
                    $depth = 0
                    $inBody = $false
                    while ($nextIdx -lt $lines.Count) {
                        $l = $lines[$nextIdx]
                        if (-not $inBody) {
                            if ($l.Trim() -eq '{') { $inBody = $true }
                        } else {
                            foreach ($ch in $l.ToCharArray()) {
                                if ($ch -eq '{') { $depth++ }
                                elseif ($ch -eq '}') { $depth-- }
                            }
                            if ($depth -le 0) { $nextIdx++; break }
                        }
                        $nextIdx++
                    }
                    $i = $nextIdx
                    continue
                }
            }
            
            # If we didn't convert, keep original
            $newLines += '{'
            $depth = 0
            $inBody = $false
            while ($nextIdx -lt $lines.Count) {
                $l = $lines[$nextIdx]
                $newLines += $l
                if (-not $inBody) {
                    if ($l.Trim() -eq '{') { $inBody = $true }
                } else {
                    foreach ($ch in $l.ToCharArray()) {
                        if ($ch -eq '{') { $depth++ }
                        elseif ($ch -eq '}') { $depth-- }
                    }
                    if ($depth -le 0) { $nextIdx++; break }
                }
                $nextIdx++
            }
            $i = $nextIdx
        } else {
            $newLines += $line
            $i++
        }
    }
    
    # Write output
    Set-Content $cppPath ($newLines -join "`r`n")
    Write-Output ("  Done: $cn")
}

# Main
Process-File $className
Write-Output "Complete"
