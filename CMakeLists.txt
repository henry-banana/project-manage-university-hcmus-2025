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

# cmake_minimum_required(VERSION 3.15)
# project(UniversityManagementSystem LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 20) # Hoặc 17 nếu chưa dùng C++20 features
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# # Tạo thư mục logs nếu chưa có (cho Logger)
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/config)

# # Tìm các file nguồn
# file(GLOB_RECURSE SOURCE_FILES
#     "src/*.cpp"
#     "src/common/*.cpp"
#     "src/core/entities/*.cpp"
#     "src/core/data_access/impl_csv/*.cpp"
#     "src/core/data_access/*.cpp" # DaoFactory.cpp
#     # Thêm các thư mục .cpp khác ở đây
#     "src/core/services/impl/*.cpp"
#     "src/core/validators/impl/*.cpp"
#     "src/core/database_adapter/impl_mysql/*.cpp"
#     "src/ui/*.cpp"
#     "src/utils/*.cpp"
# )

# add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# # Thiết lập include directories để trình biên dịch tìm thấy các file .h
# target_include_directories(${PROJECT_NAME} PUBLIC
#     "${CMAKE_CURRENT_SOURCE_DIR}/src"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/common"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/entities"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/data_access/interface"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/data_access/impl_csv"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/data_access/impl_sql"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/data_access" # Cho DaoFactory.h
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/services/interface"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/services/impl"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/validators/interface"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/validators/impl"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/database_adapter/interface"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/core/database_adapter/impl_mysql"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/ui"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/utils"
# )

# # Nếu dùng thư viện ngoài (ví dụ MySQL Connector)
# # find_package(MySQL REQUIRED)
# # target_link_libraries(${PROJECT_NAME} PRIVATE MySQL::mysqlcppconn)

# # Cho phép copy file data và config vào thư mục build
# # Ví dụ cho file config
# set(CONFIG_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/config/app_config.ini")
# set(DEST_CONFIG_PATH "${CMAKE_BINARY_DIR}/config/app_config.ini")

# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         "${CONFIG_FILE_PATH}"
#         "${DEST_CONFIG_PATH}"
#     COMMENT "Copying config file to build directory"
# )

# # Tương tự cho các file CSV trong thư mục data nếu cần
# set(DATA_FILES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/data")
# set(DEST_DATA_DIR "${CMAKE_BINARY_DIR}/data")

# # Lấy danh sách tất cả các file CSV
# file(GLOB CSV_FILES "${DATA_FILES_DIR}/*.csv")

# foreach(CSV_FILE ${CSV_FILES})
#     get_filename_component(CSV_FILENAME ${CSV_FILE} NAME)
#     add_custom_command(
#         TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#             "${CSV_FILE}"
#             "${DEST_DATA_DIR}/${CSV_FILENAME}"
#         COMMENT "Copying data file ${CSV_FILENAME} to build directory"
#     )
# endforeach()