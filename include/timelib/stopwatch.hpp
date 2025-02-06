/// @file stopwatch.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Defines the stopwatch class, used for benchmarking.
///
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
///

#pragma once

#include "timelib/duration.hpp"

#include <vector>

namespace timelib
{

/// @brief A class that represents a stopwatch for benchmarking.
class Stopwatch
{
public:
    /// @brief Constructs a Stopwatch object.
    /// @param print_mode The mode for printing durations (default is human-readable).
    /// @param format The format to be used for printing (default is an empty string).
    Stopwatch(print_mode_t print_mode = human, const std::string &format = std::string())
        : _last_time_point(timespec_t::now())
        , _total_duration(Duration::zero(), print_mode, format)
        , _partials()
        , _print_mode(print_mode)
        , _format(format)
    {
        // Nothing to do.
    }

    virtual ~Stopwatch() = default;

    /// @brief Sets the print mode for the Stopwatch.
    /// @param print_mode The new print mode to set.
    inline void set_print_mode(print_mode_t print_mode)
    {
        _print_mode = print_mode;
        _total_duration.set_print_mode(print_mode);
        for (std::vector<Duration>::iterator it = _partials.begin(); it != _partials.end(); ++it)
            it->set_print_mode(print_mode);
    }

    /// @brief Sets the format string for the Stopwatch.
    /// @param format The format string to set.
    inline void set_format(const std::string &format)
    {
        _format = format;
        _total_duration.set_format(format);
        for (std::vector<Duration>::iterator it = _partials.begin(); it != _partials.end(); ++it)
            it->set_format(format);
    }

    /// @brief Resets the Stopwatch by clearing all rounds and restarting the timer.
    inline void reset()
    {
        // Reset the total duration.
        _total_duration = timespec_t::zero();
        // Clear all the partials.
        _partials.clear();
        // Star again the timer.
        this->start();
    }

    /// @brief Starts or resumes the Stopwatch from the current time.
    inline void start() { _last_time_point = timespec_t::now(); }

    /// @brief Records a round, calculates the time since the last round, and updates the total duration.
    /// @return The Duration of the last round.
    inline Duration round()
    {
        timespec_t now     = timespec_t::now();
        timespec_t elapsed = now - _last_time_point;
        _last_time_point   = now;
        _total_duration += elapsed;
        Duration duration(elapsed, _print_mode, _format);
        _partials.push_back(duration);
        return duration;
    }

    /// @brief Returns the duration of the last recorded round.
    /// @return The Duration of the last round.
    Duration last_round() const
    {
        if (_partials.empty()) {
            return Duration(timespec_t::now() - _last_time_point, _print_mode, _format);
        }
        return _partials.back();
    }

    /// @brief Returns the total elapsed time since the Stopwatch was started.
    /// @return The total Duration.
    Duration total() const { return _total_duration; }

    /// @brief Calculates the average duration of all recorded rounds.
    /// @return The mean Duration of the rounds.
    Duration mean() const { return _total_duration / static_cast<double>(_partials.size()); }

    /// @brief Returns all the partial durations (rounds) recorded by the Stopwatch.
    /// @return A vector of Duration representing each round.
    inline std::vector<Duration> partials() const { return _partials; }

    /// @brief Converts the Stopwatch's total duration to a string.
    /// @return A string representation of the total duration.
    virtual std::string to_string() const
    {
        if (_partials.empty()) {
            return Duration(timespec_t::now() - _last_time_point, _print_mode, _format).to_string();
        }
        return _total_duration.to_string();
    }

    /// @brief Accesses the Duration of a specific round by index.
    /// @param position The index of the round.
    /// @return A reference to the Duration of the round.
    /// @throw std::out_of_range if the index is out of bounds.
    inline Duration &operator[](std::size_t position)
    {
        if (position < _partials.size())
            return _partials[position];
        throw std::out_of_range("Out of range of partial times.");
    }

    /// @brief Accesses the Duration of a specific round by index (const version).
    /// @param position The index of the round.
    /// @return A const reference to the Duration of the round.
    /// @throw std::out_of_range if the index is out of bounds.
    inline const Duration &operator[](std::size_t position) const
    {
        if (position < _partials.size())
            return _partials[position];
        throw std::out_of_range("Out of range of partial times.");
    }

    /// @brief Prints the Stopwatch's total duration to an output stream.
    /// @param lhs The output stream.
    /// @param rhs The Stopwatch to print.
    /// @return The modified output stream.
    friend std::ostream &operator<<(std::ostream &lhs, const Stopwatch &rhs)
    {
        lhs << rhs.to_string();
        return lhs;
    }

private:
    /// @brief The time point of the last round or start.
    timespec_t _last_time_point;
    /// @brief The total duration since the Stopwatch started.
    Duration _total_duration;
    /// @brief Stores all partial (round) durations.
    std::vector<Duration> _partials;
    /// @brief The print mode (e.g., human-readable or numeric).
    print_mode_t _print_mode;
    /// @brief The format string used for printing durations.
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

} // namespace timelib
