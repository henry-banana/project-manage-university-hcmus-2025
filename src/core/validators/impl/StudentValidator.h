#ifndef STUDENTVALIDATOR_H
#define STUDENTVALIDATOR_H

#include "../interface/IValidator.h"
#include "../../entities/Student.h"
#include <memory> // For std::shared_ptr for IGeneralInputValidator

class StudentValidator : public IEntityValidator<Student> {
private:
    std::shared_ptr<IGeneralInputValidator> _generalValidator;
    // (Optional) Có thể inject các DAO interface nếu cần kiểm tra sự tồn tại của FacultyId
    // std::shared_ptr<IFacultyDao> _facultyDao; 
public:
    // StudentValidator(std::shared_ptr<IGeneralInputValidator> generalValidator, std::shared_ptr<IFacultyDao> facultyDao);
    explicit StudentValidator(std::shared_ptr<IGeneralInputValidator> generalValidator);
    ~StudentValidator() override = default;

    ValidationResult validateEntity(const Student& student) const override;
};

#endif // STUDENTVALIDATOR_H