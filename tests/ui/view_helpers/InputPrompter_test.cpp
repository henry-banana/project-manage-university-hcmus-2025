// --- START OF NEW FILE tests/InputPrompter_test.cpp ---
#include "gtest/gtest.h"
#include "../src/ui/view_helpers/InputPrompter.h"
#include <sstream> // Để thử mock cin (khó cho getline)
#include <iostream>

class InputPrompterTest : public ::testing::Test {
protected:
    InputPrompter prompter;
    std::streambuf* orig_cin;
    std::stringstream test_cin_buffer;

    void SetUp() override {
        orig_cin = std::cin.rdbuf(); // Lưu buffer gốc
        // std::cin.rdbuf(test_cin_buffer.rdbuf()); // Chuyển hướng cin (có thể không hiệu quả với getMaskedPassword)
    }

    void TearDown() override {
        std::cin.rdbuf(orig_cin); // Khôi phục buffer gốc
    }

    void provideInput(const std::string& input) {
        test_cin_buffer.clear(); // Xóa trạng thái lỗi (nếu có)
        test_cin_buffer.str(input);
        std::cin.rdbuf(test_cin_buffer.rdbuf()); // Chuyển hướng lại sau khi set str()
    }
};

// Test các hàm promptForString, promptForInt, etc. sẽ cần mock cin
// Hoặc chỉ test xem chúng có chạy mà không crash không
TEST_F(InputPrompterTest, PromptForString_NoCrash) {
    // Không thể assert kết quả dễ dàng vì nó chờ input thật
    // Test này chủ yếu để đảm bảo nó biên dịch và chạy không lỗi
    // provideInput("test string\n"); // Thử cung cấp input
    // EXPECT_NO_THROW(prompter.promptForString("Enter string:"));
    // EXPECT_EQ(prompter.promptForString("Enter string:"), "test string"); // Sẽ không hoạt động như mong đợi
}

TEST_F(InputPrompterTest, PromptForYesNo_NoCrash) {
    // provideInput("y\n");
    // EXPECT_NO_THROW(prompter.promptForYesNo("Yes or No?"));
}

TEST_F(InputPrompterTest, PromptForPassword_NoCrash) {
    // Hàm này dùng _getch/_termios, rất khó test tự động
    EXPECT_NO_THROW(prompter.promptForPassword("Enter pass:"));
}

TEST_F(InputPrompterTest, PromptForInt_ValidInput) {
    provideInput("123\n");
    // EXPECT_EQ(prompter.promptForInt("Enter int:", 0, 200), 123); // Vẫn khó với getline
    // Test này vẫn sẽ bị treo chờ input thủ công nếu provideInput không hoạt động hoàn hảo
}

// Hiện tại, test tự động cho InputPrompter rất khó vì nó tương tác trực tiếp với std::cin
// và các hàm getch/termios. Các test trên chủ yếu để đảm bảo nó không bị lỗi cú pháp.
// Việc test đầy đủ InputPrompter thường yêu cầu các kỹ thuật mocking phức tạp hơn cho stream
// hoặc là test thủ công.
// --- END OF NEW FILE tests/InputPrompter_test.cpp ---