#include "User.h"

User::User(std::string id, std::string firstName, std::string lastName, std::string citizenID) 
: _id(std::move(id)), 
  _first_name(std::move(firstName)), 
  _last_name(std::move(lastName)),
  _citizen_id(std::move(citizenID)),
  _address(""),
  _birth_day() {}

// Getter cho ID
const std::string& User::getId() const {
    return _id;
}

// Getter cho tên
const std::string& User::getFirstName() const { 
    return _first_name; 
}

const std::string& User::getLastName() const {
     return _last_name; 
}

std::string User::getFullName() const {
    return _first_name + " " + _last_name; 
}

const std::string& User::getCitizenID() const{
    return _citizen_id;
}

const std::string& User::getAddress() const {
    return _address;
}

// Getter cho ngày sinh
Birthday User::getBirthday() const{
    return _birth_day;
}

// Setter
void User::setFirstName(const std::string& firstName) {
    _first_name = firstName;
}

void User::setLastName(const std::string& lastName) {
    _last_name = lastName;
}

void User::setCitizenId(const std::string& citizenId) {
    _citizen_id = citizenId;
}

void User::setAddress(const std::string& address) {
    _address = address;
}

void User::setBirthday(const Birthday& birthday) {
    _birth_day = birthday;
}
