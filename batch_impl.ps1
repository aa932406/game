# batch_impl.ps1 - Batch implement from gameserver.cc
# Usage: powershell -ExecutionPolicy Bypass -File batch_impl.ps1

$gsPath = "D:\444\gameserver_src\gameserver.cc"

# Read all lines from gameserver.cc into memory (takes ~2 seconds)
Write-Output "Loading gameserver.cc..."
$gsLines = Get-Content $gsPath
Write-Output ("Loaded " + $gsLines.Count + " lines")

# Build a simple line index: for each line, store (lineNumber, text)
# We'll search by regex later

function Find-MethodDef {
    param([string]$className, [string]$methodName)
    $pattern = "__cdecl " + [regex]::Escape($className) + "::" + [regex]::Escape($methodName) + "\("
    $matching = Select-String -LiteralPath $gsPath -Pattern $pattern
    if ($matching) {
        return $matching[0].LineNumber - 1  # 0-indexed
    }
    return -1
}

function Extract-Body {
    param([int]$startIdx)
    $body = @()
    $braceDepth = 0
    $inBody = $false
    for ($i = $startIdx + 1; $i -lt $gsLines.Count - 1; $i++) {
        $line = $gsLines[$i]
        # Skip the opening brace line itself
        if (-not $inBody) {
            if ($line -match '^\s*\{') { $inBody = $true }
            continue
        }
        # Stop at next function or separator
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
    param([string]$decompBody)
    $code = $decompBody
    
    # Remove vtable assignments
    $code = $code -replace "this->_vptr_\w+\s*=\s*\([^)]*\).*?;", ""
    
    # Remove IDA temp variable declarations on their own line
    $code = $code -replace '(?m)^\s*\w+\s+\w+;\s*//.*$', ''
    $code = $code -replace '(?m)^\s*\w+\s+\w+;\s*$', ''
    $code = $code -replace '(?m)^\s*\w+<[^>]*>\s+\w+;\s*//.*$', ''
    $code = $code -replace '(?m)^\s*_\w+\s+\w+\[.*?\];\s*$', ''
    $code = $code -replace '(?m)^\s*__int64\s+\w+;\s*$', ''
    
    # Remove "thisa = this;" patterns
    $code = $code -replace '(?m)^\s*\w+\s*=\s*this;\s*$', ''
    
    # Remove copy-to-local patterns like "stua = stu;"
    # Only when the local is clearly a temp copy
    $code = $code -replace '(?m)^\s*\w+a\s*=\s*\w+;\s*$', ''
    
    # this->x -> x
    $code = $code -replace 'this->', ''
    
    # Remove register comments but preserve code
    $code = $code -replace '\s*// \w+$', ''
    $code = $code -replace '\s*// \[.*?\] BYREF', ''
    
    # Fix MethodName(&this->x, ...) -> x.MethodName(...) for known containers
    # std::map::operator[](&map, &key) -> map[key]
    $code = $code -replace 'std::map<[^>]*>::operator\[\]\(&(\w+),\s*&(\w+)\)', '$1[$2]'
    
    # std::map::begin(&map) -> map.begin()
    $code = $code -replace 'std::map<[^>]*>::begin\(&(\w+)\)', '$1.begin()'
    $code = $code -replace 'std::map<[^>]*>::end\(&(\w+)\)', '$1.end()'
    $code = $code -replace 'std::map<[^>]*>::find\(&(\w+),\s*&(\w+)\)', '$1.find($2)'
    $code = $code -replace 'std::map<[^>]*>::clear\(&(\w+)\)', '$1.clear()'
    $code = $code -replace 'std::map<[^>]*>::insert\(&(\w+),\s*', '$1.insert('
    $code = $code -replace 'std::map<[^>]*>::empty\(&(\w+)\)', '$1.empty()'
    $code = $code -replace 'std::map<[^>]*>::size\(&(\w+)\)', '$1.size()'
    $code = $code -replace 'std::map<[^>]*>::count\(&(\w+),\s*&(\w+)\)', '$1.count($2)'
    $code = $code -replace 'std::map<[^>]*>::lower_bound\(&(\w+),\s*&(\w+)\)', '$1.lower_bound($2)'
    
    # std::list operations
    $code = $code -replace 'std::list<[^>]*>::push_back\(&(\w+),\s*', '$1.push_back('
    $code = $code -replace 'std::list<[^>]*>::push_front\(&(\w+),\s*', '$1.push_front('
    $code = $code -replace 'std::list<[^>]*>::pop_back\(&(\w+)\)', '$1.pop_back()'
    $code = $code -replace 'std::list<[^>]*>::pop_front\(&(\w+)\)', '$1.pop_front()'
    $code = $code -replace 'std::list<[^>]*>::begin\(&(\w+)\)', '$1.begin()'
    $code = $code -replace 'std::list<[^>]*>::end\(&(\w+)\)', '$1.end()'
    $code = $code -replace 'std::list<[^>]*>::clear\(&(\w+)\)', '$1.clear()'
    $code = $code -replace 'std::list<[^>]*>::empty\(&(\w+)\)', '$1.empty()'
    $code = $code -replace 'std::list<[^>]*>::remove\(&(\w+),\s*', '$1.remove('
    $code = $code -replace 'std::list<[^>]*>::erase\(&(\w+),\s*', '$1.erase('
    $code = $code -replace 'std::list<[^>]*>::size\(&(\w+)\)', '$1.size()'
    $code = $code -replace 'std::list<[^>]*>::sort\(&(\w+)\)', '$1.sort()'
    
    # std::vector operations
    $code = $code -replace 'std::vector<[^>]*>::push_back\(&(\w+),\s*', '$1.push_back('
    $code = $code -replace 'std::vector<[^>]*>::clear\(&(\w+)\)', '$1.clear()'
    $code = $code -replace 'std::vector<[^>]*>::size\(&(\w+)\)', '$1.size()'
    $code = $code -replace 'std::vector<[^>]*>::begin\(&(\w+)\)', '$1.begin()'
    $code = $code -replace 'std::vector<[^>]*>::end\(&(\w+)\)', '$1.end()'
    $code = $code -replace 'std::vector<[^>]*>::empty\(&(\w+)\)', '$1.empty()'
    $code = $code -replace 'std::vector<[^>]*>::reserve\(&(\w+),\s*', '$1.reserve('
    $code = $code -replace 'std::vector<[^>]*>::resize\(&(\w+),\s*', '$1.resize('
    
    # std::string operations
    $code = $code -replace 'std::string::operator=\(&(\w+),\s*([^)]+)\)', '$1 = $2'
    $code = $code -replace 'std::string::clear\(&(\w+)\)', '$1.clear()'
    $code = $code -replace 'std::string::empty\(&(\w+)\)', '$1.empty()'
    $code = $code -replace 'std::string::size\(&(\w+)\)', '$1.size()'
    $code = $code -replace 'std::string::append\(&(\w+),\s*', '$1.append('
    $code = $code -replace 'std::string::c_str\(&(\w+)\)', '$1.c_str()'
    $code = $code -replace 'std::string::assign\(&(\w+),\s*', '$1.assign('
    
    # Answer::NetPacket write
    $code = $code -replace 'Answer::NetPacket::writeInt32\((\w+),\s*', '$1->writeInt32('
    $code = $code -replace 'Answer::NetPacket::writeInt64\((\w+),\s*', '$1->writeInt64('
    $code = $code -replace 'Answer::NetPacket::writeInt8\((\w+),\s*', '$1->writeInt8('
    $code = $code -replace 'Answer::NetPacket::writeUInt8\((\w+),\s*', '$1->writeUInt8('
    $code = $code -replace 'Answer::NetPacket::writeUTF8\((\w+),\s*', '$1->writeUTF8('
    $code = $code -replace 'Answer::NetPacket::writeBool\((\w+),\s*', '$1->writeBool('
    $code = $code -replace 'Answer::NetPacket::writeFloat\((\w+),\s*', '$1->writeFloat('
    $code = $code -replace 'Answer::NetPacket::writeDouble\((\w+),\s*', '$1->writeDouble('
    
    # Answer::NetPacket read
    $code = $code -replace 'Answer::NetPacket::readInt32\((\w+)\)', '$1->readInt32()'
    $code = $code -replace 'Answer::NetPacket::readInt64\((\w+)\)', '$1->readInt64()'
    $code = $code -replace 'Answer::NetPacket::readInt8\((\w+)\)', '$1->readInt8()'
    $code = $code -replace 'Answer::NetPacket::readUInt8\((\w+)\)', '$1->readUInt8()'
    $code = $code -replace 'Answer::NetPacket::readUTF8\((\w+)\)', '$1->readUTF8()'
    $code = $code -replace 'Answer::NetPacket::readBool\((\w+)\)', '$1->readBool()'
    $code = $code -replace 'Answer::NetPacket::readFloat\((\w+)\)', '$1->readFloat()'
    $code = $code -replace 'Answer::NetPacket::readDouble\((\w+)\)', '$1->readDouble()'
    
    # X::operator=(a, b) -> *a = b (or a = b)
    $code = $code -replace '(?m)^\s*\w+\s*\*\s*\w+;\s*$', ''
    $code = $code -replace '(?m)^\s*\w+\s*\*\s*\w+;\s*//.*$', ''
    $code = $code -replace '\s*\*\w+\(void \*\*\)\([^)]*\)\)', ''
    $code = $code -replace '\(\(void \(__fastcall \*\*\)\([^)]*\)\)', ''
    
    # Remove generic container constructor/destructor calls (auto-managed)
    $code = $code -replace '(?m)^\s*std::map<[^>]*>::map\(&this->\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::map<[^>]*>::~map\(&this->\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::list<[^>]*>::list\(&this->\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::list<[^>]*>::~list\(&this->\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::string::string\(&this->\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::string::~string\(&this->\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::vector<[^>]*>::vector\(&this->\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::vector<[^>]*>::~vector\(&this->\w+\);\s*$', ''
    
    # Remove ::map(&this->x) in constructor body context
    $code = $code -replace '(?m)^\s*std::map<[^>]*>::map\(&\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::list<[^>]*>::list\(&\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::string::string\(&\w+\);\s*$', ''
    $code = $code -replace '(?m)^\s*std::vector<[^>]*>::vector\(&\w+\);\s*$', ''
    
    # Fix operator new calls for deletion
    $code = $code -replace 'operator delete\(this\)', ''
    $code = $code -replace 'Class\d+::~Class\d+\(this\)', ''
    
    # Fix simple string literals
    $code = $code -replace 'byte_\w+', '""'
    
    # Remove empty lines
    $code = $code -replace '(?m)^\s*\r?\n', ''
    
    # Fix indentation (4 spaces)
    $code = $code -replace '(?m)^', '    '
    $code = $code.TrimStart()
    
    return $code
}

# Test with CLibraryTable
$class = "CLibraryTable"
$methods = @(
    @{name="CLibraryTable"; type="ctor"},
    @{name="~CLibraryTable"; type="dtor"},
    @{name="AddCfgLibraryItem"; type="method"},
    @{name="AddCfgLibraryQuality"; type="method"},
    @{name="AddCfgLibraryCost"; type="method"},
    @{name="GetCfgLibraryQuality"; type="method"},
    @{name="GetCfgLibraryItem"; type="method"}
)

foreach ($m in $methods) {
    $methodName = $m.name
    $idx = Find-MethodDef $class $methodName
    if ($idx -ge 0) {
        Write-Output ("")
        Write-Output ("=== Found $class::$methodName at line $idx ===")
        $body = Extract-Body $idx
        $cpp = Convert-ToCpp $body
        Write-Output $cpp
    } else {
        Write-Output ("NOT FOUND: $class::$methodName")
    }
}
