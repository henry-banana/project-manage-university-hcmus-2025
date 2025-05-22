#ifndef COURSEVALIDATOR_H
#define COURSEVALIDATOR_H

#include "../interface/IValidator.h"
#include "../../entities/Course.h"
#include <memory>

class CourseValidator : public IEntityValidator<Course> {
private:
    std::shared_ptr<IGeneralInputValidator> _generalValidator;
public:
    explicit CourseValidator(std::shared_ptr<IGeneralInputValidator> generalValidator);
    ~CourseValidator() override = default;

    ValidationResult validateEntity(const Course& course) const override;
};

#endif // COURSEVALIDATOR_H