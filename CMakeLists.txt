# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# -----------------------------------------------------------------------------

# Set the minimum CMake version, the project name and default build type.
cmake_minimum_required(VERSION 3.1...3.18)

# Set the project name.
project(timelib CXX)

# Set the default build type to Debug.
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# -----------------------------------------------------------------------------
# OPTIONS
# -----------------------------------------------------------------------------

option(STRICT_WARNINGS "Enable strict compiler warnings" ON)
option(WARNINGS_AS_ERRORS "Treat all warnings as errors" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

# -----------------------------------------------------------------------------
# ENABLE FETCH CONTENT
# -----------------------------------------------------------------------------

# We need this in order to import external projects.
include(FetchContent)
# Hide fetchcontent variables.
mark_as_advanced(FORCE
    FETCHCONTENT_QUIET
    FETCHCONTENT_BASE_DIR
    FETCHCONTENT_FULLY_DISCONNECTED
    FETCHCONTENT_UPDATES_DISCONNECTED
)

# -----------------------------------------------------------------------------
# DEPENDENCY (SYSTEM LIBRARIES)
# -----------------------------------------------------------------------------

find_package(Doxygen)

# -----------------------------------------------------------------------------
# LIBRARY
# -----------------------------------------------------------------------------

# Add the C++ library.
add_library(timelib INTERFACE)
add_library(timelib::timelib ALIAS timelib)
# Include header directories, and link libraries.
target_include_directories(timelib INTERFACE ${PROJECT_SOURCE_DIR}/include)
# Set compiler flags.
target_compile_features(timelib INTERFACE cxx_std_11)

# =====================================
# COMPILATION FLAGS
# =====================================

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings for MSVC-specific "safe" functions like strcpy_s, etc.,
    # which are not portable and may clutter warning logs.
    target_compile_definitions(timelib INTERFACE _CRT_SECURE_NO_WARNINGS)

    # Disable warning C4702: unreachable code.
    add_compile_options(/wd4702)

    if(WARNINGS_AS_ERRORS)
        # Treat all warnings as errors to enforce stricter code quality.
        target_compile_options(timelib INTERFACE /WX)
    endif()

    if(STRICT_WARNINGS)
        # Enable external header management to suppress warnings in system and
        # external headers, making it easier to focus on project-specific issues.
        target_compile_options(timelib INTERFACE /experimental:external)
        target_compile_options(timelib INTERFACE /external:I ${CMAKE_BINARY_DIR})
        target_compile_options(timelib INTERFACE /external:anglebrackets)
        target_compile_options(timelib INTERFACE /external:W0)

        # Use a high warning level to catch as many potential issues as possible.
        target_compile_options(timelib INTERFACE /W4)

        # Enforce standards-compliant behavior to avoid relying on MSVC-specific extensions.
        target_compile_options(timelib INTERFACE /permissive-)
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(WARNINGS_AS_ERRORS)
        # Treat all warnings as errors to enforce stricter code quality.
        target_compile_options(timelib INTERFACE -Werror)
    endif()

    if(STRICT_WARNINGS)
        # Enable a broad set of warnings to catch common and subtle issues:
        target_compile_options(timelib INTERFACE
            -Wall                # Enable most general-purpose warnings.
            -Wextra              # Enable extra warnings not included in -Wall.
            -Wconversion         # Warn about implicit type conversions that may lose data.
            -pedantic            # Enforce strict compliance with the C++ standard.
            -Wshadow             # Warn about variable shadowing, which can cause subtle bugs.
            -Wnon-virtual-dtor   # Warn when a class with virtual functions lacks a virtual destructor.
            -Wnull-dereference   # Warn about potential null pointer dereferences.
            -Wformat=2           # Enable strict checks for printf/scanf format strings.
            -Woverloaded-virtual # Warn when a derived class function hides a base class virtual function.
            -Wfloat-equal        # Warn about direct comparisons of floating-point values, which can be imprecise.
        )

    endif()
endif()

# -----------------------------------------------------------------------------
# EXAMPLES
# -----------------------------------------------------------------------------

if(BUILD_EXAMPLES)

    # Add the example.
    add_executable(timelib_example_stopwatch ${PROJECT_SOURCE_DIR}/examples/example_stopwatch.cpp)
    # Specify C++11 standard for this target
    set_target_properties(timelib_example_stopwatch PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
    # Inlcude header directories.
    target_include_directories(timelib_example_stopwatch PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Set the linked libraries.
    target_link_libraries(timelib_example_stopwatch PUBLIC timelib)
    
    # Add the example.
    add_executable(timelib_example_timer ${PROJECT_SOURCE_DIR}/examples/example_timer.cpp)
    # Specify C++11 standard for this target
    set_target_properties(timelib_example_timer PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
    # Inlcude header directories.
    target_include_directories(timelib_example_timer PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Set the linked libraries.
    target_link_libraries(timelib_example_timer PUBLIC timelib)

endif()

# -----------------------------------------------------------------------------
# DOCUMENTATION
# -----------------------------------------------------------------------------

if(DOXYGEN_FOUND)
    
    # FetchContent: Doxygen Awesome CSS
    FetchContent_Declare(doxygenawesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
        GIT_TAG main
    )
    FetchContent_MakeAvailable(doxygenawesome)

    # Hide FetchContent variables to avoid clutter in ccmake.
    mark_as_advanced(FORCE
        FETCHCONTENT_UPDATES_DISCONNECTED_DOXYGENAWESOME
        FETCHCONTENT_SOURCE_DIR_DOXYGENAWESOME
    )

    # Read the file with the version.
    file(READ ${PROJECT_SOURCE_DIR}/include/timelib/version.hpp version_file)
    # Extract the version.
    string(REGEX MATCH "TIMELIB_MAJOR_VERSION ([0-9]*)" _ ${version_file})
    set(TIMELIB_MAJOR_VERSION ${CMAKE_MATCH_1})
    string(REGEX MATCH "TIMELIB_MINOR_VERSION ([0-9]*)" _ ${version_file})
    set(TIMELIB_MINOR_VERSION ${CMAKE_MATCH_1})
    string(REGEX MATCH "TIMELIB_MICRO_VERSION ([0-9]*)" _ ${version_file})
    set(TIMELIB_MICRO_VERSION ${CMAKE_MATCH_1})

    # Customization: Doxygen Configuration
    set(DOXYGEN_WARN_FORMAT "$file:$line: $text")
    set(DOXYGEN_PROJECT_NAME timelib)
    set(DOXYGEN_PROJECT_BRIEF "A simple time-keeping library for C++")
    set(DOXYGEN_PROJECT_NUMBER "${TIMELIB_MAJOR_VERSION}.${TIMELIB_MINOR_VERSION}.${TIMELIB_MICRO_VERSION}")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_GENERATE_MAN NO)

    # Styling and UX enhancements using Doxygen Awesome
    set(DOXYGEN_HTML_HEADER ${doxygenawesome_SOURCE_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygenawesome_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    )

    # Set stricter warnings for better documentation quality
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_IF_DOC_ERROR YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_WARN_AS_ERROR YES) # Treat warnings as errors for CI

    # Exclude certain files or directories from documentation (if needed)
    set(DOXYGEN_EXCLUDE_PATTERNS
        "${PROJECT_SOURCE_DIR}/tests/*"
        "${PROJECT_SOURCE_DIR}/examples/*"
    )

    # Add Doxygen documentation target
    doxygen_add_docs(
        timelib_documentation
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/LICENSE.md
        ${PROJECT_SOURCE_DIR}/include/timelib/duration.hpp
        ${PROJECT_SOURCE_DIR}/include/timelib/stopwatch.hpp
        ${PROJECT_SOURCE_DIR}/include/timelib/timer.hpp
        ${PROJECT_SOURCE_DIR}/include/timelib/timespec.hpp
        ${PROJECT_SOURCE_DIR}/include/timelib/version.hpp
        COMMENT "Generating Doxygen documentation for the library"
    )
endif()
