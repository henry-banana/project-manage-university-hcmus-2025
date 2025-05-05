#ifndef RESULT_SERVICE_H
#define RESULT_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include <optional>
#include <map>
#include "../entities/Student.h"
#include "../entities/Course.h"
#include "../entities/Result.h"
#include "../repo/interface/IResultRepository.h"
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/ICourseRepository.h"
#include "../services/EnrollmentService.h" 

class IResultService {
public:
    virtual ~IResultService() = default;

    // Result operations
    virtual std::vector<CourseResult> getStudentResults(const std::string& studentId) const = 0;
    virtual bool updateResult(const std::string& studentId, const std::map<std::string, int>& marks) = 0;
    virtual std::optional<char> getGrade(int marks) const = 0;
    virtual bool declareResult(const std::string& studentId) = 0;

    // Get results for a course
    virtual std::vector<CourseResult> getCourseResults(const std::string& courseId) const = 0;

    // Get a specific result entry
    virtual std::optional<CourseResult> getSpecificResult(const std::string& studentId, const std::string& courseId) const = 0;

    // Update/Enter marks
    virtual bool enterMarks(const std::string& studentId, const std::string& courseId, int marks) = 0;
    virtual bool enterMultipleMarks(const std::string& studentId, const std::map<std::string, int>& courseMarks) = 0;

    // Calculate GPA/CGPA
    virtual std::optional<double> calculateSGPA(const std::string& studentId) const = 0;
    virtual std::optional<double> calculateCGPA(const std::string& studentId) const = 0;

    // Generate report (thống nhất trả về string)
    virtual std::string generateResultReport(const std::string& studentId) const = 0;

    // Static helper
    static char calculateGradeFromMarks(int marks);
};

class ResultService : public IResultService {
private:
    std::shared_ptr<IResultRepository> _resultRepo;
    std::shared_ptr<IStudentRepository> _studentRepo;
    std::shared_ptr<ICourseRepository> _courseRepo;
    std::shared_ptr<IEnrollmentService> _enrollmentService; // Check enrollment status

public:
    ResultService(std::shared_ptr<IResultRepository> resultRepo,
                  std::shared_ptr<IStudentRepository> studentRepo,
                  std::shared_ptr<ICourseRepository> courseRepo,
                  std::shared_ptr<IEnrollmentService> enrollmentService);

    std::vector<CourseResult> getStudentResults(const std::string& studentId) const override;

    std::vector<CourseResult> getCourseResults(const std::string& courseId) const override;

    std::optional<CourseResult> getSpecificResult(const std::string& studentId, const std::string& courseId) const override;
    
    bool enterMarks(const std::string& studentId, const std::string& courseId, int marks) override;

    bool enterMultipleMarks(const std::string& studentId, const std::map<std::string, int>& courseMarks) override;

    // Added missing method implementations
    bool updateResult(const std::string& studentId, const std::map<std::string, int>& marks) override;
    
    std::optional<char> getGrade(int marks) const override;
    
    bool declareResult(const std::string& studentId) override;

    std::optional<double> calculateCGPA(const std::string& studentId) const override;

     // SGPA calculation would be similar but filter results by semester
    std::optional<double> calculateSGPA(const std::string& studentId /*, int semester */) const override;

    std::string generateResultReport(const std::string& studentId) const override;

};

#endif // RESULT_SERVICE_H