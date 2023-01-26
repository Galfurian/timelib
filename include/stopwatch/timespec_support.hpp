/// @file stopwatch.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Benchmarking classes.

#pragma once

#define _USE_32BIT_TIME_T

#include <time.h>

namespace stopwatch
{

namespace detail
{

static const time_t ns_per_hour        = 3600000000000UL;
static const time_t ns_per_minute      = 60000000000UL;
static const time_t ns_per_second      = 1000000000UL;
static const time_t ns_per_millisecond = 1000000UL;
static const time_t ns_per_microsecond = 1000UL;

inline time_t hours_to_ns(time_t value)
{
    return value * ns_per_hour;
}

inline time_t minutes_to_ns(time_t value)
{
    return value * ns_per_minute;
}

inline time_t seconds_to_ns(time_t value)
{
    return value * ns_per_second;
}

inline time_t milliseconds_to_ns(time_t value)
{
    return value * ns_per_millisecond;
}

inline time_t microseconds_to_ns(time_t value)
{
    return value * ns_per_microsecond;
}

inline time_t ns_to_hours(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_hour;
    if (remainder)
        *remainder = value % ns_per_hour;
    return result;
}

inline time_t ns_to_minutes(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_minute;
    if (remainder)
        *remainder = value % ns_per_minute;
    return result;
}

inline time_t ns_to_seconds(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_second;
    if (remainder)
        *remainder = value % ns_per_second;
    return result;
}

inline time_t ns_to_milliseconds(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_millisecond;
    if (remainder)
        *remainder = value % ns_per_millisecond;
    return result;
}

inline time_t ns_to_microseconds(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_microsecond;
    if (remainder)
        *remainder = value % ns_per_microsecond;
    return result;
}

} // namespace detail

class timespec_t : public timespec {
public:
    timespec_t()
        : timespec()
    {
        // Nothing to do.
    }

    timespec_t(const timespec_t &other)
        : timespec()
    {
        tv_sec = other.tv_sec, tv_nsec = other.tv_nsec;
    }

    timespec_t(time_t ns)
        : timespec()
    {
        tv_sec = detail::ns_to_seconds(ns, reinterpret_cast<time_t *>(&tv_nsec));
        this->normalize();
    }

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

    static inline timespec_t zero()
    {
        timespec_t duration;
        duration.tv_sec  = 0;
        duration.tv_nsec = 0;
        return duration;
    }

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

    template <typename T>
    inline T to_nanoseconds() const
    {
        return static_cast<T>((tv_sec * detail::ns_per_second) + tv_nsec);
    }

    template <typename T>
    inline T to_microseconds() const
    {
        return static_cast<T>(detail::ns_to_microseconds(this->to_nanoseconds<T>()));
    }

    template <typename T>
    inline T to_milliseconds() const
    {
        return static_cast<T>(detail::ns_to_milliseconds(this->to_nanoseconds<T>()));
    }

    template <typename T>
    inline T to_seconds() const
    {
        return static_cast<T>(detail::ns_to_seconds(this->to_nanoseconds<T>()));
    }

    template <typename T>
    inline T to_minutes() const
    {
        return static_cast<T>(detail::ns_to_minutes(this->to_nanoseconds<T>()));
    }

    template <typename T>
    inline T to_hours() const
    {
        return static_cast<T>(detail::ns_to_hours(this->to_nanoseconds<T>()));
    }

    inline friend timespec_t operator+(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() + rhs.to_nanoseconds<time_t>());
    }

    template <typename T>
    inline friend timespec_t operator+(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() + rhs);
    }

    inline friend timespec_t operator-(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() - rhs.to_nanoseconds<time_t>());
    }

    template <typename T>
    inline friend timespec_t operator-(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() - rhs);
    }

    inline friend timespec_t operator*(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() * rhs.to_nanoseconds<time_t>());
    }

    template <typename T>
    inline friend timespec_t operator*(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() * rhs);
    }

    inline friend timespec_t operator/(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() / rhs.to_nanoseconds<time_t>());
    }

    template <typename T>
    inline friend timespec_t operator/(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() / static_cast<time_t>(rhs));
    }

    inline friend timespec_t &operator+=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs + rhs);
    }

    template <typename T>
    inline friend timespec_t &operator+=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs + rhs);
    }

    inline friend timespec_t &operator-=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs - rhs);
    }

    template <typename T>
    inline friend timespec_t &operator-=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs - rhs);
    }

    inline friend timespec_t &operator*=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs * rhs);
    }

    template <typename T>
    inline friend timespec_t &operator*=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs * rhs);
    }

    inline friend timespec_t &operator/=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs / rhs);
    }

    template <typename T>
    inline friend timespec_t &operator/=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs / rhs);
    }

    inline timespec_t &operator=(const timespec_t &other)
    {
        tv_sec = other.tv_sec, tv_nsec = other.tv_nsec;
        return *this;
    }
};

} // namespace stopwatch
