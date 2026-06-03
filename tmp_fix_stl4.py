#!/usr/bin/env python3
"""
Fourth pass: fix STL patterns with >> nested template closing brackets
that the third-pass regex missed due to expecting single > before ::.

Fix: change >:: to >>?:: in all patterns to handle both:
  std::map<long,Player *>::begin            (single >)
  std::_Rb_tree_iterator<std::pair<...>>::  (double >>)
"""

import re

filepath = 'src/Game/GameService.cpp'

with open(filepath, 'r') as f:
    content = f.read()

original = content

# The key fix: use >>>?:: or >>?:: where we expected >::
# Pattern: TYPE_CLOSING + "::" where TYPE_CLOSING can be > or >>

# Helper: replace all >:: that follow a template type pattern with >>?::
# We'll just use >{1,2}:: in all template patterns

# ============================================================
# FIXED: Container static method calls - handle > and >> ending
# ============================================================

containers = ['std::map', 'std::list', 'std::vector']
methods_noarg = ['begin', 'end', 'empty', 'size', 'clear', 'pop_back', 'pop_front']
methods_arg = ['find', 'erase', 'push_back', 'push_front']

for container in containers:
    for method in methods_noarg:
        # Match: container<...>{1,2}::method(expr)
        pat = re.compile(
            re.escape(container) +
            # Match template params, then one or two >, then ::
            r'<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::' +
            re.escape(method) +
            r'\(([^)]*)\)'
        )
        
        def repl_noarg(m, meth=method):
            args = m.group(1).strip()
            if args.startswith('&'):
                obj = args[1:]
            else:
                obj = args
            return f"{obj}.{meth}()"
        
        content = pat.sub(repl_noarg, content)
    
    for method in methods_arg:
        pat = re.compile(
            re.escape(container) +
            r'<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::' +
            re.escape(method) +
            r'\(([^,]+),\s*([^)]+)\)'
        )
        
        def repl_arg(m, meth=method):
            arg1 = m.group(1).strip()
            arg2 = m.group(2).strip()
            if arg1.startswith('&'):
                obj = arg1[1:]
            else:
                obj = arg1
            if arg2.startswith('&'):
                val = arg2[1:]
            else:
                val = arg2
            return f"{obj}.{meth}({val})"
        
        content = pat.sub(repl_arg, content)
    
    # operator[]
    pat = re.compile(
        re.escape(container) +
        r'<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::' +
        r'operator\[\]' +
        r'\(([^,]+),\s*([^)]+)\)'
    )
    
    def repl_op(m):
        arg1 = m.group(1).strip()
        arg2 = m.group(2).strip()
        if arg1.startswith('&'):
            obj = arg1[1:]
        else:
            obj = arg1
        if arg2.startswith('&'):
            key = arg2[1:]
        else:
            key = arg2
        return f"{obj}[{key}]"
    
    content = pat.sub(repl_op, content)

# ============================================================
# FIXED: Iterator static method calls - handle >> properly
# ============================================================

# Template pattern for _Rb_tree_iterator with >> support
# Change >:: to >{1,2}:: in iterator patterns too

# _Rb_tree_iterator::operator++(&it, 0)
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::operator\+\+\(&\s*(\w+)\s*,\s*0\s*\)',
    r'++\1',
    content
)

# _List_iterator / _List_const_iterator::operator++(&it)
content = re.sub(
    r'std::_List(?:_const)?_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::operator\+\+\(&\s*(\w+)\s*(?:,\s*0\s*)?\)',
    r'++\1',
    content
)

# _Rb_tree_const_iterator::operator++(&it)
content = re.sub(
    r'std::_Rb_tree_const_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::operator\+\+\(&\s*(\w+)\s*\)',
    r'++\1',
    content
)

# _Rb_tree_iterator::operator!=(&it, &end)
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::operator!=\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 != \2',
    content
)

# _Rb_tree_const_iterator::operator!=(&it, &end)
content = re.sub(
    r'std::_Rb_tree_const_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::operator!=\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 != \2',
    content
)

# _List_const_iterator / _List_iterator::operator!=(&it, &end)
content = re.sub(
    r'std::_List(?:_const)?_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::operator!=\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 != \2',
    content
)

# _Rb_tree_iterator::operator->(&it)
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::operator->\(&\s*(\w+)\s*\)',
    r'\1',
    content
)

# _Rb_tree_const_iterator::operator->(&it)
content = re.sub(
    r'std::_Rb_tree_const_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::operator->\(&\s*(\w+)\s*\)',
    r'\1',
    content
)

# _List_iterator / _List_const_iterator::operator*(&it)
content = re.sub(
    r'std::_List(?:_const)?_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::operator\*\(&\s*(\w+)\s*\)',
    r'*\1',
    content
)

# _Rb_tree_iterator::operator==(&it, &end)
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::operator==\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 == \2',
    content
)

# _Rb_tree_const_iterator::_Rb_tree_const_iterator(&dest, &src) - manual copy construction
content = re.sub(
    r'std::_Rb_tree_const_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::_Rb_tree_const_iterator\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 = \2',
    content
)

# _List_const_iterator::_List_const_iterator(&dest, &src)
content = re.sub(
    r'std::_List_const_iterator<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::_List_const_iterator\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 = \2',
    content
)

# map<K,V>::const_iterator::operator!=(&it, &end)
content = re.sub(
    r'std::map<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::const_iterator::operator!=\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 != \2',
    content
)

# map<K,V>::const_iterator::operator++(&it)
content = re.sub(
    r'std::map<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::const_iterator::operator\+\+\(&\s*(\w+)\s*\)',
    r'++\1',
    content
)

# map<K,V>::const_iterator::operator->(&it)
content = re.sub(
    r'std::map<[^>]*(?:<[^>]*>[^>]*)?>{1,2}::const_iterator::operator->\(&\s*(\w+)\s*\)',
    r'\1',
    content
)

# ============================================================
# Fix: !iter != __x → iter == __x  (decompiler artifact)
# ============================================================
content = re.sub(
    r'!\s*(\w+)\s*!=\s*(\w+)',
    r'\1 == \2',
    content
)

# Write result
with open(filepath, 'w') as f:
    f.write(content)

# Count changes
changes = 0
for i, (old, new) in enumerate(zip(original.split('\n'), content.split('\n'))):
    if old != new:
        changes += 1

print(f"Applied transformations to {changes} lines")
print(f"Key fix: changed >:: to >{{1,2}}:: in all template-end patterns to handle >> nested closing brackets")
