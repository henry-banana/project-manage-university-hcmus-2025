// --- START OF UPDATED FILE tests/core/data_access/sql/SqlStudentDao_test.cpp ---
#include "gtest/gtest.h"
#include "../../../../src/core/data_access/sql/SqlStudentDao.h"
#include "../../../../src/core/entities/Student.h"
#include "../../../../src/core/database_adapter/sql/SQLiteAdapter.h"
#include "../../../../src/core/parsing/impl_sql_parser/StudentSqlParser.h"
#include "../../../../src/common/ErrorType.h"
#include <memory>
#include <optional>

class SqlStudentDaoTest : public ::testing::Test {
protected:
    std::shared_ptr<IDatabaseAdapter> dbAdapter;
    std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> studentParser;
    std::unique_ptr<SqlStudentDao> studentDao;
    std::string dbPath;

    void setupDatabase() {
        dbPath = ":memory:";
        dbAdapter = std::make_shared<SQLiteAdapter>();
        auto connectResult = dbAdapter->connect(dbPath);
        ASSERT_TRUE(connectResult.has_value() && connectResult.value());

        auto tableResult = std::static_pointer_cast<SQLiteAdapter>(dbAdapter)->ensureTablesExist();
        ASSERT_TRUE(tableResult.has_value() && tableResult.value());

        studentParser = std::make_shared<StudentSqlParser>();
        studentDao = std::make_unique<SqlStudentDao>(dbAdapter, studentParser);
    }

    void SetUp() override {
        setupDatabase();
        clearStudentsTable();
    }

    void TearDown() override {
        if (dbAdapter && dbAdapter->isConnected()) {
            dbAdapter->disconnect();
        }
    }

    void clearStudentsTable() {
        if (studentDao && dbAdapter->isConnected()) {
            auto result = dbAdapter->executeUpdate("DELETE FROM Students;");
            ASSERT_TRUE(result.has_value());
        }
    }

    Student addStudentDirectly(const std::string& id,
                            const std::string& firstName,
                            const std::string& lastName,
                            const std::string& facultyId,
                            LoginStatus status = LoginStatus::PENDING_APPROVAL) {
        Student s(id, firstName, lastName, facultyId, status);
        auto paramsExp = studentParser->toQueryInsertParams(s); 
        EXPECT_TRUE(paramsExp.has_value());

        auto params = paramsExp.value();
        EXPECT_EQ(params.size(), 13); // Sanity check

        // Tách params cho Users và Students
        std::vector<std::any> userParams(params.begin(), params.begin() + 12);
        std::vector<std::any> studentParams = { std::any_cast<std::string>(params[0]), params[12] }; // id, facultyId

        // Insert vào Users
        auto userResult = dbAdapter->executeUpdate(
            "INSERT INTO Users (id, firstName, lastName, birthDay, birthMonth, birthYear, "
            "address, citizenId, email, phoneNumber, role, status) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",
            userParams
        );
        EXPECT_TRUE(userResult.has_value());

        // Insert vào Students
        auto studentResult = dbAdapter->executeUpdate(
            "INSERT INTO Students (id, faculty_id) VALUES (?, ?);",
            studentParams
        );
        EXPECT_TRUE(studentResult.has_value());

        return s;
    }

};

TEST_F(SqlStudentDaoTest, AddStudent_Success) {
    Student student("S001", "Nguyen", "Van A", "F001", LoginStatus::ACTIVE);
    auto result = studentDao->add(student);
    if (!result.has_value()) {
    std::cerr << "Add failed: " << result.error().message << std::endl;
    }
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_EQ(result.value().getId(), "S001");
    EXPECT_EQ(result.value().getFirstName(), "Nguyen");
    EXPECT_EQ(result.value().getLastName(), "Van A");
    EXPECT_EQ(result.value().getFacultyId(), "F001");
}

TEST_F(SqlStudentDaoTest, AddStudent_DuplicateId_ReturnsError) {
    addStudentDirectly("S002", "Tran", "Thi B", "F002");
    Student duplicate("S002", "Tran", "Thi B Updated", "F002");
    auto result = studentDao->add(duplicate);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(SqlStudentDaoTest, GetById_ExistingStudent_ReturnsStudent) {
    addStudentDirectly("S003", "Le", "Van C", "F003");
    auto result = studentDao->getById("S003");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getFirstName(), "Le");
    EXPECT_EQ(result.value().getLastName(), "Van C");
}

TEST_F(SqlStudentDaoTest, GetById_NotExist_ReturnsError) {
    auto result = studentDao->getById("S999");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlStudentDaoTest, GetAllStudents_ReturnsAll) {
    addStudentDirectly("S004", "Pham", "Van D", "F004");
    addStudentDirectly("S005", "Hoang", "Thi E", "F005");
    auto result = studentDao->getAll();
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result.value().size(), 2);
}

TEST_F(SqlStudentDaoTest, GetAllStudents_Empty_ReturnsEmptyVector) {
    auto result = studentDao->getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value().empty());
}

TEST_F(SqlStudentDaoTest, UpdateStudent_Success) {
    addStudentDirectly("S006", "Do", "Van F", "F006");
    Student updated("S006", "Do", "Van F Updated", "F006");
    auto result = studentDao->update(updated);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_TRUE(result.value());

    auto fetched = studentDao->getById("S006");
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched.value().getLastName(), "Van F Updated");
}

TEST_F(SqlStudentDaoTest, UpdateStudent_NotExist_ReturnsError) {
    Student nonExist("S9999", "Non", "Exist", "F999");
    auto result = studentDao->update(nonExist);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::VALIDATION_ERROR);
}

TEST_F(SqlStudentDaoTest, RemoveStudent_Success) {
    addStudentDirectly("S007", "Vu", "Thi G", "F007");
    auto result = studentDao->remove("S007");
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_TRUE(result.value());

    auto fetched = studentDao->getById("S007");
    ASSERT_FALSE(fetched.has_value());
    EXPECT_EQ(fetched.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlStudentDaoTest, RemoveStudent_NotExist_ReturnsError) {
    auto result = studentDao->remove("NONEXIST_STUDENT");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlStudentDaoTest, Exists_StudentExists_ReturnsTrue) {
    addStudentDirectly("S008", "Nguyen", "Van H", "F008");
    auto result = studentDao->exists("S008");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(SqlStudentDaoTest, Exists_StudentNotExists_ReturnsFalse) {
    auto result = studentDao->exists("NOT_EXIST_ID");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

// --- END OF UPDATED FILE tests/core/data_access/sql/SqlStudentDao_test.cpp ---
