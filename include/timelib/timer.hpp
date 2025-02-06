/// @file timer.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Benchmarking classes.

#pragma once

#include <utility>

#include <algorithm>

#include "timelib/duration.hpp"

namespace timelib
{

/// @brief A class that represents a timer for benchmarking.
class Timer
{
public:
    /// @brief Constructs a Timer object.
    /// @param print_mode The mode for printing the duration (default is human-readable).
    /// @param format The format to be used for printing (default is an empty string).
    Timer(print_mode_t print_mode = human, std::string format = std::string())
        : _initial_time_point(timespec_t::now())
        , _print_mode(print_mode)
        , _format(std::move(format))
        , _accumulated(0.)
        , _timeout(0.)
    {
        // Nothing to do.
    }

    /// @brief Sets the print mode for the Timer.
    /// @param print_mode The new print mode to set.
    void set_print_mode(print_mode_t print_mode) { _print_mode = print_mode; }

    /// @brief Sets the format string for the Timer.
    /// @param format The format string to set.
    void set_format(const std::string &format) { _format = format; }

    /// @brief Sets a new target duration for the Timer.
    /// @param value The target duration (float: seconds, integral: nanoseconds).
    template <typename T>
    void set_timeout(T value)
    {
        _timeout = timespec_t(value);
    }

    /// @brief Gets the target duration.
    /// @return The target duration.
    auto get_timeout() const -> Duration { return Duration(_timeout, _print_mode, _format); }

    /// @brief Resets the Timer, clearing the total duration and setting the
    /// start time to now.
    void reset()
    {
        _initial_time_point = timespec_t::now();
        _accumulated        = 0.;
    }

    /// @brief Starts the Timer by setting the initial time point to now.
    void start() { _initial_time_point = timespec_t::now(); }

    /// @brief Stops the Timer, resets it, and returns the elapsed duration.
    /// @return The elapsed Duration since the timer started.
    auto stop() -> Duration
    {
        timespec_t elapsed = this->raw_elapsed();
        this->reset();
        return Duration(elapsed, _print_mode, _format);
    }

    /// @brief Pausese the Timer.
    void pause() { _accumulated = this->raw_elapsed(); }

    /// @brief Returns the total elapsed time without resetting the Timer.
    /// @return The total elapsed Duration.
    auto elapsed() const -> Duration { return Duration(this->raw_elapsed(), _print_mode, _format); }

    /// @brief Returns the remaining time until the target duration is reached.
    /// @return The remaining Duration until the target is reached, or zero if the target is exceeded.
    auto remaining() const -> Duration
    {
        double remaining_time = _timeout.count() - this->raw_elapsed().count();
        // No remaining time if target is exceeded.
        remaining_time        = std::max<double>(remaining_time, 0);
        return Duration(remaining_time, _print_mode, _format);
    }

    /// @brief Checks if the elapsed time of the Timer exceeds a given threshold in seconds.
    /// @return True if the elapsed time exceeds the target duration, otherwise false.
    auto has_timeout() const -> bool
    {
        // If we do not have a _timeout set, we do not perform the check.
        if (!_timeout) {
            return false;
        }
        // Compare the elapsed time with the target duration.
        return this->raw_elapsed() > _timeout;
    }

    /// @brief Converts the Timer's total duration to a string.
    /// @return A string representation of the total duration.
    auto to_string() const -> std::string { return this->elapsed().to_string(); }

    /// @brief Prints the Timer's total duration to an output stream.
    /// @param lhs The output stream.
    /// @param rhs The Timer to print.
    /// @return The modified output stream.
    friend auto operator<<(std::ostream &lhs, const Timer &rhs) -> std::ostream &
    {
        lhs << rhs.elapsed().to_string();
        return lhs;
    }

private:
    /// @brief Returns the total elapsed time without resetting the Timer.
    /// @return The total elapsed Duration.
    auto raw_elapsed() const -> timespec_t { return timespec_t::now() - _initial_time_point + _accumulated; }

    /// @brief The starting time point of the Timer.
    timespec_t _initial_time_point;
    /// @brief The print mode (e.g., human-readable or numeric).
    print_mode_t _print_mode;
    /// @brief The format string used for printing.
    std::string _format;
    /// @brief The accumulated time, in case we pause the timer.
    timespec_t _accumulated;
    /// @brief The target duration in seconds for the Timer.
    timespec_t _timeout;
};

} // namespace timelib
