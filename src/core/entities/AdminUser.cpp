// src/core/entities/AdminUser.cpp
#include "AdminUser.h"
#include <sstream>
#include "../../utils/StringUtils.h" // Cho trim
#include "../../common/UserRole.h" // Để dùng toStringUserRoleUI
#include "../../common/LoginStatus.h" // Để dùng LoginStatusUtils
#include "User.h" // Để dùng LoginStatusUtils

AdminUser::AdminUser(const std::string& id,
                     const std::string& firstName,
                     const std::string& lastName,
                     LoginStatus status)
    : User(id, firstName, lastName, UserRole::ADMIN, status) { // Gọi constructor của User với UserRole::ADMIN
    // Khởi tạo các thuộc tính riêng của AdminUser nếu có
}

std::string AdminUser::display() const {
    // Bạn có thể gọi User::display() và thêm thông tin, hoặc viết lại hoàn toàn
    std::ostringstream oss;
    oss << "--- Administrator Information ---\n"
        << "ID          : " << getId() << "\n"
        << "Full Name   : " << getFullName() << "\n"
        << "Role        : " << toStringUserRole(getRole()) << "\n" // Nên có hàm toStringUserRoleUI
        << "Status      : " << LoginStatusUtils::toString(getStatus()) << "\n";
    if (!getEmail().empty()) {
        oss << "Email       : " << getEmail() << "\n";
    }
    // Thêm các thông tin khác của Admin nếu có
    oss << "-------------------------------";
    return oss.str();
}

ValidationResult AdminUser::validateBasic() const {
    ValidationResult vr;
    // Gọi validate của User cha nếu User::validateBasic() không còn là thuần ảo và là public/protected
    // vr = User::validateBasic(); 

    // Hoặc validate các trường cơ bản lại ở đây
    if (StringUtils::trim(getId()).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Admin ID cannot be empty.");
    if (StringUtils::trim(getFirstName()).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Admin first name cannot be empty.");
    // Admin có thể không cần lastName, hoặc có thể để trống
    // if (StringUtils::trim(getLastName()).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Admin last name cannot be empty.");


    if (getRole() != UserRole::ADMIN) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid role for AdminUser object.");
    }
    // Admin thường luôn ACTIVE, nhưng có thể validate status nếu cần
    // if (getStatus() != LoginStatus::ACTIVE) {
    //     vr.addError(ErrorCode::VALIDATION_ERROR, "Admin status should typically be ACTIVE.");
    // }
    return vr;
}