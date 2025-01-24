# Timekeeping library (timelib)

[![Ubuntu](https://github.com/Galfurian/timelib/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/Galfurian/timelib/actions/workflows/ubuntu.yml)
[![Windows](https://github.com/Galfurian/timelib/actions/workflows/windows.yml/badge.svg)](https://github.com/Galfurian/timelib/actions/workflows/windows.yml)
[![MacOS](https://github.com/Galfurian/timelib/actions/workflows/macos.yml/badge.svg)](https://github.com/Galfurian/timelib/actions/workflows/macos.yml)
[![Documentation](https://github.com/Galfurian/timelib/actions/workflows/documentation.yml/badge.svg)](https://github.com/Galfurian/timelib/actions/workflows/documentation.yml)

`timelib` is a lightweight C++ library designed for precise time measurement and
benchmarking. It provides two core utilities:

- **Timer**: A versatile timer with support for elapsed, remaining, and timeout durations.
- **Stopwatch**: A class to track and log multiple rounds for benchmarking tasks.

The library is simple to integrate and supports customizable output formats,
making it ideal for performance analysis and timing critical tasks.

---

## Features

- Precise time measurement using `timespec_t`.
- Customizable output formats: human-readable, numeric, total elapsed seconds, and custom placeholders.
- Support for multiple time-related operations:
  - Elapsed time
  - Remaining time
  - Timeout checks
  - Accumulated durations
- Utilities for benchmarking functions and tasks over multiple runs.

---

## Installation

To use `timelib`, include the source files in your project and ensure you link against any required standard libraries for C++11 or later.

```bash
git clone https://github.com/Galfurian/timelib.git
# Include the "timelib" directory in your project's include path.
```

---

## Usage

### Example: Timer

```cpp
#include <iostream>
#include <thread>

#include <timelib/timer.hpp>

int main() {
    timelib::Timer timer;
    
    // Start the timer.
    timer.start();

    // Simulate a task.
    std::this_thread::sleep_for(std::chrono::seconds(2));

    timelib::Duration elapsed = timer.stop();
    std::cout << "Elapsed time: " << elapsed.to_string() << "\n";
    return 0;
}
```

### Example: Stopwatch

```cpp
#include <iostream>
#include <vector>
#include <random>

#include <timelib/stopwatch.hpp>

int main() {
    timelib::Stopwatch stopwatch;

    // Simulate data.
    std::vector<int> data(1000, 42);

    // Simulate computation.
    timelib::time(stopwatch, [&]() {
        for (auto &value : data) value += 1;
    });

    std::cout << "Elapsed time: " << stopwatch << "\n";
    return 0;
}
```

---

## API Documentation

### Timer

- **`Timer::start()`**: Starts or resumes the timer.
- **`Timer::stop()`**: Stops the timer and resets it.
- **`Timer::pause()`**: Pauses the timer, retaining elapsed time.
- **`Timer::elapsed()`**: Returns the total elapsed duration.
- **`Timer::remaining()`**: Returns the remaining time until the timeout.

### Stopwatch

- **`Stopwatch::start()`**: Starts or resumes the stopwatch.
- **`Stopwatch::round()`**: Records a round, updating total elapsed time.
- **`Stopwatch::mean()`**: Returns the mean duration across rounds.

---

## License

The library is distributed under the MIT License. See `LICENSE.md` for details.

---

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any
improvements or bug fixes.
