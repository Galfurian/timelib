/// @file details.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Support elements for the stopwatch library.
/// 
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
/// 

#pragma once

#if __cplusplus < 201103L
#include "timespec.hpp"
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

} // namespace stopwatch
