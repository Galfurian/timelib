# -----------------------------------------------------------------------------
# @brief  : Test directory.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
add_executable(test_stopwatch test_stopwatch.cpp)
target_link_libraries(test_stopwatch stopwatch)
add_test(NAME run_test_stopwatch COMMAND test_stopwatch)
