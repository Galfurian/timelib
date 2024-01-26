/// @file example_cpp11.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief A couple of examples on how to use the class with C++11.
/// 
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
/// 

#include "stopwatch/stopwatch.hpp"

#include <vector>
#include <random>

inline std::vector<double> generate_random_values(unsigned size)
{
    std::vector<double> values(size);
    std::uniform_real_distribution<double> dist(-100, +100);
    std::default_random_engine eng;
    for (std::size_t i = 0; i < values.size(); ++i)
        values[i] = dist(eng);
    return values;
}

inline std::vector<double> &compute_abs(std::vector<double> &values)
{
    for (std::size_t i = 0; i < values.size(); ++i)
        values[i] = std::abs(values[i]);
    return values;
}

inline double compute_mean(std::vector<double> &values)
{
    double mean = 0.;
    for (std::size_t i = 0; i < values.size(); ++i)
        mean += values[i];
    return mean / static_cast<double>(values.size());
}

int main(int, char *[])
{
    stopwatch::Stopwatch sw;
    std::vector<double> values;
    const unsigned size = 10000;
    const unsigned runs = 1000;

    stopwatch::time(sw, [&] { values = generate_random_values(size); });

    sw.set_print_mode(stopwatch::human);
    std::cout << "Generate : " << sw << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Generate : " << sw << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Generate : " << sw << "\n";

    stopwatch::time(sw, [&] { values = compute_abs(values); });

    sw.set_print_mode(stopwatch::human);
    std::cout << "Abs      : " << sw << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Abs      : " << sw << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Abs      : " << sw << "\n";

    stopwatch::time(sw, [&] { compute_mean(values); });

    sw.set_print_mode(stopwatch::human);
    std::cout << "Mean      : " << sw << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Mean      : " << sw << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Mean      : " << sw << "\n";

    std::cout << "\nMultiple runs:\n\n";

    stopwatch::ntimes<runs>(sw, [&] { values = generate_random_values(size); });

    sw.set_print_mode(stopwatch::human);
    std::cout << "Generate      : " << sw.mean() << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Generate      : " << sw.mean() << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Generate      : " << sw.mean() << "\n";
    sw.set_print_mode(stopwatch::custom);
    sw.set_format("%H:%M:%s:%m:%u:%n");
    std::cout << "Generate      : " << sw.mean() << "\n";

    return 0;
}
