#pragma once

#include <iostream>
#include <format>
#include <string_view>
#include <string>
#include <type_traits>

/**
 * @brief Logs the memory address of a variable along with its type and name.
 *
 * This macro wraps the `loc()` function and automatically generates a label
 * that includes the type of the variable and its name as a string.
 *
 * @param x The variable to log.
 *
 * @example
 * int a = 42;
 * L(a); // Logs: (int) a @0x...
 */
#define L(x) loc(x, std::string("(")+std::string(type<decltype(x)>())+") "+#x)

/**
 * @brief Logs the memory address of the beginning of a container or array,
 * along with its type and name.
 *
 * This macro wraps the `loc_begin()` function and automatically generates a label
 * that includes the type of the container and its name as a string.
 *
 * @param x The container or array to log.
 *
 * @example
 * std::vector<int> v = {1, 2, 3};
 * LB(v); // Logs: (std::vector<int>) v begin@0x...
 */
#define LB(x) loc_begin(x, std::string("(")+std::string(type<decltype(x)>())+") "+#x)

using std::cout, std::format, std::forward, std::string_view, std::string;

/**
 * @brief Extracts the demangled name of a type T at compile time.
 *
 * This function uses compiler-specific macros to retrieve the decorated function signature
 * and then trims it to isolate the type name of the template parameter T.
 *
 * Supported compilers:
 * - Clang: Uses __PRETTY_FUNCTION__
 * - GCC: Uses __PRETTY_FUNCTION__
 * - MSVC: Uses __FUNCSIG__
 *
 * @tparam T The type whose name is to be extracted.
 * @return A string_view containing the name of the type T.
 *
 * @note See https://stackoverflow.com/questions/81870/is-it-possible-to-print-the-name-of-a-variables-type-in-standard-c
 */
template <typename T>
constexpr auto type() {
  string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto type() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type() [with T = ";
  suffix = "]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix = "auto __cdecl type<";
  suffix = ">(void)";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}

/**
 * @brief Logs the memory address of a variable along with an optional label.
 * 
 * This function is useful for educational purposes, especially when tracking
 * object lifetimes, locations in memory, or verifying move semantics.
 * 
 * @tparam T Type of the variable (supports universal references).
 * @param x The variable to log.
 * @param label Optional label to identify the variable in the output.
 * @return T&& Perfectly forwarded variable.
 */
template <class T>
T &&loc(T &&x, const string &label="") {
    cout << format("{} @{:p}\n",
        label,
        static_cast<const void*>(&x));
    return std::forward<T>(x);
}


// Helper trait to detect presence of begin()
template <typename, typename = void>
struct has_begin : std::false_type {};

template <typename T>
struct has_begin<T, std::void_t<decltype(std::declval<T>().cbegin())>> : std::true_type {};


/**
 * @brief Logs the memory address of beginning element in a container along
 * with an optional label.
 * 
 * This overload is useful for locating elements of a container.
 * 
 * @tparam T Type of the container or array (supports universal references).
 * @param x The container or array.
 * @param label Optional label to identify the element in the output.
 * @return T&& Perfectly forwarded container or array.
 */
// Overload for containers with .cbegin()
template <class T>
std::enable_if_t<has_begin<T>::value, T&&>
&&loc_begin(T &&x, const string &label="") {
    cout << format("{} begin@{:p}\n", label, static_cast<const void*>(&*(x.cbegin())));
    return std::forward<T>(x);
}

// Overload for C-style arrays
template <class T, size_t N>
T (&loc_begin(T (&x)[N], const string &label = ""))[N] {
    cout << format("{} begin@{:p}\n", label, static_cast<const void*>(&x[0]));
    return x;
}

// For pointers or decayed arrays
template <class T>
typename std::enable_if<std::is_pointer<T>::value, T>::type
loc_begin(T x, const string& label = "") {
    cout << format("{} begin@{:p}\n", label, static_cast<const void*>(x));
    return x;
}
