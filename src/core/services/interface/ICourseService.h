#ifndef ICOURSESERVICE_H
#define ICOURSESERVICE_H

#include <string>
#include <vector>
#include <memory>
#include "../../../common/OperationResult.h"
#include "../../entities/Course.h"

class ICourseService {
public:
    virtual ~ICourseService() = default;
    virtual OperationResult<Course> getCourseById(const std::string& courseId) const = 0;
    virtual OperationResult<std::vector<Course>> getAllCourses() const = 0;
    virtual OperationResult<std::vector<Course>> getCoursesByFaculty(const std::string& facultyId) const = 0;

    // Admin only
    virtual OperationResult<Course> addCourse(const std::string& id, const std::string& name, int credits, const std::string& facultyId) = 0;
    virtual OperationResult<bool> updateCourse(const std::string& courseId, const std::string& newName, int newCredits, const std::string& newFacultyId) = 0;
    virtual OperationResult<bool> removeCourse(const std::string& courseId) = 0; // Kiểm tra ràng buộc
};

#endif // ICOURSESERVICE_H