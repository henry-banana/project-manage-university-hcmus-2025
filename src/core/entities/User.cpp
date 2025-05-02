#include "User.h"

User::User(std::string id, std::string firstName, std::string lastName) 
: _id(std::move(id)), _first_name(std::move(firstName)), _last_name(std::move(lastName)) {}


const std::string& User::getId() const {
    return _id;
}

const std::string& User::getFirstName() const { 
    return _first_name; 
}

const std::string& User::getLastName() const {
     return _last_name; 
}

std::string User::getFullName() const {
    return _first_name + " " + _last_name; 
}