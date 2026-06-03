#!/usr/bin/env python3
"""Remove empty declarations that conflict with inline constructor calls added by second-pass script.
Fixes redefinition errors for: MutexGuard, InetAddress, MySqlDBGuard, etc."""

import re

filepath = 'src/Game/GameService.cpp'

with open(filepath, 'r') as f:
    lines = f.readlines()

# Patterns for empty declarations (declared without constructor args)
empty_patterns = [
    r'^\s+Answer::MutexGuard\s+lock\s*;\s*$',  # Answer::MutexGuard lock;
    r'^\s+Answer::MutexGuard\s+lockChat\s*;\s*$',
    r'^\s+Answer::InetAddress\s+serverAddr\s*;\s*$',
    r'^\s+Answer::MySqlDBGuard\s+db\s*;\s*$',
    r'^\s+Answer::MySqlQuery\s+result\s*;\s*$',
]

# Patterns for inline construction (declared with constructor args)
inline_patterns = [
    r'^\s+Answer::MutexGuard\s+(lock|lockChat)\(',
    r'^\s+Answer::InetAddress\s+serverAddr\(',
    r'^\s+Answer::MySqlDBGuard\s+db\(',
    r'^\s+Answer::MySqlQuery\s+result\(',
]

def is_empty_decl(line):
    for p in empty_patterns:
        if re.match(p, line):
            return True
    return False

def is_inline_construction(line):
    for p in inline_patterns:
        if re.match(p, line):
            return True
    return False

# Strategy: find functions (between function definition lines), group declarations
# For each function, find all empty declarations and inline constructions.
# If the same variable name appears as both empty and inline, remove the empty one.

# Track line numbers to remove
remove_indices = set()

# Simple approach: scan through the file, keep track of function boundaries,
# and for each function find pairs of (empty_decl, inline_construction)

# Actually, the simplest correct approach:
# 1. Find all empty declarations that have an inline construction of the same var later
#    in the same scope. We can approximate "same scope" as "same function".
# 2. Remove the empty declarations.

# A function starts at a line matching 'ClassName::MethodName' or similar
# Since the decompiler output has messy formatting, let's use a simpler approach:
# Just remove ALL empty declarations that are immediately followed or preceded
# by inline constructions of the same type.

# Even simpler: for each empty declaration, check if there's an inline construction
# of the same variable anywhere in the file (they're always in the same function).

# Let's first collect info
empty_decls = []  # list of (line_num, var_name, type_name)
inline_constrs = []  # list of (line_num, var_name, type_name)

for i, line in enumerate(lines):
    stripped = line.strip()
    # Empty MutexGuard lock;
    m = re.match(r'^(Answer::MutexGuard|Answer::InetAddress|Answer::MySqlDBGuard|Answer::MySqlQuery)\s+(\w+)\s*;\s*$', stripped)
    if m:
        empty_decls.append((i, m.group(2), m.group(1)))
    # Inline MutexGuard lock(&mutex);
    m2 = re.match(r'^(Answer::MutexGuard|Answer::InetAddress|Answer::MySqlDBGuard|Answer::MySqlQuery)\s+(\w+)\(', stripped)
    if m2:
        inline_constrs.append((i, m2.group(2), m2.group(1)))

# For each empty declaration, if there's an inline construction of the same var name,
# mark the empty declaration for removal
for ed_idx, ed_var, ed_type in empty_decls:
    for ic_idx, ic_var, ic_type in inline_constrs:
        if ed_var == ic_var and ed_type == ic_type:
            remove_indices.add(ed_idx)
            break

print(f"Found {len(empty_decls)} empty declarations, {len(inline_constrs)} inline constructions")
print(f"Removing {len(remove_indices)} empty declarations that conflict with inline construction")

# Apply removals (in reverse order to preserve indices)
result = []
for i, line in enumerate(lines):
    if i not in remove_indices:
        result.append(line)

with open(filepath, 'w') as f:
    f.writelines(result)

# Print removed lines
for idx in sorted(remove_indices):
    print(f"  Removed line {idx+1}: {lines[idx].rstrip()}")
