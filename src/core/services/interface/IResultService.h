#ifndef IRESULTSERVICE_H
#define IRESULTSERVICE_H

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/CourseResult.h"

class IResultService {
public:
    virtual ~IResultService() = default;
    virtual std::expected<bool, Error> enterMarks(const std::string& studentId, const std::string& courseId, int marks) = 0;
    virtual std::expected<bool, Error> enterMultipleMarks(const std::string& studentId, const std::map<std::string, int>& courseMarksMap) = 0;
    virtual std::expected<CourseResult, Error> getSpecificResult(const std::string& studentId, const std::string& courseId) const = 0;
    virtual std::expected<std::vector<CourseResult>, Error> getResultsByStudent(const std::string& studentId) const = 0;
    virtual std::expected<std::vector<CourseResult>, Error> getResultsByCourse(const std::string& courseId) const = 0;
    virtual std::expected<std::string, Error> generateStudentResultReport(const std::string& studentId) const = 0;
    virtual std::expected<double, Error> calculateCGPA(const std::string& studentId) const = 0;
};

#endif // IRESULTSERVICE_H