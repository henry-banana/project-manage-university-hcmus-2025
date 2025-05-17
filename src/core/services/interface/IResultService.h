#ifndef IRESULTSERVICE_H
#define IRESULTSERVICE_H

#include <string>
#include <vector>
#include <map> // Cho enterMultipleMarks
#include <optional> // Cho calculate GPA
#include "../../../common/OperationResult.h"
#include "../../entities/CourseResult.h"

class IResultService {
public:
    virtual ~IResultService() = default;
    virtual OperationResult<bool> enterMarks(const std::string& studentId, const std::string& courseId, int marks) = 0;
    virtual OperationResult<bool> enterMultipleMarks(const std::string& studentId, const std::map<std::string, int>& courseMarksMap) = 0; // (➕)
    virtual OperationResult<CourseResult> getSpecificResult(const std::string& studentId, const std::string& courseId) const = 0;
    virtual OperationResult<std::vector<CourseResult>> getResultsByStudent(const std::string& studentId) const = 0;
    virtual OperationResult<std::vector<CourseResult>> getResultsByCourse(const std::string& courseId) const = 0;
    virtual OperationResult<std::string> generateStudentResultReport(const std::string& studentId) const = 0;
    virtual OperationResult<double> calculateCGPA(const std::string& studentId) const = 0;
    // virtual OperationResult<double> calculateSGPA(const std::string& studentId, int semester, int year) const = 0; (SGPA cần thêm thông tin học kỳ)
};

#endif // IRESULTSERVICE_H