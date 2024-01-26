/// @file example_cpp98.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief A couple of examples on how to use the class with C++98.
/// 
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.
/// 

#include "stopwatch/stopwatch.hpp"

#include <cstdlib>
#include <vector>

inline std::vector<double> generate_random_values(unsigned size)
{
    std::vector<double> values(size);
    std::srand(10);
    for (std::size_t i = 0; i < values.size(); ++i)
        values[i] = -100 + std::rand() % 200;
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

    sw.reset();
    values = generate_random_values(size);
    sw.round();

    sw.set_print_mode(stopwatch::human);
    std::cout << "Generate : " << sw << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Generate : " << sw << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Generate : " << sw << "\n";

    sw.reset();
    values = compute_abs(values);
    sw.round();

    sw.set_print_mode(stopwatch::human);
    std::cout << "Abs      : " << sw << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Abs      : " << sw << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Abs      : " << sw << "\n";

    sw.reset();
    compute_mean(values);
    sw.round();

    sw.set_print_mode(stopwatch::human);
    std::cout << "Mean      : " << sw << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Mean      : " << sw << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Mean      : " << sw << "\n";

    std::cout << "\nMultiple runs:\n\n";

    sw.reset();
    for (std::size_t i = 0u; i < runs; ++i) {
        values = generate_random_values(size);
        sw.round();
    }

    sw.set_print_mode(stopwatch::human);
    std::cout << "Generate      : " << sw.mean() << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Generate      : " << sw.mean() << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Generate      : " << sw.mean() << "\n";
    sw.set_print_mode(stopwatch::custom);
    sw.set_format("%H:%M:%s:%m:%u:%n");
    std::cout << "Generate      : " << sw.mean() << "\n";
    std::cout << "Generate      : " << sw.round().count() << "\n";

    return 0;
}
