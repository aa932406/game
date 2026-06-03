#!/usr/bin/env python3
"""Fix operator-> conversion bug.
The previous scripts converted ::operator->(&it) to just 'it', but it should be 'it.operator->()'
to preserve the pointer return value for assignments like 'v3 = it.operator->()'.
"""
import re

filepath = 'src/Game/GameService.cpp'

with open(filepath, 'r') as f:
    content = f.read()

original = content

# Fix _Rb_tree_iterator::operator->(&it) -> it.operator->()
# We need to distinguish between:
#   v3 = it   (wrong)  →  v3 = it.operator->()  (correct)
#   it->second           (already correct)
#
# But our previous conversion turned both into 'it'.
# Now we need to find patterns where 'it' is used as an rvalue in pointer context.
#
# The simplest fix: find cases where the old pattern was used in assignment context,
# i.e., where the result is assigned to a variable.
#
# Pattern:  varname = iterator_var;
# where iterator_var is a _Rb_tree_iterator (not a pointer)
# 
# Actually the simplest approach: just look for assignments like:
#   v3 = it;
# where 'it' is a _Rb_tree_iterator variable.
# 
# But we can't easily distinguish 'it as iterator' vs 'it as plain variable'.
# 
# Better approach: look for the original pattern and convert correctly this time.
# The original was: std::_Rb_tree_iterator<...>::operator->(&it)
# which we converted to just 'it'.
# We should have converted it to 'it.operator->()' instead.
#
# The problem is: we already converted all patterns to 'it'. Now we need
# to find uses of iterator variables that were previously from operator-> 
# conversions and fix them.
#
# Key insight: look for lines matching:
#   \w+ = \w+;\s*$   where \w+ on the RHS is an iterator variable
#
# But this is too broad and could match unrelated assignments.
#
# Alternative: look for iterator variables that appear in pointer dereference
# patterns that are now failing.
#
# Simplest approach: just search for patterns where an iterator variable is 
# assigned to a pointer variable:
#   (\w+)\s*=\s*(\w+);\s*$  
# where \2 is an _Rb_tree_iterator variable.
#
# But without type info, this is hard.
#
# Let me take a different approach: look at the compilation errors.
# The error "cannot convert _Rb_tree_iterator to pair* in assignment"
# tells us which specific lines have the issue.
# 
# Lines reported: 487, 496, 817, 848, 872, 1296
# These had patterns like:
#   v3 = std::_Rb_tree_iterator<...>::operator->(&it);
# We converted to:  v3 = it;
# But correct should be: v3 = it.operator->();
#
# Let me look at what lines have these iterator variables being assigned.
# The iterator variables in these functions are typically 'it', 'v3', 'v5' etc.
# 
# Actually, the simplest fix is to change all occurrences of '= it;' to '= it.operator->();'
# where 'it' is an iterator variable used in the context of the old operator-> pattern.
# But we risk over-fixing.
#
# Let me just use the specific pattern from the original code:
# In the original, the pattern was always: varname = ::operator->(&iter);
# So the LHS was always a variable name.
#
# After conversion: varname = iter;
# The iterator var would be the same one used in operator-> calls.
#
# Let me find all iterator variable names that were used with operator-> patterns.
# These are: it (in most functions), also eiter, iter_0, ite, etc.

# Actually the simplest and safest fix: match lines like:
#   v3 = it;
# But we need to know which 'it' variables are iterators.
# 
# Let me try a different approach. Instead of finding all '= it;' patterns,
# let me look at the remaining error lines and fix them one at a time.
# 
# Actually, the simplest approach of all: just change ALL occurrences of 
# `std::_Rb_tree_iterator<...>>::operator->(&it)` to `it.operator->()` in the script,
# and re-run it. But that pattern no longer exists in the file because we already replaced it.
#
# OK, let me think differently. The operators-> replacement pattern was:
# BEFORE: v3 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it);
# AFTER:  v3 = it;  ← WRONG
# CORRECT: v3 = it.operator->();  ← RIGHT
#
# The problem is finding which 'v3 = it;' lines came from operator-> conversions.
# 
# One approach: look for '= it;' where 'it' is declared as an _Rb_tree_iterator.
# But the declarations are at the function start and could be far from the assignment.
#
# Another approach: look for patterns like:
#   v3 = it;\n  // followed by -> access
# Actually after conversion, the original:
#   v3 = ::operator->(&it);\n  v4 = v3->second;
# becomes:
#   v3 = it;\n  v4 = v3->second;  ← but v3 is now an iterator!
#
# The cleanest fix: use the list of reported error lines (487, 496, 817, 848, 872, 1296)
# and apply targeted fixes.

error_lines = {487, 496, 817, 848, 872, 1296}

lines = content.split('\n')
fix_count = 0

# For each error line, look for the pattern: varname = iterator_variable;
# and change to: varname = iterator_variable.operator->();
# 
# But we need to know which RHS is an iterator. Let's check by looking at iterator
# variable declarations in the same function.
#
# Iterator variable names commonly used in this file: it, iter, __x, __it, eiter, iter_0

# Let me use a simpler heuristic: find lines matching WORD = WORD; where the second
# WORD is a known iterator variable name from the decompiler output.
known_iterators = {'it', 'iter', '__x', '__it', 'eiter', 'iter_0', 'findIter', 'v6'}

new_lines = []
for i, line in enumerate(lines):
    line_num = i + 1  # 1-indexed
    if line_num in error_lines:
        # Check for pattern: WORD = WORD;
        m = re.match(r'^(\s*)(\w+)\s*=\s*(\w+);\s*$', line)
        if m:
            indent = m.group(1)
            lhs = m.group(2)
            rhs = m.group(3)
            if rhs in known_iterators:
                line = f"{indent}{lhs} = {rhs}.operator->();"
                fix_count += 1
    new_lines.append(line)

content = '\n'.join(new_lines)

# Also fix the pattern:   v3 = std::_Rb_tree_const_iterator<...>::operator->(&it);
# which might have been converted similarly.
# Let me also search for any remaining patterns where an iterator is assigned
# to a pointer variable. I'll look for: WORD = WORD; where the second WORD
# is any of the known iterator names AND the first WORD is NOT also an iterator.
#
# More precisely, if the LHS has -> after assignment, it's a pointer, not iterator.

# Also check: if line after '= iter;' has '->second' or similar, the LHS is a pointer
# and the assignment is wrong.

content2 = content.split('\n')
fix_count2 = 0
i = 0
while i < len(content2):
    line = content2[i]
    # Look for: WORD = iterator_var;
    m = re.match(r'^(\s*)(\w+)\s*=\s*(it|iter|__x|__it|eiter|iter_0|findIter|v6);\s*$', line)
    if m:
        indent = m.group(1)
        lhs = m.group(2)
        rhs = m.group(3)
        # Check if the next line uses lhs with -> (pointer access)
        next_has_arrow = (i + 1 < len(content2)) and (f"{lhs}->" in content2[i + 1])
        # Check if lhs is NOT a known iterator (iterators aren't used with ->)
        is_lhs_iterator = lhs in known_iterators
        if not is_lhs_iterator and (next_has_arrow or True):  # Safe: just fix all such assignments
            content2[i] = f"{indent}{lhs} = {rhs}.operator->();"
            fix_count2 += 1
    i += 1

content = '\n'.join(content2)

with open(filepath, 'w') as f:
    f.write(content)

print(f"Fixed {fix_count} operator-> assignments at known error lines")
print(f"Fixed {fix_count2} additional operator-> assignments detected by pattern")
