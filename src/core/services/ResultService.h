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

class IResultService {
public:
    virtual ~IResultService() = default;
    
    // Result operations
    virtual std::optional<Result> getStudentResult(const std::string& studentId) const = 0;
    virtual bool updateResult(const std::string& studentId, const std::map<std::string, int>& marks) = 0;
    virtual std::optional<float> calculateCGPA(const std::string& studentId) const = 0;
    virtual std::optional<char> getGrade(int marks) const = 0;
    virtual bool declareResult(const std::string& studentId) = 0;
    virtual bool generateResultReport(const std::string& studentId) = 0;
};

class ResultService : public IResultService {
private:
    std::shared_ptr<IResultRepository> _resultRepo;
    std::shared_ptr<IStudentRepository> _studentRepo;
    std::shared_ptr<ICourseRepository> _courseRepo;
    
    // Helper methods
    int getTotalCredits(const std::vector<Course>& courses) const;
    int getEarnedCredits(const std::map<std::string, int>& marks, const std::vector<Course>& courses) const;
    
public:
    ResultService(
        std::shared_ptr<IResultRepository> resultRepo,
        std::shared_ptr<IStudentRepository> studentRepo,
        std::shared_ptr<ICourseRepository> courseRepo
    );
    
    // Result operations
    std::optional<Result> getStudentResult(const std::string& studentId) const override;
    bool updateResult(const std::string& studentId, const std::map<std::string, int>& marks) override;
    std::optional<float> calculateCGPA(const std::string& studentId) const override;
    std::optional<char> getGrade(int marks) const override;
    bool declareResult(const std::string& studentId) override;
    bool generateResultReport(const std::string& studentId) override;
};

#endif // RESULT_SERVICE_H