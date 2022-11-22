#include "stopwatch/stopwatch.hpp"

#include <vector>

#if __cplusplus < 201103L
#include <cstdlib>
#else
#include <random>
#endif

inline std::vector<double> generate_random_values(unsigned size)
{
    std::vector<double> values(size);
#if __cplusplus < 201103L
    std::srand(10);
    for (std::size_t i = 0; i < values.size(); ++i)
        values[i] = -100 + std::rand() % 200;
#else
    std::uniform_real_distribution<double> dist(-100, +100);
    std::default_random_engine eng;
    for (std::size_t i = 0; i < values.size(); ++i)
        values[i] = dist(eng);
#endif
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

#if __cplusplus < 201103L
    sw.reset();
    values = generate_random_values(size);
    sw.round();
#else
    stopwatch::time(sw, [&] { values = generate_random_values(size); });
#endif
    sw.set_print_mode(stopwatch::human);
    std::cout << "Generate : " << sw << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Generate : " << sw << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Generate : " << sw << "\n";

#if __cplusplus < 201103L
    sw.reset();
    values = compute_abs(values);
    sw.round();
#else
    stopwatch::time(sw, [&] { values = compute_abs(values); });
#endif

    sw.set_print_mode(stopwatch::human);
    std::cout << "Abs      : " << sw << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Abs      : " << sw << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Abs      : " << sw << "\n";

#if __cplusplus < 201103L
    sw.reset();
    compute_mean(values);
    sw.round();
#else
    stopwatch::time(sw, [&] { compute_mean(values); });
#endif

    sw.set_print_mode(stopwatch::human);
    std::cout << "Mean      : " << sw << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Mean      : " << sw << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Mean      : " << sw << "\n";

    std::cout << "\nMultiple runs:\n\n";

#if __cplusplus < 201103L
    sw.reset();
    for (std::size_t i = 0u; i < runs; ++i) {
        values = generate_random_values(size);
        sw.round();
    }
#else
    stopwatch::ntimes<runs>(sw, [&] { values = generate_random_values(size); });
#endif

    sw.set_print_mode(stopwatch::human);
    std::cout << "Generate      : " << sw.mean() << "\n";
    sw.set_print_mode(stopwatch::numeric);
    std::cout << "Generate      : " << sw.mean() << "\n";
    sw.set_print_mode(stopwatch::total);
    std::cout << "Generate      : " << sw.mean() << "\n";
    return 0;
}
