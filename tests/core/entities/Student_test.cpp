#include <gtest/gtest.h>
#include "../src/core/entities/Student.h"
#include "../src/core/entities/User.h"
#include "../src/common/LoginStatus.h"
#include "../src/common/UserRole.h"

// Giả lập hoặc include đầy đủ các enum, struct, class cần thiết như LoginStatus, ValidationResult
// Giả lập class User nếu chưa có
// Giả lập class Faculty nếu cần (ở đây Student chỉ dùng string facultyId nên không cần)

// Test constructor và getter
TEST(StudentTest, ConstructorAndGetters) {
    Student s("SV100", "Minh", "Le", "CS", LoginStatus::PENDING_APPROVAL);

    EXPECT_EQ(s.getId(), "SV100");                // kế thừa User
    EXPECT_EQ(s.getFirstName(), "Minh");
    EXPECT_EQ(s.getLastName(), "Le");
    EXPECT_EQ(s.getFacultyId(), "CS");
    EXPECT_EQ(s.getStatus(), LoginStatus::PENDING_APPROVAL);
}

// Test setter hợp lệ
TEST(StudentTest, SetFacultyIdValid) {
    Student s("SV101", "Lan", "Pham", "Math");

    EXPECT_TRUE(s.setFacultyId("Physics"));
    EXPECT_EQ(s.getFacultyId(), "Physics");
}

// Test setter không hợp lệ
TEST(StudentTest, SetFacultyIdInvalid) {
    Student s("SV102", "Hoa", "Tran", "Chem");
    
    // Giả sử setFacultyId trả về false nếu empty
    EXPECT_FALSE(s.setFacultyId(""));
    EXPECT_EQ(s.getFacultyId(), "Chem");  // Không đổi giá trị
}

// Test hàm display
TEST(StudentTest, DisplayIncludesFaculty) {
    Student s("SV103", "Nam", "Nguyen", "Biology");

    std::string disp = s.display();

    EXPECT_NE(disp.find("Nam"), std::string::npos);
    EXPECT_NE(disp.find("Nguyen"), std::string::npos);
    EXPECT_NE(disp.find("Biology"), std::string::npos);
    EXPECT_NE(disp.find("SV103"), std::string::npos);
}

// Test validateBasic trả về VALID khi dữ liệu đầy đủ
TEST(StudentTest, ValidateBasicReturnsValid) {
    Student s("SV104", "Quang", "Do", "Economics");
    s.setAddress("123 Street");
    s.setCitizenId("123456789012");
    s.setEmail("sd@d.com");
    s.setPhoneNumber("0987654321");
    s.setBirthday(1, 1, 2000); // Giả sử ngày sinh hợp lệ
    s.setRole(UserRole::STUDENT); // Giả sử role hợp lệ
    s.setStatus(LoginStatus::ACTIVE); // Giả sử status hợp lệ

    auto result = s.validateBasic();

    EXPECT_TRUE(result.isValid);
    EXPECT_TRUE(result.errors.empty());
}

// Test validateBasic trả về INVALID khi dữ liệu thiếu
TEST(StudentTest, ValidateBasicReturnsInvalidWhenMissingFields) {
    Student s1("", "An", "Hoang", "CS");
    Student s2("SV105", "", "Hoang", "CS");
    Student s3("SV106", "An", "", "CS");
    Student s4("SV107", "An", "Hoang", "");  // thiếu facultyId

    EXPECT_FALSE(s1.validateBasic().isValid);
    EXPECT_FALSE(s2.validateBasic().isValid);
    EXPECT_FALSE(s3.validateBasic().isValid);
    EXPECT_FALSE(s4.validateBasic().isValid);
}
