/// @file timespec.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Defines the timespec_t class, which extends the timespec structure.
///
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
///

#pragma once

#define _USE_32BIT_TIME_T

#include <type_traits>
#include <iostream>
#include <iomanip>
#include <ctime>

namespace timelib
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
        : timespec({ 0, 0 })
    {
        // No additional initialization needed.
    }

    /// @brief Constructor that accepts seconds and nanoseconds.
    /// @param sec The seconds component.
    /// @param nsec The nanoseconds component.
    timespec_t(time_t sec, long nsec)
        : timespec()
    {
        tv_sec  = sec;
        tv_nsec = nsec;
    }

    /// @brief Copy constructor for timespec_t.
    /// @param other Another timespec_t object to copy from.
    timespec_t(const timespec_t &other)
        : timespec()
    {
        tv_sec  = other.tv_sec;
        tv_nsec = other.tv_nsec;
    }

    /// @brief Constructor that initializes timespec_t from a floating-point value.
    /// @param value The time value in seconds (floating-point).
    template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    timespec_t(T value)
        : timespec()
    {
        // Handle floating-point values (expressed in seconds).
        tv_sec  = static_cast<time_t>(value);
        tv_nsec = static_cast<long>((static_cast<double>(value) - static_cast<double>(tv_sec)) * 1e9);
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

    /// @brief Converts the timespec_t object to a double representing the time in seconds.
    /// @return The equivalent value in seconds as a double.
    inline double count() const
    {
        return static_cast<double>(tv_sec) + static_cast<double>(tv_nsec) / 1e9;
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
    /// @param lhs Left-hand operand (a timespec_t object).
    /// @param rhs Right-hand operand (a timespec_t object).
    /// @return A new timespec_t representing the sum of the two timespec_t objects.
    inline friend timespec_t operator+(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.count() + rhs.count());
    }

    /// @brief Addition operator for a timespec_t and a scalar (floating-point or integral).
    /// @param lhs Left-hand operand (a timespec_t object).
    /// @param rhs Right-hand scalar value representing time in seconds.
    /// @return A new timespec_t representing the sum of the timespec_t and the scalar.
    template <typename T>
    inline friend timespec_t operator+(const timespec_t &lhs, const T &rhs)
    {
        return lhs + timespec_t(rhs);
    }

    /// @brief Addition operator for a scalar (floating-point or integral) and a timespec_t.
    /// @param lhs Left-hand scalar value representing time in seconds.
    /// @param rhs Right-hand operand (a timespec_t object).
    /// @return A new timespec_t representing the sum of the scalar and the timespec_t.
    template <typename T>
    inline friend timespec_t operator+(const T &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs) + rhs;
    }

    /// @brief Subtraction operator for two timespec_t objects.
    /// @param lhs Left-hand operand (a timespec_t object).
    /// @param rhs Right-hand operand (a timespec_t object).
    /// @return A new timespec_t representing the difference between the two timespec_t objects.
    inline friend timespec_t operator-(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.count() - rhs.count());
    }

    /// @brief Subtraction operator for a timespec_t and a scalar (floating-point or integral).
    /// @param lhs Left-hand operand (a timespec_t object).
    /// @param rhs Right-hand scalar value representing time in seconds.
    /// @return A new timespec_t representing the result of subtracting the scalar from the timespec_t.
    template <typename T>
    inline friend timespec_t operator-(const timespec_t &lhs, const T &rhs)
    {
        return lhs - timespec_t(rhs);
    }

    /// @brief Subtraction operator for a scalar (floating-point or integral) and a timespec_t.
    /// @param lhs Left-hand scalar value representing time in seconds.
    /// @param rhs Right-hand operand (a timespec_t object).
    /// @return A new timespec_t representing the result of subtracting the timespec_t from the scalar.
    template <typename T>
    inline friend timespec_t operator-(const T &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs) - rhs;
    }

    /// @brief Multiplication operator for two timespec_t objects.
    /// @param lhs Left-hand operand (a timespec_t object).
    /// @param rhs Right-hand operand (a timespec_t object).
    /// @return A new timespec_t representing the product of the two timespec_t objects.
    inline friend timespec_t operator*(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.count() * rhs.count());
    }

    /// @brief Multiplication operator for a timespec_t and a scalar (floating-point or integral).
    /// @param lhs Left-hand operand (a timespec_t object).
    /// @param rhs Right-hand scalar value representing a multiplier.
    /// @return A new timespec_t representing the scaled timespec_t.
    template <typename T>
    inline friend timespec_t operator*(const timespec_t &lhs, const T &rhs)
    {
        return lhs * timespec_t(rhs);
    }

    /// @brief Multiplication operator for a scalar (floating-point or integral) and a timespec_t.
    /// @param lhs Left-hand scalar value representing a multiplier.
    /// @param rhs Right-hand operand (a timespec_t object).
    /// @return A new timespec_t representing the scaled timespec_t.
    template <typename T>
    inline friend timespec_t operator*(const T &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs) * rhs;
    }

    /// @brief Division operator for two timespec_t objects.
    /// @param lhs Left-hand operand (a timespec_t object).
    /// @param rhs Right-hand operand (a timespec_t object).
    /// @return A scalar value representing the division result.
    inline friend timespec_t operator/(const timespec_t &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs.count() / rhs.count());
    }

    /// @brief Division operator for a timespec_t and a scalar (floating-point or integral).
    /// @param lhs Left-hand operand (a timespec_t object).
    /// @param rhs Right-hand scalar value representing the divisor.
    /// @return A new timespec_t representing the scaled-down timespec_t.
    template <typename T>
    inline friend timespec_t operator/(const timespec_t &lhs, const T &rhs)
    {
        return lhs / timespec_t(rhs);
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

    /// @brief Equality operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return True if both timespec_t objects are equal.
    inline friend bool operator==(const timespec_t &lhs, const timespec_t &rhs)
    {
        return (lhs.tv_sec == rhs.tv_sec) && (lhs.tv_nsec == rhs.tv_nsec);
    }

    /// @brief Equality operator for timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand timespec_t operand.
    /// @param rhs Right-hand scalar operand.
    /// @return True if timespec_t is equal to the scalar.
    template <typename T>
    inline friend bool operator==(const timespec_t &lhs, const T &rhs)
    {
        return lhs == timespec_t(rhs);
    }

    /// @brief Equality operator for a scalar and timespec_t.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand scalar operand.
    /// @param rhs Right-hand timespec_t operand.
    /// @return True if the scalar is equal to timespec_t.
    template <typename T>
    inline friend bool operator==(const T &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs) == rhs;
    }

    /// @brief Inequality operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return True if both timespec_t objects are not equal.
    inline friend bool operator!=(const timespec_t &lhs, const timespec_t &rhs)
    {
        return !(lhs == rhs);
    }

    /// @brief Inequality operator for timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand timespec_t operand.
    /// @param rhs Right-hand scalar operand.
    /// @return True if timespec_t is not equal to the scalar.
    template <typename T>
    inline friend bool operator!=(const timespec_t &lhs, const T &rhs)
    {
        return !(lhs == rhs);
    }

    /// @brief Inequality operator for a scalar and timespec_t.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand scalar operand.
    /// @param rhs Right-hand timespec_t operand.
    /// @return True if the scalar is not equal to timespec_t.
    template <typename T>
    inline friend bool operator!=(const T &lhs, const timespec_t &rhs)
    {
        return !(lhs == rhs);
    }

    /// @brief Less-than operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return True if lhs is less than rhs.
    inline friend bool operator<(const timespec_t &lhs, const timespec_t &rhs)
    {
        if (lhs.tv_sec < rhs.tv_sec) {
            return true;
        }
        if (lhs.tv_sec == rhs.tv_sec && lhs.tv_nsec < rhs.tv_nsec) {
            return true;
        }
        return false;
    }

    /// @brief Less-than operator for timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand timespec_t operand.
    /// @param rhs Right-hand scalar operand.
    /// @return True if lhs is less than the scalar.
    template <typename T>
    inline friend bool operator<(const timespec_t &lhs, const T &rhs)
    {
        return lhs < timespec_t(rhs);
    }

    /// @brief Less-than operator for a scalar and timespec_t.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand scalar operand.
    /// @param rhs Right-hand timespec_t operand.
    /// @return True if the scalar is less than rhs.
    template <typename T>
    inline friend bool operator<(const T &lhs, const timespec_t &rhs)
    {
        return timespec_t(lhs) < rhs;
    }

    /// @brief Greater-than operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return True if lhs is greater than rhs.
    inline friend bool operator>(const timespec_t &lhs, const timespec_t &rhs)
    {
        return rhs < lhs;
    }

    /// @brief Greater-than operator for timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand timespec_t operand.
    /// @param rhs Right-hand scalar operand.
    /// @return True if lhs is greater than the scalar.
    template <typename T>
    inline friend bool operator>(const timespec_t &lhs, const T &rhs)
    {
        return timespec_t(rhs) < lhs;
    }

    /// @brief Greater-than operator for a scalar and timespec_t.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand scalar operand.
    /// @param rhs Right-hand timespec_t operand.
    /// @return True if the scalar is greater than rhs.
    template <typename T>
    inline friend bool operator>(const T &lhs, const timespec_t &rhs)
    {
        return rhs < timespec_t(lhs);
    }

    /// @brief Less-than-or-equal-to operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return True if lhs is less than or equal to rhs.
    inline friend bool operator<=(const timespec_t &lhs, const timespec_t &rhs)
    {
        return !(rhs < lhs);
    }

    /// @brief Less-than-or-equal-to operator for timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand timespec_t operand.
    /// @param rhs Right-hand scalar operand.
    /// @return True if lhs is less than or equal to the scalar.
    template <typename T>
    inline friend bool operator<=(const timespec_t &lhs, const T &rhs)
    {
        return !(timespec_t(rhs) < lhs);
    }

    /// @brief Less-than-or-equal-to operator for a scalar and timespec_t.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand scalar operand.
    /// @param rhs Right-hand timespec_t operand.
    /// @return True if the scalar is less than or equal to rhs.
    template <typename T>
    inline friend bool operator<=(const T &lhs, const timespec_t &rhs)
    {
        return !(rhs < timespec_t(lhs));
    }

    /// @brief Greater-than-or-equal-to operator for two timespec_t objects.
    /// @param lhs Left-hand operand.
    /// @param rhs Right-hand operand.
    /// @return True if lhs is greater than or equal to rhs.
    inline friend bool operator>=(const timespec_t &lhs, const timespec_t &rhs)
    {
        return !(lhs < rhs);
    }

    /// @brief Greater-than-or-equal-to operator for timespec_t and a scalar.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand timespec_t operand.
    /// @param rhs Right-hand scalar operand.
    /// @return True if lhs is greater than or equal to the scalar.
    template <typename T>
    inline friend bool operator>=(const timespec_t &lhs, const T &rhs)
    {
        return !(lhs < timespec_t(rhs));
    }

    /// @brief Greater-than-or-equal-to operator for a scalar and timespec_t.
    /// @tparam T The scalar type.
    /// @param lhs Left-hand scalar operand.
    /// @param rhs Right-hand timespec_t operand.
    /// @return True if the scalar is greater than or equal to rhs.
    template <typename T>
    inline friend bool operator>=(const T &lhs, const timespec_t &rhs)
    {
        return !(timespec_t(lhs) < rhs);
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

    /// @brief Output stream operator to print timespec_t.
    /// @param os The output stream.
    /// @param ts The timespec_t instance.
    /// @return Reference to the output stream.
    friend std::ostream &operator<<(std::ostream &os, const timespec_t &ts)
    {
        os << "<s: " << ts.tv_sec << ", ns: " << ts.tv_nsec << ">";
        return os;
    }
};

} // namespace timelib
