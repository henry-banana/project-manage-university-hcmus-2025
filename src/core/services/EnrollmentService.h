#ifndef ENROLLMENT_SERVICE_H
#define ENROLLMENT_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include <optional>
#include "../entities/Student.h"
#include "../entities/Course.h"
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/ICourseRepository.h"

class IEnrollmentService {
public:
    virtual ~IEnrollmentService() = default;
    
    // Enrollment operations
    virtual bool enrollStudentInCourse(const std::string& studentId, const std::string& courseId) = 0;
    virtual bool dropCourse(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::vector<Course> getEnrolledCourses(const std::string& studentId) = 0;
    virtual std::vector<Student> getEnrolledStudents(const std::string& courseId) = 0;
    virtual bool isStudentEnrolled(const std::string& studentId, const std::string& courseId) = 0;
};

class EnrollmentService : public IEnrollmentService {
private:
    std::shared_ptr<IStudentRepository> _studentRepo;
    std::shared_ptr<ICourseRepository> _courseRepo;
    
public:
    EnrollmentService(
        std::shared_ptr<IStudentRepository> studentRepo,
        std::shared_ptr<ICourseRepository> courseRepo
    );
    
    // Enrollment operations
    bool enrollStudentInCourse(const std::string& studentId, const std::string& courseId) override;
    bool dropCourse(const std::string& studentId, const std::string& courseId) override;
    std::vector<Course> getEnrolledCourses(const std::string& studentId) override;
    std::vector<Student> getEnrolledStudents(const std::string& courseId) override;
    bool isStudentEnrolled(const std::string& studentId, const std::string& courseId) override;
};

#endif // ENROLLMENT_SERVICE_H