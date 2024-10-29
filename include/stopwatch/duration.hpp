/// @file duration.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Defines the duration class.
///
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
///

#pragma once

#include "stopwatch/details.hpp"

#include <sstream>
#include <iomanip>
#include <string>

namespace stopwatch
{

class Duration {
public:
    Duration(duration_type_t duration, print_mode_t print_mode, const std::string &format)
        : _duration(duration),
          _print_mode(print_mode),
          _format(format)
    {
        // Nothing to do.
    }

    /// @brief Returns a zero duration.
    /// @return A zero duration value.
    static inline duration_type_t zero()
    {
        return duration_type_t::zero();
    }

    /// @brief Returns the duration count.
    /// @return The duration as a double value.
    inline double count() const
    {
        return _duration.count();
    }

    /// @brief Operator to get the duration count.
    /// @return The duration as a double value.
    inline double operator()() const
    {
        return _duration.count();
    }

    /// @brief Sets the print mode.
    /// @param print_mode The new print mode to set.
    inline void set_print_mode(print_mode_t print_mode)
    {
        _print_mode = print_mode;
    }

    /// @brief Sets the format for printing the duration.
    /// @param format The format string to set.
    inline void set_format(const std::string &format)
    {
        _format = format;
    }

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

    /// @brief Divides two Duration objects.
    /// @param rhs The right-hand side Duration to divide.
    /// @return A new Duration object representing the quotient.
    template <typename T>
    inline Duration operator/(const Duration &rhs) const
    {
        return Duration(duration_type_t(_duration / rhs._duration), _print_mode, _format);
    }

    /// @brief Divides the Duration by a scalar value.
    /// @param rhs The scalar value to divide by.
    /// @return A new Duration object representing the quotient.
    template <typename T>
    inline Duration operator/(const T &rhs) const
    {
        return Duration(_duration / rhs, _print_mode, _format);
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
        _duration = duration_type_t(_duration / rhs._duration);
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
    inline Duration &operator=(const duration_type_t &rhs)
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
#if __cplusplus < 201103L
            ss << _duration.to_nanoseconds<double>() * 1e-09;
#else
            ss << (static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(_duration).count()) * 1e-09);
#endif
        } else {
#if __cplusplus < 201103L
            time_t h, m, s, ms, us, ns = _duration.to_nanoseconds<time_t>();
            // Extract the values.
            h  = detail::ns_to_hours(ns, &ns);
            m  = detail::ns_to_minutes(ns, &ns);
            s  = detail::ns_to_seconds(ns, &ns);
            ms = detail::ns_to_milliseconds(ns, &ns);
            us = detail::ns_to_microseconds(ns, &ns);
#else
            duration_type_t duration = _duration;
            // Extract the values.
            time_t h = std::chrono::duration_cast<std::chrono::hours>(duration).count();
            duration -= std::chrono::duration_cast<std::chrono::hours>(duration);
            time_t m = std::chrono::duration_cast<std::chrono::minutes>(duration).count();
            duration -= std::chrono::duration_cast<std::chrono::minutes>(duration);
            time_t s = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
            duration -= std::chrono::duration_cast<std::chrono::seconds>(duration);
            time_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            duration -= std::chrono::duration_cast<std::chrono::milliseconds>(duration);
            time_t us = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
            duration -= std::chrono::duration_cast<std::chrono::microseconds>(duration);
            time_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
#endif
            if (_print_mode == human) {
                if (h)
                    ss << std::setw(3) << h << "H ";
                if (m)
                    ss << std::setw(3) << m << "M ";
                if (s)
                    ss << std::setw(3) << s << "s ";
                if (ms)
                    ss << std::setw(3) << ms << "m ";
                if (us)
                    ss << std::setw(3) << us << "u ";
                if (ns)
                    ss << std::setw(3) << ns << "n ";
            } else if (_print_mode == numeric) {
                ss << h << "." << m << "." << s << "." << ms << "." << us << "." << ns;
            } else if (!_format.empty()) {
                std::string output = _format;
                replace(output, "%H", to_string(h));
                replace(output, "%M", to_string(m));
                replace(output, "%s", to_string(s));
                replace(output, "%m", to_string(ms));
                replace(output, "%u", to_string(us));
                replace(output, "%n", to_string(ns));
                ss << output;
            }
        }
        return ss.str();
    }

    /// @brief Prints the Duration to an output stream.
    /// @param lhs The output stream.
    /// @param rhs The Duration to print.
    /// @return The modified output stream.
    friend std::ostream &operator<<(std::ostream &lhs, const Duration &rhs)
    {
        lhs << rhs.to_string();
        return lhs;
    }

private:
    /// @brief Replaces all occurrences of a substring in a string.
    /// @param s The string to modify.
    /// @param substring The substring to replace.
    /// @param substitute The string to replace it with.
    /// @param occurences The number of occurrences to replace (-1 for all).
    /// @return A reference to the modified string.
    static inline std::string &replace(std::string &s, const std::string &substring, const std::string &substitute, int occurences = -1)
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

    /// @brief Transforms the given value into a string.
    /// @param value the value to transform.
    /// @return the string representation of the value.
    template <typename T>
    static inline std::string to_string(const T &value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    /// @brief Stores the duration value.
    duration_type_t _duration;
    /// @brief Specifies the print mode (e.g., human-readable, numeric).
    print_mode_t _print_mode;
    /// @brief Format string used for custom printing.
    std::string _format;
};

} // namespace stopwatch
