# CMake build configuration for the MyProject C++ project.
# Defines compiler settings, libraries, and executable build/linking steps.

# Checks such as the minimum required version of CMake
cmake_minimum_required(VERSION 3.14)
include(CheckIncludeFileCXX)
set(CMAKE_WARN_UNUSED_CLI YES)

# Define the build directory where generated files will be placed.
set(CMAKE_BINARY_DIR build)

# Define the project name, version, and programming language
project("Toolchain" VERSION 0.2.0 LANGUAGES CXX)

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
# Where static libraries (*.a) go — next to the shared libraries (*.so).
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# Where the *installed* executable should look for the shared library:
# $ORIGIN is the directory containing the executable itself, so the
# binary finds libmymath.so in ../lib next to itself, no matter where
# the project is installed. No LD_LIBRARY_PATH or sudo required.
# See: https://man7.org/linux/man-pages/man8/ld.so.8.html
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")

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
# Version the library file and its symbolic links:
# - real file : libmymath.so.0.2.0   (full version, from VERSION)
# - SONAME    : libmymath.so.0       (major version, from SOVERSION;
#                                     what the executable looks up at run time)
# - dev link  : libmymath.so         (what you link against at build time)
# Executables keep working across minor updates; bump the major number
# only when you break the ABI.
# See: https://cmake.org/cmake/help/latest/prop_tgt/SOVERSION.html
set_target_properties(mymath PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

# Static library to be built:
# - File extension: .a (Unix), .lib (Windows)
# - To be included in all intended targets at build time.
add_library(util STATIC util/io.cpp)

add_subdirectory(src)

# =====================================================================
# Installation: where `make install` places the artifacts
# ---------------------------------------------------------------------

# Standard, platform-appropriate install directory names (bin/, lib/,
# include/ on Unix) — the same names no matter which compiler is used.
# See: https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html
include(GNUInstallDirs)

# The install prefix (e.g. ~/.local) is set from the Makefile via
# -DCMAKE_INSTALL_PREFIX=$(PREFIX).
install(TARGETS addmul RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
# RUNTIME: executables, LIBRARY: shared libraries, ARCHIVE: static libraries
install(TARGETS mymath LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS util   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Generate a script that removes the installed files, so that the
# `uninstall` target in the Makefile can undo `make install` by
# following the install manifest that `cmake --install` writes out.
configure_file(cmake_uninstall.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    @ONLY)
