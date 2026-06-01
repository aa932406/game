$gsPath = "D:\444\gameserver_src\gameserver.cc"

function Get-DecompiledBody($className, $methodName) {
    $escaped = [regex]::Escape("__cdecl $className" + "::$methodName(")
    $match = Select-String -LiteralPath $gsPath -Pattern $escaped
    if (-not $match) { return $null }
    $startIdx = $match[0].LineNumber
    $allLines = Get-Content $gsPath -TotalCount ($startIdx + 300) | Select-Object -Skip ($startIdx)
    $body = @()
    $braceDepth = 0
    $inBody = $false
    foreach ($line in $allLines) {
        if (-not $inBody) {
            if ($line.Trim() -eq '{') { $inBody = $true }
            continue
        }
        if ($line -match '^#{3,}') { break }
        if ($line -match '^\w+ __cdecl \w+::') { break }
        if ($line -match '^\w+ __fastcall \w+::') { break }
        foreach ($ch in $line.ToCharArray()) {
            if ($ch -eq '{') { $braceDepth++ }
            elseif ($ch -eq '}') { $braceDepth-- }
        }
        $body += $line
        if ($braceDepth -le 0) { break }
    }
    return $body -join "`r`n"
}

function Convert-ToCpp($decompBody, $retType = "void") {
    if ([string]::IsNullOrWhiteSpace($decompBody)) {
        if ($retType -ne 'void') { return "    return $($retType)();" }
        return ""
    }
    $code = $decompBody
    $code = $code -replace "this->_vptr_\w+\s*=\s*\([^)]*\).*?;", ""
    $code = $code -replace '(?m)^\s*\w+(?:<[^>]*>)?\s+\*?\w+(?:\[.*?\])?\s*;\s*(?://.*)?$', ''
    $code = $code -replace '(?m)^\s*_\w+\s+\w+(?:\[.*?\])?\s*;\s*(?://.*)?$', ''
    $code = $code -replace '(?m)^\s*(?:\w+\s+\**)?\w+a\s*=\s*(?:this|\w+);\s*$', ''
    $code = $code -replace '(?<!// )this->', ''
    $code = $code -replace '\s*// \w+$', ''
    $code = $code -replace '\s*// \[[^\]]*\] BYREF', ''
    $code = $code -replace 'std::map<[^>]*>::operator\[\]\(&(\w+),\s*&(\w+)\)', '$1[$2]'
    $code = $code -replace 'std::map<[^>]*>::find\(&(\w+),\s*&(\w+)\)', '$1.find($2)'
    $code = $code -replace 'std::map<[^>]*>::lower_bound\(&(\w+),\s*&(\w+)\)', '$1.lower_bound($2)'
    $code = $code -replace 'std::map<[^>]*>::count\(&(\w+),\s*&(\w+)\)', '$1.count($2)'
    $code = $code -replace 'std::map<[^>]*>::begin\(&(\w+)\)', '$1.begin()'
    $code = $code -replace 'std::map<[^>]*>::end\(&(\w+)\)', '$1.end()'
    $code = $code -replace 'std::map<[^>]*>::clear\(&(\w+)\)', '$1.clear()'
    $code = $code -replace 'std::map<[^>]*>::operator=\(&(\w+),\s*&(\w+)\)', '$1 = $2'
    $code = $code -replace 'std::list<[^>]*>::push_back\(&(\w+),\s*', '$1.push_back('
    $code = $code -replace 'std::list<[^>]*>::begin\(&(\w+)\)', '$1.begin()'
    $code = $code -replace 'std::list<[^>]*>::end\(&(\w+)\)', '$1.end()'
    $code = $code -replace 'std::list<[^>]*>::clear\(&(\w+)\)', '$1.clear()'
    $code = $code -replace 'std::list<[^>]*>::empty\(&(\w+)\)', '$1.empty()'
    $code = $code -replace 'std::list<[^>]*>::size\(&(\w+)\)', '$1.size()'
    $code = $code -replace 'std::vector<[^>]*>::push_back\(&(\w+),\s*', '$1.push_back('
    $code = $code -replace 'std::vector<[^>]*>::clear\(&(\w+)\)', '$1.clear()'
    $code = $code -replace 'std::string::operator=\(&(\w+),\s*([^)]+)\)', '$1 = $2'
    $code = $code -replace 'std::string::clear\(&(\w+)\)', '$1.clear()'
    $code = $code -replace 'Answer::NetPacket::writeInt32\((\w+),\s*', '$1->writeInt32('
    $code = $code -replace 'Answer::NetPacket::writeInt64\((\w+),\s*', '$1->writeInt64('
    $code = $code -replace 'Answer::NetPacket::writeInt8\((\w+),\s*', '$1->writeInt8('
    $code = $code -replace 'Answer::NetPacket::writeUInt8\((\w+),\s*', '$1->writeUInt8('
    $code = $code -replace 'Answer::NetPacket::writeUTF8\((\w+),\s*', '$1->writeUTF8('
    $code = $code -replace 'Answer::NetPacket::writeBool\((\w+),\s*', '$1->writeBool(')
    $code = $code -replace 'Answer::NetPacket::writeFloat\((\w+),\s*', '$1->writeFloat(')
    $code = $code -replace 'Answer::NetPacket::writeDouble\((\w+),\s*', '$1->writeDouble(')
    $code = $code -replace 'Answer::NetPacket::readInt32\((\w+)\)', '$1->readInt32()'
    $code = $code -replace 'Answer::NetPacket::readInt64\((\w+)\)', '$1->readInt64()'
    $code = $code -replace 'Answer::NetPacket::readUInt8\((\w+)\)', '$1->readUInt8()'
    $code = $code -replace 'Answer::NetPacket::readInt8\((\w+)\)', '$1->readInt8()'
    $code = $code -replace 'Answer::NetPacket::readUTF8\((\w+)\)', '$1->readUTF8()'
    $code = $code -replace 'Answer::NetPacket::readBool\((\w+)\)', '$1->readBool()'
    $code = $code -replace 'Answer::NetPacket::readFloat\((\w+)\)', '$1->readFloat()'
    $code = $code -replace 'Answer::NetPacket::readDouble\((\w+)\)', '$1->readDouble()'
    $code = $code -replace '(?m)^\s*std::map<[^>]*>::map\(&\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::list<[^>]*>::list\(&\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::string::string\(&\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::vector<[^>]*>::vector\(&\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::map<[^>]*>::~map\(&\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::list<[^>]*>::~list\(&\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::string::~string\(&\w+\);\s*$', ''
    $code = $code -replace 'operator delete\(this\)', ''
    $code = $code -replace '(?m)^\s*\r?\n', ''
    $code = $code -replace '(?m)^', '    '
    $code = $code.TrimStart()
    return $code
}

# -- Extracted from previous analysis: classes with unimplemented methods declared in headers --
$targets = @{
    "CExtSystemBase" = "Character"
    "CfgBFZLEnterCostTable" = "Config"
    "CfgBlacketMarketTable" = "Config"
}

$baseDir = "D:\444\gameserver_src"

foreach ($cn in $targets.Keys) {
    $dir = $targets[$cn]
    $hdrPath = "$baseDir\include\$dir\$cn.h"
    $cppPath = "$baseDir\src\$dir\$cn.cpp"
    
    if (-not (Test-Path $hdrPath)) {
        Write-Host "SKIP $cn: no header"
        continue
    }
    
    $hdr = Get-Content $hdrPath -Raw
    $cpp = Get-Content $cppPath -Raw
    
    # Find all methods declared in header
    $declMethods = @()
    foreach ($m in [regex]::Matches($hdr, "(?:(?:virtual\s+)?\w+\s+)?(\w+)\s*\([^)]*\)")) {
        $mn = $m.Groups[1].Value
        if ($mn -notin @($cn, "~$cn", 'override', 'const', 'virtual', 'public', 'private', 'protected', 'class', 'struct')) {
            $declMethods += $mn
        }
    }
    
    # Find already implemented methods (excluding constructor/destructor)
    $implMethods = @()
    foreach ($im in [regex]::Matches($cpp, "${cn}::(\w+)\s*\(")) {
        $imn = $im.Groups[1].Value
        if ($imn -ne $cn -and $imn -ne "~$cn") {
            $implMethods += $imn
        }
    }
    
    # Find methods that need extraction
    $missing = $declMethods | Where-Object { $_ -notin $implMethods }
    
    if ($missing.Count -eq 0) {
        Write-Host "SKIP $cn: all methods implemented ($($implMethods.Count))"
        continue
    }
    
    Write-Host ""
    Write-Host "=== $cn ($dir) ==="
    Write-Host "Missing methods: $($missing -join ', ')"
    
    $newBody = ""
    foreach ($methodName in $missing) {
        Write-Host "  Extracting $methodName..."
        $body = Get-DecompiledBody $cn $methodName
        if ($body) {
            $cppBody = Convert-ToCpp $body
            $newBody += "`r`n$cn" + "::$methodName" + "()`r`n{`r`n" + $cppBody + "`r`n}`r`n"
            Write-Host "    OK ($($cppBody.Length) chars)"
        } else {
            Write-Host "    NOT FOUND in gameserver.cc"
        }
    }
    
    if ($newBody) {
        $out = $cpp.TrimEnd() + $newBody
        Set-Content $cppPath $out
        Write-Host "  Updated $cppPath"
    }
}

Write-Host ""
Write-Host "Done"