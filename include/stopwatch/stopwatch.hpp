/// @file stopwatch.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Benchmarking classes.

#pragma once

#include <iostream>
#include <utility>
#include <iomanip>
#include <vector>
#include <sstream>

#if __cplusplus < 201103L
#include "timespec_support.hpp"
#else
#include <chrono>
#endif

namespace stopwatch
{

#if __cplusplus < 201103L
typedef timespec_t clock_type_t;
typedef timespec_t time_point_type_t;
typedef timespec_t duration_type_t;
typedef timespec_t elapsed_time_t;
#else
typedef std::chrono::high_resolution_clock clock_type_t;
typedef std::chrono::high_resolution_clock::time_point time_point_type_t;
typedef std::chrono::duration<double> duration_type_t;
typedef std::chrono::nanoseconds elapsed_time_t;
#endif

/// @brief The way the stopwatch prints the elapsed time.
enum PrintMode {
    human,   ///< Human readable   :  1h  4m  2s   1m 153u 399n
    numeric, ///< Numeric          :  1.4.2.1.153.399
    total,   ///< Total elapsed    :
    custom   ///< Use placeholders : %H,%M,%s,%m,%u,%n
};

class Duration {
public:
    Duration(duration_type_t duration, PrintMode print_mode, const std::string &format)
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

    inline void set_print_mode(PrintMode print_mode)
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
        return Duration(_duration + rhs._duration, _print_mode, _format);
    }

    template <typename T>
    inline Duration operator/(const T &rhs) const
    {
        return Duration(_duration + rhs, _print_mode, _format);
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
        _duration = duration_type_t(_duration / rhs);
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
    /// @brief Replaces all occurences of the given substring.
    /// @param s the input string.
    /// @param substring the substring that should be replaced.
    /// @param substitute the substitute.
    /// @param occurences how many occurences should we replace (-1 = all of them).
    /// @return a reference to the modified string.
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

    duration_type_t _duration;
    PrintMode _print_mode;
    std::string _format;
};

class Stopwatch {
public:
    Stopwatch(PrintMode print_mode = human, const std::string &format = std::string())
        : _last_time_point(clock_type_t::now()),
          _total_duration(Duration::zero(), print_mode, format),
          _partials(),
          _print_mode(print_mode),
          _format(format)
    {
        // Nothing to do.
    }

    inline void set_print_mode(PrintMode print_mode)
    {
        _print_mode = print_mode;
        _total_duration.set_print_mode(print_mode);
        for (std::vector<Duration>::iterator it = _partials.begin(); it != _partials.end(); ++it)
            it->set_print_mode(print_mode);
    }

    inline void set_format(const std::string &format)
    {
        _format = format;
        _total_duration.set_format(format);
        for (std::vector<Duration>::iterator it = _partials.begin(); it != _partials.end(); ++it)
            it->set_format(format);
    }

    inline void reset()
    {
        // Reset the total duration.
        _total_duration = duration_type_t::zero();
        // Clear all the partials.
        _partials.clear();
        // Star again the timer.
        this->start();
    }

    inline void start()
    {
        _last_time_point = clock_type_t::now();
    }

    inline void round()
    {
        time_point_type_t now  = clock_type_t::now();
        elapsed_time_t elapsed = now - _last_time_point;
        _last_time_point       = now;
        _total_duration += elapsed;
        _partials.push_back(Duration(elapsed, _print_mode, _format));
    }

    Duration last_round() const
    {
        if (_partials.empty())
            return Duration(clock_type_t::now() - _last_time_point, _print_mode, _format);
        return _partials.back();
    }

    Duration total() const
    {
        return _total_duration;
    }

    Stopwatch mean() const
    {
        Stopwatch sw(*this);
        sw._total_duration /= _partials.size();
        sw._partials.clear();
        return sw;
    }

    inline std::vector<Duration> partials() const
    {
        return _partials;
    }

    virtual std::string to_string() const
    {
        return _total_duration.to_string();
    }

    inline Duration &operator[](std::size_t position)
    {
        if (position < _partials.size())
            return _partials[position];
        throw std::out_of_range("Out of range of partial times.");
    }

    inline const Duration &operator[](std::size_t position) const
    {
        if (position < _partials.size())
            return _partials[position];
        throw std::out_of_range("Out of range of partial times.");
    }

    friend std::ostream &operator<<(std::ostream &lhs, const Stopwatch &rhs)
    {
        lhs << rhs.to_string();
        return lhs;
    }

private:
    time_point_type_t _last_time_point;
    Duration _total_duration;
    std::vector<Duration> _partials;
    PrintMode _print_mode;
    std::string _format;
};

/// @brief Runs the function and samples the elapsed time.
/// @param stopwatch the stopwatch used to retrieve the elapse time.
/// @param function the function to sample.
/// @return the same stopwatch passed as argument.
template <class Function>
inline Stopwatch &time(Stopwatch &stopwatch, const Function &function)
{
    stopwatch.reset();
    function();
    stopwatch.round();
    return stopwatch;
}

/// @brief Runs the function N times and samples the elapsed time.
/// @param stopwatch the stopwatch used to retrieve the elapse time.
/// @param function the function to sample.
/// @return the same stopwatch passed as argument.
template <std::size_t N, class Function>
inline Stopwatch &ntimes(Stopwatch &stopwatch, const Function &function)
{
    stopwatch.reset();
    for (std::size_t i = 0u; i < N; ++i) {
        function();
        stopwatch.round();
    }
    return stopwatch;
}

} // namespace stopwatch
