# 遍历所有 src/Other/ 下 .cpp 文件，将注释中的反汇编代码翻译成 C++ 实现
# 对没有反汇编代码的 TODO: impl 文件，至少加上 return 语句使其可编译

$files = Get-ChildItem -Filter "*.cpp" -Path "D:\444\gameserver_src\src\Other" | Where-Object {
    $c = Get-Content $_.FullName -Raw
    $c -match 'TODO'
}

$total = $files.Count
$converted = 0
foreach ($f in $files) {
    $cn = $f.BaseName
    $cpp = Get-Content $f.FullName -Raw
    
    # Step 1: Fix constructor initializer list: 
    #   ClassName::ClassName(params)
    #   {
    #       : BaseClass(params)
    #       {
    #         ...
    #       }
    #   }
    #   ->
    #   ClassName::ClassName(params)
    #       : BaseClass(params)
    #   {
    #       ...
    #   }
    $cpp = $cpp -replace "(?s)($cn::$cn\([^)]*\)\s*)\r?\n\{\r?\n\s*:\s*(\w+)\(([^)]*)\)\r?\n\{", "`$1`r`n    : `$2(`$3)`r`n{"
    
    # Step 2: Remove duplicate destructors
    # Find all destructors, keep only the last one (or first)
    $dtorPattern = "(?s)$cn::~$cn\(\)\r?\n\{.*?\r?\n\}"
    $dtors = [regex]::Matches($cpp, $dtorPattern)
    if ($dtors.Count -gt 1) {
        # Keep first, remove rest
        for ($i = 1; $i -lt $dtors.Count; $i++) {
            $cpp = $cpp.Remove($dtors[$i].Index, $dtors[$i].Length)
            # Update indices for subsequent removals
            $dtors = [regex]::Matches($cpp, $dtorPattern)
        }
    }
    
    # Step 3: For each method, extract decompiled comment and convert
    # Find all method bodies that have decompiled comments
    
    # Pattern: match the method body from opening brace to closing brace
    # where the body starts with TODO comments containing decompiled code
    $cpp = Process-MethodBody $cn $cpp
    
    Set-Content $f.FullName $cpp
    $converted++
    Write-Output $cn
}

Write-Output ("Processed $converted/$total files")
