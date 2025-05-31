// --- START OF MODIFIED FILE src/core/data_access/mock/MockFacultyDao.cpp ---
#include "MockFacultyDao.h"
#include "../../entities/Faculty.h"
#include "../../../common/ErrorType.h"
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <expected>

// (➕) Biến static phải được định nghĩa ở file .cpp
namespace { 
    // Giữ private cho file cpp này, quản lý qua hàm static của class
    std::map<std::string, Faculty> mock_faculties_data;
    bool mock_faculty_data_initialized = false; // Cờ này giờ dùng để initializeDefaultMockData chỉ chạy 1 lần nếu cần
}

// (➕) Hàm khởi tạo dữ liệu mặc định
void MockFacultyDao::initializeDefaultMockData() {
    if (!mock_faculty_data_initialized) { // Chỉ chạy nếu chưa init hoặc đã clear
        mock_faculties_data.clear(); // Đảm bảo sạch trước khi init
        mock_faculties_data.emplace("IT", Faculty("IT", "Information Technology"));
        mock_faculties_data.emplace("CS", Faculty("CS", "Computer Science"));
        mock_faculties_data.emplace("EE", Faculty("EE", "Electrical Engineering"));
        mock_faculties_data.emplace("CHEM", Faculty("CHEM", "Chemistry Department"));
        mock_faculties_data.emplace("TRAN", Faculty("TRAN", "Transfiguration Studies"));
        mock_faculties_data.emplace("CHAR", Faculty("CHAR", "Charms & Enchantments"));
        mock_faculties_data.emplace("LAW", Faculty("LAW", "Magical Law"));
        mock_faculty_data_initialized = true;
    }
}

// (➕) Hàm xóa dữ liệu mock
void MockFacultyDao::clearMockData() {
    mock_faculties_data.clear();
    mock_faculty_data_initialized = false;
}


MockFacultyDao::MockFacultyDao() {
    // (➖) Không gọi initializeDefaultMockData() ở đây nữa
    // Test fixture sẽ quản lý việc này
}

std::expected<Faculty, Error> MockFacultyDao::getById(const std::string& id) const {
    auto it = mock_faculties_data.find(id);
    if (it != mock_faculties_data.end()) {
        return it->second;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Faculty with ID " + id + " not found"});
}

std::expected<std::vector<Faculty>, Error> MockFacultyDao::getAll() const {
    std::vector<Faculty> result;
    for (const auto& pair : mock_faculties_data) {
        result.push_back(pair.second);
    }
    return result;
}

std::expected<Faculty, Error> MockFacultyDao::add(const Faculty& faculty) {
    if (mock_faculties_data.count(faculty.getId())) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Faculty with ID " + faculty.getId() + " already exists"});
    }
    for(const auto& pair : mock_faculties_data) {
        if (pair.second.getName() == faculty.getName()) {
             return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Faculty with Name " + faculty.getName() + " already exists"});
        }
    }
    
    auto insert_result = mock_faculties_data.emplace(faculty.getId(), faculty);
    if(!insert_result.second){
         return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to emplace faculty into mock data."});
    }
    return faculty;
}

std::expected<bool, Error> MockFacultyDao::update(const Faculty& faculty) {
    auto it = mock_faculties_data.find(faculty.getId());
    if (it != mock_faculties_data.end()) {
        for(const auto& pair : mock_faculties_data) {
            if (pair.first != faculty.getId() && pair.second.getName() == faculty.getName()) {
                return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Faculty Name " + faculty.getName() + " conflicts with another faculty."});
            }
        }
        it->second = faculty; 
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Faculty with ID " + faculty.getId() + " not found for update"});
}

std::expected<bool, Error> MockFacultyDao::remove(const std::string& id) {
    if (mock_faculties_data.erase(id) > 0) {
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Faculty with ID " + id + " not found for removal"});
}

std::expected<bool, Error> MockFacultyDao::exists(const std::string& id) const {
    return mock_faculties_data.count(id) > 0;
}

std::expected<Faculty, Error> MockFacultyDao::findByName(const std::string& name) const {
    for (const auto& pair : mock_faculties_data) {
        if (pair.second.getName() == name) {
            return pair.second;
        }
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Faculty with name '" + name + "' not found"});
}
// --- END OF MODIFIED FILE src/core/data_access/mock/MockFacultyDao.cpp ---