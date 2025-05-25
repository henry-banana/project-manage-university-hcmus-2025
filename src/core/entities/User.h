/**
 * @file User.h
 * @brief Định nghĩa lớp cơ sở User cho các loại người dùng trong hệ thống
 * 
 * File này định nghĩa lớp User, làm cơ sở cho các loại người dùng như Student, Teacher,
 * và AdminUser trong hệ thống quản lý trường đại học. Lớp này cung cấp các thuộc tính
 * và phương thức chung cho tất cả các loại người dùng.
 */
#ifndef USER_H
#define USER_H

#include "IEntity.h"
#include "Birthday.h"
#include "../../common/UserRole.h"
#include "../../common/LoginStatus.h" // (➕) Trạng thái tài khoản
#include <string>

// Forward declaration nếu cần cho các mối quan hệ
// class Faculty;

/**
 * @class User
 * @brief Lớp cơ sở trừu tượng cho tất cả các loại người dùng trong hệ thống
 * 
 * Lớp này định nghĩa các thuộc tính và phương thức chung cho tất cả các loại
 * người dùng trong hệ thống, bao gồm sinh viên, giảng viên và quản trị viên.
 * Nó triển khai giao diện IEntity và cung cấp chức năng cơ bản cho việc quản lý
 * thông tin người dùng.
 */
class User : public IEntity {
protected:
    std::string _id; ///< ID duy nhất (Student ID, Teacher ID, Admin ID)
    std::string _firstName; ///< Tên của người dùng
    std::string _lastName; ///< Họ của người dùng
    Birthday _birthday; ///< Ngày sinh của người dùng
    std::string _address; ///< Địa chỉ của người dùng
    std::string _citizenId; ///< Số CCCD của người dùng
    std::string _email; ///< Địa chỉ email của người dùng
    std::string _phoneNumber; ///< Số điện thoại của người dùng
    UserRole _role; ///< Vai trò của người dùng trong hệ thống
    LoginStatus _status; ///< Trạng thái tài khoản: ACTIVE, PENDING_APPROVAL, DISABLED

    // Thông tin đăng nhập, có thể không lưu trực tiếp ở User Entity mà ở LoginDao.
    // Tuy nhiên, nếu lưu ở User, cần đảm bảo an toàn.
    // Coi như LoginDao sẽ quản lý phần này.

public:
    /**
     * @brief Constructor tạo đối tượng User
     * 
     * Constructor protected để User là lớp trừu tượng
     * 
     * @param id ID duy nhất của người dùng
     * @param firstName Tên của người dùng
     * @param lastName Họ của người dùng
     * @param role Vai trò của người dùng
     * @param status Trạng thái tài khoản (mặc định là ACTIVE)
     */
    User(std::string id,
         std::string firstName,
         std::string lastName,
         UserRole role,
         LoginStatus status = LoginStatus::ACTIVE);

    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~User() override = default;

    /**
     * @brief Lấy ID của người dùng dưới dạng chuỗi
     * @return ID của người dùng
     */
    std::string getStringId() const override;

    /**
     * @brief Lấy ID của người dùng
     * @return ID của người dùng
     */
    const std::string& getId() const;

    /**
     * @brief Lấy tên của người dùng
     * @return Tên của người dùng
     */
    const std::string& getFirstName() const;

    /**
     * @brief Lấy họ của người dùng
     * @return Họ của người dùng
     */
    const std::string& getLastName() const;

    /**
     * @brief Lấy họ và tên đầy đủ của người dùng
     * @return Họ và tên đầy đủ của người dùng
     */
    std::string getFullName() const;

    /**
     * @brief Lấy ngày sinh của người dùng
     * @return Ngày sinh của người dùng
     */
    const Birthday& getBirthday() const;

    /**
     * @brief Lấy địa chỉ của người dùng
     * @return Địa chỉ của người dùng
     */
    const std::string& getAddress() const;

    /**
     * @brief Lấy số CCCD của người dùng
     * @return Số CCCD của người dùng
     */
    const std::string& getCitizenId() const;

    /**
     * @brief Lấy địa chỉ email của người dùng
     * @return Địa chỉ email của người dùng
     */
    const std::string& getEmail() const;

    /**
     * @brief Lấy số điện thoại của người dùng
     * @return Số điện thoại của người dùng
     */
    const std::string& getPhoneNumber() const;

    /**
     * @brief Lấy vai trò của người dùng
     * @return Vai trò của người dùng
     */
    UserRole getRole() const;

    /**
     * @brief Lấy trạng thái tài khoản của người dùng
     * @return Trạng thái tài khoản của người dùng
     */
    LoginStatus getStatus() const;

    /**
     * @brief Đặt tên cho người dùng
     * @param firstName Tên mới
     * @return true nếu thành công, false nếu thất bại
     */
    virtual bool setFirstName(const std::string& firstName);

    /**
     * @brief Đặt họ cho người dùng
     * @param lastName Họ mới
     * @return true nếu thành công, false nếu thất bại
     */
    virtual bool setLastName(const std::string& lastName);

    /**
     * @brief Đặt ngày sinh cho người dùng
     * @param birthday Đối tượng Birthday mới
     * @return true nếu thành công, false nếu thất bại
     */
    virtual bool setBirthday(const Birthday& birthday);

    /**
     * @brief Đặt ngày sinh cho người dùng
     * @param day Ngày
     * @param month Tháng
     * @param year Năm
     * @return true nếu thành công, false nếu thất bại
     */
    virtual bool setBirthday(int day, int month, int year);

    /**
     * @brief Đặt địa chỉ cho người dùng
     * @param address Địa chỉ mới
     * @return true nếu thành công, false nếu thất bại
     */
    virtual bool setAddress(const std::string& address);

    /**
     * @brief Đặt số CCCD cho người dùng
     * @param citizenId Số CCCD mới
     * @return true nếu thành công, false nếu thất bại
     */
    virtual bool setCitizenId(const std::string& citizenId);

    /**
     * @brief Đặt email cho người dùng
     * @param email Email mới (cần validation định dạng)
     * @return true nếu thành công, false nếu thất bại
     */
    virtual bool setEmail(const std::string& email);

    /**
     * @brief Đặt số điện thoại cho người dùng
     * @param phoneNumber Số điện thoại mới (cần validation định dạng)
     * @return true nếu thành công, false nếu thất bại
     */
    virtual bool setPhoneNumber(const std::string& phoneNumber);

    /**
     * @brief Đặt vai trò cho người dùng
     * @param role Vai trò mới
     */
    virtual void setRole(UserRole role);

    /**
     * @brief Đặt trạng thái tài khoản cho người dùng
     * @param status Trạng thái mới
     */
    virtual void setStatus(LoginStatus status);
};

/**
 * @brief Chuyển đổi vai trò người dùng thành chuỗi
 * @param role Vai trò người dùng cần chuyển đổi
 * @return Chuỗi mô tả vai trò
 */
std::string toStringUserRole(UserRole role);

#endif // USER_H