#include "TeacherSqlParser.h"

std::expected<Teacher, Error> TeacherSqlParser::parse(const DbQueryResultRow& row) const {
    try {
        std::string id = SqlParserUtils::getOptional<std::string>(row, "userId");
        if (id.empty()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Teacher ID (userId) not found or empty in SQL row."});
        }
        std::string firstName = SqlParserUtils::getOptional<std::string>(row, "firstName");
        std::string lastName = SqlParserUtils::getOptional<std::string>(row, "lastName");
        std::string facultyId = SqlParserUtils::getOptional<std::string>(row, "facultyId");
        
        UserRole role = static_cast<UserRole>(SqlParserUtils::getOptional<long long>(row, "role", static_cast<long long>(UserRole::UNKNOWN)));
        LoginStatus status = static_cast<LoginStatus>(SqlParserUtils::getOptional<long long>(row, "status", static_cast<long long>(LoginStatus::DISABLED)));

        Teacher teacher(id, firstName, lastName, facultyId, status);
        teacher.setRole(role);

        // Các trường tùy chọn của User
        teacher.setAddress(SqlParserUtils::getOptional<std::string>(row, "address"));
        teacher.setCitizenId(SqlParserUtils::getOptional<std::string>(row, "citizenId"));
        teacher.setEmail(SqlParserUtils::getOptional<std::string>(row, "email"));
        teacher.setPhoneNumber(SqlParserUtils::getOptional<std::string>(row, "phoneNumber"));

        int day = SqlParserUtils::getOptional<long long>(row, "birthDay", 0);
        int month = SqlParserUtils::getOptional<long long>(row, "birthMonth", 0);
        int year = SqlParserUtils::getOptional<long long>(row, "birthYear", 0);
        if (day != 0 && month != 0 && year != 0) {
            teacher.setBirthday(day, month, year);
        }

        // Các trường của Teacher
        teacher.setQualification(SqlParserUtils::getOptional<std::string>(row, "qualification"));
        teacher.setSpecializationSubjects(SqlParserUtils::getOptional<std::string>(row, "specializationSubjects"));
        teacher.setDesignation(SqlParserUtils::getOptional<std::string>(row, "designation"));
        teacher.setExperienceYears(SqlParserUtils::getOptional<long long>(row, "experienceYears", 0));

        return teacher;

    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse Teacher from SQL row: " + std::string(e.what())});
    } catch (const std::exception& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Generic error parsing Teacher from SQL row: " + std::string(e.what())});
    }
}

std::expected<DbQueryResultRow, Error> TeacherSqlParser::serialize(const Teacher& teacher) const {
    DbQueryResultRow row;
    // User fields
    row["id"] = teacher.getId(); // id trong Users table
    row["userId"] = teacher.getId(); // userId trong Teachers table
    row["firstName"] = teacher.getFirstName();
    row["lastName"] = teacher.getLastName();
    if (teacher.getBirthday().isSet()) {
        row["birthDay"] = teacher.getBirthday().getDay();
        row["birthMonth"] = teacher.getBirthday().getMonth();
        row["birthYear"] = teacher.getBirthday().getYear();
    } else {
        row["birthDay"] = std::any{}; 
        row["birthMonth"] = std::any{};
        row["birthYear"] = std::any{};
    }
    row["address"] = teacher.getAddress().empty() ? std::any{} : std::any{teacher.getAddress()};
    row["citizenId"] = teacher.getCitizenId().empty() ? std::any{} : std::any{teacher.getCitizenId()};
    row["email"] = teacher.getEmail().empty() ? std::any{} : std::any{teacher.getEmail()};
    row["phoneNumber"] = teacher.getPhoneNumber().empty() ? std::any{} : std::any{teacher.getPhoneNumber()};
    row["role"] = static_cast<int>(teacher.getRole());
    row["status"] = static_cast<int>(teacher.getStatus());

    // Teacher specific fields
    row["facultyId"] = teacher.getFacultyId().empty() ? std::any{} : std::any{teacher.getFacultyId()};
    row["qualification"] = teacher.getQualification().empty() ? std::any{} : std::any{teacher.getQualification()};
    row["specializationSubjects"] = teacher.getSpecializationSubjects().empty() ? std::any{} : std::any{teacher.getSpecializationSubjects()};
    row["designation"] = teacher.getDesignation().empty() ? std::any{} : std::any{teacher.getDesignation()};
    row["experienceYears"] = teacher.getExperienceYears();
    
    return row;
}

std::expected<std::vector<std::any>, Error> TeacherSqlParser::toQueryInsertParams(const Teacher& teacher) const {
    std::vector<std::any> params;
    // Users table params
    params.push_back(teacher.getId());
    params.push_back(teacher.getFirstName());
    params.push_back(teacher.getLastName());
    if (teacher.getBirthday().isSet()) {
        params.push_back(teacher.getBirthday().getDay());
        params.push_back(teacher.getBirthday().getMonth());
        params.push_back(teacher.getBirthday().getYear());
    } else {
        params.push_back(std::any{}); 
        params.push_back(std::any{}); 
        params.push_back(std::any{}); 
    }
    params.push_back(teacher.getAddress().empty() ? std::any{} : std::any{teacher.getAddress()});
    params.push_back(teacher.getCitizenId().empty() ? std::any{} : std::any{teacher.getCitizenId()});
    params.push_back(teacher.getEmail().empty() ? std::any{} : std::any{teacher.getEmail()});
    params.push_back(teacher.getPhoneNumber().empty() ? std::any{} : std::any{teacher.getPhoneNumber()});
    params.push_back(static_cast<int>(teacher.getRole()));
    params.push_back(static_cast<int>(teacher.getStatus()));
    
    // Teachers table params (ngoài userId)
    // params.push_back(teacher.getId()); // userId for Teachers table
    params.push_back(teacher.getFacultyId().empty() ? std::any{} : std::any{teacher.getFacultyId()});
    params.push_back(teacher.getQualification().empty() ? std::any{} : std::any{teacher.getQualification()});
    params.push_back(teacher.getSpecializationSubjects().empty() ? std::any{} : std::any{teacher.getSpecializationSubjects()});
    params.push_back(teacher.getDesignation().empty() ? std::any{} : std::any{teacher.getDesignation()});
    params.push_back(teacher.getExperienceYears());
    
    return params;
}

std::expected<std::vector<std::any>, Error> TeacherSqlParser::toQueryUpdateParams(const Teacher& teacher) const {
    std::vector<std::any> params;
    // Users table params
    params.push_back(teacher.getFirstName());
    params.push_back(teacher.getLastName());
    if (teacher.getBirthday().isSet()) {
        params.push_back(teacher.getBirthday().getDay());
        params.push_back(teacher.getBirthday().getMonth());
        params.push_back(teacher.getBirthday().getYear());
    } else {
        params.push_back(std::any{});
        params.push_back(std::any{});
        params.push_back(std::any{});
    }
    params.push_back(teacher.getAddress().empty() ? std::any{} : std::any{teacher.getAddress()});
    params.push_back(teacher.getCitizenId().empty() ? std::any{} : std::any{teacher.getCitizenId()});
    params.push_back(teacher.getEmail().empty() ? std::any{} : std::any{teacher.getEmail()});
    params.push_back(teacher.getPhoneNumber().empty() ? std::any{} : std::any{teacher.getPhoneNumber()});
    params.push_back(static_cast<int>(teacher.getRole()));
    params.push_back(static_cast<int>(teacher.getStatus()));

    // Teachers table params
    params.push_back(teacher.getFacultyId().empty() ? std::any{} : std::any{teacher.getFacultyId()});
    params.push_back(teacher.getQualification().empty() ? std::any{} : std::any{teacher.getQualification()});
    params.push_back(teacher.getSpecializationSubjects().empty() ? std::any{} : std::any{teacher.getSpecializationSubjects()});
    params.push_back(teacher.getDesignation().empty() ? std::any{} : std::any{teacher.getDesignation()});
    params.push_back(teacher.getExperienceYears());
    
    // ID for WHERE clause
    params.push_back(teacher.getId()); 
    
    return params;
}