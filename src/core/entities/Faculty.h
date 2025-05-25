#ifndef FACULTY_H
#define FACULTY_H

#include "IEntity.h"
#include <string>

class Faculty : public IEntity {
private:
    std::string _id;   // Mã khoa (e.g., "IT", "EE")
    std::string _name; // Tên khoa (e.g., "Information Technology")

public:
    Faculty(std::string id, std::string name);

    // Getters & Setters
    const std::string& getId() const; // Implement cụ thể cho _id
    const std::string& getName() const;

    bool setName(const std::string& name); // Cần validation (non-empty)

    // Implement IEntity
    std::string getStringId() const override;
    std::string display() const override;
    ValidationResult validateBasic() const override;
};

#endif // FACULTY_H