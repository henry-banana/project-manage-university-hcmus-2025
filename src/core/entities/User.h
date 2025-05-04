#ifndef USER_H
#define USER_H

#include <string>

class Birthday {
private:
    int _day;
    int _month;
    int _year;
public:
    Birthday();
    Birthday(int day, int month, int year);

    bool setBirthday(int day, int month, int year);
    bool setBirthday(std::string date); // Chuyển đổi từ chuỗi sang ngày tháng năm

    std::string birthday() const; // Trả về chuỗi định dạng "dd/mm/yyyy"
    int getDay() const;
    int getMonth() const;
    int getYear() const;
};

enum class UserRole {
    STUDENT,
    TEACHER,
    ADMIN
};

// Lớp cơ sở trừu tượng cho các loại người dùng
class User {
private:
    std::string _id;
    std::string _first_name;
    std::string _last_name;
    Birthday _birth_day;
    std::string _address;
    std::string _citizen_id; // Căn cước công dân
    std::string _password_hash; // Mã băm của mật khẩu
    std::string _salt; // Muối cho mật khẩu
    UserRole _role; // Vai trò của người dùng (học sinh, giáo viên, quản trị viên)

protected: // Cho phép lớp con truy cập trực tiếp nếu cần, hoặc dùng getter
    void setBirthday(const Birthday& birthday);

public:
    // Constructor ảo thuần túy -> làm cho lớp này trừu tượng
    // Sử dụng std::move để tối ưu hóa hiệu suất khi truyền tham số và tiết kiệm chi phí bộ nhớ
    // Chú ý: std::move không phải lúc nào cũng cần thiết, nhưng trong trường hợp này,
    // nó giúp tránh sao chép không cần thiết
    User() = default;
    User(std::string id, std::string firstName, std::string lastName, std::string citizenID);
    User(const User& other);
    User& operator=(const User& other); // Toán tử gán sao chép
    
    // Destructor ảo để đảm bảo dọn dẹp đúng cách trong kế thừa
    virtual ~User() = default;

    // Các hàm getter chung
    const std::string& id() const;
    const std::string& firstName() const;
    const std::string& lastName() const;
    std::string fullName() const;
    Birthday birthday() const;
    const std::string& address() const;
    const std::string& citizenID() const;
    const std::string& passwordHash() const;
    const std::string& salt() const;
    UserRole role() const;

    // Setter
    void setFirstName(const std::string& firstName);
    void setLastName(const std::string& lastName);
    void setCitizenId(const std::string& citizenId);
    void setAddress(const std::string& address);
    // Setter cho ngày sinh (nếu muốn sửa ngày sinh sau này)
    void setBirthday(int day, int month, int year);
    void setBirthday(const std::string& date);
    void setPasswordHash(const std::string& passwordHash);
    void setSalt(const std::string& salt); // Setter cho salt

    virtual void display() const = 0;
};

#endif // USER_H