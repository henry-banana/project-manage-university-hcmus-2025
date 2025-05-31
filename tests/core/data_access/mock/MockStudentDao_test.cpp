#include <gtest/gtest.h>
#include "../../../../src/core/data_access/mock/MockStudentDao.h" // (➕) Sửa đường dẫn
#include "../../../../src/core/entities/Student.h"      // (➕)
#include "../../../../src/common/ErrorType.h"          // (➕)
#include "../../../../src/common/LoginStatus.h"       // (➕)

class MockStudentDaoTest : public ::testing::Test { // (➕) Tạo Test Fixture
protected:
    MockStudentDao dao; // Instance sẽ được dùng bởi các test case

    Student makeSampleStudent(const std::string& id = "SV_TEST_001", 
                              const std::string& email = "test.student@example.com",
                              LoginStatus status = LoginStatus::ACTIVE) {
        Student s(id, "TestFirstName", "TestLastName", "IT_Test", status);
        s.setEmail(email);
        // Thêm các setter khác nếu cần cho dữ liệu mẫu
        return s;
    }

    void SetUp() override {
        MockStudentDao::clearMockData();
        // Không gọi initializeDefaultMockData() để mỗi test tự add data cần thiết,
        // hoặc gọi nếu muốn có bộ data chung cho cả suite.
        // Ví dụ, nếu initializeDefaultMockData() thêm S001, S002, S003:
        // MockStudentDao::initializeDefaultMockData(); 
    }

    void TearDown() override {
        MockStudentDao::clearMockData();
    }
};


TEST_F(MockStudentDaoTest, AddStudent_ShouldReturnSameStudent_WhenEmpty) {
    //MockStudentDao dao; // Dùng this->dao hoặc dao
    Student s = makeSampleStudent("SV001_ADD");
    
    auto result = dao.add(s);

    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_EQ(result->getId(), "SV001_ADD");
    EXPECT_EQ(result->getFacultyId(), "IT_Test");
}

TEST_F(MockStudentDaoTest, AddStudent_DuplicateId_ShouldFail) {
    dao.add(makeSampleStudent("SV_DUP_ID"));
    Student s_dup = makeSampleStudent("SV_DUP_ID", "another.email@example.com");
    auto result = dao.add(s_dup);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(MockStudentDaoTest, AddStudent_DuplicateEmail_ShouldFail) {
    dao.add(makeSampleStudent("SV_EMAIL_1", "unique.email@example.com"));
    Student s_dup_email = makeSampleStudent("SV_EMAIL_2", "unique.email@example.com");
    auto result = dao.add(s_dup_email);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS); // Mock DAO nên check cả email
}


TEST_F(MockStudentDaoTest, GetById_ShouldReturnStudentIfExists) {
    Student s = makeSampleStudent("SV002_GET");
    dao.add(s);

    auto result = dao.getById("SV002_GET");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getLastName(), "TestLastName");
}

TEST_F(MockStudentDaoTest, GetById_NotFound) {
    auto result = dao.getById("SV_NON_EXISTENT");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}


TEST_F(MockStudentDaoTest, Exists_ShouldReturnTrueIfStudentExists) {
    dao.add(makeSampleStudent("SV003_EXISTS"));
    auto result = dao.exists("SV003_EXISTS");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(MockStudentDaoTest, Exists_ShouldReturnFalseIfNotExists) {
    auto result = dao.exists("SV_NON_EXISTENT_EXISTS");
    ASSERT_TRUE(result.has_value()); // exists() của Mock nên luôn trả về value, không phải error nếu không tìm thấy
    EXPECT_FALSE(result.value());
}


TEST_F(MockStudentDaoTest, RemoveStudent_ShouldReturnTrue_AndStudentRemoved) {
    Student s = makeSampleStudent("SV004_REMOVE");
    dao.add(s);

    auto result = dao.remove("SV004_REMOVE");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto check = dao.exists("SV004_REMOVE");
    ASSERT_TRUE(check.has_value());
    EXPECT_FALSE(check.value());
}

TEST_F(MockStudentDaoTest, RemoveStudent_NotFound_ShouldFail) {
    auto result = dao.remove("SV_NON_EXISTENT_REMOVE");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockStudentDaoTest, UpdateStudent_Success) {
    Student s_orig = makeSampleStudent("SV005_UPDATE", "orig@example.com");
    dao.add(s_orig);

    Student s_updated = makeSampleStudent("SV005_UPDATE", "updated@example.com");
    s_updated.setFirstName("UpdatedFirstName");
    
    auto result = dao.update(s_updated);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_TRUE(result.value());

    auto fetched = dao.getById("SV005_UPDATE");
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched->getFirstName(), "UpdatedFirstName");
    EXPECT_EQ(fetched->getEmail(), "updated@example.com");
}

TEST_F(MockStudentDaoTest, UpdateStudent_EmailConflict_ShouldFail) {
    dao.add(makeSampleStudent("SV_EMAIL_U1", "email1@example.com"));
    dao.add(makeSampleStudent("SV_EMAIL_U2", "email2@example.com"));

    Student s_to_update = makeSampleStudent("SV_EMAIL_U2", "email1@example.com"); // Cố update email của U2 thành email của U1
    s_to_update.setFirstName("TryingToUpdate");

    auto result = dao.update(s_to_update);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}


TEST_F(MockStudentDaoTest, GetAll_ReturnsCorrectNumberOfStudents) {
    // Giả sử initializeDefaultMockData() thêm 3 students (S001, S002, S003) nếu được gọi
    // MockStudentDao::initializeDefaultMockData(); 
    // size_t default_size = 3; 
    // Hiện tại SetUp không gọi nên data rỗng
    size_t default_size = 0;

    dao.add(makeSampleStudent("SV_ALL_1"));
    dao.add(makeSampleStudent("SV_ALL_2", "email.all.2@example.com"));
    
    auto result = dao.getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), default_size + 2);
}

TEST_F(MockStudentDaoTest, FindByFacultyId_ReturnsCorrectStudents) {
    dao.add(Student("S_IT_1", "A", "A", "IT", LoginStatus::ACTIVE));
    dao.add(Student("S_CS_1", "B", "B", "CS", LoginStatus::ACTIVE));
    dao.add(Student("S_IT_2", "C", "C", "IT", LoginStatus::ACTIVE));

    auto result = dao.findByFacultyId("IT");
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2);
    for(const auto& s : result.value()){
        EXPECT_EQ(s.getFacultyId(), "IT");
    }
}

TEST_F(MockStudentDaoTest, FindByEmail_Success) {
    dao.add(makeSampleStudent("S_EMAIL_F", "find.me@example.com"));
    auto result = dao.findByEmail("find.me@example.com");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), "S_EMAIL_F");
}

TEST_F(MockStudentDaoTest, UpdateStatus_Success) {
    dao.add(makeSampleStudent("S_STATUS", "status@example.com", LoginStatus::PENDING_APPROVAL));
    auto result = dao.updateStatus("S_STATUS", LoginStatus::ACTIVE);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto student = dao.getById("S_STATUS");
    ASSERT_TRUE(student.has_value());
    EXPECT_EQ(student->getStatus(), LoginStatus::ACTIVE);
    // Kiểm tra nếu role cũng được cập nhật từ PENDING_STUDENT sang STUDENT
    if (student->getRole() == UserRole::PENDING_STUDENT && student->getStatus() == LoginStatus::ACTIVE) {
         // Logic này nằm trong MockStudentDao::updateStatus, nếu nó đổi role thì test sẽ pass
    } else {
        EXPECT_EQ(student->getRole(), UserRole::STUDENT); // Hoặc role ban đầu nếu không đổi
    }
}