#ifndef ICOURSERESULTDAO_H
#define ICOURSERESULTDAO_H

#include "../../entities/CourseResult.h" // Cần full definition
#include "../../../common/OperationResult.h"
#include <string>
#include <vector>

class ICourseResultDao {
public:
    virtual ~ICourseResultDao() = default;
    virtual OperationResult<CourseResult> find(const std::string& studentId, const std::string& courseId) const = 0;
    virtual OperationResult<std::vector<CourseResult>> findByStudentId(const std::string& studentId) const = 0;
    virtual OperationResult<std::vector<CourseResult>> findByCourseId(const std::string& courseId) const = 0;
    virtual OperationResult<bool> addOrUpdate(const CourseResult& result) = 0;
    virtual OperationResult<bool> remove(const std::string& studentId, const std::string& courseId) = 0;
    virtual OperationResult<bool> removeAllForStudent(const std::string& studentId) = 0;
    virtual OperationResult<bool> removeAllForCourse(const std::string& courseId) = 0;
};

#endif // ICOURSERESULTDAO_H