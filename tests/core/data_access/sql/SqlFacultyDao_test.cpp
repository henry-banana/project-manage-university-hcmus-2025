// --- START OF NEW FILE tests/core/data_access/sql/SqlFacultyDao_test.cpp ---
#include "gtest/gtest.h"
#include "../../../../src/core/data_access/sql/SqlFacultyDao.h"
#include "../../../../src/core/entities/Faculty.h"
#include "../../../../src/core/database_adapter/sql/SQLiteAdapter.h" // Để tạo DB adapter
#include "../../../../src/core/parsing/impl_sql_parser/FacultySqlParser.h" // Để tạo parser
#include "../../../../src/common/ErrorType.h"
#include <memory>
#include <filesystem> // Để có thể xóa file DB nếu test với file thật

// Test Fixture cho SqlFacultyDaoTest
class SqlFacultyDaoTest : public ::testing::Test {
protected:
    std::shared_ptr<IDatabaseAdapter> dbAdapter;
    std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> facultyParser;
    std::unique_ptr<SqlFacultyDao> facultyDao;
    std::string dbPath; // Sẽ dùng :memory:

    // Hàm tiện ích để tạo CSDL và schema cho mỗi test (hoặc mỗi test suite)
    void setupDatabase() {
        // Sử dụng CSDL trong bộ nhớ cho mỗi test để đảm bảo tính độc lập
        dbPath = ":memory:"; 
        
        dbAdapter = std::make_shared<SQLiteAdapter>();
        auto connectResult = dbAdapter->connect(dbPath);
        ASSERT_TRUE(connectResult.has_value()) << "Failed to connect to in-memory DB: " << (connectResult.has_value() ? "" : connectResult.error().message);
        ASSERT_TRUE(connectResult.value()) << "Connection to in-memory DB returned false.";

        // Đảm bảo bảng Faculties tồn tại (SQLiteAdapter::ensureTablesExist sẽ làm việc này)
        auto tableResult = std::static_pointer_cast<SQLiteAdapter>(dbAdapter)->ensureTablesExist();
        ASSERT_TRUE(tableResult.has_value()) << "Failed to ensure tables exist: " << (tableResult.has_value() ? "" : tableResult.error().message);
        ASSERT_TRUE(tableResult.value()) << "Ensuring tables failed.";
        
        facultyParser = std::make_shared<FacultySqlParser>();
        facultyDao = std::make_unique<SqlFacultyDao>(dbAdapter, facultyParser);
    }

    void SetUp() override {
        setupDatabase();
        // Xóa dữ liệu từ test trước (nếu dùng file DB và không xóa file sau mỗi test)
        // Với :memory:, mỗi lần setupDatabase là một DB mới
        clearFacultiesTable(); 
    }

    void TearDown() override {
        // Với :memory:, CSDL tự động bị xóa khi disconnect.
        if (dbAdapter && dbAdapter->isConnected()) {
            dbAdapter->disconnect();
        }
        // Nếu test với file thật, có thể xóa file ở đây:
        // if (std::filesystem::exists(dbPath) && dbPath != ":memory:") {
        //     std::filesystem::remove(dbPath);
        // }
    }

    void clearFacultiesTable() {
        if (facultyDao && dbAdapter->isConnected()) { // (Thêm) Kiểm tra dbAdapter->isConnected()
            auto result = dbAdapter->executeUpdate("DELETE FROM Faculties;");
            ASSERT_TRUE(result.has_value()) << "Failed to clear Faculties table: " << result.error().message;
        }
    }

    // Hàm tiện ích để thêm faculty vào DB cho test
    Faculty addFacultyDirectly(const std::string& id, const std::string& name) {
        Faculty f(id, name);
        auto paramsExp = facultyParser->toQueryInsertParams(f);
        EXPECT_TRUE(paramsExp.has_value());
        auto execResult = dbAdapter->executeUpdate("INSERT INTO Faculties (id, name) VALUES (?, ?);", paramsExp.value());
        EXPECT_TRUE(execResult.has_value()) << "Direct add failed: " << (execResult.has_value() ? "" : execResult.error().message);
        if (!execResult.has_value() || execResult.value() == 0) {
            throw std::runtime_error("Failed to add faculty directly for test setup.");
        }
        return f;
    }
};

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

TEST_F(SqlFacultyDaoTest, AddFaculty_DuplicateId_ReturnsError) {
    addFacultyDirectly("CS", "Computer Science"); // Thêm một khoa trước
    Faculty duplicateFaculty("CS", "Computational Science");
    auto result = facultyDao->add(duplicateFaculty);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS); // SQLite sẽ trả về SQLITE_CONSTRAINT_PRIMARYKEY
}

TEST_F(SqlFacultyDaoTest, AddFaculty_DuplicateName_ReturnsError) {
    addFacultyDirectly("EE", "Electrical Engineering");
    Faculty facultyWithSameName("ME", "Electrical Engineering"); // ID khác, tên trùng
    auto result = facultyDao->add(facultyWithSameName);
    ASSERT_FALSE(result.has_value());
    // SQLiteAdapter nên chuyển SQLITE_CONSTRAINT_UNIQUE thành ALREADY_EXISTS
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS); 
}


TEST_F(SqlFacultyDaoTest, GetById_FacultyExists_ReturnsFaculty) {
    addFacultyDirectly("LAW", "Law Faculty");
    auto result = facultyDao->getById("LAW");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getId(), "LAW");
    EXPECT_EQ(result.value().getName(), "Law Faculty");
}

TEST_F(SqlFacultyDaoTest, GetById_FacultyNotExists_ReturnsError) {
    auto result = facultyDao->getById("NONEXIST");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlFacultyDaoTest, GetAllFaculties_ReturnsAll) {
    addFacultyDirectly("FA1", "Faculty Alpha");
    addFacultyDirectly("FB2", "Faculty Beta");
    
    auto result = facultyDao->getAll();
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result.value().size(), 2);
    // Có thể kiểm tra chi tiết hơn nếu muốn
}

TEST_F(SqlFacultyDaoTest, GetAllFaculties_Empty_ReturnsEmptyVector) {
    auto result = facultyDao->getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value().empty());
}


TEST_F(SqlFacultyDaoTest, UpdateFaculty_Success) {
    addFacultyDirectly("MED", "Medicine");
    Faculty updatedFaculty("MED", "Medical Science");
    auto result = facultyDao->update(updatedFaculty);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_TRUE(result.value());

    auto fetched = facultyDao->getById("MED");
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched.value().getName(), "Medical Science");
}

TEST_F(SqlFacultyDaoTest, UpdateFaculty_NameToExisting_ReturnsError) {
    addFacultyDirectly("ART", "Arts");
    addFacultyDirectly("SCI", "Science");
    Faculty facultyToUpdate("SCI", "Arts"); // Cố update tên của SCI thành tên của ART

    auto result = facultyDao->update(facultyToUpdate);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS); // Do UNIQUE constraint trên name
}

TEST_F(SqlFacultyDaoTest, UpdateFaculty_NotExists_ReturnsError) {
    Faculty nonExistentFaculty("XYZ", "Non Existent");
    auto result = facultyDao->update(nonExistentFaculty);
    ASSERT_FALSE(result.has_value());
    // Sql DAO sẽ trả về NOT_FOUND nếu executeUpdate trả về 0 rows affected
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND); 
}

TEST_F(SqlFacultyDaoTest, RemoveFaculty_Success) {
    addFacultyDirectly("ENG", "Engineering");
    auto result = facultyDao->remove("ENG");
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_TRUE(result.value());

    auto fetched = facultyDao->getById("ENG");
    ASSERT_FALSE(fetched.has_value());
    EXPECT_EQ(fetched.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlFacultyDaoTest, RemoveFaculty_NotExists_ReturnsError) {
    auto result = facultyDao->remove("NONEXIST_REM");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlFacultyDaoTest, Exists_FacultyExists_ReturnsTrue) {
    addFacultyDirectly("MUS", "Music");
    auto result = facultyDao->exists("MUS");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(SqlFacultyDaoTest, Exists_FacultyNotExists_ReturnsFalse) {
    auto result = facultyDao->exists("XYZ_EXISTS");
    ASSERT_TRUE(result.has_value()); // exists nên trả về value (false), không phải error
    EXPECT_FALSE(result.value());
}

TEST_F(SqlFacultyDaoTest, FindByName_FacultyExists_ReturnsFaculty) {
    addFacultyDirectly("PHY", "Physics Department");
    auto result = facultyDao->findByName("Physics Department");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getId(), "PHY");
}

TEST_F(SqlFacultyDaoTest, FindByName_FacultyNotExists_ReturnsError) {
    auto result = facultyDao->findByName("Non Existent Faculty Name");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}
// --- END OF NEW FILE tests/core/data_access/sql/SqlFacultyDao_test.cpp ---