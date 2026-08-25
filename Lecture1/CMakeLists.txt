# CMake build configuration for the MyProject C++ project.
# Defines compiler settings, libraries, and executable build/linking steps.

# Checks such as the minimum required version of CMake
cmake_minimum_required(VERSION 3.14)
include(CheckIncludeFileCXX)
set(CMAKE_WARN_UNUSED_CLI YES)

# Define the build directory where generated files will be placed.
set(CMAKE_BINARY_DIR build)

# Define the project name, version, and programming language
project("Toolchain" VERSION 0.1.0 LANGUAGES CXX)

# Export compile commands for use with tools like clangd
# See:
# - clangd manual: https://clangd.llvm.org/installation.html#project-setup
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the default build type to Debug if none is specified
# Set a default build type if none was specified
# See https://www.kitware.com/cmake-and-the-default-build-type/
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Set the possible values of build type for cmake-gui
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
endif()

# Set default C++ standard for all targets.
set(CMAKE_CXX_STANDARD 23)

# Specify where to place binaries and libraries by default
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# Add the include subfolder to the list of paths to search for
# *.hpp: header files containing declarations.
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add compiler flags to keep intermediate files for debugging/inspection:
# - *.ii: Preprocessed C++ source file (after macros and includes are expanded)
# - *.s: Assembly code generated from the source file
# - *.o: Object file (machine code, not yet linked)
# - *.d: Dependency file (lists header dependencies for makefile builds)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps")

# Dynamic library to be built:
# - File extension: .so (Unix), .dll (Windows), .dylib (macOS)
# - To be linked by all intended targets at runtime.
add_library(mymath SHARED mymath/add.cpp mymath/mul.cpp)

# Static library to be built:
# - File extension: .a (Unix), .lib (Windows)
# - To be included in all intended targets at build time.
add_library(util STATIC util/io.cpp)

add_subdirectory(src)