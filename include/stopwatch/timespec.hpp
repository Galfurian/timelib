/// @file timespec.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Defines the timespec_t class, which extends the timespec structure.
///
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
///

#pragma once

#define _USE_32BIT_TIME_T

#include <time.h>

namespace stopwatch
{

namespace detail
{

static const time_t ns_per_hour        = 3600000000000UL; ///< For the conversion between nanoseconds and hours.
static const time_t ns_per_minute      = 60000000000UL;   ///< For the conversion between nanoseconds and minutes.
static const time_t ns_per_second      = 1000000000UL;    ///< For the conversion between nanoseconds and seconds.
static const time_t ns_per_millisecond = 1000000UL;       ///< For the conversion between nanoseconds and milliseconds.
static const time_t ns_per_microsecond = 1000UL;          ///< For the conversion between nanoseconds and microseconds.

/// @brief Transforms the value from hours to nanoseconds.
/// @param value the value to transform.
/// @return the transformed value.
inline time_t hours_to_ns(time_t value)
{
    return value * ns_per_hour;
}

/// @brief Transforms the value from minutes to nanoseconds.
/// @param value the value to transform.
/// @return the transformed value.
inline time_t minutes_to_ns(time_t value)
{
    return value * ns_per_minute;
}

/// @brief Transforms the value from seconds to nanoseconds.
/// @param value the value to transform.
/// @return the transformed value.
inline time_t seconds_to_ns(time_t value)
{
    return value * ns_per_second;
}

/// @brief Transforms the value from milliseconds to nanoseconds.
/// @param value the value to transform.
/// @return the transformed value.
inline time_t milliseconds_to_ns(time_t value)
{
    return value * ns_per_millisecond;
}

/// @brief Transforms the value from microseconds to nanoseconds.
/// @param value the value to transform.
/// @return the transformed value.
inline time_t microseconds_to_ns(time_t value)
{
    return value * ns_per_microsecond;
}

/// @brief Turns nanoseconds into hours.
/// @param value the value to transform.
/// @param remainder the reminder of the transformation.
/// @return the time value in hours.
inline time_t ns_to_hours(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_hour;
    if (remainder)
        *remainder = value % ns_per_hour;
    return result;
}

/// @brief Turns nanoseconds into minutes.
/// @param value the value to transform.
/// @param remainder the reminder of the transformation.
/// @return the time value in minutes.
inline time_t ns_to_minutes(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_minute;
    if (remainder)
        *remainder = value % ns_per_minute;
    return result;
}

/// @brief Turns nanoseconds into seconds.
/// @param value the value to transform.
/// @param remainder the reminder of the transformation.
/// @return the time value in seconds.
inline time_t ns_to_seconds(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_second;
    if (remainder)
        *remainder = value % ns_per_second;
    return result;
}

/// @brief Turns nanoseconds into milliseconds.
/// @param value the value to transform.
/// @param remainder the reminder of the transformation.
/// @return the time value in milliseconds.
inline time_t ns_to_milliseconds(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_millisecond;
    if (remainder)
        *remainder = value % ns_per_millisecond;
    return result;
}

/// @brief Turns nanoseconds into microseconds.
/// @param value the value to transform.
/// @param remainder the reminder of the transformation.
/// @return the time value in microseconds.
inline time_t ns_to_microseconds(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_microsecond;
    if (remainder)
        *remainder = value % ns_per_microsecond;
    return result;
}

} // namespace detail

/// @brief A wrapper class for the timespec.
class timespec_t : public timespec {
public:
    /// @brief Creates an empty timespec.
    timespec_t()
        : timespec()
    {
        // Nothing to do.
    }

    /// @brief Copies another timespec.
    timespec_t(const timespec_t &other)
        : timespec()
    {
        tv_sec = other.tv_sec, tv_nsec = other.tv_nsec;
    }

    /// @brief Creates a new timespec starting from a nanosecond value.
    /// @param ns
    timespec_t(time_t ns)
        : timespec()
    {
        tv_sec = detail::ns_to_seconds(ns, reinterpret_cast<time_t *>(&tv_nsec));
        this->normalize();
    }

    /// @brief Returns the timespec for now.
    static inline timespec_t now()
    {
        timespec_t ts;
#ifdef _WIN32
        timespec_get(&ts, TIME_UTC);
#else
        clock_gettime(CLOCK_REALTIME, &ts);
#endif
        return ts;
    }

    /// @brief Creates a zero timespec.
    static inline timespec_t zero()
    {
        timespec_t ts;
        ts.tv_sec  = 0;
        ts.tv_nsec = 0;
        return ts;
    }

    /// @brief Normalizes the timespec.
    inline timespec_t &normalize()
    {
        while (tv_nsec >= detail::ns_per_second) {
            ++(tv_sec);
            tv_nsec -= detail::ns_per_second;
        }
        while (tv_nsec <= -detail::ns_per_second) {
            --(tv_sec);
            tv_nsec += detail::ns_per_second;
        }
        if (tv_nsec < 0) {
            --(tv_sec);
            tv_nsec = (detail::ns_per_second + tv_nsec);
        }
        return *this;
    }

    /// @brief Transforms the timespec into nanoseconds.
    template <typename T>
    inline T to_nanoseconds() const
    {
        return static_cast<T>((tv_sec * detail::ns_per_second) + tv_nsec);
    }

    /// @brief Transforms the timespec into microseconds.
    template <typename T>
    inline T to_microseconds() const
    {
        return static_cast<T>(detail::ns_to_microseconds(this->to_nanoseconds<T>()));
    }

    /// @brief Transforms the timespec into milliseconds.
    template <typename T>
    inline T to_milliseconds() const
    {
        return static_cast<T>(detail::ns_to_milliseconds(this->to_nanoseconds<T>()));
    }

    /// @brief Transforms the timespec into seconds.
    template <typename T>
    inline T to_seconds() const
    {
        return static_cast<T>(detail::ns_to_seconds(this->to_nanoseconds<T>()));
    }

    /// @brief Transforms the timespec into minutes.
    template <typename T>
    inline T to_minutes() const
    {
        return static_cast<T>(detail::ns_to_minutes(this->to_nanoseconds<T>()));
    }

    /// @brief Transforms the timespec into hours.
    template <typename T>
    inline T to_hours() const
    {
        return static_cast<T>(detail::ns_to_hours(this->to_nanoseconds<T>()));
    }

    /// @brief Sum operator.
    inline friend timespec_t operator+(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() + rhs.to_nanoseconds<time_t>());
    }

    /// @brief Sum operator.
    template <typename T>
    inline friend timespec_t operator+(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() + rhs);
    }

    /// @brief Subtract operator.
    inline friend timespec_t operator-(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() - rhs.to_nanoseconds<time_t>());
    }

    /// @brief Subtract operator.
    template <typename T>
    inline friend timespec_t operator-(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() - rhs);
    }

    /// @brief Multiply operator.
    inline friend timespec_t operator*(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() * rhs.to_nanoseconds<time_t>());
    }

    /// @brief Multiply operator.
    template <typename T>
    inline friend timespec_t operator*(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() * rhs);
    }

    /// @brief Divide operator.
    inline friend timespec_t operator/(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() / rhs.to_nanoseconds<time_t>());
    }

    /// @brief Divide operator.
    template <typename T>
    inline friend timespec_t operator/(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() / static_cast<time_t>(rhs));
    }

    /// @brief Sum and assign operator.
    inline friend timespec_t &operator+=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs + rhs);
    }

    /// @brief Sum and assign operator.
    template <typename T>
    inline friend timespec_t &operator+=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs + rhs);
    }

    /// @brief Subtract and assign operator.
    inline friend timespec_t &operator-=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs - rhs);
    }

    /// @brief Subtract and assign operator.
    template <typename T>
    inline friend timespec_t &operator-=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs - rhs);
    }

    /// @brief Multiply and assign operator.
    inline friend timespec_t &operator*=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs * rhs);
    }

    /// @brief Multiply and assign operator.
    template <typename T>
    inline friend timespec_t &operator*=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs * rhs);
    }

    /// @brief Divide and assign operator.
    inline friend timespec_t &operator/=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs / rhs);
    }

    /// @brief Divide and assign operator.
    template <typename T>
    inline friend timespec_t &operator/=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs / rhs);
    }

    /// @brief Assignment operator.
    inline timespec_t &operator=(const timespec_t &other)
    {
        tv_sec = other.tv_sec, tv_nsec = other.tv_nsec;
        return *this;
    }
};

} // namespace stopwatch
