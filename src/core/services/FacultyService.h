#ifndef FACULTY_SERVICE_H
#define FACULTY_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include <optional>
#include "../entities/Faculty.h"
#include "../repo/interface/IFacultyRepository.h"
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/ITeacherRepository.h"
#include "../repo/interface/ICourseRepository.h"

class IFacultyService {
public:
    virtual ~IFacultyService() = default;

    // Get faculty details
    virtual std::optional<Faculty> getFacultyById(const std::string& facultyId) const = 0;
    virtual std::optional<Faculty> getFacultyByName(const std::string& name) const = 0;

    // Get all faculties
    virtual std::vector<Faculty> getAllFaculties() const = 0;

    // Add a new faculty (requires authorization)
    virtual bool addFaculty(const Faculty& faculty) = 0;

    // Update faculty information (requires authorization)
    virtual bool updateFaculty(const Faculty& faculty) = 0;

    // Remove a faculty (requires authorization)
    // Note: Should consider impact on existing Students/Teachers/Courses referencing this faculty.
    // May require checks or cascade updates/deletes handled here or at DB level.
    virtual bool removeFaculty(const std::string& facultyId) = 0;
};

class FacultyService : public IFacultyService {
private:
    std::shared_ptr<IFacultyRepository> _facultyRepo;
    // Inject other repos if needed for referential integrity checks during removal
    std::shared_ptr<IStudentRepository> _studentRepo;
    std::shared_ptr<ITeacherRepository> _teacherRepo;
    std::shared_ptr<ICourseRepository> _courseRepo;


public:
    // Constructor with dependency injection
    FacultyService(std::shared_ptr<IFacultyRepository> facultyRepo,
                    std::shared_ptr<IStudentRepository> studentRepo, // Optional for checks
                    std::shared_ptr<ITeacherRepository> teacherRepo, // Optional for checks
                    std::shared_ptr<ICourseRepository> courseRepo);   // Optional for checks

    std::optional<Faculty> getFacultyById(const std::string& facultyId) const override;

    std::optional<Faculty> getFacultyByName(const std::string& name) const override;

    std::vector<Faculty> getAllFaculties() const override;

    bool addFaculty(const Faculty& faculty) override;

    bool updateFaculty(const Faculty& faculty) override;

    bool removeFaculty(const std::string& facultyId) override;
};

#endif // FACULTY_SERVICE_H