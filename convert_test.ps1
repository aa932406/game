function Convert-DecompiledCode {
    param([string]$code)
    # Remove // prefix from each line
    $code = $code -replace '(?m)^// ', ''
    $code = $code -replace '(?m)^//', ''
    # Remove IDA function signature comment (void __cdecl ...)
    $code = $code -replace '(?m)^\w+ __cdecl \w+::\w+\(.*\)\s*$', ''
    # Remove IDA register comments (// rax, // rbx, etc.)
    $code = $code -replace '\s*// \w+$', ''
    $code = $code -replace '\s*// rax$', ''
    # Remove BYREF comments
    $code = $code -replace '\s*// \[.*?\] BYREF', ''
    # Remove IDA variable declarations on separate lines
    $code = $code -replace '(?m)^\s*\w+\s+\w+;\s*$', ''
    $code = $code -replace '(?m)^\s*\w+<[^>]*>\s+\w+;\s*$', ''
    $code = $code -replace '(?m)^\s*\w+\s+\w+;\s*//.*$', ''
    # Remove empty lines from removed declarations
    $code = $code -replace '(?m)^\s*\r?\n', ''
    # Remove IDA vtable address casts: ((void (__fastcall *)(...))...)
    $code = $code -replace '\(\*\(void \(\*\*\)\([^)]*\)\)\(', ''
    $code = $code -replace '\(\*\*\(void \(__fastcall \*\*\)\([^)]*\)\)\*\*\(', ''
    $code = $code -replace 'void \(__fastcall \*\*\)\([^)]*\)\)\s*', ''
    return $code
}

function Impl-Constructor {
    param([string]$cn, [string]$code)
    # Remove map constructor calls (automatic in C++)
    $code = $code -replace '(?m)^\s*std::map<[^>]*>::map\(&this->\w+\);\s*$', ''
    # Remove string constructor calls
    $code = $code -replace '(?m)^\s*std::string::string\(&this->\w+\);\s*$', ''
    # Remove list constructor calls
    $code = $code -replace '(?m)^\s*std::list<[^>]*>::list\(&this->\w+\);\s*$', ''
    # Remove vector constructor calls
    $code = $code -replace '(?m)^\s*std::vector<[^>]*>::vector\(&this->\w+\);\s*$', ''
    # Fix remaining this->member
    $code = $code -replace 'this->', ''
    # Convert CleanUp(this) to CleanUp()
    $code = $code -replace '\bCleanUp\(this\)', 'CleanUp()'
    # Convert base class calls 
    $code = $code -replace '\bplatform_log_base::CleanUp\(this\)', 'platform_log_base::CleanUp()'
    # Clean up empty lines
    $code = $code -replace '(?m)^\s*\r?\n', ''
    return $code
}

function Impl-Destructor {
    param([string]$cn, [string]$code)
    $code = $code -replace '(?m)^\s*std::map<[^>]*>::~map\(&this->\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::string::~string\(&this->\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::list<[^>]*>::~list\(&this->\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*$cn::~$cn\([^)]*\);\s*$', ''
    $code = $code -replace '(?m)^\s*operator delete\([^)]*\);\s*$', ''
    $code = $code -replace '(?m)^\s*\r?\n', ''
    return $code
}

function Impl-Method {
    param([string]$cn, [string]$name, [string]$code, [string]$retType)
    
    # Remove IDA function header
    $code = $code -replace '(?m)^\w+ __cdecl \w+::\w+\(.*\)\s*$', ''
    
    # Fix this->member -> member
    $code = $code -replace 'this->', ''
    
    # Fix Answer::NetPacket::writeFoo(packet, ...) -> packet->writeFoo(...)
    $code = $code -replace 'Answer::NetPacket::writeInt32\((\w+),\s*', '$1->writeInt32('
    $code = $code -replace 'Answer::NetPacket::writeInt64\((\w+),\s*', '$1->writeInt64('
    $code = $code -replace 'Answer::NetPacket::writeUInt8\((\w+),\s*', '$1->writeUInt8('
    $code = $code -replace 'Answer::NetPacket::writeInt8\((\w+),\s*', '$1->writeInt8('
    $code = $code -replace 'Answer::NetPacket::writeUTF8\((\w+),\s*', '$1->writeUTF8('
    $code = $code -replace 'Answer::NetPacket::writeBool\((\w+),\s*', '$1->writeBool('
    $code = $code -replace 'Answer::NetPacket::writeFloat\((\w+),\s*', '$1->writeFloat('
    $code = $code -replace 'Answer::NetPacket::writeDouble\((\w+),\s*', '$1->writeDouble('
    
    # Fix Answer::NetPacket::readFoo(inPacket) -> inPacket->readFoo()
    $code = $code -replace 'Answer::NetPacket::readInt32\((\w+)\)', '$1->readInt32()'
    $code = $code -replace 'Answer::NetPacket::readInt64\((\w+)\)', '$1->readInt64()'
    $code = $code -replace 'Answer::NetPacket::readUInt8\((\w+)\)', '$1->readUInt8()'
    $code = $code -replace 'Answer::NetPacket::readInt8\((\w+)\)', '$1->readInt8()'
    $code = $code -replace 'Answer::NetPacket::readUTF8\((\w+),\s*(\w+)\)', '$1->readUTF8($2)'
    $code = $code -replace 'Answer::NetPacket::readBool\((\w+)\)', '$1->readBool()'
    $code = $code -replace 'Answer::NetPacket::readFloat\((\w+)\)', '$1->readFloat()'
    $code = $code -replace 'Answer::NetPacket::readDouble\((\w+)\)', '$1->readDouble()'
    
    # Fix std::map::operator[](&map, &key) -> map[key]
    $code = $code -replace 'std::map<[^>]*>::operator\[\]\(&this->(\w+),\s*&(\w+)\)', '$1[$2]'
    $code = $code -replace 'std::map<[^>]*>::operator\[\]\(&(\w+),\s*&(\w+)\)', '$1[$2]'
    
    # Fix X::operator=(&target, &source) -> target = source
    $code = $code -replace '\w+::operator=\(&(\w+),\s*&(\w+)\)', '$1 = $2'
    $code = $code -replace '\w+::operator=\(&(\w+),\s*(\w+)\)', '$1 = $2'
    $code = $code -replace '\w+::operator=\((\w+),\s*(\w+)\)', '$1 = $2'
    
    # Fix std::string::operator=
    $code = $code -replace 'std::string::operator=\(&(\w+),\s*([^)]+)\)', '$1 = $2'
    
    # Fix std::string::string(...) - remove (already constructed)
    $code = $code -replace '(?m)^\s*std::string::string\(&this->\w+\);', ''
    $code = $code -replace '(?m)^\s*std::string::string\(&(\w+),\s*([^)]+)\);', '$1 = $2;'
    
    # Remove register variable declarations
    $code = $code -replace '(?m)^\s*\w+\s+\w+;\s*//.*$', ''
    $code = $code -replace '(?m)^\s*\w+\s+\w+;\s*$', ''
    $code = $code -replace '(?m)^\s*\w+<[^>]*>\s+\w+;\s*$', ''
    $code = $code -replace '(?m)^\s*_\w+\s+\w+\[.*?\];\s*$', ''
    
    # Remove register comments
    $code = $code -replace '\s*// \w+$', ''
    
    # Remove BYREF annotations
    $code = $code -replace '\s*// \[.*\] BYREF', ''
    
    # Fix local var references: "int32_t Levela; // [rsp+...]" pattern
    $code = $code -replace '(?m)^\s*int32_t \w+a;\s*$', ''
    $code = $code -replace '(?m)^\s*\w+ \*thisa;\s*$', ''
    
    # Fix std::pair<...>::... references
    $code = $code -replace 'std::pair<[^>]*>\s+\w+;', ''
    
    # Clean up empty lines
    $code = $code -replace '(?m)^\s*\r?\n', ''
    
    # If code is empty and method returns non-void, add default return
    $codeTrimmed = $code.Trim()
    if ([string]::IsNullOrEmpty($codeTrimmed)) {
        if ($retType -ne 'void' -and $retType -ne '') {
            $code = "    $($retType) result{};`r`n    return result;"
        }
    }
    
    return $code
}

# Test with BossDistribution
$cn = "BossDistribution"
$cppPath = "D:\444\gameserver_src\src\Other\$cn.cpp"
$lines = Get-Content $cppPath

# Parse into sections: constructor, destructor, methods
$sections = @()
$current = @{}
$inCommentBlock = $false
$commentLines = @()

foreach ($line in $lines) {
    if ($line -match '^\s*// TODO') {
        $inCommentBlock = $true
        $commentLines = @()
        continue
    }
    if ($line -match '^\s*// 反编译') {
        continue
    }
    if ($inCommentBlock) {
        if ($line -match '^\s*// ') {
            $commentLines += $line
        } else {
            # End of comment block, process it
            if ($commentLines.Count -gt 0) {
                $inCommentBlock = $false
            }
        }
    }
}

Write-Output "Comment blocks found: $($commentLines.Count)"
$commentLines | ForEach-Object { Write-Output $_ }
