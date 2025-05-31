// --- START OF MODIFIED FILE tests/core/data_access/mock/MockCourseResultDao_test.cpp ---
#include <gtest/gtest.h>
#include "../../../../src/core/data_access/mock/MockCourseResultDao.h" // (➕)
#include "../../../../src/core/entities/CourseResult.h"     // (➕)
#include "../../../../src/common/ErrorType.h"         // (➕)
#include <expected>

CourseResult makeResult(const std::string& studentId, const std::string& courseId, int marks = 75) {
    return CourseResult(studentId, courseId, marks);
}

class MockCourseResultDaoTest : public ::testing::Test { // (➕)
protected:
    MockCourseResultDao dao;

    void SetUp() override {
        MockCourseResultDao::clearMockData();
        // MockCourseResultDao::initializeDefaultMockData(); // Gọi nếu muốn data mặc định
    }
    void TearDown() override {
        MockCourseResultDao::clearMockData();
    }
};

TEST_F(MockCourseResultDaoTest, AddOrUpdateAndFind) {
    auto result = makeResult("SV001_Test", "CS101_Test", 90);
    auto addRes = dao.addOrUpdate(result);
    ASSERT_TRUE(addRes.has_value()) << addRes.error().message;
    EXPECT_TRUE(addRes.value());

    auto findRes = dao.find("SV001_Test", "CS101_Test");
    ASSERT_TRUE(findRes.has_value());
    EXPECT_EQ(findRes->getStudentId(), "SV001_Test");
    EXPECT_EQ(findRes->getCourseId(), "CS101_Test");
    EXPECT_EQ(findRes->getMarks(), 90);
    // EXPECT_EQ(findRes->getGrade(), 'A'); // Grade phụ thuộc vào logic của CourseResult
}

TEST_F(MockCourseResultDaoTest, UpdateExistingRecord) {
    dao.addOrUpdate(makeResult("SV002_Upd", "CS101_Upd", 60));
    auto addOrUpdateAgain = dao.addOrUpdate(makeResult("SV002_Upd", "CS101_Upd", 70)); // Update
    ASSERT_TRUE(addOrUpdateAgain.has_value()) << addOrUpdateAgain.error().message;


    auto res = dao.find("SV002_Upd", "CS101_Upd");
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res->getMarks(), 70);
}

TEST_F(MockCourseResultDaoTest, Find_NotFound) {
    auto res = dao.find("NOONE_Find", "NONE_Find");
    EXPECT_FALSE(res.has_value());
    EXPECT_EQ(res.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockCourseResultDaoTest, FindByStudentId_Success) {
    MockCourseResultDao::initializeDefaultMockData(); // Dùng data mặc định để test hàm này
    // initializeDefaultMockData thêm S001-CS101, S001-IT202, S002-CS101, S003-IT202
    dao.addOrUpdate(makeResult("SV003_Test", "CS_Test_101", 80));
    dao.addOrUpdate(makeResult("SV003_Test", "CS_Test_102", 70));
    dao.addOrUpdate(makeResult("SV004_Test", "CS_Test_101", 50));

    auto results = dao.findByStudentId("SV003_Test");
    ASSERT_TRUE(results.has_value());
    EXPECT_EQ(results->size(), 2); // Chỉ có 2 record cho SV003_Test
}

TEST_F(MockCourseResultDaoTest, FindByStudentId_WithDefaultData) {
    MockCourseResultDao::initializeDefaultMockData();
    auto results = dao.findByStudentId("S001"); // S001 có 2 record trong default data
    ASSERT_TRUE(results.has_value());
    EXPECT_EQ(results->size(), 2);
}


TEST_F(MockCourseResultDaoTest, FindByCourseId_Success) {
    MockCourseResultDao::initializeDefaultMockData();
    dao.addOrUpdate(makeResult("SV005_Test", "CS_Test_202", 65));
    dao.addOrUpdate(makeResult("SV006_Test", "CS_Test_202", 55));

    auto results = dao.findByCourseId("CS_Test_202");
    ASSERT_TRUE(results.has_value());
    EXPECT_EQ(results->size(), 2);
}

TEST_F(MockCourseResultDaoTest, FindByCourseId_WithDefaultData) {
    MockCourseResultDao::initializeDefaultMockData();
    auto results = dao.findByCourseId("CS101"); // CS101 có S001 và S002 trong default
    ASSERT_TRUE(results.has_value());
    EXPECT_EQ(results->size(), 2);
}


TEST_F(MockCourseResultDaoTest, RemoveSpecificResult_Success) {
    dao.addOrUpdate(makeResult("SV007_Rem", "CS303_Rem", 77));
    auto removeRes = dao.remove("SV007_Rem", "CS303_Rem");
    ASSERT_TRUE(removeRes.has_value()) << removeRes.error().message;
    EXPECT_TRUE(removeRes.value());

    auto res = dao.find("SV007_Rem", "CS303_Rem");
    EXPECT_FALSE(res.has_value());
    if (!res.has_value()) { // Thêm kiểm tra này để an toàn hơn
        EXPECT_EQ(res.error().code, ErrorCode::NOT_FOUND);
    }
}

TEST_F(MockCourseResultDaoTest, RemoveSpecificResult_NotFound_ShouldFail) {
    auto removeRes = dao.remove("SV_NON_REM", "CS_NON_REM");
    ASSERT_FALSE(removeRes.has_value());
    EXPECT_EQ(removeRes.error().code, ErrorCode::NOT_FOUND);
}


TEST_F(MockCourseResultDaoTest, RemoveAllForStudent_Success) {
    dao.addOrUpdate(makeResult("SV008_RemAll", "CS401_RemAll", 50));
    dao.addOrUpdate(makeResult("SV008_RemAll", "CS402_RemAll", 60));
    dao.addOrUpdate(makeResult("SV009_Other", "CS401_RemAll", 60)); // Record khác không bị xóa

    auto rmAll = dao.removeAllForStudent("SV008_RemAll");
    ASSERT_TRUE(rmAll.has_value()) << rmAll.error().message;
    EXPECT_TRUE(rmAll.value());

    auto byStudent = dao.findByStudentId("SV008_RemAll");
    ASSERT_TRUE(byStudent.has_value());
    EXPECT_TRUE(byStudent->empty());

    auto otherStudent = dao.findByStudentId("SV009_Other"); // Kiểm tra student khác còn
    ASSERT_TRUE(otherStudent.has_value());
    EXPECT_EQ(otherStudent->size(), 1);
}

TEST_F(MockCourseResultDaoTest, RemoveAllForCourse_Success) {
    dao.addOrUpdate(makeResult("SV009_RemCrs", "CS500_RemCrs", 60));
    dao.addOrUpdate(makeResult("SV010_RemCrs", "CS500_RemCrs", 70));
    dao.addOrUpdate(makeResult("SV009_RemCrs", "CS501_Other", 70)); // Record khác không bị xóa

    auto rmAll = dao.removeAllForCourse("CS500_RemCrs");
    ASSERT_TRUE(rmAll.has_value()) << rmAll.error().message;
    EXPECT_TRUE(rmAll.value());

    auto byCourse = dao.findByCourseId("CS500_RemCrs");
    ASSERT_TRUE(byCourse.has_value());
    EXPECT_TRUE(byCourse->empty());

    auto otherCourse = dao.findByCourseId("CS501_Other"); // Kiểm tra course khác còn
    ASSERT_TRUE(otherCourse.has_value());
    EXPECT_EQ(otherCourse->size(), 1);
}
// --- END OF MODIFIED FILE tests/core/data_access/mock/MockCourseResultDao_test.cpp ---