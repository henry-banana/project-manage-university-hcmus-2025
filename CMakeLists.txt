cmake_minimum_required(VERSION 3.22) 

project(UniversityManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SQLite3 REQUIRED)

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
    src/core/parsing/impl
    src/core/parsing/impl/SqlParsers
    src/core/services
    src/core/services/interface
    src/core/services/impl
    src/core/validators
    src/core/validators/interface
    src/core/validators/impl
    src/ui
    src/utils
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

# Thêm file thực thi
add_executable(university_app ${SOURCES})

# Liên kết với SQLite3
target_link_libraries(university_app PRIVATE SQLite::SQLite3)

# Cờ biên dịch cho std::filesystem
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(university_app PRIVATE stdc++fs)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0 AND NOT APPLE)
    target_link_libraries(university_app PRIVATE stdc++fs)
endif()

# Liên kết với pthread cho getMaskedPassword trên Linux/macOS
if (UNIX)
    include(CheckIncludeFile)
    check_include_file("termios.h" HAVE_TERMIOS_H)
    if (HAVE_TERMIOS_H)
        target_compile_definitions(university_app PRIVATE HAVE_TERMIOS_H)
        if(NOT APPLE) 
             find_package(Threads REQUIRED)
             target_link_libraries(university_app PRIVATE Threads::Threads)
        endif()
    endif()
endif()

message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "CXX Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "SQLite3 Library: ${SQLite3_LIBRARY}")
message(STATUS "SQLite3 Include Dirs: ${SQLite3_INCLUDE_DIRS}")