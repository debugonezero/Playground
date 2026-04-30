#!/usr/bin/env bash
# Quick Development Helper for Codespace Backend
# Usage: ./quickdev.sh <project_name>

set -euo pipefail

if [ $# -ne 1 ]; then
    echo "Usage: $0 <project_name>"
    echo "Creates a new quick development project"
    exit 1
fi

PROJECT_NAME=$1
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Create project directory structure
mkdir -p "$PROJECT_NAME/src"
mkdir -p "$PROJECT_NAME/build"

# Create main.cpp template
cat > "$PROJECT_NAME/src/main.cpp" << 'EOF'
#include <iostream>
#include <chrono>
#include "csb/tensor.hpp"

int main() {
    std::cout << "Quick Start Template\n";
    std::cout << "====================\n\n";

    // TODO: Add your tensor code here
    csb::Tensor<float> a({10, 10});
    csb::Tensor<float> b({10, 10});
    
    a.fill(1.0f);
    b.fill(2.0f);

    auto start = std::chrono::high_resolution_clock::now();
    auto c = a.add(b);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Result c[0] = " << c[0] << "\n";
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Time: " << elapsed.count() << " ms\n";

    return 0;
}
EOF

# Create CMakeLists.txt template
cat > "$PROJECT_NAME/CMakeLists.txt" << 'EOF'
cmake_minimum_required(VERSION 3.16)
project(quick_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_compile_options(-O3 -march=native -Wall -Wextra)

# Point to the backend include directory
set(BACKEND_INCLUDE_DIR ../include)

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE ${BACKEND_INCLUDE_DIR})
target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)
EOF

# Create build script
cat > "$PROJECT_NAME/build.sh" << 'EOF'
#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")"
mkdir -p build
cd build
cmake ..
make -j$(nproc)
echo "Build complete. Run: ./main"
EOF

chmod +x "$PROJECT_NAME/build.sh"

echo "✅ Created project: $PROJECT_NAME"
echo ""
echo "Quick start:"
echo "  cd $PROJECT_NAME"
echo "  ./build.sh"
echo "  ./build/main"
echo ""
echo "Edit src/main.cpp to add your tensor code!"
