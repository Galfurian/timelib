/// @file stopwatch.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Benchmarking classes.

#pragma once

#include <time.h>

namespace stopwatch
{

static const unsigned long ns_per_sec = 1000000000UL;

namespace detail
{
inline unsigned long hours_to_ns(unsigned long value)
{
    return (value * 3600) * ns_per_sec;
}
inline unsigned long minutes_to_ns(unsigned long value)
{
    return (value * 60) * ns_per_sec;
}
inline unsigned long seconds_to_ns(unsigned long value)
{
    return value * ns_per_sec;
}
inline unsigned long milliseconds_to_ns(unsigned long value)
{
    return value * 1000000UL;
}
inline unsigned long microseconds_to_ns(unsigned long value)
{
    return value * 1000UL;
}
} // namespace detail

class timespec_t : public timespec {
public:
    timespec_t()
        : timespec()
    {
        // Nothing to do.
    }

    template <typename T>
    timespec_t(const T &ns)
        : timespec()
    {
        tv_sec  = static_cast<time_t>(ns) / ns_per_sec;
        tv_nsec = static_cast<long>(ns) % ns_per_sec;
        this->normalize();
    }

    static inline timespec_t now()
    {
        timespec_t ts;
        clock_gettime(CLOCK_REALTIME, &ts);
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
        while (tv_nsec >= static_cast<long>(ns_per_sec)) {
            ++(tv_sec);
            tv_nsec -= ns_per_sec;
        }
        while (tv_nsec <= -static_cast<long>(ns_per_sec)) {
            --(tv_sec);
            tv_nsec += ns_per_sec;
        }
        if (tv_nsec < 0) {
            --(tv_sec);
            tv_nsec = (ns_per_sec + tv_nsec);
        }
        return *this;
    }

    template <typename T>
    inline T to_nanoseconds() const
    {
        return static_cast<T>((tv_sec * ns_per_sec) + tv_nsec);
    }

    template <typename T>
    inline T to_microseconds() const
    {
        return static_cast<T>(this->to_nanoseconds<unsigned long>() / 1000UL);
    }

    template <typename T>
    inline T to_milliseconds() const
    {
        return static_cast<T>(this->to_nanoseconds<unsigned long>() / 1000000UL);
    }

    template <typename T>
    inline T to_seconds() const
    {
        return static_cast<T>(this->to_nanoseconds<unsigned long>() / ns_per_sec);
    }

    template <typename T>
    inline T to_minutes() const
    {
        return static_cast<T>((this->to_nanoseconds<unsigned long>() / ns_per_sec) / 60);
    }

    template <typename T>
    inline T to_hours() const
    {
        return static_cast<T>((this->to_nanoseconds<unsigned long>() / ns_per_sec) / 3600);
    }

    inline friend timespec_t operator+(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<unsigned long>() + rhs.to_nanoseconds<unsigned long>());
    }

    template <typename T>
    inline friend timespec_t operator+(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<unsigned long>() + rhs);
    }

    inline friend timespec_t operator-(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<unsigned long>() - rhs.to_nanoseconds<unsigned long>());
    }

    template <typename T>
    inline friend timespec_t operator-(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<T>() - rhs);
    }

    inline friend timespec_t operator/(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<unsigned long>() / rhs.to_nanoseconds<unsigned long>());
    }

    template <typename T>
    inline friend timespec_t operator/(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<T>() / rhs);
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

    inline friend timespec_t &operator/=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs / rhs);
    }

    template <typename T>
    inline friend timespec_t &operator/=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs / rhs);
    }
};

} // namespace stopwatch