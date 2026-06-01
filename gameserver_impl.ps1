# gameserver_impl.ps1
# Full pipeline: read from gameserver.cc, convert to C++, write to .cpp files
# Uses Select-String for fast lookup in the 30MB file

param(
    [string]$mode = "all"  # "test" for one file, "all" for everything
)

$gsPath = "D:\444\gameserver_src\gameserver.cc"
$srcDir = "D:\444\gameserver_src\src\Other"
$incDir = "D:\444\gameserver_src\include\Other"

function Get-DecompiledBody($className, $methodName) {
    $pattern = "__cdecl " + [regex]::Escape($className) + "::" + [regex]::Escape($methodName) + "\("
    $match = Select-String -LiteralPath $gsPath -Pattern $pattern -SimpleMatch:$false
    if (-not $match) { return $null }
    
    $startIdx = $match[0].LineNumber  # 1-indexed
    $allLines = Get-Content $gsPath -TotalCount ($startIdx + 200) | Select-Object -Skip ($startIdx)
    
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

function Convert-ToCpp {
    param([string]$body, [string]$retType = "void")
    if ([string]::IsNullOrWhiteSpace($body)) {
        if ($retType -ne 'void') { return "    return $($retType)();" }
        return ""
    }
    
    $code = $body
    
    # Remove vtable assignments
    $code = $code -replace "this->_vptr_\w+\s*=\s*\([^)]*\)[^;]*;", ""
    
    # Remove IDA var declarations (Type varname; // comment)
    $code = $code -replace '(?m)^\s*\w+(?:<[^>]*>)?\s+\*?\w+(?:\[.*?\])?\s*;\s*(?://.*)?$', ''
    $code = $code -replace '(?m)^\s*_\w+\s+\w+(?:\[.*?\])?\s*;\s*(?://.*)?$', ''
    
    # Remove "thisa = this;" and "Xa = X;" patterns  
    $code = $code -replace '(?m)^\s*(?:\w+\s+\**)?\w+a\s*=\s*(?:this|\w+);\s*$', ''
    
    # this-> -> remove
    $code = $code -replace '(?<!// )this->', ''
    
    # Remove register/BYREF comments
    $code = $code -replace '\s*// \w+$', ''
    $code = $code -replace '\s*// \[[^\]]*\] BYREF', ''
    
    # std::container::method(&var, ...) -> var.method(...)
    $patterns = @(
        @{regex='std::map<[^>]*>::([a-z_]+)\(&(\w+),\s*'; replacement='$2.$1('}
        @{regex='std::map<[^>]*>::([a-z_]+)\(&(\w+)\)'; replacement='$2.$1()'}
        @{regex='std::list<[^>]*>::([a-z_]+)\(&(\w+),\s*'; replacement='$2.$1('}
        @{regex='std::list<[^>]*>::([a-z_]+)\(&(\w+)\)'; replacement='$2.$1()'}
        @{regex='std::vector<[^>]*>::([a-z_]+)\(&(\w+),\s*'; replacement='$2.$1('}
        @{regex='std::vector<[^>]*>::([a-z_]+)\(&(\w+)\)'; replacement='$2.$1()'}
        @{regex='std::string::([a-z_]+)\(&(\w+),\s*'; replacement='$2.$1('}
        @{regex='std::string::([a-z_]+)\(&(\w+)\)'; replacement='$2.$1()'}
    )
    # Use single-line mode for multi-line calls
    $code = $code -replace '(?s)std::map<[^>]*>::operator\[\]\(&(\w+),\s*&(\w+)\)', '$1[$2]'
    
    # Fix map::find(&map, &key) -> map.find(key)
    $code = $code -replace '(?s)std::map<[^>]*>::find\(&(\w+),\s*&(\w+)\)', '$1.find($2)'
    
    # Fix map::lower_bound
    $code = $code -replace '(?s)std::map<[^>]*>::lower_bound\(&(\w+),\s*&(\w+)\)', '$1.lower_bound($2)'
    
    # Fix map::count  
    $code = $code -replace '(?s)std::map<[^>]*>::count\(&(\w+),\s*&(\w+)\)', '$1.count($2)'
    
    # Fix container::operator=
    $code = $code -replace '(?s)std::map<[^>]*>::operator=\(&(\w+),\s*&(\w+)\)', '$1 = $2'
    $code = $code -replace '(?s)std::string::operator=\(&(\w+),\s*&(\w+)\)', '$1 = $2'
    
    # Fix Answer::NetPacket
    $code = $code -replace 'Answer::NetPacket::writeInt32\((\w+),\s*', '$1->writeInt32('
    $code = $code -replace 'Answer::NetPacket::writeInt64\((\w+),\s*', '$1->writeInt64('
    $code = $code -replace 'Answer::NetPacket::writeUInt8\((\w+),\s*', '$1->writeUInt8('
    $code = $code -replace 'Answer::NetPacket::writeInt8\((\w+),\s*', '$1->writeInt8('
    $code = $code -replace 'Answer::NetPacket::writeUTF8\((\w+),\s*', '$1->writeUTF8('
    $code = $code -replace 'Answer::NetPacket::writeBool\((\w+),\s*', '$1->writeBool('
    $code = $code -replace 'Answer::NetPacket::writeFloat\((\w+),\s*', '$1->writeFloat('
    $code = $code -replace 'Answer::NetPacket::writeDouble\((\w+),\s*', '$1->writeDouble('
    $code = $code -replace 'Answer::NetPacket::readInt32\((\w+)\)', '$1->readInt32()'
    $code = $code -replace 'Answer::NetPacket::readInt64\((\w+)\)', '$1->readInt64()'
    $code = $code -replace 'Answer::NetPacket::readUInt8\((\w+)\)', '$1->readUInt8()'
    $code = $code -replace 'Answer::NetPacket::readInt8\((\w+)\)', '$1->readInt8()'
    $code = $code -replace 'Answer::NetPacket::readUTF8\((\w+)\)', '$1->readUTF8()'
    $code = $code -replace 'Answer::NetPacket::readBool\((\w+)\)', '$1->readBool()'
    $code = $code -replace 'Answer::NetPacket::readFloat\((\w+)\)', '$1->readFloat()'
    $code = $code -replace 'Answer::NetPacket::readDouble\((\w+)\)', '$1->readDouble()'
    
    # Fix "operator delete(this)"
    $code = $code -replace 'operator delete\([^)]*\)', ''
    
    # Fix "this->ClassName::~ClassName(this)" pattern
    $code = $code -replace '[^;]*::~[^(]+\(this\)', ''
    
    # Simplify std::make_pair
    $code = $code -replace 'std::make_pair<int,int>\(', 'std::make_pair('
    
    # Remove empty lines
    $code = $code -replace '(?m)^\s*\r?\n', ''
    
    # Indent
    $code = $code -replace '(?m)^', '    '
    $code = $code.TrimStart()
    
    return $code
}

function Get-HeaderType($gsType) {
    $gsType = $gsType.Trim()
    # Map types  
    if ($gsType -match 'std::map<') {
        $inner = $gsType -replace '^std::map<([^>]+)>$', '$1'
        $parts = $inner -split ','  # May contain nested commas
        # Simple case: map<int,X>
        $keyType = "int32_t"
        $valType = "int32_t"
        if ($parts.Count -ge 2) {
            $keyType = $parts[0].Trim()
            $valType = $parts[1].Trim()
        }
        return "std::map<$keyType, $valType>"
    }
    if ($gsType -match 'std::list<') {
        return $gsType
    }
    if ($gsType -match 'std::string') { return "std::string" }
    if ($gsType -match 'std::vector<') { return $gsType }
    return $gsType
}

function Fix-HeaderMembers {
    param([string]$className)
    $hdrPath = "$incDir\$className.h"
    if (-not (Test-Path $hdrPath)) { return }
    
    # Search gameserver.cc for member variable types in constructor
    $constructor = "__cdecl $className::$className("
    $match = Select-String -LiteralPath $gsPath -Pattern $constructor -SimpleMatch
    if (-not $match) { return }
    
    $startIdx = $match[0].LineNumber
    $body = Get-DecompiledBody $className $className
    if (-not $body) { return }
    
    # Find member types from vtable assignment and member inits
    $memberTypes = @{}
    $body -split "`r?`n" | ForEach-Object {
        # Match: this->memberName = initial_value;
        if ($_ -match 'this->(\w+)\s*=\s*(\([^)]*\))?\s*(\w[\w<>]*)') {
            $memberTypes[$matches[1]] = $matches[3]
        }
        # Match: this->memberName = ACTIVITY_STATE::...
        if ($_ -match 'this->(\w+)\s*=\s*\w+::') {
            if (-not $memberTypes.ContainsKey($matches[1])) {
                $memberTypes[$matches[1]] = "int32_t"
            }
        }
    }
    
    # Read current header
    $hdr = Get-Content $hdrPath -Raw
    
    # Replace TODO members with real types (can't fully automate this)
    # Just fix obvious patterns
    
    Set-Content $hdrPath $hdr
}

function Process-File {
    param([string]$className)
    $cppPath = "$srcDir\$className.cpp"
    if (-not (Test-Path $cppPath)) { return $false }
    
    $cpp = Get-Content $cppPath -Raw
    if ($cpp -notmatch 'TODO') { return $false }  # Skip clean files
    
    # Extract methods from cpp by matching signatures
    $methods = @()
    foreach ($line in (Get-Content $cppPath)) {
        if ($line -match "^\w+(?:\s+\w+)*\s+$className::(\w+)\s*\(") {
            $mname = $matches[1]
            $retType = ($line -replace "^\s*(\w+(?:\s+\w+)*)\s+$className.*", '$1').Trim()
            if ($mname -ne $className -and $mname -ne "~$className") {
                $methods += @{name=$mname; ret=$retType}
            }
        }
    }
    
    Write-Output ("  Methods found: " + $methods.Count)
    foreach ($m in $methods) {
        $body = Get-DecompiledBody $className $m.name
        if ($body) {
            $cppBody = Convert-ToCpp $body $m.ret
            Write-Output ("    " + $m.name + " -> converted (" + $cppBody.Length + " chars)")
        } else {
            Write-Output ("    " + $m.name + " -> NOT FOUND in gameserver.cc")
        }
    }
    
    return $true
}

# ---- MAIN ----
$files = @(
    "CLibraryTable"
)

foreach ($f in $files) {
    Write-Output ("Processing $f...")
    Process-File $f
}

Write-Output "Done"
