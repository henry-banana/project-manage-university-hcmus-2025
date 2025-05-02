#ifndef USER_H
#define USER_H

#include <string>

// Lớp cơ sở trừu tượng cho các loại người dùng
class User {
private:
    std::string _id;
    std::string _firstName;
    std::string _lastName;

protected: // Cho phép lớp con truy cập trực tiếp nếu cần, hoặc dùng getter

public:
    // Constructor ảo thuần túy -> làm cho lớp này trừu tượng
    // Sử dụng std::move để tối ưu hóa hiệu suất khi truyền tham số và tiết kiệm chi phí bộ nhớ
    // Chú ý: std::move không phải lúc nào cũng cần thiết, nhưng trong trường hợp này,
    // nó giúp tránh sao chép không cần thiết
    User(std::string id, std::string firstName, std::string lastName);

    // Destructor ảo để đảm bảo dọn dẹp đúng cách trong kế thừa
    virtual ~User() = default;

    // Các hàm getter chung
    const std::string& getId() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    std::string getFullName() const;

    // Có thể thêm các phương thức ảo thuần túy khác nếu cần
    // virtual void displayDetails() const = 0;
};

#endif // USER_H