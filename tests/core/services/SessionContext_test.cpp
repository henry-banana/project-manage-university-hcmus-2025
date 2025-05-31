#include "gtest/gtest.h"
#include "../../../src/core/services/SessionContext.h"
#include "../../../src/core/entities/User.h" // Để tạo User
#include "../../../src/core/entities/Student.h" // Để tạo Student (ví dụ)

// Helper class kế thừa User để có thể tạo instance
class TestUserForSession : public User {
public:
    TestUserForSession(const std::string& id, UserRole role)
        : User(id, "Test", "User", role) {}

    std::string display() const override { return "TestUserDisplay"; }
    ValidationResult validateBasic() const override { return ValidationResult{}; }
};


TEST(SessionContextTest, InitialState) {
    SessionContext ctx;
    ASSERT_FALSE(ctx.isAuthenticated());
    ASSERT_FALSE(ctx.getCurrentUserId().has_value());
    ASSERT_FALSE(ctx.getCurrentUserRole().has_value());
    ASSERT_FALSE(ctx.getCurrentUser().has_value());
}

TEST(SessionContextTest, SetAndGetCurrentUser) {
    SessionContext ctx;
    auto user = std::make_shared<TestUserForSession>("user123", UserRole::STUDENT);
    
    ctx.setCurrentUser(user);
    
    ASSERT_TRUE(ctx.isAuthenticated());
    ASSERT_TRUE(ctx.getCurrentUserId().has_value());
    EXPECT_EQ(ctx.getCurrentUserId().value(), "user123");
    
    ASSERT_TRUE(ctx.getCurrentUserRole().has_value());
    EXPECT_EQ(ctx.getCurrentUserRole().value(), UserRole::STUDENT);
    
    ASSERT_TRUE(ctx.getCurrentUser().has_value());
    EXPECT_EQ(ctx.getCurrentUser().value()->getId(), "user123");
    EXPECT_EQ(ctx.getCurrentUser().value(), user);
}

TEST(SessionContextTest, ClearCurrentUser) {
    SessionContext ctx;
    auto user = std::make_shared<TestUserForSession>("user456", UserRole::ADMIN);
    ctx.setCurrentUser(user);
    
    ASSERT_TRUE(ctx.isAuthenticated());
    
    ctx.clearCurrentUser();
    
    ASSERT_FALSE(ctx.isAuthenticated());
    ASSERT_FALSE(ctx.getCurrentUserId().has_value());
    ASSERT_FALSE(ctx.getCurrentUserRole().has_value());
    ASSERT_FALSE(ctx.getCurrentUser().has_value());
}

TEST(SessionContextTest, SetNullUser) {
    SessionContext ctx;
    // Giả sử trước đó đã có user
    auto userOld = std::make_shared<TestUserForSession>("userOld", UserRole::TEACHER);
    ctx.setCurrentUser(userOld);
    ASSERT_TRUE(ctx.isAuthenticated());

    // Set currentUser thành nullptr (thông qua shared_ptr rỗng)
    std::shared_ptr<User> nullUser = nullptr;
    ctx.setCurrentUser(nullUser);

    // Trạng thái nên giống như clearCurrentUser
    ASSERT_FALSE(ctx.isAuthenticated());
    ASSERT_FALSE(ctx.getCurrentUserId().has_value());
    ASSERT_FALSE(ctx.getCurrentUserRole().has_value());
    ASSERT_FALSE(ctx.getCurrentUser().has_value());
}

TEST(SessionContextTest, GettersWhenNoUserIsSet) {
    SessionContext ctx;
    EXPECT_FALSE(ctx.isAuthenticated());
    EXPECT_EQ(ctx.getCurrentUserId(), std::nullopt);
    EXPECT_EQ(ctx.getCurrentUserRole(), std::nullopt);
    EXPECT_EQ(ctx.getCurrentUser(), std::nullopt);
}

TEST(SessionContextTest, SetUserWithDifferentRoles) {
    SessionContext ctx;
    
    auto studentUser = std::make_shared<TestUserForSession>("student01", UserRole::STUDENT);
    ctx.setCurrentUser(studentUser);
    ASSERT_EQ(ctx.getCurrentUserRole().value(), UserRole::STUDENT);

    auto adminUser = std::make_shared<TestUserForSession>("admin01", UserRole::ADMIN);
    ctx.setCurrentUser(adminUser);
    ASSERT_EQ(ctx.getCurrentUserRole().value(), UserRole::ADMIN);
    ASSERT_EQ(ctx.getCurrentUserId().value(), "admin01"); // Đảm bảo user đã được thay đổi
}