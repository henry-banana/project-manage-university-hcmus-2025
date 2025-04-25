cmake_minimum_required(VERSION 3.30.2)
project(manage-university-project VERSION 1.0)

# Use modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify build directory (Optional)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Include all relevant directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/entities
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/services
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/repositories
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/repositories/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/repositories/csv
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/repositories/mock
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# Find all source files
file(GLOB_RECURSE SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Create executable
add_executable(university_manager ${SRC_FILES})

set_target_properties(university_manager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/release  
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
