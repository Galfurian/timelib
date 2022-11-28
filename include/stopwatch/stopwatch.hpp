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

/// @brief The way the stopwatch prints the elapsed time.
enum PrintMode {
    human,   ///< Human readable :  1h  4m  2s   1m 153u 399n
    numeric, ///< Numeric        :  1.4.2.1.153.399
    total    ///< Total elapsed  :
};

class Duration {
public:
#if __cplusplus < 201103L
    typedef timespec_t duration_type_t;
#else
    typedef std::chrono::duration<double> duration_type_t;
#endif

    Duration(duration_type_t duration, PrintMode print_mode)
        : _duration(duration),
          _print_mode(print_mode)
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

    inline Duration operator+(const Duration &rhs) const
    {
        Duration new_duration = *this;
        new_duration._duration += rhs._duration;
        return new_duration;
    }

    template <typename T>
    inline Duration operator+(const T &rhs) const
    {
        Duration new_duration = *this;
        new_duration._duration += rhs;
        return new_duration;
    }

    template <typename T>
    inline Duration operator/(const T &rhs) const
    {
        Duration new_duration = *this;
        new_duration._duration /= rhs;
        return new_duration;
    }

    inline Duration &operator+=(const Duration &rhs)
    {
        _duration += rhs._duration;
        return *this;
    }

    template <typename T>
    inline Duration &operator+=(const T &rhs)
    {
        _duration += rhs;
        return *this;
    }

    template <typename T>
    inline Duration &operator/=(const T &rhs)
    {
        _duration /= rhs;
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
        // Get the duration.
        duration_type_t duration = _duration;
        if (_print_mode == human) {
#if __cplusplus < 201103L
            unsigned h, m, s, ms, us, ns;
            if ((h = duration.to_hours<unsigned>())) {
                ss << std::setw(3) << h << "H ";
                duration -= detail::hours_to_ns(h);
            }
            if ((m = duration.to_minutes<unsigned>())) {
                ss << std::setw(3) << m << "M ";
                duration -= detail::minutes_to_ns(m);
            }
            if ((s = duration.to_seconds<unsigned>())) {
                ss << std::setw(3) << s << "s ";
                duration -= detail::seconds_to_ns(s);
            }
            if ((ms = duration.to_milliseconds<unsigned>())) {
                ss << std::setw(3) << ms << "m ";
                duration -= detail::milliseconds_to_ns(ms);
            }
            if ((us = duration.to_microseconds<unsigned>())) {
                ss << std::setw(3) << us << "u ";
                duration -= detail::microseconds_to_ns(us);
            }
            if ((ns = duration.to_nanoseconds<unsigned>())) {
                ss << std::setw(3) << ns << "n ";
                duration -= ns;
            }
#else
            auto h = std::chrono::duration_cast<std::chrono::hours>(duration);
            if (h.count()) {
                ss << std::setw(3) << h.count() << "H ";
                duration -= h;
            }
            auto m = std::chrono::duration_cast<std::chrono::minutes>(duration);
            if (m.count()) {
                ss << std::setw(3) << m.count() << "M ";
                duration -= m;
            }
            auto s = std::chrono::duration_cast<std::chrono::seconds>(duration);
            if (s.count()) {
                ss << std::setw(3) << s.count() << "s ";
                duration -= s;
            }
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
            if (ms.count()) {
                ss << std::setw(3) << ms.count() << "m ";
                duration -= ms;
            }
            auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
            if (us.count()) {
                ss << std::setw(3) << us.count() << "u ";
                duration -= us;
            }
            auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
            if (ns.count()) {
                ss << std::setw(3) << ns.count() << "n ";
                duration -= ns;
            }
#endif
        } else if (_print_mode == numeric) {
#if __cplusplus < 201103L
            unsigned h, m, s, ms, us, ns;
            ss << (h = duration.to_hours<unsigned>()) << ".";
            duration -= detail::hours_to_ns(h);
            ss << (m = duration.to_minutes<unsigned>()) << ".";
            duration -= detail::minutes_to_ns(m);
            ss << (s = duration.to_seconds<unsigned>()) << ".";
            duration -= detail::seconds_to_ns(s);
            ss << (ms = duration.to_milliseconds<unsigned>()) << ".";
            duration -= detail::milliseconds_to_ns(ms);
            ss << (us = duration.to_microseconds<unsigned>()) << ".";
            duration -= detail::microseconds_to_ns(us);
            ss << (ns = duration.to_nanoseconds<unsigned>()) << ".";
            duration -= ns;
#else
            auto h = std::chrono::duration_cast<std::chrono::hours>(duration);
            ss << h.count() << ".";
            duration -= h;
            auto m = std::chrono::duration_cast<std::chrono::minutes>(duration);
            ss << m.count() << ".";
            duration -= m;
            auto s = std::chrono::duration_cast<std::chrono::seconds>(duration);
            ss << s.count() << ".";
            duration -= s;
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
            ss << ms.count() << ".";
            duration -= ms;
            auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
            ss << us.count() << ".";
            duration -= us;
            auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
            ss << ns.count();
#endif
            duration -= ns;
        } else {
#if __cplusplus < 201103L
            ss << duration.to_nanoseconds<double>() * 1e-09;
#else
            ss << (static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) * 1e-09);
#endif
        }
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &lhs, const Duration &rhs)
    {
        lhs << rhs.to_string();
        return lhs;
    }

private:
    duration_type_t _duration;
    PrintMode _print_mode;
};

class Stopwatch {
public:
#if __cplusplus < 201103L
    typedef timespec_t clock_type_t;
    typedef timespec_t time_point_type_t;
#else
    typedef std::chrono::high_resolution_clock clock_type_t;
    typedef clock_type_t::time_point time_point_type_t;
#endif

    Stopwatch(PrintMode print_mode = human)
        : _last_time_point(clock_type_t::now()),
          _total_duration(Duration::zero(), print_mode),
          _partials(),
          _print_mode(print_mode)
    {
        // Nothing to do.
    }

    inline void set_print_mode(PrintMode print_mode)
    {
        _print_mode = print_mode;
        _total_duration.set_print_mode(print_mode);
    }

    inline void reset()
    {
#if __cplusplus < 201103L
        _total_duration = Duration(Duration::zero(), _print_mode);
#else
        _total_duration                  = std::chrono::duration<double>::zero();
#endif
        _partials.clear();
        // Star again the timer.
        this->start();
    }

    inline void start()
    {
#if __cplusplus < 201103L
        _last_time_point = time_point_type_t::now();
#else
        _last_time_point                 = clock_type_t::now();
#endif
    }

    inline void round()
    {
#if __cplusplus < 201103L
        time_point_type_t now     = time_point_type_t::now();
        time_point_type_t elapsed = now - _last_time_point;
#else
        time_point_type_t now            = clock_type_t::now();
        std::chrono::nanoseconds elapsed = now - _last_time_point;
#endif
        _last_time_point = now;
        _total_duration += elapsed;
        _partials.push_back(Duration(elapsed, _print_mode));
    }

    Duration last_round() const
    {
        if (_partials.empty()) {
#if __cplusplus < 201103L
            time_point_type_t now = time_point_type_t::now();
#else
            time_point_type_t now = clock_type_t::now();
#endif
            return Duration(now - _last_time_point, _print_mode);
        }
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
