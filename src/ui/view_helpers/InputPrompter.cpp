#include "InputPrompter.h"
#include "../../utils/StringUtils.h"   // For trim
#include "../../utils/PasswordInput.h" // For getMaskedPassword

/**
 * @brief Yêu cầu người dùng nhập một chuỗi
 * 
 * Phương thức này hiển thị thông báo và yêu cầu người dùng nhập một chuỗi.
 * Nó cũng xử lý các trường hợp lỗi và kiểm tra chuỗi rỗng.
 * 
 * @param promptMessage Thông báo hiển thị cho người dùng
 * @param allowEmpty Có cho phép nhập chuỗi rỗng không
 * @return std::string Chuỗi người dùng đã nhập
 */
std::string InputPrompter::promptForString(const std::string& promptMessage, bool allowEmpty) const {
    std::string input;
    while (true) {
        std::cout << promptMessage << " ";
        std::getline(std::cin, input);
        if (std::cin.eof()) { // Xử lý EOF (Ctrl+D/Ctrl+Z)
             std::cout << "Input stream ended. Exiting or using default.\n";
             // Có thể throw exception hoặc trả về giá trị đặc biệt
             // Hiện tại, nếu không cho phép rỗng thì sẽ lặp lại
             if (allowEmpty) return ""; // Hoặc một giá trị báo hiệu EOF
        }
        if (std::cin.fail()) { // Xử lý lỗi stream khác
            std::cin.clear(); // Xóa cờ lỗi
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Bỏ qua input lỗi
            std::cout << "Invalid input. Please try again.\n";
            continue;
        }
        input = StringUtils::trim(input);
        if (!allowEmpty && input.empty()) {
            std::cout << "Input cannot be empty. Please try again.\n";
        } else {
            break;
        }
    }
    return input;
}

/**
 * @brief Yêu cầu người dùng nhập một số nguyên lớn (long)
 * 
 * Phương thức này hiển thị thông báo và yêu cầu người dùng nhập một số nguyên lớn
 * trong khoảng từ minVal đến maxVal. Nó xử lý các trường hợp lỗi đầu vào.
 * 
 * @param promptMessage Thông báo hiển thị cho người dùng
 * @param minVal Giá trị tối thiểu cho phép
 * @param maxVal Giá trị tối đa cho phép
 * @return long Số nguyên lớn người dùng đã nhập
 */
long InputPrompter::promptForLong(const std::string& promptMessage, long minVal, long maxVal) const {
    long value;
    std::string line;
    while (true) {
        std::cout << promptMessage << " (" << minVal << "-" << maxVal << "): ";
        std::getline(std::cin, line);
         if (std::cin.eof()) {
            std::cout << "Input stream ended. Using default or exiting.\n";
            // throw std::runtime_error("Input stream ended during long prompt.");
            return minVal; // Hoặc một giá trị báo hiệu
        }
        try {
            size_t processedChars;
            value = std::stoll(line, &processedChars); // Dùng stoll để xử lý số lớn
            if (processedChars == line.length() && value >= minVal && value <= maxVal) { // Toàn bộ chuỗi được parse và nằm trong khoảng
                break;
            } else {
                std::cout << "Invalid input. Please enter a number between " << minVal << " and " << maxVal << ".\n";
            }
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid format. Please enter a valid number.\n";
        } catch (const std::out_of_range&) {
            std::cout << "Number out of range. Please enter a smaller number.\n";
        }
        if (std::cin.fail()) {
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return value;
}

/**
 * @brief Yêu cầu người dùng nhập một số nguyên (int)
 * 
 * Phương thức này sử dụng promptForLong để nhận đầu vào và chuyển đổi kết quả sang int.
 * 
 * @param promptMessage Thông báo hiển thị cho người dùng
 * @param minVal Giá trị tối thiểu cho phép
 * @param maxVal Giá trị tối đa cho phép
 * @return int Số nguyên người dùng đã nhập
 */
int InputPrompter::promptForInt(const std::string& promptMessage, int minVal, int maxVal) const {
    long longVal = promptForLong(promptMessage, minVal, maxVal); // Dùng lại promptForLong
    return static_cast<int>(longVal);
}

/**
 * @brief Yêu cầu người dùng nhập một số thực (double)
 * 
 * Phương thức này hiển thị thông báo và yêu cầu người dùng nhập một số thực
 * trong khoảng từ minVal đến maxVal. Nó xử lý các trường hợp lỗi đầu vào.
 * 
 * @param promptMessage Thông báo hiển thị cho người dùng
 * @param minVal Giá trị tối thiểu cho phép
 * @param maxVal Giá trị tối đa cho phép
 * @return double Số thực người dùng đã nhập
 */
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
            std::cout << "Input stream ended. Using default or exiting.\n";
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
                std::cout << ".\n";
            }
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid format. Please enter a valid number.\n";
        } catch (const std::out_of_range&) {
            std::cout << "Number out of range. Please enter a number within the valid range.\n";
        }
         if (std::cin.fail()) {
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return value;
}

/**
 * @brief Yêu cầu người dùng nhập lựa chọn Yes/No
 * 
 * Phương thức này hiển thị thông báo và yêu cầu người dùng nhập một lựa chọn Yes/No.
 * Người dùng chỉ cần nhập ký tự đại diện cho lựa chọn.
 * 
 * @param promptMessage Thông báo hiển thị cho người dùng
 * @param yesChar Ký tự đại diện cho lựa chọn "Yes" (mặc định là 'y')
 * @param noChar Ký tự đại diện cho lựa chọn "No" (mặc định là 'n')
 * @return true Nếu người dùng chọn "Yes"
 * @return false Nếu người dùng chọn "No"
 */
bool InputPrompter::promptForYesNo(const std::string& promptMessage, char yesChar, char noChar) const {
    std::string input;
    char choice;
    yesChar = static_cast<char>(tolower(yesChar));
    noChar = static_cast<char>(tolower(noChar));

    while (true) {
        std::cout << promptMessage << " (" << yesChar << "/" << noChar << "): ";
        std::getline(std::cin, input);
         if (std::cin.eof()) {
            std::cout << "Input stream ended. Assuming 'no'.\n";
            return false;
        }
        if (input.length() == 1) {
            choice = static_cast<char>(tolower(input[0]));
            if (choice == yesChar) return true;
            if (choice == noChar) return false;
        }
        std::cout << "Invalid input. Please enter '" << yesChar << "' or '" << noChar << "'.\n";
         if (std::cin.fail()) {
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

/**
 * @brief Yêu cầu người dùng nhập mật khẩu
 * 
 * Phương thức này hiển thị thông báo và yêu cầu người dùng nhập mật khẩu.
 * Nếu masked=true, các ký tự sẽ được che dấu bằng ký tự '*'.
*/
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