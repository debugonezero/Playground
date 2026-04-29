cmake_minimum_required(VERSION 3.16)
project(codespace_backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(OpenMP REQUIRED)

add_compile_options(-O3 -march=native -Wall -Wextra -Wpedantic)

add_library(codespace_backend INTERFACE)
target_include_directories(codespace_backend INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(codespace_backend INTERFACE OpenMP::OpenMP_CXX)

add_executable(codespace_example examples/main.cpp)
target_link_libraries(codespace_example PRIVATE codespace_backend)

add_executable(codespace_test tests/test_tensor.cpp)
target_link_libraries(codespace_test PRIVATE codespace_backend)

add_executable(codespace_benchmark benchmarks/benchmark_add.cpp)
target_link_libraries(codespace_benchmark PRIVATE codespace_backend)

add_executable(codespace_benchmark_all benchmarks/benchmark_all.cpp)
target_link_libraries(codespace_benchmark_all PRIVATE codespace_backend)
