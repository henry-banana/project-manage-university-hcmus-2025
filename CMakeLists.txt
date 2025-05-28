cmake_minimum_required(VERSION 3.22) 

project(UniversityManagementSystem VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Thêm thư mục Google Test vào dự án
# Đảm bảo bạn đã tải mã nguồn Google Test và giải nén nó thành thư mục "googletest-main"
# trong thư mục gốc của dự án.
add_subdirectory(googletest-main)

# Bật tính năng kiểm thử (testing) cho dự án
enable_testing()

# Thêm các thư mục include (vẫn giữ nguyên phần này để trình biên dịch biết tìm header ở đâu)
include_directories(
    src
    src/common
    src/core/entities
    src/core/data_access
    src/core/data_access/interface
    src/core/data_access/mock
    src/core/data_access/sql
    src/core/database_adapter
    src/core/database_adapter/interface
    src/core/database_adapter/sql
    src/core/parsing
    src/core/parsing/interface
    src/core/parsing/impl_sql_parser
    src/core/services
    src/core/services/interface
    src/core/services/impl
    src/core/validators
    src/core/validators/interface
    src/core/validators/impl
    src/ui
    src/utils
    # Thêm thư mục test vào include paths để các file test có thể tìm thấy các header của dự án
    tests
    googletest-main/googletest/include # Thêm đường dẫn đến thư mục include của Google Test
)

# --- Thu thập các file nguồn bằng GLOB_RECURSE ---
file(GLOB_RECURSE COMMON_SOURCES src/common/*.cpp)
file(GLOB_RECURSE ENTITIES_SOURCES src/core/entities/*.cpp)

file(GLOB_RECURSE DAO_MOCK_SOURCES src/core/data_access/mock/*.cpp)
file(GLOB_RECURSE DAO_SQL_SOURCES src/core/data_access/sql/*.cpp)
file(GLOB DAO_FACTORY_SOURCES src/core/data_access/*.cpp) # DaoFactory là file riêng

file(GLOB_RECURSE DB_ADAPTER_SQL_SOURCES src/core/database_adapter/sql/*.cpp)

file(GLOB_RECURSE PARSING_SQL_SOURCES src/core/parsing/impl_sql_parser/*.cpp)

file(GLOB_RECURSE SERVICES_IMPL_SOURCES src/core/services/impl/*.cpp)
file(GLOB_RECURSE SERVICES_IMPL_SOURCES src/core/services/*.cpp)

file(GLOB_RECURSE VALIDATORS_IMPL_SOURCES src/core/validators/impl/*.cpp)

file(GLOB_RECURSE UI_SOURCES src/ui/view_helpers/*.cpp)
file(GLOB_RECURSE UI_SOURCES src/ui/*.cpp)
file(GLOB_RECURSE UTILS_SOURCES src/utils/*.cpp)

# File main.cpp
set(MAIN_SOURCE src/main.cpp)

# Gộp tất cả các nguồn lại
set(SOURCES
    ${MAIN_SOURCE}
    ${COMMON_SOURCES}
    ${ENTITIES_SOURCES}
    ${DAO_MOCK_SOURCES}
    ${DAO_SQL_SOURCES}
    ${DAO_FACTORY_SOURCES}
    ${DB_ADAPTER_SQL_SOURCES}
    ${PARSING_SQL_SOURCES}
    ${SERVICES_IMPL_SOURCES}
    ${VALIDATORS_IMPL_SOURCES}
    ${UI_SOURCES}
    ${UTILS_SOURCES}
)

# Loại bỏ các file header khỏi danh sách (GLOB_RECURSE có thể lấy cả .h nếu không cẩn thận)
# Tuy nhiên, với pattern "*.cpp", nó chỉ lấy file cpp.
# Nếu bạn dùng pattern rộng hơn, bạn có thể cần lọc:
# list(FILTER SOURCES EXCLUDE REGEX ".+\\.h$")

# Tạo một danh sách các file nguồn của ứng dụng CHỈ DÙNG CHO TEST (không bao gồm main.cpp)
set(APP_SOURCES_FOR_TESTS
    ${COMMON_SOURCES}
    ${ENTITIES_SOURCES}
    ${DAO_MOCK_SOURCES}
    ${DAO_SQL_SOURCES}
    ${DAO_FACTORY_SOURCES}
    ${DB_ADAPTER_SQL_SOURCES}
    ${PARSING_SQL_SOURCES}
    ${SERVICES_IMPL_SOURCES}
    ${VALIDATORS_IMPL_SOURCES}
    ${UI_SOURCES}
    ${UTILS_SOURCES}
    src/core/database_adapter/sql/sqlite3.c # Thêm sqlite3.c vào đây vì nó là một phần của logic ứng dụng
)

# Thêm file thực thi
add_executable(university_manager
    ${SOURCES}
    src/core/database_adapter/sql/sqlite3.c
)

set_target_properties(university_manager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Liên kết với SQLite3
# target_link_libraries(university_manager PRIVATE SQLite::SQLite3)

# Cờ biên dịch cho std::filesystem
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(university_manager PRIVATE stdc++fs)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0 AND NOT APPLE)
    target_link_libraries(university_manager PRIVATE stdc++fs)
endif()

# Liên kết với pthread cho getMaskedPassword trên Linux/macOS
if (UNIX)
    include(CheckIncludeFile)
    check_include_file("termios.h" HAVE_TERMIOS_H)
    if (HAVE_TERMIOS_H)
        target_compile_definitions(university_manager PRIVATE HAVE_TERMIOS_H)
        if(NOT APPLE) 
             find_package(Threads REQUIRED)
             target_link_libraries(university_manager PRIVATE Threads::Threads)
        endif()
    endif()
endif()

set_source_files_properties(src/core/database_adapter/sql/sqlite3.c PROPERTIES LANGUAGE C)

# --- Cấu hình cho Unit Tests ---

# Thu thập tất cả các file nguồn test từ thư mục tests/
# Đảm bảo bạn đặt các file test của mình (ví dụ: my_class_test.cpp) vào thư mục tests/
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

# Tạo executable cho các unit test
# Executable này sẽ bao gồm các file test VÀ tất cả các file nguồn của ứng dụng (trừ main.cpp)
add_executable(university_manager_tests
    ${TEST_SOURCES}
    ${APP_SOURCES_FOR_TESTS} # Bao gồm tất cả các file nguồn của ứng dụng (trừ main.cpp)
)

# Các test file cần liên kết với thư viện gtest và gtest_main
target_link_libraries(university_manager_tests
    PRIVATE
    GTest::gtest_main
)

# Thêm test vào CTest
# CTest sẽ chạy executable này khi bạn chạy "make test" hoặc "ctest"
add_test(NAME UniversityManagementSystemUnitTests COMMAND university_manager_tests)

message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "CXX Standard: ${CMAKE_CXX_STANDARD}")