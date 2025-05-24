#include "StudentSqlParser.h"
#include "../../../common/UserRole.h" // For UserRole enum to int conversion
#include "../../../common/LoginStatus.h" // For LoginStatus enum to int conversion

std::expected<Student, Error> StudentSqlParser::parse(const DbQueryResultRow& row) const {
    try {
        std::string id = SqlParserUtils::getOptional<std::string>(row, "userId"); // Hoặc "id" từ bảng Users
        if (id.empty()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Student ID (userId) not found or empty in SQL row."});
        }
        std::string firstName = SqlParserUtils::getOptional<std::string>(row, "firstName");
        std::string lastName = SqlParserUtils::getOptional<std::string>(row, "lastName");
        std::string facultyId = SqlParserUtils::getOptional<std::string>(row, "facultyId");
        
        // UserRole và LoginStatus được lưu dưới dạng int trong DB
        UserRole role = static_cast<UserRole>(SqlParserUtils::getOptional<long long>(row, "role", static_cast<long long>(UserRole::UNKNOWN))); // SQLite thường trả int64
        LoginStatus status = static_cast<LoginStatus>(SqlParserUtils::getOptional<long long>(row, "status", static_cast<long long>(LoginStatus::DISABLED)));

        Student student(id, firstName, lastName, facultyId, status);
        student.setRole(role); // User constructor có thể đã set, nhưng đảm bảo

        // Các trường tùy chọn của User
        student.setAddress(SqlParserUtils::getOptional<std::string>(row, "address"));
        student.setCitizenId(SqlParserUtils::getOptional<std::string>(row, "citizenId"));
        student.setEmail(SqlParserUtils::getOptional<std::string>(row, "email"));
        student.setPhoneNumber(SqlParserUtils::getOptional<std::string>(row, "phoneNumber"));

        // Birthday
        int day = SqlParserUtils::getOptional<long long>(row, "birthDay", 0);
        int month = SqlParserUtils::getOptional<long long>(row, "birthMonth", 0);
        int year = SqlParserUtils::getOptional<long long>(row, "birthYear", 0);
        if (day != 0 && month != 0 && year != 0) {
            student.setBirthday(day, month, year);
        }

        return student;

    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse Student from SQL row: " + std::string(e.what())});
    } catch (const std::exception& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Generic error parsing Student from SQL row: " + std::string(e.what())});
    }
}

std::expected<DbQueryResultRow, Error> StudentSqlParser::serialize(const Student& student) const {
    DbQueryResultRow row;
    // User fields
    row["id"] = student.getId();
    row["firstName"] = student.getFirstName();
    row["lastName"] = student.getLastName();
    if (student.getBirthday().isSet()) {
        row["birthDay"] = student.getBirthday().getDay();
        row["birthMonth"] = student.getBirthday().getMonth();
        row["birthYear"] = student.getBirthday().getYear();
    } else {
        row["birthDay"] = std::any{}; // NULL
        row["birthMonth"] = std::any{};
        row["birthYear"] = std::any{};
    }
    row["address"] = student.getAddress().empty() ? std::any{} : std::any{student.getAddress()};
    row["citizenId"] = student.getCitizenId().empty() ? std::any{} : std::any{student.getCitizenId()};
    row["email"] = student.getEmail().empty() ? std::any{} : std::any{student.getEmail()};
    row["phoneNumber"] = student.getPhoneNumber().empty() ? std::any{} : std::any{student.getPhoneNumber()};
    row["role"] = static_cast<int>(student.getRole());
    row["status"] = static_cast<int>(student.getStatus());

    // Student specific field
    row["facultyId"] = student.getFacultyId().empty() ? std::any{} : std::any{student.getFacultyId()};
    // 'userId' for Students table is usually the same as 'id' from Users table
    row["userId"] = student.getId(); 

    return row;
}

std::expected<std::vector<std::any>, Error> StudentSqlParser::toQueryInsertParams(const Student& student) const {
    std::vector<std::any> params;
    // Users table params
    params.push_back(student.getId());
    params.push_back(student.getFirstName());
    params.push_back(student.getLastName());
    if (student.getBirthday().isSet()) {
        params.push_back(student.getBirthday().getDay());
        params.push_back(student.getBirthday().getMonth());
        params.push_back(student.getBirthday().getYear());
    } else {
        params.push_back(std::any{}); // birthDay
        params.push_back(std::any{}); // birthMonth
        params.push_back(std::any{}); // birthYear
    }
    params.push_back(student.getAddress().empty() ? std::any{} : std::any{student.getAddress()});
    params.push_back(student.getCitizenId().empty() ? std::any{} : std::any{student.getCitizenId()});
    params.push_back(student.getEmail().empty() ? std::any{} : std::any{student.getEmail()});
    params.push_back(student.getPhoneNumber().empty() ? std::any{} : std::any{student.getPhoneNumber()});
    params.push_back(static_cast<int>(student.getRole()));
    params.push_back(static_cast<int>(student.getStatus()));
    
    // Students table params (ngoài userId đã có từ Users.id)
    // Giả sử Users.id và Students.userId là một khi INSERT.
    // params.push_back(student.getId()); // userId (for Students table, same as User.id)
    params.push_back(student.getFacultyId().empty() ? std::any{} : std::any{student.getFacultyId()});
    
    return params;
}

std::expected<std::vector<std::any>, Error> StudentSqlParser::toQueryUpdateParams(const Student& student) const {
    std::vector<std::any> params;
    // Users table params (theo thứ tự trong câu UPDATE)
    params.push_back(student.getFirstName());
    params.push_back(student.getLastName());
    if (student.getBirthday().isSet()) {
        params.push_back(student.getBirthday().getDay());
        params.push_back(student.getBirthday().getMonth());
        params.push_back(student.getBirthday().getYear());
    } else {
        params.push_back(std::any{}); // birthDay
        params.push_back(std::any{}); // birthMonth
        params.push_back(std::any{}); // birthYear
    }
    params.push_back(student.getAddress().empty() ? std::any{} : std::any{student.getAddress()});
    params.push_back(student.getCitizenId().empty() ? std::any{} : std::any{student.getCitizenId()});
    params.push_back(student.getEmail().empty() ? std::any{} : std::any{student.getEmail()});
    params.push_back(student.getPhoneNumber().empty() ? std::any{} : std::any{student.getPhoneNumber()});
    params.push_back(static_cast<int>(student.getRole()));   // Thường thì role không update trực tiếp ở đây
    params.push_back(static_cast<int>(student.getStatus())); // Status cũng nên qua hàm chuyên biệt
    
    // Students table params
    params.push_back(student.getFacultyId().empty() ? std::any{} : std::any{student.getFacultyId()});
    
    // ID cho WHERE clause (cuối cùng)
    params.push_back(student.getId()); 
    
    return params;
}