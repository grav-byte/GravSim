#!/usr/bin/env python3
"""
Post-process auto.puml into namespaced PUML based on folder hierarchy
and fix unbalanced braces / duplicate @startuml.
"""

from pathlib import Path
import re

SRC_DIR = Path("../../src")  # adjust relative to postprocessor
INPUT_PUML = Path("auto.puml")
OUTPUT_PUML = Path("auto_namespaced.puml")

lines = INPUT_PUML.read_text().splitlines()

# remove original @startuml / @enduml
lines = [l for l in lines if not l.strip().startswith("@startuml") and not l.strip().startswith("@enduml")]

# Regex to detect class/struct start
class_start_re = re.compile(r'^\s*(class|struct|abstract class)\s+(\w+)\s*{')
# Regex to detect enum start
enum_start_re = re.compile(r'^\s*enum\s+(\w+)\s*{')

# Map package -> list of class/enum blocks
packages = {}
current_pkg = "Global"

# helper to track multi-line class blocks
inside_block = False
block_lines = []
class_name = None

for line in lines:
    # Detect start of class/enum block
    m_class = class_start_re.match(line)
    m_enum = enum_start_re.match(line)

        # Detect start of class/enum block
    if (m_class or m_enum) and not inside_block:
        inside_block = True
        block_lines = [line]

        if m_class:
            class_name = m_class.group(2)
        else:  # enum
            class_name = m_enum.group(1)

        # map to header in src
        path_candidates = list(SRC_DIR.rglob(f"{class_name}.h"))
        if path_candidates:
            rel_path = path_candidates[0].relative_to(SRC_DIR)
            pkg = "::".join(rel_path.parent.parts) or "Global"
        else:
            pkg = "Global"

        current_pkg = pkg
        packages.setdefault(current_pkg, []).append(block_lines)
        continue

    if inside_block:
        block_lines.append(line)
        if line.strip() == "}":
            inside_block = False
        continue

    # non-class lines
    packages.setdefault("Other", []).append([line])

# write output
with OUTPUT_PUML.open("w") as f:
    f.write("@startuml\n\n")
    # write packages
    for pkg, blocks in packages.items():
        if pkg == "Other":
            continue
        f.write(f'package "{pkg}" {{\n')
        for block in blocks:
            for l in block:
                f.write(f"{l}\n")
        f.write("}\n\n")
    # write other lines (relationships, enums, etc.)
    for block in packages.get("Other", []):
        for l in block:
            f.write(f"{l}\n")
    f.write("\n@enduml\n")

print(f"Namespaced PUML written to {OUTPUT_PUML}")