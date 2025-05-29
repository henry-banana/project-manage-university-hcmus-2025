#include <gtest/gtest.h>
#include "../src/core/data_access/mock/MockCourseResultDao.h"
#include "../src/core/entities/CourseResult.h"
#include <expected>

// Hàm hỗ trợ tạo kết quả học tập mẫu
CourseResult makeResult(const std::string& studentId, const std::string& courseId, int marks = 75) {
    return CourseResult(studentId, courseId, marks);
}

// Lớp test kế thừa ::testing::Test để cô lập dữ liệu
class CourseResultDaoTest : public ::testing::Test {
protected:
    MockCourseResultDao dao;

    void SetUp() override {
        // Không cần reset nếu MockCourseResultDao không dùng static
    }
};

TEST_F(CourseResultDaoTest, AddOrUpdateAndFind) {
    auto result = makeResult("SV001", "CS101", 90);
    auto addRes = dao.addOrUpdate(result);
    ASSERT_TRUE(addRes.has_value());
    EXPECT_TRUE(addRes.value());

    auto findRes = dao.find("SV001", "CS101");
    ASSERT_TRUE(findRes.has_value());
    EXPECT_EQ(findRes->getStudentId(), "SV001");
    EXPECT_EQ(findRes->getCourseId(), "CS101");
    EXPECT_EQ(findRes->getMarks(), 90);
    EXPECT_EQ(findRes->getGrade(), 'A');
}

TEST_F(CourseResultDaoTest, UpdateExistingRecord) {
    dao.addOrUpdate(makeResult("SV002", "CS101", 60));
    dao.addOrUpdate(makeResult("SV002", "CS101", 70));

    auto res = dao.find("SV002", "CS101");
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res->getMarks(), 70);
}

TEST_F(CourseResultDaoTest, Find_NotFound) {
    auto res = dao.find("NOONE", "NONE");
    EXPECT_FALSE(res.has_value());
}

TEST_F(CourseResultDaoTest, FindByStudentId) {
    dao.addOrUpdate(makeResult("SV003", "CS101", 80));
    dao.addOrUpdate(makeResult("SV003", "CS102", 70));
    dao.addOrUpdate(makeResult("SV004", "CS101", 50));

    auto results = dao.findByStudentId("SV003");
    ASSERT_TRUE(results.has_value());
    EXPECT_EQ(results->size(), 2);
}

TEST_F(CourseResultDaoTest, FindByCourseId) {
    dao.addOrUpdate(makeResult("SV005", "CS202", 65));
    dao.addOrUpdate(makeResult("SV006", "CS202", 55));

    auto results = dao.findByCourseId("CS202");
    ASSERT_TRUE(results.has_value());
    EXPECT_EQ(results->size(), 2);
}

TEST_F(CourseResultDaoTest, RemoveSpecificResult) {
    dao.addOrUpdate(makeResult("SV007", "CS303", 77));
    auto removeRes = dao.remove("SV007", "CS303");
    ASSERT_TRUE(removeRes.has_value());
    EXPECT_TRUE(removeRes.value());

    auto res = dao.find("SV007", "CS303");
    EXPECT_FALSE(res.has_value());
}

TEST_F(CourseResultDaoTest, RemoveAllForStudent) {
    dao.addOrUpdate(makeResult("SV008", "CS401", 50));
    dao.addOrUpdate(makeResult("SV008", "CS402", 60));

    auto rmAll = dao.removeAllForStudent("SV008");
    ASSERT_TRUE(rmAll.has_value());
    EXPECT_TRUE(rmAll.value());

    auto byStudent = dao.findByStudentId("SV008");
    ASSERT_TRUE(byStudent.has_value());
    EXPECT_TRUE(byStudent->empty());
}

TEST_F(CourseResultDaoTest, RemoveAllForCourse) {
    dao.addOrUpdate(makeResult("SV009", "CS500", 60));
    dao.addOrUpdate(makeResult("SV010", "CS500", 70));

    auto rmAll = dao.removeAllForCourse("CS500");
    ASSERT_TRUE(rmAll.has_value());
    EXPECT_TRUE(rmAll.value());

    auto byCourse = dao.findByCourseId("CS500");
    ASSERT_TRUE(byCourse.has_value());
    EXPECT_TRUE(byCourse->empty());
}
