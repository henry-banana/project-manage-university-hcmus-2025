#ifndef TEACHERVALIDATOR_H
#define TEACHERVALIDATOR_H

#include "../interface/IValidator.h"
#include "../../entities/Teacher.h"
#include <memory>

class TeacherValidator : public IEntityValidator<Teacher> {
private:
    std::shared_ptr<IGeneralInputValidator> _generalValidator;
public:
    explicit TeacherValidator(std::shared_ptr<IGeneralInputValidator> generalValidator);
    ~TeacherValidator() override = default;

    ValidationResult validateEntity(const Teacher& teacher) const override;
};

#endif // TEACHERVALIDATOR_H