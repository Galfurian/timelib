/// @file example_cpp11.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief A couple of examples on how to use the class with C++11.
/// @copyright (c) 2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#include "timelib/timer.hpp"

#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    using namespace timelib;

    // Example 1: Simple timing of a task.
    {
        std::cout << "Example 1: Timing a simple task...\n";

        // Create a Timer object with default print mode.
        Timer timer;

        // Start the timer.
        timer.start();

        // Simulate a task by sleeping for 2 seconds.
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Stop the timer and print the elapsed time.
        Duration elapsed_time = timer.stop();
        std::cout << "Task completed in: " << elapsed_time.to_string() << "\n";
    }

    // Example 2: Timing multiple tasks with reset.
    {
        std::cout << "\nExample 2: Timing multiple tasks with reset...\n";

        // Create another Timer object.
        Timer timer;

        // Start timing task 1.
        timer.start();
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate task 1.

        // Print elapsed time without stopping.
        std::cout << "Time after task 1: " << timer.elapsed().to_string() << "\n";

        // Reset the timer before starting task 2.
        timer.reset();
        std::cout << "Timer reset.\n";

        // Start timing task 2.
        timer.start();
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate task 2.

        // Stop the timer and print the total time for task 2.
        std::cout << "Task 2 completed in: " << timer.stop().to_string() << "\n";
    }

    // Example 3: Checking if elapsed time exceeds a threshold.
    {
        std::cout << "\nExample 3: Checking if elapsed time exceeds a threshold...\n";

        // Create a Timer object.
        Timer timer;

        timer.set_timeout(1.0);

        // Start the timer.
        timer.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Simulate a 1.5-second task.

        // Check if the timer has exceeded 1 second.
        if (timer.has_timeout()) {
            std::cout << "More than 1 second has passed.\n";
        } else {
            std::cout << "Less than 1 second has passed.\n";
        }

        // Print the actual elapsed time.
        std::cout << "Actual elapsed time: " << timer.elapsed().to_string() << "\n";
    }

    return 0;
}
