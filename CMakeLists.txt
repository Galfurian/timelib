# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# -----------------------------------------------------------------------------
# Set the minimum CMake version, the project name and default build type.
cmake_minimum_required(VERSION 3.1...3.18)

# Set the project name.
project(stopwatch CXX)

# Set the default build type to Debug.
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# -----------------------------------------------------------------------------
# OPTIONS
# -----------------------------------------------------------------------------

option(STOPWATCH_BUILD_EXAMPLES "Build examples" OFF)
option(STOPWATCH_STRICT_WARNINGS "Enable strict compiler warnings" ON)
option(STOPWATCH_WARNINGS_AS_ERRORS "Treat all warnings as errors" OFF)

# -----------------------------------------------------------------------------
# COMPILATION FLAGS
# -----------------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings that suggest using MSVC-specific safe functions
    set(STOPWATCH_COMPILE_OPTIONS ${STOPWATCH_COMPILE_OPTIONS} -D_CRT_SECURE_NO_WARNINGS)

    if(STOPWATCH_WARNINGS_AS_ERRORS)
        set(STOPWATCH_COMPILE_OPTIONS ${STOPWATCH_COMPILE_OPTIONS} /WX)
    endif(STOPWATCH_WARNINGS_AS_ERRORS)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(STOPWATCH_WARNINGS_AS_ERRORS)
        set(STOPWATCH_COMPILE_OPTIONS ${STOPWATCH_COMPILE_OPTIONS} -Werror)
    endif(STOPWATCH_WARNINGS_AS_ERRORS)
endif()

if(STOPWATCH_STRICT_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # Mark system headers as external for MSVC explicitly
        # https://devblogs.microsoft.com/cppblog/broken-warnings-theory
        set(STOPWATCH_COMPILE_OPTIONS ${STOPWATCH_COMPILE_OPTIONS} /experimental:external)
        set(STOPWATCH_COMPILE_OPTIONS ${STOPWATCH_COMPILE_OPTIONS} /external:I ${CMAKE_BINARY_DIR})
        set(STOPWATCH_COMPILE_OPTIONS ${STOPWATCH_COMPILE_OPTIONS} /external:anglebrackets)
        set(STOPWATCH_COMPILE_OPTIONS ${STOPWATCH_COMPILE_OPTIONS} /external:W0)
        set(STOPWATCH_COMPILE_OPTIONS ${STOPWATCH_COMPILE_OPTIONS} /W4)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(STOPWATCH_COMPILE_OPTIONS ${STOPWATCH_COMPILE_OPTIONS} -Wall -Wextra -Wconversion -pedantic)
    endif()
endif(STOPWATCH_STRICT_WARNINGS)

# -----------------------------------------------------------------------------
# LIBRARY
# -----------------------------------------------------------------------------

# Add the C++ library.
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
# Inlcude header directories.
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)
# Set compilation flags.
target_compile_options(${PROJECT_NAME} INTERFACE ${STOPWATCH_COMPILE_OPTIONS})

# -----------------------------------------------------------------------------
# EXAMPLES
# -----------------------------------------------------------------------------

if(STOPWATCH_BUILD_EXAMPLES)

    # Add the example.
    add_executable(${PROJECT_NAME}_example_cpp11 ${PROJECT_SOURCE_DIR}/examples/example_cpp11.cpp)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_example_cpp11 PUBLIC ${STOPWATCH_COMPILE_OPTIONS})
    # Inlcude header directories.
    target_include_directories(${PROJECT_NAME}_example_cpp11 PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_example_cpp11 PUBLIC ${PROJECT_NAME})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_example_cpp11 PUBLIC cxx_std_11)
    
    # Add the example.
    add_executable(${PROJECT_NAME}_example_cpp98 ${PROJECT_SOURCE_DIR}/examples/example_cpp98.cpp)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_example_cpp98 PUBLIC ${STOPWATCH_COMPILE_OPTIONS})
    # Inlcude header directories.
    target_include_directories(${PROJECT_NAME}_example_cpp98 PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_example_cpp98 PUBLIC ${PROJECT_NAME})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_example_cpp98 PUBLIC cxx_std_98)

endif(STOPWATCH_BUILD_EXAMPLES)

# -----------------------------------------------------------------------------
# DOCUMENTATION
# -----------------------------------------------------------------------------

find_package(Doxygen)

if(DOXYGEN_FOUND)
    
    # = RETIVAL ===============================================================
    # Include fetch content.
    include(FetchContent)
    # Record the options that describe how to populate the specified content.
    FetchContent_Declare(
        doxygenawesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
        GIT_TAG 4cd62308d825fe0396d2f66ffbab45d0e247724c # 2.0.3
    )
    # Retrieve the properties related to the content.
    FetchContent_GetProperties(doxygenawesome)
    # If not populated, make the content available.
    if(NOT doxygenawesome_POPULATED)
        message(STATUS "Retrieving `doxygen-awesome-css`...")
        # Ensures the named dependencies have been populated.
        FetchContent_MakeAvailable(doxygenawesome)
        # Hide fetchcontent variables, otherwise with ccmake it's a mess.
        mark_as_advanced(FORCE
            FETCHCONTENT_QUIET FETCHCONTENT_BASE_DIR FETCHCONTENT_FULLY_DISCONNECTED FETCHCONTENT_UPDATES_DISCONNECTED
            FETCHCONTENT_UPDATES_DISCONNECTED_DOXYGENAWESOME FETCHCONTENT_SOURCE_DIR_DOXYGENAWESOME
        )
    endif()

    # = CUSTOMIZATION =========================================================
    set(DOXYGEN_PROJECT_NAME "Stopwatch")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_WARN_FORMAT "$file:$line: $text")
    set(DOXYGEN_HTML_HEADER ${doxygenawesome_SOURCE_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygenawesome_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    )
    doxygen_add_docs(
        ${PROJECT_NAME}_documentation
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/include/stopwatch/timespec_support.hpp
        ${PROJECT_SOURCE_DIR}/include/stopwatch/stopwatch.hpp
    )
endif()
