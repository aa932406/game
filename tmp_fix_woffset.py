#!/usr/bin/env python3
"""Add uint32_t wOffset = 0; declaration before first wOffset = in each function scope."""

import re

filepath = 'src/Game/GameService.cpp'

with open(filepath, 'r') as f:
    content = f.read()

lines = content.split('\n')

# Find all lines with 'wOffset =' (assignment, not declaration)
# Track which function scopes already have uint32_t wOffset declared
# A function scope starts at a method definition and ends at the next method definition
# We'll find all wOffset = assignments and add uint32_t wOffset = 0; before the first in each scope

# Strategy: find all line indices where wOffset = appears
# For each, check if there's already a 'uint32_t wOffset' declaration before it in the same function
# A function boundary is a line matching '^void |^int8_t |^int32_t |^bool |^std::string |^Player\* |^ConnType\* '

woffset_assign_indices = []
for i, line in enumerate(lines):
    stripped = line.strip()
    if stripped.startswith('wOffset = '):
        woffset_assign_indices.append(i)

# Find function boundaries (lines that start a new function definition)
func_boundaries = set()
func_boundaries.add(0)  # start of file
for i, line in enumerate(lines):
    if re.match(r'^(void |int8_t |int32_t |bool |std::string |Player\* |ConnType\* |const .*)?(GameService::|Answer::)', line.strip()):
        func_boundaries.add(i)

# For each wOffset assignment, check if uint32_t wOffset is already declared
# in the current function scope (between the last method definition and this line)
last_func_start = 0
declarations_in_scope = set()

# Build result lines
result = []
insertions = {}

for i, line in enumerate(lines):
    # Check if this line starts a new function
    if i in func_boundaries and i != 0:
        declarations_in_scope = set()
    
    # Check if this line has a declaration we care about
    if 'uint32_t wOffset' in line:
        declarations_in_scope.add(line.strip())
    
    # Check if this is a wOffset = assignment and needs a declaration
    stripped = line.strip()
    if stripped.startswith('wOffset = '):
        # Check if uint32_t wOffset is already declared in this scope
        has_decl = any('uint32_t wOffset' in d for d in declarations_in_scope)
        if not has_decl:
            # Check if we already added one for this scope
            # We need to add uint32_t wOffset = 0; before this line
            # But we need to insert it at a good position - right before the first use
            if i not in insertions:
                insertions[i] = '  uint32_t wOffset = 0;'
            declarations_in_scope.add('uint32_t wOffset')

# Now apply insertions
result = []
for i, line in enumerate(lines):
    if i in insertions:
        result.append(insertions[i])
    result.append(line)

new_content = '\n'.join(result)
with open(filepath, 'w') as f:
    f.write(new_content)

print(f"Added {len(insertions)} uint32_t wOffset declarations")
for idx in sorted(insertions.keys()):
    print(f"  Line {idx+1}: {insertions[idx]}")
