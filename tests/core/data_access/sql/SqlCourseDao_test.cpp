// --- START OF NEW FILE tests/core/data_access/sql/SqlCourseDao_test.cpp ---
#include "gtest/gtest.h"
#include "../../../../src/core/data_access/sql/SqlCourseDao.h"
#include "../../../../src/core/entities/Course.h"
#include "../../../../src/core/entities/Faculty.h"
#include "../../../../src/core/database_adapter/sql/SQLiteAdapter.h"
#include "../../../../src/core/parsing/impl_sql_parser/CourseSqlParser.h"
#include "../../../../src/common/ErrorType.h"
#include <memory>
#include <filesystem>

// Test Fixture cho SqlCourseDaoTest
class SqlCourseDaoTest : public ::testing::Test {
protected:
    std::shared_ptr<IDatabaseAdapter> dbAdapter;
    std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> courseParser;
    std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> facultyParser;
    std::unique_ptr<SqlCourseDao> courseDao;
    std::string dbPath;

    void setupDatabase() {
        dbPath = ":memory:"; // Sử dụng DB trong bộ nhớ để test độc lập
        dbAdapter = std::make_shared<SQLiteAdapter>();
        auto connectResult = dbAdapter->connect(dbPath);
        ASSERT_TRUE(connectResult.has_value()) << "Failed to connect to in-memory DB: " << (connectResult.has_value() ? "" : connectResult.error().message);
        ASSERT_TRUE(connectResult.value()) << "Connection to in-memory DB returned false.";

        // Đảm bảo bảng Courses tồn tại, giả sử hàm này tạo bảng nếu chưa có
        auto tableResult = std::static_pointer_cast<SQLiteAdapter>(dbAdapter)->ensureTablesExist();
        ASSERT_TRUE(tableResult.has_value()) << "Failed to ensure tables exist: " << (tableResult.has_value() ? "" : tableResult.error().message);
        ASSERT_TRUE(tableResult.value()) << "Ensuring tables failed.";

        courseParser = std::make_shared<CourseSqlParser>();
        courseDao = std::make_unique<SqlCourseDao>(dbAdapter, courseParser);
    }

    void SetUp() override {
        setupDatabase();
        clearCoursesTable();
    }

    void TearDown() override {
        if (dbAdapter && dbAdapter->isConnected()) {
            dbAdapter->disconnect();
        }
    }

    void clearCoursesTable() {
        if (courseDao && dbAdapter->isConnected()) {
            auto result = dbAdapter->executeUpdate("DELETE FROM Courses;");
            ASSERT_TRUE(result.has_value()) << "Failed to clear Courses table: " << result.error().message;
        }
    }
    bool facultyExists(const std::string& facultyId) {
    auto result = dbAdapter->executeQuery("SELECT 1 FROM Faculties WHERE id = ? LIMIT 1;", {facultyId});
    return result.has_value() && !result.value().empty();
    }
    // Thêm Course trực tiếp vào DB cho chuẩn bị test
    Course addCourseDirectly(const std::string& id, const std::string& name, int credits, const std::string& facultyId) {
        // 1. Đảm bảo Faculty tồn tại trước
        auto facultyResult = dbAdapter->executeQuery("SELECT 1 FROM Faculties WHERE id = ? LIMIT 1;", {facultyId});
        if (!facultyResult.has_value() || facultyResult.value().empty()) {
            // Nếu chưa có Faculty, thêm một cái tạm
            Faculty f(facultyId, "Default Faculty Name");
            auto facultyParams = facultyParser->toQueryInsertParams(f);
            EXPECT_TRUE(facultyParams.has_value());

            auto insertFaculty = dbAdapter->executeUpdate("INSERT INTO Faculties (id, name) VALUES (?, ?);", facultyParams.value());
            EXPECT_TRUE(insertFaculty.has_value()) << "Failed to insert faculty: " << insertFaculty.error().message;
        }

        // 2. Thêm Course
        Course c(id, name, credits, facultyId);
        auto paramsExp = courseParser->toQueryInsertParams(c);
        EXPECT_TRUE(paramsExp.has_value());

        auto execResult = dbAdapter->executeUpdate(
            "INSERT INTO Courses (id, name, credits, faculty_id) VALUES (?, ?, ?, ?);", 
            paramsExp.value());

        EXPECT_TRUE(execResult.has_value()) << "Direct add failed: " << (execResult.has_value() ? "" : execResult.error().message);
        if (!execResult.has_value() || execResult.value() == 0) {
            throw std::runtime_error("Failed to add course directly for test setup.");
        }

        return c;
    }
};

TEST_F(SqlCourseDaoTest, AddCourse_Success) {
    // Đảm bảo faculty "IT" tồn tại trước
    Faculty f("IT", "Information Technology");
    auto facParams = facultyParser->toQueryInsertParams(f);
    ASSERT_TRUE(facParams.has_value());
    auto facResult = dbAdapter->executeUpdate("INSERT INTO Faculties (id, name) VALUES (?, ?);", facParams.value());
    ASSERT_TRUE(facResult.has_value()) << "Failed to insert faculty: " << facResult.error().message;

    // Bắt đầu test thêm course
    Course course("CS101", "Intro to Programming", 3, "IT");
    auto result = courseDao->add(course);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_EQ(result.value().getId(), "CS101");
    EXPECT_EQ(result.value().getName(), "Intro to Programming");
    EXPECT_EQ(result.value().getCredits(), 3);
    EXPECT_EQ(result.value().getFacultyId(), "IT");

    // Kiểm tra lại bằng getById
    auto fetched = courseDao->getById("CS101");
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched.value().getName(), "Intro to Programming");
}

/*
TEST_F(SqlFacultyDaoTest, AddFaculty_Success) {
    Faculty faculty("IT", "Information Technology");
    auto result = facultyDao->add(faculty);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_EQ(result.value().getId(), "IT");
    EXPECT_EQ(result.value().getName(), "Information Technology");

    // Kiểm tra lại bằng cách getById
    auto fetched = facultyDao->getById("IT");
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched.value().getName(), "Information Technology");
}
*/
TEST_F(SqlCourseDaoTest, AddCourse_DuplicateId_ReturnsError) {
    addCourseDirectly("CS102", "Data Structures", 4, "IT");
    Course duplicate("CS102", "Algorithms", 4, "IT");
    auto result = courseDao->add(duplicate);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(SqlCourseDaoTest, GetById_ExistingCourse_ReturnsCourse) {
    addCourseDirectly("MA101", "Calculus I", 4, "Math");
    auto result = courseDao->getById("MA101");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getName(), "Calculus I");
}

TEST_F(SqlCourseDaoTest, GetById_NonExisting_ReturnsError) {
    auto result = courseDao->getById("NONEXIST");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlCourseDaoTest, GetAllCourses_ReturnsAll) {
    addCourseDirectly("PHY101", "Physics I", 3, "Science");
    addCourseDirectly("CHE101", "Chemistry I", 3, "Science");
    auto result = courseDao->getAll();
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result.value().size(), 2);
}

TEST_F(SqlCourseDaoTest, UpdateCourse_Success) {
    addCourseDirectly("CS103", "Programming Basics", 3, "IT");
    Course updated("CS103", "Advanced Programming", 4, "IT");
    auto result = courseDao->update(updated);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto fetched = courseDao->getById("CS103");
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched.value().getName(), "Advanced Programming");
    EXPECT_EQ(fetched.value().getCredits(), 4);
}

TEST_F(SqlCourseDaoTest, UpdateCourse_NotExists_ReturnsError) {
    Course nonExist("XX999", "Nonexistent", 3, "IT");
    auto result = courseDao->update(nonExist);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlCourseDaoTest, RemoveCourse_Success) {
    addCourseDirectly("CS104", "Discrete Math", 3, "IT");
    auto result = courseDao->remove("CS104");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto fetched = courseDao->getById("CS104");
    ASSERT_FALSE(fetched.has_value());
    EXPECT_EQ(fetched.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlCourseDaoTest, RemoveCourse_NotExists_ReturnsError) {
    auto result = courseDao->remove("NONEXIST_REM");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlCourseDaoTest, Exists_CourseExists_ReturnsTrue) {
    addCourseDirectly("CS105", "Operating Systems", 3, "IT");
    auto result = courseDao->exists("CS105");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(SqlCourseDaoTest, Exists_CourseNotExists_ReturnsFalse) {
    auto result = courseDao->exists("NONEXIST_EXISTS");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

TEST_F(SqlCourseDaoTest, FindByFacultyId_ReturnsCourses) {
    addCourseDirectly("CS201", "Networks", 3, "IT");
    addCourseDirectly("CS202", "Databases", 3, "IT");
    addCourseDirectly("MA201", "Linear Algebra", 3, "Math");

    auto result = courseDao->findByFacultyId("IT");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);

    for (const auto& c : result.value()) {
        EXPECT_EQ(c.getFacultyId(), "IT");
    }
}

TEST_F(SqlCourseDaoTest, FindByFacultyId_NoCourses_ReturnsEmptyVector) {
    auto result = courseDao->findByFacultyId("NONEXIST_FAC");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value().empty());
}
// --- END OF NEW FILE tests/core/data_access/sql/SqlCourseDao_test.cpp ---
