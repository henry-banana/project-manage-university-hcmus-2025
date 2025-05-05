cmake_minimum_required(VERSION 3.30.2)
project(manage-university-project VERSION 1.0)

# Use modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify build directory
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Include all relevant directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Find all source files
file(GLOB_RECURSE SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# DEBUG: Print the source files found to see if they're all included
message(STATUS "Source files:")
foreach(SRC_FILE ${SRC_FILES})
    message(STATUS "  ${SRC_FILE}")
endforeach()

# Create executable
add_executable(university_manager ${SRC_FILES})

# Set output directory
set_target_properties(university_manager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Enable testing (optional)
enable_testing()

# Detect and add test files
file(GLOB_RECURSE TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/**/*.cpp
)

if(TEST_FILES)
    add_executable(university_tests ${TEST_FILES})
    add_test(NAME RunTests COMMAND university_tests)
endif()
