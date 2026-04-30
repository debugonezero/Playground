// Quick Snippet: Compile Everything
// Run: ./compile_all.sh

#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INCLUDE_DIR="$SCRIPT_DIR/../include"

echo "Compiling all snippets..."
echo ""

for file in $SCRIPT_DIR/*.cpp; do
    name=$(basename "$file" .cpp)
    echo "Building: $name"
    g++ -O3 -march=native -std=c++23 -I"$INCLUDE_DIR" -fopenmp "$file" -o "$SCRIPT_DIR/$name"
done

echo ""
echo "✅ All snippets compiled!"
echo ""
echo "Run with:"
for file in $SCRIPT_DIR/*.cpp; do
    name=$(basename "$file" .cpp)
    echo "  ./$name"
done
