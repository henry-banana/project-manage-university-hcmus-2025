// --- START OF MODIFIED FILE src/core/data_access/mock/MockCourseResultDao.h ---
#ifndef MOCK_COURSE_RESULT_DAO_H
#define MOCK_COURSE_RESULT_DAO_H

#include "../interface/ICourseResultDao.h"
#include <map> 
#include <string> // (➕) Thêm cho makeCourseResultKey và map key
#include <vector> // (➕)

class MockCourseResultDao : public ICourseResultDao {
public:
    MockCourseResultDao();
    ~MockCourseResultDao() override = default;

    std::expected<CourseResult, Error> find(const std::string& studentId, const std::string& courseId) const override;
    std::expected<std::vector<CourseResult>, Error> findByStudentId(const std::string& studentId) const override;
    std::expected<std::vector<CourseResult>, Error> findByCourseId(const std::string& courseId) const override;
    std::expected<bool, Error> addOrUpdate(const CourseResult& result) override;
    std::expected<bool, Error> remove(const std::string& studentId, const std::string& courseId) override;
    std::expected<bool, Error> removeAllForStudent(const std::string& studentId) override;
    std::expected<bool, Error> removeAllForCourse(const std::string& courseId) override;

    static void initializeDefaultMockData();
    static void clearMockData();
};

#endif // MOCK_COURSE_RESULT_DAO_H
// --- END OF MODIFIED FILE src/core/data_access/mock/MockCourseResultDao.h ---