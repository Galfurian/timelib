/// @file duration.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Defines the duration class.
/// 
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
/// 

#pragma once

#include "stopwatch/details.hpp"

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

    static inline duration_type_t zero()
    {
        return duration_type_t::zero();
    }

    inline double count() const
    {
        return _duration.count();
    }

    inline double operator()() const
    {
        return _duration.count();
    }

    inline void set_print_mode(print_mode_t print_mode)
    {
        _print_mode = print_mode;
    }

    inline void set_format(const std::string &format)
    {
        _format = format;
    }

    inline Duration operator+(const Duration &rhs) const
    {
        return Duration(_duration + rhs._duration, _print_mode, _format);
    }

    template <typename T>
    inline Duration operator+(const T &rhs) const
    {
        return Duration(_duration + rhs, _print_mode, _format);
    }

    inline Duration operator-(const Duration &rhs) const
    {
        return Duration(_duration - rhs._duration, _print_mode, _format);
    }

    template <typename T>
    inline Duration operator-(const T &rhs) const
    {
        return Duration(_duration - rhs, _print_mode, _format);
    }

    template <typename T>
    inline Duration operator/(const Duration &rhs) const
    {
        return Duration(duration_type_t(_duration / rhs._duration), _print_mode, _format);
    }

    template <typename T>
    inline Duration operator/(const T &rhs) const
    {
        return Duration(_duration / rhs, _print_mode, _format);
    }

    inline Duration &operator+=(const Duration &rhs)
    {
        _duration = _duration + rhs._duration;
        return *this;
    }

    template <typename T>
    inline Duration &operator+=(const T &rhs)
    {
        _duration = _duration + rhs;
        return *this;
    }

    inline Duration &operator-=(const Duration &rhs)
    {
        _duration = _duration - rhs._duration;
        return *this;
    }

    template <typename T>
    inline Duration &operator-=(const T &rhs)
    {
        _duration = _duration - rhs;
        return *this;
    }

    template <typename T>
    inline Duration &operator/=(const Duration &rhs)
    {
        _duration = duration_type_t(_duration / rhs._duration);
        return *this;
    }

    template <typename T>
    inline Duration &operator/=(const T &rhs)
    {
        _duration = _duration / rhs;
        return *this;
    }

    inline Duration &operator=(const duration_type_t &rhs)
    {
        _duration = rhs;
        return *this;
    }

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

    friend std::ostream &operator<<(std::ostream &lhs, const Duration &rhs)
    {
        lhs << rhs.to_string();
        return lhs;
    }

private:
    /// @brief Replaces all occurrences of the given substring.
    /// @param s the input string.
    /// @param substring the substring that should be replaced.
    /// @param substitute the substitute.
    /// @param occurrences how many occurrences should we replace (-1 = all of them).
    /// @return a reference to the modified string.
    static inline std::string &replace(std::string &s, const std::string &substring, const std::string &substitute, int occurrences = -1)
    {
        // Find the first occurrence.
        std::string::size_type pos = s.find(substring);
        // Iterate until the end of the string.
        while (pos != std::string::npos) {
            // Replace the occurrence.
            s.replace(pos, substring.size(), substitute);
            // Check how many of them we still need to replace.
            if ((occurrences > 0) && ((--occurrences) == 0))
                break;
            // Advance to the next occurrence.
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

    duration_type_t _duration;
    print_mode_t _print_mode;
    std::string _format;
};

} // namespace stopwatch
