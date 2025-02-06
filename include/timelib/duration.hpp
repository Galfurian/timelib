/// @file duration.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Defines the duration class.
///
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
///

#pragma once

#include "timelib/timespec.hpp"

#include <iomanip>
#include <sstream>
#include <string>

namespace timelib
{

/// @brief The way the stopwatch prints the elapsed time.
typedef enum {
    human,   ///< Human readable     :  1h  4m  2s   1m 153u 399n
    numeric, ///< Numeric            :  1.4.2.1.153.399
    total,   ///< Elapsed in seconds : 1245.12
    custom   ///< Use placeholders   : %H,%M,%s,%m,%u,%n
} print_mode_t;

/// @brief A class that represents a duration of time.
class Duration
{
public:
    /// @brief Constructs a Duration object.
    /// @param duration the initial amount for the duration.
    /// @param print_mode the way the duration should be printed.
    /// @param format the format to be used for printing.
    Duration(timespec_t duration, print_mode_t print_mode, const std::string &format)
        : _duration(duration)
        , _print_mode(print_mode)
        , _format(format)
    {
        // Nothing to do.
    }

    /// @brief Returns a zero duration.
    /// @return A zero duration value.
    static inline timespec_t zero() { return timespec_t::zero(); }

    /// @brief Returns the internal duration as a timespec_t.
    /// @return The timespec_t object representing the raw duration.
    inline timespec_t raw() const { return _duration; }

    /// @brief Returns the duration count.
    /// @return The duration as a double value.
    inline double count() const { return _duration.count(); }

    /// @brief Operator to get the duration count.
    /// @return The duration as a double value.
    inline double operator()() const { return _duration.count(); }

    /// @brief Sets the print mode.
    /// @param print_mode The new print mode to set.
    inline void set_print_mode(print_mode_t print_mode) { _print_mode = print_mode; }

    /// @brief Sets the format for printing the duration.
    /// @param format The format string to set.
    inline void set_format(const std::string &format) { _format = format; }

    /// @brief Adds two Duration objects.
    /// @param rhs The right-hand side Duration to add.
    /// @return A new Duration object representing the sum.
    inline Duration operator+(const Duration &rhs) const
    {
        return Duration(_duration + rhs._duration, _print_mode, _format);
    }

    /// @brief Adds a duration of type T.
    /// @param rhs The value to add.
    /// @return A new Duration object representing the sum.
    template <typename T>
    inline Duration operator+(const T &rhs) const
    {
        return Duration(_duration + rhs, _print_mode, _format);
    }

    /// @brief Adds a scalar value of type T to a Duration object.
    /// @param lhs The scalar value (left-hand side).
    /// @param rhs The Duration object (right-hand side).
    /// @return A new Duration object representing the sum.
    template <typename T>
    inline friend Duration operator+(const T &lhs, const Duration &rhs)
    {
        return Duration(lhs + rhs._duration, rhs._print_mode, rhs._format);
    }

    /// @brief Subtracts two Duration objects.
    /// @param rhs The right-hand side Duration to subtract.
    /// @return A new Duration object representing the difference.
    inline Duration operator-(const Duration &rhs) const
    {
        return Duration(_duration - rhs._duration, _print_mode, _format);
    }

    /// @brief Subtracts a duration of type T.
    /// @param rhs The value to subtract.
    /// @return A new Duration object representing the difference.
    template <typename T>
    inline Duration operator-(const T &rhs) const
    {
        return Duration(_duration - rhs, _print_mode, _format);
    }

    /// @brief Subtracts a Duration object from a scalar value of type T.
    /// @param lhs The scalar value (left-hand side).
    /// @param rhs The Duration object (right-hand side).
    /// @return A new Duration object representing the difference.
    template <typename T>
    inline friend Duration operator-(const T &lhs, const Duration &rhs)
    {
        return Duration(lhs - rhs._duration, rhs._print_mode, rhs._format);
    }

    /// @brief Divides two Duration objects.
    /// @param rhs The right-hand side Duration to divide.
    /// @return A new Duration object representing the quotient.
    template <typename T>
    inline Duration operator/(const Duration &rhs) const
    {
        return Duration(timespec_t(_duration / rhs._duration), _print_mode, _format);
    }

    /// @brief Divides the Duration by a scalar value.
    /// @param rhs The scalar value to divide by.
    /// @return A new Duration object representing the quotient.
    template <typename T>
    inline Duration operator/(const T &rhs) const
    {
        return Duration(_duration / rhs, _print_mode, _format);
    }

    /// @brief Divides a scalar value of type T by a Duration object.
    /// @param lhs The scalar value (left-hand side).
    /// @param rhs The Duration object (right-hand side).
    /// @return A new Duration object representing the quotient.
    template <typename T>
    inline friend Duration operator/(const T &lhs, const Duration &rhs)
    {
        return Duration(timespec_t(lhs / rhs._duration), rhs._print_mode, rhs._format);
    }

    /// @brief Multiplies two Duration objects (optional, though uncommon).
    /// @param rhs The right-hand side Duration object to multiply by.
    /// @return A new Duration object representing the product.
    inline Duration operator*(const Duration &rhs) const
    {
        return Duration(_duration * rhs._duration, _print_mode, _format);
    }

    /// @brief Multiplies the Duration object by a scalar value of type T.
    /// @param rhs The scalar value to multiply by.
    /// @return A new Duration object representing the product.
    template <typename T>
    inline Duration operator*(const T &rhs) const
    {
        return Duration(_duration * rhs, _print_mode, _format);
    }

    /// @brief Multiplies a scalar value of type T by a Duration object.
    /// @param lhs The scalar value (left-hand side).
    /// @param rhs The Duration object (right-hand side).
    /// @return A new Duration object representing the product.
    template <typename T>
    inline friend Duration operator*(const T &lhs, const Duration &rhs)
    {
        return Duration(lhs * rhs._duration, rhs._print_mode, rhs._format);
    }

    /// @brief Adds another Duration to the current Duration.
    /// @param rhs The right-hand side Duration to add.
    /// @return A reference to the updated Duration.
    inline Duration &operator+=(const Duration &rhs)
    {
        _duration = _duration + rhs._duration;
        return *this;
    }

    /// @brief Adds a scalar value to the Duration.
    /// @param rhs The value to add.
    /// @return A reference to the updated Duration.
    template <typename T>
    inline Duration &operator+=(const T &rhs)
    {
        _duration = _duration + rhs;
        return *this;
    }

    /// @brief Subtracts another Duration from the current Duration.
    /// @param rhs The right-hand side Duration to subtract.
    /// @return A reference to the updated Duration.
    inline Duration &operator-=(const Duration &rhs)
    {
        _duration = _duration - rhs._duration;
        return *this;
    }

    /// @brief Subtracts a scalar value from the Duration.
    /// @param rhs The value to subtract.
    /// @return A reference to the updated Duration.
    template <typename T>
    inline Duration &operator-=(const T &rhs)
    {
        _duration = _duration - rhs;
        return *this;
    }

    /// @brief Divides the current Duration by another Duration.
    /// @param rhs The right-hand side Duration to divide by.
    /// @return A reference to the updated Duration.
    template <typename T>
    inline Duration &operator/=(const Duration &rhs)
    {
        _duration = timespec_t(_duration / rhs._duration);
        return *this;
    }

    /// @brief Divides the current Duration by a scalar value.
    /// @param rhs The value to divide by.
    /// @return A reference to the updated Duration.
    template <typename T>
    inline Duration &operator/=(const T &rhs)
    {
        _duration = _duration / rhs;
        return *this;
    }

    /// @brief Assigns a new duration value.
    /// @param rhs The duration value to assign.
    /// @return A reference to the updated Duration.
    inline Duration &operator=(const timespec_t &rhs)
    {
        _duration = rhs;
        return *this;
    }

    /// @brief Converts the Duration to a string representation.
    /// @return A string representing the Duration.
    inline std::string to_string() const
    {
        std::stringstream ss;
        if (_print_mode == total) {
            ss << _duration.to_nanoseconds<double>() * 1e-09;
        } else {
            time_t h, m, s, ms, us, ns = _duration.to_nanoseconds<time_t>();
            // Extract the values.
            h  = detail::ns_to_hours(ns, &ns);
            m  = detail::ns_to_minutes(ns, &ns);
            s  = detail::ns_to_seconds(ns, &ns);
            ms = detail::ns_to_milliseconds(ns, &ns);
            us = detail::ns_to_microseconds(ns, &ns);
            if (_print_mode == human) {
                if (h) {
                    ss << std::setw(3) << h << "H ";
                }
                if (m) {
                    ss << std::setw(3) << m << "M ";
                }
                if (s) {
                    ss << std::setw(3) << s << "s ";
                }
                if (ms) {
                    ss << std::setw(3) << ms << "m ";
                }
                if (us) {
                    ss << std::setw(3) << us << "u ";
                }
                if (ns) {
                    ss << std::setw(3) << ns << "n ";
                }
            } else if (_print_mode == numeric) {
                ss << h << "." << m << "." << s << "." << ms << "." << us << "." << ns;
            } else if (!_format.empty()) {
                std::string output = _format;
                this->replace(output, "%H", std::to_string(h));
                this->replace(output, "%M", std::to_string(m));
                this->replace(output, "%s", std::to_string(s));
                this->replace(output, "%m", std::to_string(ms));
                this->replace(output, "%u", std::to_string(us));
                this->replace(output, "%n", std::to_string(ns));
                ss << output;
            }
        }
        return ss.str();
    }

    /// @brief Prints the Duration to an output stream.
    /// @param lhs The output stream.
    /// @param rhs The Duration to print.
    /// @return The modified output stream.
    friend std::ostream &operator<<(std::ostream &lhs, const Duration &rhs) { return (lhs << rhs.to_string()); }

private:
    /// @brief Replaces all occurrences of a substring in a string.
    /// @param s The string to modify.
    /// @param substring The substring to replace.
    /// @param substitute The string to replace it with.
    /// @param occurences The number of occurrences to replace (-1 for all).
    /// @return A reference to the modified string.
    static inline std::string &
    replace(std::string &s, const std::string &substring, const std::string &substitute, int occurences = -1)
    {
        // Find the first occurence.
        std::string::size_type pos = s.find(substring);
        // Iterate until the end of the string.
        while (pos != std::string::npos) {
            // Replace the occurence.
            s.replace(pos, substring.size(), substitute);
            // Check how many of them we still need to replace.
            if ((occurences > 0) && ((--occurences) == 0))
                break;
            // Advance to the next occurence.
            pos = s.find(substring, pos + substitute.size());
        }
        return s;
    }

    /// @brief Stores the duration value.
    timespec_t _duration;
    /// @brief Specifies the print mode (e.g., human-readable, numeric).
    print_mode_t _print_mode;
    /// @brief Format string used for custom printing.
    std::string _format;
};

} // namespace timelib
