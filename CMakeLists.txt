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

# =====================================
# COMPILATION FLAGS
# =====================================

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings that suggest using MSVC-specific safe functions
    target_compile_definitions(timelib INTERFACE _CRT_SECURE_NO_WARNINGS)
    if(WARNINGS_AS_ERRORS)
        target_compile_options(timelib INTERFACE /WX)
    endif()
    if(STRICT_WARNINGS)
        # Mark system headers as external for MSVC explicitly
        # https://devblogs.microsoft.com/cppblog/broken-warnings-theory
        target_compile_options(timelib INTERFACE /experimental:external)
        target_compile_options(timelib INTERFACE /external:I ${CMAKE_BINARY_DIR})
        target_compile_options(timelib INTERFACE /external:anglebrackets)
        target_compile_options(timelib INTERFACE /external:W0)
        target_compile_options(timelib INTERFACE /W4)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(WARNINGS_AS_ERRORS)
        target_compile_options(timelib INTERFACE -Werror)
    endif()
    if(STRICT_WARNINGS)
        target_compile_options(timelib INTERFACE -Wall -Wextra -Wconversion -pedantic)
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
    # = FETCH DOXYGEN-AWESOME =================================================
    # Record the options that describe how to populate the specified content.
    FetchContent_Declare(doxygenawesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
        GIT_TAG main
    )
    # Retrieve the properties related to the content.
    FetchContent_MakeAvailable(doxygenawesome)
    # Hide fetchcontent variables, otherwise with ccmake it's a mess.
    mark_as_advanced(FORCE
        FETCHCONTENT_UPDATES_DISCONNECTED_DOXYGENAWESOME
        FETCHCONTENT_SOURCE_DIR_DOXYGENAWESOME
    )
    # = CUSTOMIZATION =========================================================
    set(DOXYGEN_WARN_FORMAT "$file:$line: $text")
    set(DOXYGEN_PROJECT_NAME timelib)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HTML_HEADER ${doxygenawesome_SOURCE_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygenawesome_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    )
    doxygen_add_docs(
        timelib_documentation
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/include/timelib/details.hpp
        ${PROJECT_SOURCE_DIR}/include/timelib/duration.hpp
        ${PROJECT_SOURCE_DIR}/include/timelib/timespec.hpp
        ${PROJECT_SOURCE_DIR}/include/timelib/stopwatch.hpp
    )
endif()
