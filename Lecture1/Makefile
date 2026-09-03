# Makefile for a C++ project

# =====================================================================
# For building a C++ project with CMake
# ---------------------------------------------------------------------

# Directory where the build artifacts will be stored
BUILD_DIR ?= build

# See https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html
# Options: "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
# Default is set to "Debug" in ./CMakelists.txt
CMAKE_BUILD_TYPE ?=

# Set the C++ compiler
# Default to $CXX 
CMAKE_CXX_COMPILER ?= clang++

export CXX CMAKE_BUILD_TYPE

# Use bash for Make commands
SHELL:=/bin/bash

# Path to cmake executable
CMAKE_COMMAND ?= $(shell which cmake)

# Directory where `make install` places the artifacts.
# Defaults to the user-local prefix ~/.local (no sudo required).
# Override with, e.g., make install PREFIX=/home/me/.local
PREFIX ?= $(HOME)/.local

# Phony targets that are not actual files, but rather a command to be executed
.PHONY: all generate build run clean install uninstall clean-all vscode-clangd

# The main target to build the project
all: generate build run

# Generate compile commands for the project
# See:
# - CMake manual: https://cmake.org/cmake/help/latest/manual/cmake.1.html#generate-a-project-buildsystem
generate:
	$(CMAKE_COMMAND) -B "$(BUILD_DIR)" -S . -DCMAKE_CXX_COMPILER=$(CMAKE_CXX_COMPILER) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -DCMAKE_INSTALL_PREFIX=$(PREFIX)

# Build the project after generate
# See: https://cmake.org/cmake/help/latest/manual/cmake.1.html#build-a-project
build:
	$(CMAKE_COMMAND) --build "$(BUILD_DIR)"

# Run the executable after build
run:
	"./bin/addmul"

# Install the artifacts (executable, libraries, headers) under $(PREFIX)
# See: https://cmake.org/cmake/help/latest/manual/cmake.1.html#install-a-project
# The executable finds the shared library relative to itself ($ORIGIN/../lib),
# so no LD_LIBRARY_PATH is needed; only $(PREFIX)/bin may need a PATH entry.
# `generate` is re-run so that overriding PREFIX (e.g.
# `make install PREFIX=/home/me/.local`) always takes effect — CMake caches
# the prefix from the previous configure otherwise.
install: generate build
	$(CMAKE_COMMAND) --install "$(BUILD_DIR)"
	@echo "Installed to $(PREFIX)."
	@echo "If $(PREFIX)/bin is not on your PATH, add the following to ~/.bashrc:"
	@echo "  export PATH=\"$(PREFIX)/bin:\$$PATH\""

# Remove the files previously installed by `make install`
# (the uninstall script is generated at configure time by CMakeLists.txt)
uninstall:
	$(CMAKE_COMMAND) -P "$(BUILD_DIR)/cmake_uninstall.cmake"

# Clean up the artifacts
clean:
	$(CMAKE_COMMAND) --build "$(BUILD_DIR)" --target clean

# Clean up the build system
clean-all:
	rm -rf "$(BUILD_DIR)" bin/ lib/ .cache/


# =====================================================================
# For one-time setup
# ---------------------------------------------------------------------

# Install and configure clangd a debugger extension for Coder Server
# See:
# - CodeLLDB manual: https://github.com/vadimcn/codelldb/blob/master/MANUAL.md#expressions
# - VSCode clangd: https://github.com/clangd/vscode-clangd
# - VSCode debug: https://code.visualstudio.com/docs/debugtest/debugging
setup:
	@echo "Setting up clangd for Code Server..." && \
	code-server --install-extension llvm-vs-code-extensions.vscode-clangd && \
	code-server --install-extension vadimcn.vscode-lldb && \
	echo "Done!"