#include "Faculty.h"
#include <iostream> // Thêm thư viện iostream để sử dụng std::cout
#include <format>

Faculty::Faculty() : _id(""), _name("") {}

// Rule of Three
Faculty::Faculty(std::string id, std::string name) 
: _id(std::move(id)), _name(std::move(name)) {}

Faculty::Faculty(const Faculty& other) : _id(other._id), _name(other._name) {}

Faculty& Faculty::operator=(const Faculty& other) {
    if (this != &other) { // Kiểm tra tự gán
        _id = other._id;
        _name = other._name;
    }
    return *this;
}

// Getter & Setter
const std::string& Faculty::id() const {
    return _id;
}

const std::string& Faculty::name() const {
    return _name;
}

void Faculty::setId(const std::string& id) {
    _id = id;
}

void Faculty::setName(const std::string& name) {
    _name = name;
}

// Phương thức hiển thị thông tin khoa
void Faculty::display() const {
    std::cout << std::format(
        "--- Faculty Information ---\n"
        "ID: {}\n"
        "Name: {}\n",
        id(), name()
    );
}