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

    void setBirthday(int day, int month, int year);
    void setBirthday(std::string date); // Chuyển đổi từ chuỗi sang ngày tháng năm

    std::string getBirthday() const; // Trả về chuỗi định dạng "dd/mm/yyyy"
    int getDay() const;
    int getMonth() const;
    int getYear() const;
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

protected: // Cho phép lớp con truy cập trực tiếp nếu cần, hoặc dùng getter
    void setBirthday(const Birthday& birthday);

public:
    // Constructor ảo thuần túy -> làm cho lớp này trừu tượng
    // Sử dụng std::move để tối ưu hóa hiệu suất khi truyền tham số và tiết kiệm chi phí bộ nhớ
    // Chú ý: std::move không phải lúc nào cũng cần thiết, nhưng trong trường hợp này,
    // nó giúp tránh sao chép không cần thiết
    User(std::string id, std::string firstName, std::string lastName, std::string citizenID);

    // Destructor ảo để đảm bảo dọn dẹp đúng cách trong kế thừa
    virtual ~User() = default;

    // Các hàm getter chung
    const std::string& getId() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    std::string getFullName() const;
    Birthday getBirthday() const;
    const std::string& getAddress() const;
    const std::string& getCitizenID() const;

    // Setter
    void setFirstName(const std::string& firstName);
    void setLastName(const std::string& lastName);
    void setCitizenId(const std::string& citizenId);
    void setAddress(const std::string& address);
    // Setter cho ngày sinh (nếu muốn sửa ngày sinh sau này)
    void setBirthday(int day, int month, int year);
    void setBirthday(const std::string& date);


    // Có thể thêm các phương thức ảo thuần túy khác nếu cần
    // virtual void displayDetails() const = 0;
};

#endif // USER_H