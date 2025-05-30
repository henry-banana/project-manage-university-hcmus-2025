// #include "gtest/gtest.h"
// #include "gmock/gmock.h"
// #include "../../../src/core/services/impl/CourseService.h"
// #include "../../../src/core/data_access/mock/MockCourseDao.h"
// #include "../../../src/core/data_access/mock/MockFacultyDao.h"
// #include "../../../src/core/data_access/mock/MockEnrollmentDao.h"
// #include "../../../src/core/data_access/mock/MockCourseResultDao.h"
// #include "../../../src/core/validators/interface/IValidator.h"
// #include "../../../src/core/services/SessionContext.h"
// #include "../../../src/core/entities/Course.h"
// #include "../../../src/common/ErrorType.h"

// #include <memory>
// #include <vector>
// #include <expected>

// using ::testing::Return;
// using ::testing::_;

// // Mock validator kế thừa đúng interface
// class MockGeneralInputValidator : public IGeneralInputValidator {
// public:
//     MOCK_METHOD(ValidationResult, validateEmail, (const std::string&), (const, override));
//     MOCK_METHOD(ValidationResult, validatePhoneNumber, (const std::string&, const std::string&), (const, override));
//     MOCK_METHOD(ValidationResult, validatePasswordComplexity, (const std::string&), (const, override));
//     MOCK_METHOD(ValidationResult, validateIdFormat, (const std::string&, const std::string&, size_t, size_t), (const, override));
//     MOCK_METHOD(ValidationResult, validateRequiredString, (const std::string&, const std::string&, size_t), (const, override));
//     MOCK_METHOD(ValidationResult, validateOptionalString, (const std::string&, const std::string&, size_t), (const, override));
//     MOCK_METHOD(ValidationResult, validateInteger, (long long, const std::string&, long long, long long), (const, override));
//     MOCK_METHOD(ValidationResult, validateDate, (int, int, int, const std::string&), (const, override));
//     MOCK_METHOD(ValidationResult, validateCitizenId, (const std::string&, const std::string&), (const, override));
// };

// // Mock session context
// class MockSessionContext : public SessionContext {
// public:
//     MOCK_METHOD(bool, isAuthenticated, (), (const, override));
//     MOCK_METHOD(std::optional<std::string>, getCurrentUserId, (), (const, override));
//     MOCK_METHOD(std::optional<UserRole>, getCurrentUserRole, (), (const, override));
//     MOCK_METHOD(std::optional<std::shared_ptr<User>>, getCurrentUser, (), (const, override));
// };

// // Fake/mock cho getById (do Google Mock chưa hỗ trợ std::expected tốt)
// class MyMockCourseDao : public MockCourseDao {
// public:
//     std::function<std::expected<Course, Error>(const std::string&)> getByIdFunc;
//     std::function<std::expected<std::vector<Course>, Error>()> getAllFunc;
//     std::function<std::expected<std::vector<Course>, Error>(const std::string&)> findByFacultyIdFunc;
//     std::function<std::expected<Course, Error>(const Course&)> addFunc;
//     std::function<std::expected<bool, Error>(const Course&)> updateFunc;
//     std::function<std::expected<bool, Error>(const std::string&)> removeFunc;

//     std::expected<Course, Error> getById(const std::string& id) const override {
//         return getByIdFunc ? getByIdFunc(id) : std::unexpected(Error{ErrorCode::NOT_FOUND, "not found"});
//     }
//     std::expected<std::vector<Course>, Error> getAll() const override {
//         return getAllFunc ? getAllFunc() : std::vector<Course>{};
//     }
//     std::expected<std::vector<Course>, Error> findByFacultyId(const std::string& facultyId) const override {
//         return findByFacultyIdFunc ? findByFacultyIdFunc(facultyId) : std::vector<Course>{};
//     }
//     std::expected<Course, Error> add(const Course& course) override {
//         return addFunc ? addFunc(course) : std::unexpected(Error{ErrorCode::OPERATION_FAILED, "add failed"});
//     }
//     std::expected<bool, Error> update(const Course& course) override {
//         return updateFunc ? updateFunc(course) : std::unexpected(Error{ErrorCode::NOT_FOUND, "update failed"});
//     }
//     std::expected<bool, Error> remove(const std::string& id) override {
//         return removeFunc ? removeFunc(id) : std::unexpected(Error{ErrorCode::NOT_FOUND, "remove failed"});
//     }
// };

// class MyMockFacultyDao : public MockFacultyDao {
// public:
//     std::function<bool(const std::string&)> existsFunc;

//     bool exists(const std::string& id) {
//         return existsFunc ? existsFunc(id) : false;
//     }
// };

// struct Enrollment {
//     // Dummy struct for testing
//     std::string studentId;
//     std::string courseId;
// };


// class MyMockEnrollmentDao : public MockEnrollmentDao {
// public:
//     std::function<std::expected<std::vector<EnrollmentRecord>, Error>(const std::string&)> findByCourseIdFunc;

//     std::expected<std::vector<EnrollmentRecord>, Error> findByCourseId(const std::string& courseId) const {
//         return findByCourseIdFunc ? findByCourseIdFunc(courseId) : std::vector<EnrollmentRecord>{};
//     }
//     // Nếu cần mock thêm các hàm khác, bổ sung tương tự
// };

// class MyMockCourseResultDao : public MockCourseResultDao {
// public:
//     std::function<std::expected<std::vector<CourseResult>, Error>(const std::string&)> findByCourseIdFunc;

//     std::expected<std::vector<CourseResult>, Error> findByCourseId(const std::string& courseId) const override {
//         return findByCourseIdFunc ? findByCourseIdFunc(courseId) : std::vector<CourseResult>{};
//     }
//     // Nếu cần mock thêm các hàm khác, bổ sung tương tự
// };

// class CourseServiceTest : public ::testing::Test {
// protected:
//     std::shared_ptr<MyMockCourseDao> mockCourseDao = std::make_shared<MyMockCourseDao>();
//     std::shared_ptr<MyMockFacultyDao> mockFacultyDao = std::make_shared<MyMockFacultyDao>();
//     std::shared_ptr<MyMockEnrollmentDao> mockEnrollmentDao = std::make_shared<MyMockEnrollmentDao>();
//     std::shared_ptr<MyMockCourseResultDao> mockCourseResultDao = std::make_shared<MyMockCourseResultDao>();
//     std::shared_ptr<MockGeneralInputValidator> mockValidator = std::make_shared<MockGeneralInputValidator>();
//     std::shared_ptr<MockSessionContext> mockSession = std::make_shared<MockSessionContext>();
//     CourseService service{mockCourseDao, mockFacultyDao, mockEnrollmentDao, mockCourseResultDao, mockValidator, mockSession};
// };

// TEST_F(CourseServiceTest, GetCourseById_Success) {
//     std::string id = "CS101";
//     Course course(id, "Intro", 3, "CS");
//     mockCourseDao->getByIdFunc = [=](const std::string& paramId) -> std::expected<Course, Error> {
//         if (paramId == id) return std::expected<Course, Error>(course);
//         return std::unexpected(Error{ErrorCode::NOT_FOUND, "not found"});
//     };
//     auto result = service.getCourseById(id);
//     ASSERT_TRUE(result.has_value());
//     ASSERT_EQ(result->getId(), id);
// }

// TEST_F(CourseServiceTest, GetCourseById_NotFound) {
//     mockCourseDao->getByIdFunc = [](const std::string&) {
//         return std::unexpected(Error{ErrorCode::NOT_FOUND, "not found"});
//     };
//     auto result = service.getCourseById("X");
//     ASSERT_FALSE(result.has_value());
//     ASSERT_EQ(result.error().code, ErrorCode::NOT_FOUND);
// }

// TEST_F(CourseServiceTest, GetAllCourses_Success) {
//     std::vector<Course> courses = {Course("A", "A", 1, "F"), Course("B", "B", 2, "F")};
//     mockCourseDao->getAllFunc = [=]() { return courses; };
//     auto result = service.getAllCourses();
//     ASSERT_TRUE(result.has_value());
//     ASSERT_EQ(result->size(), 2);
// }

// TEST_F(CourseServiceTest, GetCoursesByFaculty_Success) {
//     std::string facultyId = "F";
//     std::vector<Course> courses = {Course("A", "A", 1, facultyId)};
//     mockCourseDao->findByFacultyIdFunc = [=](const std::string& f) { return courses; };
//     auto result = service.getCoursesByFaculty(facultyId);
//     ASSERT_TRUE(result.has_value());
//     ASSERT_EQ(result->size(), 1);
//     ASSERT_EQ(result->at(0).getFacultyId(), facultyId);
// }

// TEST_F(CourseServiceTest, AddCourse_Success) {
//     std::string id = "C1", name = "Course1", facultyId = "F1";
//     int credits = 3;
//     Course course(id, name, credits, facultyId);

//     EXPECT_CALL(*mockValidator, validateIdFormat(id, _, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockValidator, validateRequiredString(name, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockValidator, validateInteger(credits, _, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     mockFacultyDao->existsFunc = [=](const std::string& id) { return id == facultyId; };
//     mockCourseDao->addFunc = [=](const Course&) { return course; };

//     auto result = service.addCourse(id, name, credits, facultyId);
//     ASSERT_TRUE(result.has_value());
//     ASSERT_EQ(result->getId(), id);
// }

// TEST_F(CourseServiceTest, AddCourse_FacultyNotFound) {
//     std::string id = "C1", name = "Course1", facultyId = "F1";
//     int credits = 3;
//     EXPECT_CALL(*mockValidator, validateIdFormat(id, _, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockValidator, validateRequiredString(name, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockValidator, validateInteger(credits, _, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     mockFacultyDao->existsFunc = [=](const std::string& id) { return id == facultyId; };
//     auto result = service.addCourse(id, name, credits, facultyId);
//     ASSERT_FALSE(result.has_value());
//     ASSERT_EQ(result.error().code, ErrorCode::NOT_FOUND);
// }

// TEST_F(CourseServiceTest, UpdateCourse_Success) {
//     std::string id = "C1", newName = "NewName", newFacultyId = "F2";
//     int newCredits = 4;
//     Course oldCourse(id, "Old", 3, "F1");
//     Course updatedCourse(id, newName, newCredits, newFacultyId);

//     mockCourseDao->getByIdFunc = [=](const std::string&) { return oldCourse; };
//     EXPECT_CALL(*mockValidator, validateIdFormat(id, _, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockValidator, validateRequiredString(newName, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockValidator, validateInteger(newCredits, _, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     mockFacultyDao->existsFunc = [=](const std::string& id) { return id == newFacultyId; };
//     mockCourseDao->updateFunc = [=](const Course&) { return true; };

//     auto result = service.updateCourse(id, newName, newCredits, newFacultyId);
//     ASSERT_TRUE(result.has_value());
//     ASSERT_TRUE(result.value());
// }

// TEST_F(CourseServiceTest, UpdateCourse_NotFound) {
//     std::string id = "C1", newName = "NewName", newFacultyId = "F2";
//     int newCredits = 4;
//     mockCourseDao->getByIdFunc = [](const std::string&) {
//         return std::unexpected(Error{ErrorCode::NOT_FOUND, "not found"});
//     };
//     EXPECT_CALL(*mockValidator, validateIdFormat(id, _, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockValidator, validateRequiredString(newName, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockValidator, validateInteger(newCredits, _, _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     mockFacultyDao->existsFunc = [=](const std::string& id) { return id == newFacultyId; };
//     auto result = service.updateCourse(id, newName, newCredits, newFacultyId);
//     ASSERT_FALSE(result.has_value());
//     ASSERT_EQ(result.error().code, ErrorCode::NOT_FOUND);
// }

// TEST_F(CourseServiceTest, RemoveCourse_Success) {
//     std::string id = "C1";
//     mockEnrollmentDao->findByCourseIdFunc = [=](const std::string&) { return std::vector<EnrollmentRecord>{}; };
//     mockCourseResultDao->findByCourseIdFunc = [=](const std::string&) { return std::vector<CourseResult>{}; };
//     mockCourseDao->removeFunc = [=](const std::string&) { return true; };
//     auto result = service.removeCourse(id);
//     ASSERT_TRUE(result.has_value());
//     ASSERT_TRUE(result.value());
// }

// TEST_F(CourseServiceTest, RemoveCourse_NotFound) {
//     std::string id = "C1";
//     mockEnrollmentDao->findByCourseIdFunc = [=](const std::string&) { return std::vector<EnrollmentRecord>{}; };
//     mockCourseResultDao->findByCourseIdFunc = [=](const std::string&) { return std::vector<CourseResult>{}; };
//     mockCourseDao->removeFunc = [=](const std::string&) {
//         return std::unexpected(Error{ErrorCode::NOT_FOUND, "not found"});
//     };
//     auto result = service.removeCourse(id);
//     ASSERT_FALSE(result.has_value());
//     ASSERT_EQ(result.error().code, ErrorCode::NOT_FOUND);
// }