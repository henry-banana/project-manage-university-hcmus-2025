#include "User.h"

User::User(std::string id, std::string firstName, std::string lastName) 
: _id(std::move(id)), _firstName(std::move(firstName)), _lastName(std::move(lastName)) {}


const std::string& User::getId() const {
    return _id;
}

const std::string& User::getFirstName() const { 
    return _firstName; 
}

const std::string& User::getLastName() const {
     return _lastName; 
}

std::string User::getFullName() const {
    return _firstName + " " + _lastName; 
}