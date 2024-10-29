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

/// @brief Number of nanoseconds in one hour.
static const time_t ns_per_hour = 3600000000000UL;

/// @brief Number of nanoseconds in one minute.
static const time_t ns_per_minute = 60000000000UL;

/// @brief Number of nanoseconds in one second.
static const time_t ns_per_second = 1000000000UL;

/// @brief Number of nanoseconds in one millisecond.
static const time_t ns_per_millisecond = 1000000UL;

/// @brief Number of nanoseconds in one microsecond.
static const time_t ns_per_microsecond = 1000UL;

/// @brief Converts hours to nanoseconds.
/// @param value The number of hours.
/// @return The equivalent value in nanoseconds.
inline time_t hours_to_ns(time_t value)
{
    return value * ns_per_hour;
}

/// @brief Converts minutes to nanoseconds.
/// @param value The number of minutes.
/// @return The equivalent value in nanoseconds.
inline time_t minutes_to_ns(time_t value)
{
    return value * ns_per_minute;
}

/// @brief Converts seconds to nanoseconds.
/// @param value The number of seconds.
/// @return The equivalent value in nanoseconds.
inline time_t seconds_to_ns(time_t value)
{
    return value * ns_per_second;
}

/// @brief Converts milliseconds to nanoseconds.
/// @param value The number of milliseconds.
/// @return The equivalent value in nanoseconds.
inline time_t milliseconds_to_ns(time_t value)
{
    return value * ns_per_millisecond;
}

/// @brief Converts microseconds to nanoseconds.
/// @param value The number of microseconds.
/// @return The equivalent value in nanoseconds.
inline time_t microseconds_to_ns(time_t value)
{
    return value * ns_per_microsecond;
}

/// @brief Converts nanoseconds to hours.
/// @param value The number of nanoseconds.
/// @param remainder Optional pointer to store the remainder after the division.
/// @return The equivalent value in hours.
inline time_t ns_to_hours(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_hour;
    if (remainder)
        *remainder = value % ns_per_hour;
    return result;
}

/// @brief Converts nanoseconds to minutes.
/// @param value The number of nanoseconds.
/// @param remainder Optional pointer to store the remainder after the division.
/// @return The equivalent value in minutes.
inline time_t ns_to_minutes(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_minute;
    if (remainder)
        *remainder = value % ns_per_minute;
    return result;
}

/// @brief Converts nanoseconds to seconds.
/// @param value The number of nanoseconds.
/// @param remainder Optional pointer to store the remainder after the division.
/// @return The equivalent value in seconds.
inline time_t ns_to_seconds(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_second;
    if (remainder)
        *remainder = value % ns_per_second;
    return result;
}

/// @brief Converts nanoseconds to milliseconds.
/// @param value The number of nanoseconds.
/// @param remainder Optional pointer to store the remainder after the division.
/// @return The equivalent value in milliseconds.
inline time_t ns_to_milliseconds(time_t value, time_t *remainder = NULL)
{
    time_t result = value / ns_per_millisecond;
    if (remainder)
        *remainder = value % ns_per_millisecond;
    return result;
}

/// @brief Converts nanoseconds to microseconds.
/// @param value The number of nanoseconds.
/// @param remainder Optional pointer to store the remainder after the division.
/// @return The equivalent value in microseconds.
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
    /// @brief Default constructor for timespec_t.
    /// Initializes the timespec structure.
    timespec_t()
        : timespec()
    {
        // No additional initialization needed.
    }

    /// @brief Copy constructor for timespec_t.
    /// @param other Another timespec_t object to copy from.
    timespec_t(const timespec_t &other)
        : timespec()
    {
        tv_sec  = other.tv_sec;
        tv_nsec = other.tv_nsec;
    }

    /// @brief Constructor that initializes timespec_t from nanoseconds.
    /// @param ns Nanoseconds to initialize from.
    timespec_t(time_t ns)
        : timespec()
    {
        tv_sec = detail::ns_to_seconds(ns, reinterpret_cast<time_t *>(&tv_nsec));
        this->normalize();
    }

    /// @brief Returns the current time.
    /// @return A timespec_t object representing the current time.
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

    /// @brief Returns a zero timespec_t object.
    /// @return A timespec_t object representing zero time.
    static inline timespec_t zero()
    {
        timespec_t ts;
        ts.tv_sec  = 0;
        ts.tv_nsec = 0;
        return ts;
    }

    /// @brief Normalizes the timespec_t object.
    /// Ensures that the nanoseconds field is within valid range.
    /// @return A reference to the normalized timespec_t object.
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

    /// @brief Converts the timespec_t object to nanoseconds.
    /// @tparam T The type to convert to (e.g., int, float).
    /// @return The equivalent value in nanoseconds.
    template <typename T>
    inline T to_nanoseconds() const
    {
        return static_cast<T>((tv_sec * detail::ns_per_second) + tv_nsec);
    }

    /// @brief Converts the timespec_t object to microseconds.
    /// @tparam T The type to convert to (e.g., int, float).
    /// @return The equivalent value in microseconds.
    template <typename T>
    inline T to_microseconds() const
    {
        return static_cast<T>(detail::ns_to_microseconds(this->to_nanoseconds<T>()));
    }

    /// @brief Converts the timespec_t object to milliseconds.
    /// @tparam T The type to convert to (e.g., int, float).
    /// @return The equivalent value in milliseconds.
    template <typename T>
    inline T to_milliseconds() const
    {
        return static_cast<T>(detail::ns_to_milliseconds(this->to_nanoseconds<T>()));
    }

    /// @brief Converts the timespec_t object to seconds.
    /// @tparam T The type to convert to (e.g., int, float).
    /// @return The equivalent value in seconds.
    template <typename T>
    inline T to_seconds() const
    {
        return static_cast<T>(detail::ns_to_seconds(this->to_nanoseconds<T>()));
    }

    /// @brief Converts the timespec_t object to minutes.
    /// @tparam T The type to convert to (e.g., int, float).
    /// @return The equivalent value in minutes.
    template <typename T>
    inline T to_minutes() const
    {
        return static_cast<T>(detail::ns_to_minutes(this->to_nanoseconds<T>()));
    }

    /// @brief Converts the timespec_t object to hours.
    /// @tparam T The type to convert to (e.g., int, float).
    /// @return The equivalent value in hours.
    template <typename T>
    inline T to_hours() const
    {
        return static_cast<T>(detail::ns_to_hours(this->to_nanoseconds<T>()));
    }

    /// @brief Addition operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return A new timespec_t representing the sum of the two objects.
    inline friend timespec_t operator+(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() + rhs.to_nanoseconds<time_t>());
    }

    /// @brief Addition operator for a timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand scalar value.
    /// @return A new timespec_t representing the sum.
    template <typename T>
    inline friend timespec_t operator+(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() + rhs);
    }

    /// @brief Subtraction operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return A new timespec_t representing the difference.
    inline friend timespec_t operator-(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() - rhs.to_nanoseconds<time_t>());
    }

    /// @brief Subtraction operator for a timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand scalar value.
    /// @return A new timespec_t representing the difference.
    template <typename T>
    inline friend timespec_t operator-(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() - rhs);
    }

    /// @brief Multiplication operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return A new timespec_t representing the product.
    inline friend timespec_t operator*(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() * rhs.to_nanoseconds<time_t>());
    }

    /// @brief Multiplication operator for a timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand scalar value.
    /// @return A new timespec_t representing the product.
    template <typename T>
    inline friend timespec_t operator*(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() * rhs);
    }

    /// @brief Division operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return A new timespec_t representing the quotient.
    inline friend timespec_t operator/(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() / rhs.to_nanoseconds<time_t>());
    }

    /// @brief Division operator for a timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand scalar value.
    /// @return A new timespec_t representing the quotient.
    template <typename T>
    inline friend timespec_t operator/(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(lhs.to_nanoseconds<time_t>() / static_cast<time_t>(rhs));
    }

    /// @brief Addition assignment operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return A reference to the updated timespec_t.
    inline friend timespec_t &operator+=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs + rhs);
    }

    /// @brief Addition assignment operator for a timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand scalar value.
    /// @return A reference to the updated timespec_t.
    template <typename T>
    inline friend timespec_t &operator+=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs + rhs);
    }

    /// @brief Subtraction assignment operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return A reference to the updated timespec_t.
    inline friend timespec_t &operator-=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs - rhs);
    }

    /// @brief Subtraction assignment operator for a timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand scalar value.
    /// @return A reference to the updated timespec_t.
    template <typename T>
    inline friend timespec_t &operator-=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs - rhs);
    }

    /// @brief Multiplication assignment operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return A reference to the updated timespec_t.
    inline friend timespec_t &operator*=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs * rhs);
    }

    /// @brief Multiplication assignment operator for a timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand scalar value.
    /// @return A reference to the updated timespec_t.
    template <typename T>
    inline friend timespec_t &operator*=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs * rhs);
    }

    /// @brief Division assignment operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return A reference to the updated timespec_t.
    inline friend timespec_t &operator/=(timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs = lhs / rhs);
    }

    /// @brief Division assignment operator for a timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand scalar value.
    /// @return A reference to the updated timespec_t.
    template <typename T>
    inline friend timespec_t &operator/=(timespec_t &lhs, const T &rhs)
    {
        return (lhs = lhs / rhs);
    }

    /// @brief Assignment operator for timespec_t.
    /// @param other Another timespec_t object to assign from.
    /// @return A reference to the updated timespec_t.
    inline timespec_t &operator=(const timespec_t &other)
    {
        tv_sec  = other.tv_sec;
        tv_nsec = other.tv_nsec;
        return *this;
    }
};

} // namespace stopwatch
