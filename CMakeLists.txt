# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# -----------------------------------------------------------------------------
# Set the minimum CMake version, the project name and default build type.
cmake_minimum_required(VERSION 3.1...3.18)

# Set the project name.
project(stopwatch CXX)

# -----------------------------------------------------------------------------
# Add options.
# -----------------------------------------------------------------------------
option(STOPWATCH_BUILD_EXAMPLES "Build stopwatch examples" OFF)
option(STOPWATCH_STRICT_WARNINGS "Enable strict compiler warnings for stopwatch" ON)
option(STOPWATCH_WARNINGS_AS_ERRORS "Treat all warnings as errors for stopwatch" OFF)

# -----------------------------------------------------------------------------
# Set the compilation flags.
# -----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings that suggest using MSVC-specific safe functions
    set(STOPWATCH_COMPILE_OPTIONS "${STOPWATCH_COMPILE_OPTIONS} -D_CRT_SECURE_NO_WARNINGS")

    if(STOPWATCH_WARNINGS_AS_ERRORS)
        set(STOPWATCH_COMPILE_OPTIONS "${STOPWATCH_COMPILE_OPTIONS} /WX")
    endif(STOPWATCH_WARNINGS_AS_ERRORS)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(STOPWATCH_WARNINGS_AS_ERRORS)
        set(STOPWATCH_COMPILE_OPTIONS "${STOPWATCH_COMPILE_OPTIONS} -Werror")
    endif(STOPWATCH_WARNINGS_AS_ERRORS)
endif()

if(STOPWATCH_STRICT_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # Mark system headers as external for MSVC explicitly
        # https://devblogs.microsoft.com/cppblog/broken-warnings-theory
        set(STOPWATCH_COMPILE_OPTIONS "${STOPWATCH_COMPILE_OPTIONS} /experimental:external")
        set(STOPWATCH_COMPILE_OPTIONS "${STOPWATCH_COMPILE_OPTIONS} /external:I ${CMAKE_BINARY_DIR}")
        set(STOPWATCH_COMPILE_OPTIONS "${STOPWATCH_COMPILE_OPTIONS} /external:anglebrackets")
        set(STOPWATCH_COMPILE_OPTIONS "${STOPWATCH_COMPILE_OPTIONS} /external:W0")
        set(STOPWATCH_COMPILE_OPTIONS "${STOPWATCH_COMPILE_OPTIONS} /W4")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(STOPWATCH_COMPILE_OPTIONS "${STOPWATCH_COMPILE_OPTIONS} -Wall -Wextra -Wconversion -pedantic")
    endif()
endif(STOPWATCH_STRICT_WARNINGS)

string(REPLACE " " ";" STOPWATCH_COMPILE_OPTIONS ${STOPWATCH_COMPILE_OPTIONS})

# -----------------------------------------------------------------------------
# LIBRARY
# -----------------------------------------------------------------------------
# Add the C++ library.
add_library(stopwatch INTERFACE)

# Inlcude header directories.
target_include_directories(stopwatch INTERFACE ${PROJECT_SOURCE_DIR}/include)

# Set compilation flags.
target_compile_options(stopwatch INTERFACE ${STOPWATCH_COMPILE_OPTIONS})

# Set compilation definitions.
target_compile_definitions(stopwatch INTERFACE ${STOPWATCH_COMPILE_DEFINITIONS})

# -----------------------------------------------------------------------------
# EXAMPLES
# -----------------------------------------------------------------------------
if(STOPWATCH_BUILD_EXAMPLES)
    # Add the example.
    add_executable(example_stopwatch example/example_stopwatch.cpp)

    # Inlcude header directories.
    target_include_directories(example_stopwatch PUBLIC ${PROJECT_SOURCE_DIR}/include)

    # Link libraries.
    target_link_libraries(example_stopwatch PUBLIC stopwatch)

    # Set compilation flags.
    target_compile_options(example_stopwatch PUBLIC ${STOPWATCH_COMPILE_OPTIONS})

    # Set compilation definitions.
    target_compile_definitions(example_stopwatch INTERFACE ${STOPWATCH_COMPILE_DEFINITIONS})
endif(STOPWATCH_BUILD_EXAMPLES)
