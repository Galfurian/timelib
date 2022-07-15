#include "stopwatch/stopwatch.hpp"

#include <vector>
#include <random>

inline auto generate_random_values(unsigned size)
{
    std::vector<double> values(size);
    std::uniform_real_distribution<double> dist(-100, +100);
    std::default_random_engine eng;
    for (std::size_t i = 0; i < values.size(); ++i)
        values[i] = dist(eng);
    return values;
}

inline auto compute_abs(std::vector<double> &values)
{
    for (std::size_t i = 0; i < values.size(); ++i)
        values[i] = std::abs(values[i]);
}

inline auto compute_mean(std::vector<double> &values)
{
    double mean = 0.;
    for (std::size_t i = 0; i < values.size(); ++i)
        mean += values[i];
    return mean / static_cast<double>(values.size());
}

int main(int argc, char *argv[])
{
    stopwatch::StopWatch sw;
    std::vector<double> values;
    const unsigned size = 10000;
    const unsigned runs = 1000;

    sw.set_print_mode(stopwatch::human);
    std::cout << "Generate : " << stopwatch::time(sw, [&] { values = generate_random_values(size); }) << "\n";

    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Abs      : " << stopwatch::time(sw, [&values] { compute_abs(values); }) << "\n";

    sw.set_print_mode(stopwatch::total);
    std::cout << "Mean     : " << stopwatch::time(sw, [&values] { compute_mean(values); }) << "\n";

    std::cout << "Average:\n";

    sw.set_print_mode(stopwatch::human);
    std::cout << "  Generate : " << stopwatch::ntimes<runs>(sw, [&] { values = generate_random_values(size); }).mean() << "\n";

    sw.set_print_mode(stopwatch::numeric);
    std::cout << "  Abs      : " << stopwatch::ntimes<runs>(sw, [&values] { compute_abs(values); }).mean() << "\n";

    sw.set_print_mode(stopwatch::total);
    std::cout << "  Mean     : " << stopwatch::ntimes<runs>(sw, [&values] { compute_mean(values); }).mean() << "\n";
    return 0;
}
