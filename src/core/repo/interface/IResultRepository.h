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
    virtual std::optional<CourseResult> findByCourseAndStudent(const std::string& courseId, const std::string& studentId) = 0;
    virtual std::vector<CourseResult> findByStudent(const std::string& studentId) = 0;
    virtual std::vector<CourseResult> findByCourse(const std::string& courseId) = 0;
    virtual bool add(const CourseResult& result) = 0;
    virtual bool update(const CourseResult& result) = 0;
    virtual bool remove(const std::string& courseId, const std::string& studentId) = 0;

    // Quản lý kết quả học kỳ
    virtual std::optional<StudentSemesterResult> findSemesterResult(const std::string& studentId, int semester) = 0;
    virtual bool addSemesterResult(const StudentSemesterResult& semesterResult) = 0;
    // ...
};

#endif // IRESULTREPOSITORY_H