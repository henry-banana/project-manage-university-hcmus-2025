#ifndef ICOURSERESULTDAO_H
#define ICOURSERESULTDAO_H

#include "../../entities/CourseResult.h"
#include <string>
#include <vector>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

class ICourseResultDao {
public:
    virtual ~ICourseResultDao() = default;

    virtual std::expected<CourseResult, Error> find(const std::string& studentId, const std::string& courseId) const = 0;
    virtual std::expected<std::vector<CourseResult>, Error> findByStudentId(const std::string& studentId) const = 0;
    virtual std::expected<std::vector<CourseResult>, Error> findByCourseId(const std::string& courseId) const = 0;
    virtual std::expected<bool, Error> addOrUpdate(const CourseResult& result) = 0;
    virtual std::expected<bool, Error> remove(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::expected<bool, Error> removeAllForStudent(const std::string& studentId) = 0;
    virtual std::expected<bool, Error> removeAllForCourse(const std::string& courseId) = 0;
};

#endif // ICOURSERESULTDAO_H