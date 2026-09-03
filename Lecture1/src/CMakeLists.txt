# Executable to be built.
add_executable(addmul addmul.cpp)

# Link libraries to the target executable at build time
target_link_libraries(addmul util mymath)

# Embed the project version (from project(VERSION ...) in the top-level
# CMakeLists.txt) into the executable so it can report it with --version.
target_compile_definitions(addmul PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")
