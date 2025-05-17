#ifndef USER_H
#define USER_H

#include "IEntity.h"
#include "Birthday.h"
#include "../../common/UserRole.h"
#include "../../common/LoginStatus.h" // (➕) Trạng thái tài khoản
#include <string>

// Forward declaration nếu cần cho các mối quan hệ
// class Faculty;

class User : public IEntity {
protected:
    std::string _id; // ID duy nhất (Student ID, Teacher ID, Admin ID)
    std::string _firstName;
    std::string _lastName;
    Birthday _birthday;
    std::string _address;
    std::string _citizenId; // CCCD
    std::string _email;
    std::string _phoneNumber;
    UserRole _role;
    LoginStatus _status; // (➕) Trạng thái tài khoản: ACTIVE, PENDING_APPROVAL, DISABLED

    // Thông tin đăng nhập, có thể không lưu trực tiếp ở User Entity mà ở LoginDao.
    // Tuy nhiên, nếu lưu ở User, cần đảm bảo an toàn.
    // Coi như LoginDao sẽ quản lý phần này.

public:
    // Constructor protected để User là lớp trừu tượng
    User(std::string id,
         std::string firstName,
         std::string lastName,
         UserRole role,
         LoginStatus status = LoginStatus::ACTIVE); // Mặc định là active

    virtual ~User() override = default;

    // Implement IEntity
    std::string getStringId() const override;
    // toString() sẽ được override bởi lớp con

    // Getters
    const std::string& getId() const; // Getter cho _id (cùng kiểu với getStringId)
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    std::string getFullName() const;
    const Birthday& getBirthday() const;
    const std::string& getAddress() const;
    const std::string& getCitizenId() const;
    const std::string& getEmail() const;
    const std::string& getPhoneNumber() const;
    UserRole getRole() const;
    LoginStatus getStatus() const;

    // Setters (trả về bool để chỉ báo thành công validation)
    // Hoặc trả về ValidationResult nếu muốn chi tiết lỗi
    virtual bool setFirstName(const std::string& firstName);
    virtual bool setLastName(const std::string& lastName);
    virtual bool setBirthday(const Birthday& birthday);
    virtual bool setBirthday(int day, int month, int year);
    virtual bool setAddress(const std::string& address);
    virtual bool setCitizenId(const std::string& citizenId);
    virtual bool setEmail(const std::string& email);         // Cần validation định dạng
    virtual bool setPhoneNumber(const std::string& phoneNumber); // Cần validation định dạng
    virtual void setRole(UserRole role); // Role thường được set bởi hệ thống
    virtual void setStatus(LoginStatus status);
};

#endif // USER_H