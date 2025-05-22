#ifndef ICOURSESERVICE_H
#define ICOURSESERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/Course.h"

class ICourseService {
public:
    virtual ~ICourseService() = default;
    virtual std::expected<Course, Error> getCourseById(const std::string& courseId) const = 0;
    virtual std::expected<std::vector<Course>, Error> getAllCourses() const = 0;
    virtual std::expected<std::vector<Course>, Error> getCoursesByFaculty(const std::string& facultyId) const = 0;
    virtual std::expected<Course, Error> addCourse(const std::string& id, const std::string& name, int credits, const std::string& facultyId) = 0;
    virtual std::expected<bool, Error> updateCourse(const std::string& courseId, const std::string& newName, int newCredits, const std::string& newFacultyId) = 0;
    virtual std::expected<bool, Error> removeCourse(const std::string& courseId) = 0;
};

#endif // ICOURSESERVICE_H