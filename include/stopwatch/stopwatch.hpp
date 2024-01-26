/// @file stopwatch.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Defines the stopwatch class, used for benchmarking.
/// 
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
/// 

#pragma once

#include <iostream>
#include <utility>
#include <iomanip>
#include <vector>
#include <sstream>

#include "stopwatch/duration.hpp"

namespace stopwatch
{

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

    inline Duration round()
    {
        time_point_type_t now  = clock_type_t::now();
        elapsed_time_t elapsed = now - _last_time_point;
        _last_time_point       = now;
        _total_duration += elapsed;
        Duration duration(elapsed, _print_mode, _format);
        _partials.push_back(duration);
        return duration;
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

    Duration mean() const
    {
        return _total_duration / _partials.size();
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
