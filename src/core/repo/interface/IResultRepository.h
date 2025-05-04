#ifndef IRESULTREPOSITORY_H
#define IRESULTREPOSITORY_H

#include <optional>
#include <vector>
#include <string>
#include "../../entities/Result.h"

// Interface cho Result Repository (quản lý điểm)
class IResultRepository {
private:

protected:

public:
    virtual ~IResultRepository() = default;

    // CRUD cho một kết quả của một môn cho một sinh viên
    virtual std::optional<CourseResult> findCourseResult(const std::string& studentId, const std::string& courseId) const = 0;
    virtual std::vector<CourseResult> findCourseResultsByStudent(const std::string& studentId) const = 0;
    virtual std::vector<CourseResult> findCourseResultsByCourse(const std::string& courseId) const = 0;
    virtual bool add(const CourseResult& result) = 0;
    virtual bool update(const CourseResult& result) = 0;
    virtual bool remove(const std::string& courseId, const std::string& studentId) = 0;

    // Quản lý kết quả học kỳ
    // Optional: Operations for managing semester results as a whole (if needed)
    // virtual std::optional<StudentSemesterResult> findSemesterResult(const std::string& studentId, int semester) = 0;
    // virtual bool addSemesterResult(const StudentSemesterResult& semesterResult) = 0;
    // virtual std::vector<StudentSemesterResult> findAllSemesterResults(const std::string& studentId) const = 0;
};

#endif // IRESULTREPOSITORY_H