#pragma once

/**
 * @class printer
 * @brief A flexible, type-safe printing utility for streaming multiple values with customizable separators and endings.
 *
 * This class template provides:
 * - Customizable separator (`sep_`) and end-of-line marker (`end_`).
 * - Default singleton instances for common use cases (`get_print` for std::string, `get_cprint` for const char*).
 * - Immutable and mutable configuration methods for chaining.
 * - Variadic operator() for printing multiple arguments.
 *
 * @tparam StringT  Type used for separator and end strings (default: std::string).
 * @tparam StreamT  Output stream type (default: std::ostream).
 */
#include <iostream>
#include <string>

template <class StringT = std::string, class StreamT = std::ostream>
class printer final {
    StringT sep_, end_;
    StreamT* out_;

    /**
     * @brief Constructor to initialize separator, end marker, and output stream.
     * @param sep Separator string (default: " ").
     * @param end End string (default: "\n").
     * @param out Pointer to output stream (default: &std::cout).
     */
    printer(StringT sep = " ", StringT end = "\n", StreamT* out = &std::cout)
    : sep_(std::move(sep)), end_(std::move(end)), out_(out) {}

    // Disable copy and move semantics for singleton safety
    printer(const printer&) = delete;
    printer(printer&&) = delete;
    printer& operator=(const printer&) = delete;
    printer& operator=(printer&&) = delete;

public:
    // ---------------- Default singleton printers ----------------

    /**
     * @brief Get a default printer instance.
     * @return Reference to a static printer object.
     */
    const static printer& get_print() {
        const static printer print;
        return print;
    }

    // ---------------- Immutable configuration methods ----------------

    /**
     * @brief Return a new printer with updated separator.
     * @param s New separator string.
     * @return A new printer instance.
     */
    printer sep(const StringT& s) const& { return printer(s, end_, out_); }

    /**
     * @brief Return a new printer with updated end marker.
     * @param s New end string.
     * @return A new printer instance.
     */
    printer end(const StringT& s) const& { return printer(sep_, s, out_); }

    /**
     * @brief Return a new printer with updated output stream.
     * @param s New output stream reference.
     * @return A new printer instance.
     */
    printer stream(StreamT& s) const& { return printer(sep_, end_, &s); }

    // -------------- Mutable configuration methods for rvalues --------------

    /**
     * @brief Update separator in-place (for rvalue printer).
     * @param s New separator value.
     * @return Reference to the modified printer.
     */
    template <class T>
    printer&& sep(T&& s) && {
        sep_ = StringT(std::forward<T>(s));
        return std::move(*this);
    }

    /**
     * @brief Update end marker in-place (for rvalue printer).
     * @param s New end value.
     * @return Reference to the modified printer.
     */
    template <class T>
    printer&& end(T&& s) && {
        end_ = StringT(std::forward<T>(s));
        return std::move(*this);
    }

    /**
     * @brief Update output stream in-place (for rvalue printer).
     * @param s New stream reference.
     * @return Reference to the modified printer.
     * @note Unlike StringT, StreamT is stored by pointer rather than value.
     *       Only lvalue streams are accepted to prevent dangling pointers
     *       caused by temporary streams.
     */
    printer&& stream(StreamT& s) && {
        out_ = &s;
        return std::move(*this);
    }

    // ---------------- Printing methods ----------------

    /**
     * @brief Print only the end marker (useful for empty lines).
     */
    void operator()() const {
        *out_ << end_;
    }

    /**
     * @brief Print multiple arguments separated by `sep_` and terminated by `end_`.
     * @param first First argument.
     * @param rest Remaining arguments.
     */
    void operator()(const auto &first, const auto &... rest) const {
        *out_ << first;
        ((*out_ << sep_ << rest), ..., (*out_ << end_));
    }

};
