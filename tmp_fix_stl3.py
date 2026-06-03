#!/usr/bin/env python3
"""
Third pass STL pattern fix script for GameService.cpp.
Handles remaining decompiler STL patterns that the first two scripts missed.

Pattern categories:
1. Container static calls: std::map/list::begin/end/find/empty/size/push_back/etc(&obj)
2. Iterator static calls: _Rb_tree_iterator/const_iterator::operator++/!=/->/== etc(&it)
3. Decompiler artifact patterns: !iter comparison, manual iterator construction
"""

import re

filepath = 'src/Game/GameService.cpp'

with open(filepath, 'r') as f:
    content = f.read()

original = content

# ============================================================
# PATTERN GROUP 1: Container static method calls
# std::map<K,V>::method(&obj, ...)  →  obj.method(...)
# std::list<T>::method(&obj, ...)   →  obj.method(...)
# ============================================================

# Match std::map<...>::method(&obj) or std::list<...>::method(&obj)
# The tricky part is matching the template parameters with possible nesting.
# Strategy: match std:: followed by container name, then <...> with balanced brackets,
# then ::method_name(&arg, ...)

def fix_container_call(m):
    prefix = m.group(1)  # e.g. "std::map<long,Player *>"
    method = m.group(2)  # e.g. "begin"
    # Get the first argument with optional &
    args_str = m.group(3)  # e.g. "&this->m_players" or "&v3" or "indexMap"
    
    # Remove leading & if present
    if args_str.startswith('&'):
        obj = args_str[1:]
    else:
        obj = args_str
    
    # For methods that take additional arguments (find, push_back, push_front, erase, operator[])
    # The full arg list might be like "&this->m_players, &key" or "&this->m_players, it"
    # Let's handle this by looking at the full match
    return f"{obj}.{method}()"

# More robust: handle container calls with or without extra args
# Pattern:  std::map<...>::method(&arg, ...rest)  or  std::map<...>::method(&arg)
# We need to handle nested <> in template params

def fix_balanced_container_calls(text):
    """Fix all std::map<...>::method(&obj, ...) and std::list<...>::method(&obj, ...) patterns."""
    
    # Pattern for container method calls - matches the container name and method,
    # then captures the arguments in parentheses
    # We match the template args conservatively - no nested <> for simplicity
    
    containers = ['std::map', 'std::list', 'std::vector']
    methods_1arg = ['begin', 'end', 'empty', 'size', 'clear', 'pop_back', 'pop_front']
    methods_2args = ['find', 'erase', 'push_back', 'push_front']
    methods_op = [r'operator\[\]']
    all_methods = methods_1arg + methods_2args + methods_op
    
    for container in containers:
        for method in methods_1arg:
            # Match: std::map<...>::method(&arg)  or  ::method(&arg) with template ending >>
            # Pattern: std::map<...>::method(  &arg  )
            # The template params: capture anything between < and > that doesn't contain <
            # But some have nested templates like std::list<short>
            # Use a pragmatic approach: match <...> up to >> ::
            
            # Pattern 1: simple template like <long,Player *>
            # Pattern 2: nested template like <int8_t,std::list<short>>
            
            # Try: match container<template_params>::method(expr)
            pat = re.compile(
                re.escape(container) + 
                r'<[^>]*(?:<[^>]*>[^>]*)?>::' + 
                re.escape(method) + 
                r'\(([^)]*)\)'
            )
            
            def repl_1arg(m):
                args = m.group(1).strip()
                # Remove leading &
                if args.startswith('&'):
                    obj = args[1:]
                else:
                    obj = args
                return f"{obj}.{method}()"
            
            text = pat.sub(repl_1arg, text)
        
        for method in methods_2args:
            # For 2-arg methods like find(&map, &key) or push_back(&list, &val)
            # Match: container<...>::method(arg1, arg2)
            pat = re.compile(
                re.escape(container) +
                r'<[^>]*(?:<[^>]*>[^>]*)?>::' +
                re.escape(method) +
                r'\(([^,]+),\s*([^)]+)\)'
            )
            
            def make_repl_2args(method_name):
                def repl(m):
                    arg1 = m.group(1).strip()
                    arg2 = m.group(2).strip()
                    if arg1.startswith('&'):
                        obj = arg1[1:]
                    else:
                        obj = arg1
                    # For find: map.find(key) - remove & from key
                    # For push_back: list.push_back(val) - remove & from val
                    if arg2.startswith('&'):
                        val = arg2[1:]
                    else:
                        val = arg2
                    return f"{obj}.{method_name}({val})"
                return repl
            
            text = pat.sub(make_repl_2args(method), text)
        
        # Special handling for operator[]
        # std::map<...>::operator[](&map, &key)
        pat = re.compile(
            re.escape(container) +
            r'<[^>]*(?:<[^>]*>[^>]*)?>::' +
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
        
        text = pat.sub(repl_op, text)
    
    return text

content = fix_balanced_container_calls(content)

# ============================================================
# PATTERN GROUP 2: Iterator static method calls  
# ============================================================

# Fix _Rb_tree_iterator::operator++(&it, 0)  →  ++it
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]*(?::<[^>]*>)?>::operator\+\+\(&\s*(\w+)\s*,\s*0\s*\)',
    r'++\1',
    content
)

# Fix _List_iterator::operator++(&it) or _List_const_iterator::operator++(&it)
content = re.sub(
    r'std::_List(?:_const)?_iterator<[^>]*(?::<[^>]*>)?>::operator\+\+\(&\s*(\w+)\s*(?:,\s*0\s*)?\)',
    r'++\1',
    content
)

# Fix _Rb_tree_const_iterator::operator++(&it)
content = re.sub(
    r'std::_Rb_tree_const_iterator<[^>]*(?::<[^>]*>)?>::operator\+\+\(&\s*(\w+)\s*\)',
    r'++\1',
    content
)

# Fix _Rb_tree_iterator::operator!=(&it, &end)  →  it != end
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]*(?::<[^>]*>)?>::operator!=\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 != \2',
    content
)

# Fix _Rb_tree_const_iterator::operator!=(&it, &end)
content = re.sub(
    r'std::_Rb_tree_const_iterator<[^>]*(?::<[^>]*>)?>::operator!=\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 != \2',
    content
)

# Fix _List_const_iterator::operator!=(&it, &end)
content = re.sub(
    r'std::_List(?:_const)?_iterator<[^>]*(?::<[^>]*>)?>::operator!=\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 != \2',
    content
)

# Fix _Rb_tree_iterator::operator->(&it)  →  it
# (used in patterns like: std::_Rb_tree_iterator<...>::operator->(&it)->second)
# Or: v3 = std::_Rb_tree_iterator<...>::operator->(&it);
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]*(?::<[^>]*>)?>::operator->\(&\s*(\w+)\s*\)',
    r'\1',
    content
)

# Fix _Rb_tree_const_iterator::operator->(&it)
content = re.sub(
    r'std::_Rb_tree_const_iterator<[^>]*(?::<[^>]*>)?>::operator->\(&\s*(\w+)\s*\)',
    r'\1',
    content
)

# Fix _List_iterator::operator*(&it)  →  *it
# Used in: v7 = std::_List_const_iterator<short>::operator*(&iter);
content = re.sub(
    r'std::_List(?:_const)?_iterator<[^>]*(?::<[^>]*>)?>::operator\*\(&\s*(\w+)\s*\)',
    r'*\1',
    content
)

# Fix _Rb_tree_iterator::operator==(&it, &end)  →  it == end
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]*(?::<[^>]*>)?>::operator==\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 == \2',
    content
)

# Fix manual iterator copy construction: std::_Rb_tree_const_iterator<...>::_Rb_tree_const_iterator(&dest, &src)
# → dest = src
content = re.sub(
    r'std::_Rb_tree_const_iterator<[^>]*(?::<[^>]*>)?>::_Rb_tree_const_iterator\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 = \2',
    content
)

# Fix manual _List_const_iterator copy construction
content = re.sub(
    r'std::_List_const_iterator<[^>]*(?::<[^>]*>)?>::_List_const_iterator\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 = \2',
    content
)

# Fix const_iterator operator!=(&it, &end) for the map<...>::const_iterator pattern
content = re.sub(
    r'std::map<[^>]*(?::<[^>]*>)?>::const_iterator::operator!=\(&\s*(\w+)\s*,\s*&\s*(\w+)\s*\)',
    r'\1 != \2',
    content
)

# Fix const_iterator::operator++(&it)
content = re.sub(
    r'std::map<[^>]*(?::<[^>]*>)?>::const_iterator::operator\+\+\(&\s*(\w+)\s*\)',
    r'++\1',
    content
)

# Fix const_iterator::operator->(&it)
content = re.sub(
    r'std::map<[^>]*(?::<[^>]*>)?>::const_iterator::operator->\(&\s*(\w+)\s*\)',
    r'\1',
    content
)

# Fix common pattern:  !iter != v28  →  iter != v28  (decompiler artifact, the ! was for something else)
# More carefully: fix !iterator_variable != something → iterator_variable != something
# Pattern: !std::_Rb_tree_iterator<...>::operator!=(...)  this is hard to match post-fix
# Let's handle:  !\w+\s*!=\s*\w+  — this is unlikely in valid C++

# Fix std::map<K,V>::const_iterator::operator++(&it)  →  ++it
# (already handled above)

# ============================================================
# PATTERN GROUP 3: Specific decompiler artifact patterns  
# ============================================================

# Fix:  Answer::NetPacket::readUTF8(&str, inPacket)  →  inPacket->readUTF8(str)
# Actually let me check the exact pattern first
# The pattern seems to be: Answer::NetPacket::readUTF8(packet, &str)  →  packet->readUTF8(str)
# But I already handled this in a previous script. Let me check if there are remaining ones.

# Fix: !std::_Rb_tree_iterator<...>::operator!=(...)  — this should be it == end
# After previous fixes, this would be:  !iter != __x  →  iter == __x  or just normal
# Actually:  if ( !std::_Rb_tree_iterator<...>::operator!=(&iter, &__x) )  becomes  
#  if ( !(iter != __x) )  which is correct C++ for  if ( iter == __x )
# So we don't need to fix !(iter != end) because that's valid.

# But there's another common decompiler pattern:  if ( !iter != end )
# which after our fix becomes  if ( !iter != end )
# This is actually  if ( (!iter) != end )  which is wrong.
# The original meant  if ( iter == end )  →  i.e., "if not (iter != end)"
# So  if ( !iter != __x )  →  if ( iter == __x )
content = re.sub(
    r'!\s*(\w+)\s*!=\s*(\w+)',
    r'\1 == \2',
    content
)

# Fix:  for ( iter = ... begin; ; ++iter )  where the original was a while loop
# This is already handled

# Write result
with open(filepath, 'w') as f:
    f.write(content)

# Count changes
changes = 0
for i, (old, new) in enumerate(zip(original.split('\n'), content.split('\n'))):
    if old != new:
        changes += 1

print(f"Applied transformations to {changes} lines")
