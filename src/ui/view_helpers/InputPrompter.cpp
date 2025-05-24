#include "InputPrompter.h"
#include "../../utils/StringUtils.h"   // For trim
#include "../../utils/PasswordInput.h" // For getMaskedPassword

std::string InputPrompter::promptForString(const std::string& promptMessage, bool allowEmpty) const {
    std::string input;
    while (true) {
        std::cout << promptMessage << " ";
        std::getline(std::cin, input);
        if (std::cin.eof()) { // Xử lý EOF (Ctrl+D/Ctrl+Z)
             std::cout << "Input stream ended. Exiting or using default." << std::endl;
             // Có thể throw exception hoặc trả về giá trị đặc biệt
             // Hiện tại, nếu không cho phép rỗng thì sẽ lặp lại
             if (allowEmpty) return ""; // Hoặc một giá trị báo hiệu EOF
        }
        if (std::cin.fail()) { // Xử lý lỗi stream khác
            std::cin.clear(); // Xóa cờ lỗi
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Bỏ qua input lỗi
            std::cout << "Invalid input. Please try again." << std::endl;
            continue;
        }
        input = StringUtils::trim(input);
        if (!allowEmpty && input.empty()) {
            std::cout << "Input cannot be empty. Please try again." << std::endl;
        } else {
            break;
        }
    }
    return input;
}

long InputPrompter::promptForLong(const std::string& promptMessage, long minVal, long maxVal) const {
    long value;
    std::string line;
    while (true) {
        std::cout << promptMessage << " (" << minVal << "-" << maxVal << "): ";
        std::getline(std::cin, line);
         if (std::cin.eof()) {
            std::cout << "Input stream ended. Using default or exiting." << std::endl;
            // throw std::runtime_error("Input stream ended during long prompt.");
            return minVal; // Hoặc một giá trị báo hiệu
        }
        try {
            size_t processedChars;
            value = std::stoll(line, &processedChars); // Dùng stoll để xử lý số lớn
            if (processedChars == line.length() && value >= minVal && value <= maxVal) { // Toàn bộ chuỗi được parse và nằm trong khoảng
                break;
            } else {
                std::cout << "Invalid input. Please enter a number between " << minVal << " and " << maxVal << "." << std::endl;
            }
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid format. Please enter a valid number." << std::endl;
        } catch (const std::out_of_range&) {
            std::cout << "Number out of range. Please enter a smaller number." << std::endl;
        }
        if (std::cin.fail()) {
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return value;
}

int InputPrompter::promptForInt(const std::string& promptMessage, int minVal, int maxVal) const {
    long longVal = promptForLong(promptMessage, minVal, maxVal); // Dùng lại promptForLong
    return static_cast<int>(longVal);
}

double InputPrompter::promptForDouble(const std::string& promptMessage, double minVal, double maxVal) const {
    double value;
    std::string line;
    while (true) {
        std::cout << promptMessage;
        if (minVal != std::numeric_limits<double>::lowest() || maxVal != std::numeric_limits<double>::max()) {
            std::cout << " (" << minVal << "-" << maxVal << ")";
        }
        std::cout << ": ";
        std::getline(std::cin, line);
        if (std::cin.eof()) {
            std::cout << "Input stream ended. Using default or exiting." << std::endl;
            return minVal; // Hoặc giá trị báo hiệu
        }
        try {
            size_t processedChars;
            value = std::stod(line, &processedChars);
            if (processedChars == line.length() && value >= minVal && value <= maxVal) {
                break;
            } else {
                 std::cout << "Invalid input. Please enter a number";
                if (minVal != std::numeric_limits<double>::lowest() || maxVal != std::numeric_limits<double>::max()) {
                     std::cout << " between " << minVal << " and " << maxVal;
                }
                std::cout << "." << std::endl;
            }
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid format. Please enter a valid number." << std::endl;
        } catch (const std::out_of_range&) {
            std::cout << "Number out of range. Please enter a number within the valid range." << std::endl;
        }
         if (std::cin.fail()) {
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return value;
}

bool InputPrompter::promptForYesNo(const std::string& promptMessage, char yesChar, char noChar) const {
    std::string input;
    char choice;
    yesChar = static_cast<char>(tolower(yesChar));
    noChar = static_cast<char>(tolower(noChar));

    while (true) {
        std::cout << promptMessage << " (" << yesChar << "/" << noChar << "): ";
        std::getline(std::cin, input);
         if (std::cin.eof()) {
            std::cout << "Input stream ended. Assuming 'no'." << std::endl;
            return false;
        }
        if (input.length() == 1) {
            choice = static_cast<char>(tolower(input[0]));
            if (choice == yesChar) return true;
            if (choice == noChar) return false;
        }
        std::cout << "Invalid input. Please enter '" << yesChar << "' or '" << noChar << "'." << std::endl;
         if (std::cin.fail()) {
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string InputPrompter::promptForPassword(const std::string& promptMessage, bool masked) const {
    if (masked) {
        return PasswordUtils::getMaskedPassword(promptMessage);
    } else {
        return promptForString(promptMessage, false); // Mật khẩu không được rỗng
    }
}

std::string InputPrompter::promptForEmail(const std::string& promptMessage) const {
    // Validation email sẽ được thực hiện ở tầng Service.
    // UI chỉ lấy input.
    return promptForString(promptMessage, false); // Email không được rỗng
}