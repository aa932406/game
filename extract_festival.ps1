$cc = Get-Content "D:\444\gameserver_src\gameserver.cc" -Raw -Encoding UTF8
$lines = $cc -split "`n"

# Find all CFestivalDoubleEleven method line numbers
$methodLines = @()
for ($i = 0; $i -lt $lines.Count; $i++) {
    if ($lines[$i] -match '__cdecl CFestivalDoubleEleven::') {
        $methodName = if ($lines[$i] -match '::(\w+)\(') { $matches[1] } else { 'unknown' }
        $methodLines += @{ Line = $i; Name = $methodName; Sig = $lines[$i] }
    }
}

Write-Output "Found $($methodLines.Count) methods"

# Extract each method body (brace matching)
$methods = @{}
for ($idx = 0; $idx -lt $methodLines.Count; $idx++) {
    $start = $methodLines[$idx].Line
    $name = $methodLines[$idx].Name
    $sig = $methodLines[$idx].Sig

    # Check if just a signature (no '{' on same line)
    $openCount = 0
    $depth = 0
    $bodyLines = @()

    for ($j = $start; $j -lt $lines.Count; $j++) {
        $line = $lines[$j]
        $bodyLines += $line
        
        $openCount += ($line.ToCharArray() | ? { $_ -eq '{' }).Count
        $openCount -= ($line.ToCharArray() | ? { $_ -eq '}' }).Count
        
        if ($openCount -eq 0 -and $bodyLines.Count -gt 1) {
            break
        }
    }

    $methods[$name] = @{ Body = $bodyLines; Sig = $sig }
}

# Convert each method
$output = @'
#include "Other/CFestivalDoubleEleven.h"
#include <cstring>
#include <algorithm>
#include "Other/...various_includes_here"

'@

foreach ($method in $methods.GetEnumerator() | Sort-Object Name) {
    $name = $method.Key
    $body = $method.Value.Body

    $clean = ""
    $inBody = $false
    foreach ($line in $body) {
        # Start converting after the opening brace
        if ($line -match '^\s*\{') {
            $inBody = $true
            # Generate function header
            $clean += $method.Value.Sig -replace 'void __cdecl ', 'void ' -replace 'bool __cdecl ', 'bool ' -replace 'int32_t __cdecl ', 'int32_t ' -replace 'int8_t __cdecl ', 'int8_t ' -replace 'ShowIcon \*__cdecl ', 'ShowIcon* ' -replace '\(CFestivalDoubleEleven \*const this,?\s*', '(' -replace '\(CFestivalDoubleEleven \*const this\)', '()'
            continue
        }
        if (-not $inBody) { continue }

        # Conversion rules
        $l = $line
        # Remove this-> 
        $l = $l -replace 'this->', ''
        # Remove IDA variable comments
        $l = $l -replace '// rax|// rbx|// rcx|// rdx|// rsi|// rdi|// r8|// r9|// r10|// r11|// r12|// r13|// r14|// r15|// ebx|// ecx|// edx|// esi|// edi|// r8d|// r9d|// r10d|// r11d|// r12d|// r13d|// r14d|// r15d|// eax', ''
        # Remove BYREF
        $l = $l -replace '// BYREF', ''
        # Remove [rbp...] stack vars (whole arrays)
        $l = $l -replace '// \[rsp[\+\-][^\]]*\]', ''
        # Remove vtable
        $l = $l -replace '_vptr_\w+\s*=\s*[^;]+;', ''
        # Remove operator delete
        $l = $l -replace 'operator delete\([^)]+\);', ''
        # Singleton pattern
        $l = $l -replace "Answer::Singleton<([^>]+)>::instance\(\)", '$1::instance()'
        # NetPacket read/write
        $l = $l -replace 'Answer::NetPacket::writeInt8', 'WriteInt8'
        $l = $l -replace 'Answer::NetPacket::writeInt32', 'WriteInt32'
        $l = $l -replace 'Answer::NetPacket::readInt8', 'ReadInt8'
        $l = $l -replace 'Answer::NetPacket::readInt32', 'ReadInt32'
        # Remove container ctor/dtor calls
        $l = $l -replace 'std::vector<[^>]+>::~vector\(&[^)]+\);', ''
        $l = $l -replace 'std::map<[^>]+>::map\(&[^)]+\);', ''
        $l = $l -replace 'std::list<[^>]+>::list\(&[^)]+\);', ''
        $l = $l -replace 'std::string::string\(&[^)]+\);', ''
        # Clean up IDA iterator patterns
        $l = $l -replace 'std::_List_iterator<[^>]+>::operator\+\+\(&[^)]+\);', '++iter;'
        $l = $l -replace 'std::_List_iterator<[^>]+>::operator!=\\(&[^,]+, &[^)]+\\)', 'iter != end'
        $l = $l -replace '\*std::_List_iterator<[^>]+>::operator\*\(&[^)]+\)', '*iter'
        # Self-calls
        $l = $l -replace 'CFestivalDoubleEleven::(\w+)\(this,?\s*', '$1('

        $clean += $l + "`n"
    }
    $output += $clean + "`n"
}

Set-Content -Path "D:\444\gameserver_src\src\Other\CFestivalDoubleEleven_new.cpp" -Value $output -Encoding UTF8
Write-Output "Done. Written to CFestivalDoubleEleven_new.cpp"
Write-Output "Methods processed: $($methods.Count)"