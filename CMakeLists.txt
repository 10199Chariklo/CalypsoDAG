cmake_minimum_required(VERSION 3.16)
project(CalypsoDAG C)

# Enforce strict standard C11 compilation guidelines across Clang
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configure output directories for compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Automate discovery of header inclusion zones
include_directories(include)
include_directories(src)

# --------------------------------------------------------------------------
# SYSTEM CORE KERNEL CHASSIS TARGET
# --------------------------------------------------------------------------
# Gather all system engine files living in our directories and ANY sub-directories
file(GLOB_RECURSE CORE_SOURCES "src/*.c")

# Now we set all our gathered directories into a single entry; KERNEL_SOURCES
set(KERNEL_SOURCES ${CORE_SOURCES})

# --------------------------------------------------------------------------
# INDEPENDENT UNIT TEST TARGETS
# --------------------------------------------------------------------------
add_executable(RevisionTest01
    unittest/RevisionTest01
    ${KERNEL_SOURCES}
)