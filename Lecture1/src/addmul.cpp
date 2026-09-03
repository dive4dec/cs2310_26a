// Preprocessor directives to include necessary headers.
// See: https://en.cppreference.com/w/cpp/preprocessor/include
#include "mymath.hpp" // Quote form "..." : Search in the current before system directories.
#include "util.hpp"   // Cannot include more than one header at a time.
#include <format>
#include <iostream> // Angular bracket form <...>: Only search in system include directories.
#include <string_view> // Compare command line arguments without copying them.
// See: https://en.cppreference.com/w/cpp/string/basic_string/string_view

// Using directive to bring names into the current scope.
// See: https://en.cppreference.com/w/cpp/language/using_declaration
using std::cout, std::format, mymath::add, mymath::mul;

// Main function as the entry point of the program.
// See: https://en.cppreference.com/w/cpp/language/main_function
int main(int argc, char* argv[]) {
    // Command line arguments: argv[0] is the program name itself.
    // See: https://en.cppreference.com/w/cpp/utility/program/argc
    // Options are recognized before asking for any inputs.
    if (argc > 1 && std::string_view(argv[1]) == "--help") {
        cout << "Usage: addmul [OPTION]\n"
             << "  --help     show this help and exit\n"
             << "  --version  show the version and exit\n";
        return 0;
    }
    if (argc > 1 && std::string_view(argv[1]) == "--version") {
        // The version comes from project(VERSION ...) in CMakeLists.txt,
        // compiled in by target_compile_definitions in src/CMakeLists.txt.
        cout << "addmul " << PROJECT_VERSION << '\n';
        return 0;
    }
    // Variable declarations.
    // See: https://en.cppreference.com/w/cpp/language/declarations
    int a, b, c;

    // Ask for user inputs using a function from the static library util.
    input_int(a);
    input_int(b);
    input_int(c);

    // clang-format off
	  cout << '\n' <<
		// Perform addition & multiplication using functions from the dynamic library mymath.
        "add(" << a << ", " << b << ") == " << add(a, b) << "\n" <<
		// See https://en.cppreference.com/w/cpp/utility/format/format.html
		format("mul({}, {}, {}) = {}\n", a, b, c, mul(a, b, c));
    // clang-format on
    // See:
    // https://clang.llvm.org/docs/ClangFormatStyleOptions.html#disabling-formatting-on-a-piece-of-code

    return 0;
}