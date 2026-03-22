#!/usr/bin/env bash

set -e

CMD="hpp2plantuml"
TEMPLATE="template.puml"
OUT="auto.puml"
POST="post_process.py"


ARGS=()

# include all subfolders of ../src
while IFS= read -r dir; do
    ARGS+=("-i" "$dir/*.h")
done < <(find ../../src -type d)

$CMD "${ARGS[@]}" -o "$OUT" -t "$TEMPLATE"

# run post-processor
if [ -f "$POST" ]; then
    echo "Running post-processing..."
    python3 "$POST"
else
    echo "Post-processor not found: $POST"
fi

echo "Done. Namespaced PUML should be in auto_namespaced.puml"