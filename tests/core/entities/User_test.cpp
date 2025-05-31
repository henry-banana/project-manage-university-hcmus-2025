#include <gtest/gtest.h>
#include "../src/core/entities/User.h"
#include "../src/common/UserRole.h"
#include "../src/common/LoginStatus.h"
#include "../src/core/entities/Birthday.h"

// Subclass of User to instantiate abstract class for testing
class TestUser : public User {
public:
    TestUser(std::string id,
             std::string firstName,
             std::string lastName,
             UserRole role = UserRole::STUDENT,
             LoginStatus status = LoginStatus::ACTIVE)
        : User(id, firstName, lastName, role, status) {}

    std::string display() const override {
        return getFullName();
    }

    ValidationResult validateBasic() const override {
        return ValidationResult(); // Stub
    }
};

TEST(UserTest, ConstructorAndGetters) {
    TestUser user("SV001", "Linh", "Nguyen", UserRole::STUDENT);

    EXPECT_EQ(user.getId(), "SV001");
    EXPECT_EQ(user.getFirstName(), "Nguyen");
    EXPECT_EQ(user.getLastName(), "Linh");
    EXPECT_EQ(user.getFullName(), "Nguyen Linh");
    EXPECT_EQ(user.getRole(), UserRole::STUDENT);
    EXPECT_EQ(user.getStatus(), LoginStatus::ACTIVE);
}

TEST(UserTest, SettersValid) {
    TestUser user("GV001", "An", "Tran", UserRole::TEACHER);
    EXPECT_TRUE(user.setFirstName("Binh"));
    EXPECT_TRUE(user.setLastName("Pham"));
    EXPECT_TRUE(user.setAddress("123 ABC St"));
    EXPECT_TRUE(user.setCitizenId("012345678901"));
    EXPECT_TRUE(user.setEmail("abc@gmail.com"));
    EXPECT_TRUE(user.setPhoneNumber("0912345678"));

    Birthday bd(1, 1, 1990);
    EXPECT_TRUE(user.setBirthday(bd));
    EXPECT_TRUE(user.setBirthday(2, 2, 2000));

    EXPECT_EQ(user.getFirstName(), "Binh");
    EXPECT_EQ(user.getLastName(), "Pham");
    EXPECT_EQ(user.getAddress(), "123 ABC St");
    EXPECT_EQ(user.getCitizenId(), "012345678901");
    EXPECT_EQ(user.getEmail(), "abc@gmail.com");
    EXPECT_EQ(user.getPhoneNumber(), "0912345678");
}

TEST(UserTest, SetRoleAndStatus) {
    TestUser user("AD001", "Duy", "Le", UserRole::ADMIN);
    user.setRole(UserRole::TEACHER);
    user.setStatus(LoginStatus::DISABLED);

    EXPECT_EQ(user.getRole(), UserRole::TEACHER);
    EXPECT_EQ(user.getStatus(), LoginStatus::DISABLED);
}

TEST(UserTest, GetStringId) {
    TestUser user("USR999", "Nam", "Vo", UserRole::STUDENT);
    EXPECT_EQ(user.getStringId(), "USR999");
}

TEST(UserTest, ToStringUserRole) {
    EXPECT_EQ(toStringUserRole(UserRole::STUDENT), "Student");
    EXPECT_EQ(toStringUserRole(UserRole::TEACHER), "Teacher");
    EXPECT_EQ(toStringUserRole(UserRole::ADMIN), "Admin");
}