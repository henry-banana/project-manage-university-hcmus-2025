#include "SqlTeacherDao.h"
#include <vector>
#include <stdexcept>

SqlTeacherDao::SqlTeacherDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                             std::shared_ptr<IEntityParser<Teacher, DbQueryResultRow>> parser)
    : _dbAdapter(std::move(dbAdapter)), _parser(std::move(parser)) {
    if (!_dbAdapter) {
        throw std::invalid_argument("Database adapter cannot be null for SqlTeacherDao.");
    }
    if (!_parser) {
        throw std::invalid_argument("Teacher parser cannot be null for SqlTeacherDao.");
    }
}

std::expected<Teacher, Error> SqlTeacherDao::getById(const std::string& id) const {
    std::string sql = "SELECT U.id as userId, U.firstName, U.lastName, U.birthDay, U.birthMonth, U.birthYear, "
                      "U.address, U.citizenId, U.email, U.phoneNumber, U.role, U.status, "
                      "T.facultyId, T.qualification, T.specializationSubjects, T.designation, T.experienceYears "
                      "FROM Users U JOIN Teachers T ON U.id = T.userId "
                      "WHERE U.id = ?;";
    std::vector<DbQueryParam> params = {id};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Teacher with ID " + id + " not found."});
    }
    return _parser->parse(table[0]);
}

std::expected<std::vector<Teacher>, Error> SqlTeacherDao::getAll() const {
    std::string sql = "SELECT U.id as userId, U.firstName, U.lastName, U.birthDay, U.birthMonth, U.birthYear, "
                      "U.address, U.citizenId, U.email, U.phoneNumber, U.role, U.status, "
                      "T.facultyId, T.qualification, T.specializationSubjects, T.designation, T.experienceYears "
                      "FROM Users U JOIN Teachers T ON U.id = T.userId;";
    
    auto queryResult = _dbAdapter->executeQuery(sql);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<Teacher> teachers;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            teachers.push_back(parseResult.value());
        } else {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more teachers."});
        }
    }
    return teachers;
}

std::expected<Teacher, Error> SqlTeacherDao::add(const Teacher& teacher) {
     ValidationResult vr = teacher.validateBasic();
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid Teacher data for add: " + vr.getErrorMessagesCombined()});
    }
     // Kiểm tra xem email hoặc citizenId đã tồn tại chưa (trong bảng Users)
    auto emailCheck = findByEmail(teacher.getEmail()); // findByEmail của TeacherDao chỉ tìm teacher
    if(emailCheck.has_value()){
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Email " + teacher.getEmail() + " already exists for another teacher."});
    }
    // TODO: Cần một hàm check email chung cho tất cả User trong Users table nếu email là unique toàn hệ thống.

    auto transResult = _dbAdapter->beginTransaction();
    if (!transResult.has_value()) {
        return std::unexpected(transResult.error());
    }

    // 1. Insert vào bảng Users
    std::string userSql = "INSERT INTO Users (id, firstName, lastName, birthDay, birthMonth, birthYear, address, citizenId, email, phoneNumber, role, status) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    auto paramsResult = _parser->toQueryInsertParams(teacher);
    if (!paramsResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(paramsResult.error());
    }
    const auto& allParams = paramsResult.value();
    // Lấy params cho Users table (12 đầu)
    std::vector<DbQueryParam> userInsertParams(allParams.begin(), allParams.begin() + 12);

    auto userExecResult = _dbAdapter->executeUpdate(userSql, userInsertParams);
    if (!userExecResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(userExecResult.error());
    }

    // 2. Insert vào bảng Teachers
    std::string teacherSql = "INSERT INTO Teachers (userId, facultyId, qualification, specializationSubjects, designation, experienceYears) "
                             "VALUES (?, ?, ?, ?, ?, ?);";
    // Lấy params cho Teachers table (bắt đầu từ param thứ 13, sau 12 params của User, không bao gồm id đã bind cho user)
    // userId, facultyId, qualification, specializationSubjects, designation, experienceYears
    std::vector<DbQueryParam> teacherInsertParams;
    teacherInsertParams.push_back(teacher.getId()); // userId
    for(size_t i = 12; i < allParams.size(); ++i) { // facultyId, qualification, ...
        teacherInsertParams.push_back(allParams[i]);
    }


    auto teacherExecResult = _dbAdapter->executeUpdate(teacherSql, teacherInsertParams);
    if (!teacherExecResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(teacherExecResult.error());
    }

    auto commitResult = _dbAdapter->commitTransaction();
    if (!commitResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(commitResult.error());
    }

    return teacher;
}

std::expected<bool, Error> SqlTeacherDao::update(const Teacher& teacher) {
    ValidationResult vr = teacher.validateBasic();
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid Teacher data for update: " + vr.getErrorMessagesCombined()});
    }
    // Kiểm tra email
    auto byEmail = findByEmail(teacher.getEmail());
    if(byEmail.has_value() && byEmail.value().getId() != teacher.getId()){
         return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Email " + teacher.getEmail() + " already exists for another teacher."});
    }
     if (!byEmail.has_value() && byEmail.error().code != ErrorCode::NOT_FOUND) {
        return std::unexpected(byEmail.error());
    }

    auto transResult = _dbAdapter->beginTransaction();
    if (!transResult.has_value()) {
        return std::unexpected(transResult.error());
    }

    // 1. Update bảng Users
    std::string userSql = "UPDATE Users SET firstName = ?, lastName = ?, birthDay = ?, birthMonth = ?, birthYear = ?, "
                          "address = ?, citizenId = ?, email = ?, phoneNumber = ?, role = ?, status = ? "
                          "WHERE id = ?;";
    
    auto paramsResult = _parser->toQueryUpdateParams(teacher);
     if (!paramsResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(paramsResult.error());
    }
    const auto& allParams = paramsResult.value();
    // Params cho Users: 11 đầu + id ở cuối
    std::vector<DbQueryParam> userUpdateParams(allParams.begin(), allParams.begin() + 11);
    userUpdateParams.push_back(allParams.back()); // id

    auto userExecResult = _dbAdapter->executeUpdate(userSql, userUpdateParams);
    if (!userExecResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(userExecResult.error());
    }

    // 2. Update bảng Teachers
    std::string teacherSql = "UPDATE Teachers SET facultyId = ?, qualification = ?, specializationSubjects = ?, "
                             "designation = ?, experienceYears = ? WHERE userId = ?;";
    // Params cho Teachers: từ index 11 đến trước id (cuối cùng)
    // facultyId, qualification, specializationSubjects, designation, experienceYears, userId (là allParams.back())
    std::vector<DbQueryParam> teacherUpdateParams;
    for(size_t i = 11; i < allParams.size() -1; ++i){ // Bỏ qua id ở cuối cùng
        teacherUpdateParams.push_back(allParams[i]);
    }
    teacherUpdateParams.push_back(allParams.back()); // userId


    auto teacherExecResult = _dbAdapter->executeUpdate(teacherSql, teacherUpdateParams);
     if (!teacherExecResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(teacherExecResult.error());
    }

    if (userExecResult.value() == 0 && teacherExecResult.value() == 0) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Teacher with ID " + teacher.getId() + " not found for update, or no changes made."});
    }

    auto commitResult = _dbAdapter->commitTransaction();
    if (!commitResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(commitResult.error());
    }
    return true;
}

std::expected<bool, Error> SqlTeacherDao::remove(const std::string& id) {
    // Xóa User sẽ tự động cascade xóa Teacher, Login, SalaryRecords (theo schema)
    std::string sql = "DELETE FROM Users WHERE id = ? AND role = ?;"; // Chỉ xóa nếu là Teacher
    std::vector<DbQueryParam> params = {id, static_cast<int>(UserRole::TEACHER)};

    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Teacher with ID " + id + " not found for removal."});
    }
    return true;
}

std::expected<bool, Error> SqlTeacherDao::exists(const std::string& id) const {
    std::string sql = "SELECT 1 FROM Users U JOIN Teachers T ON U.id = T.userId WHERE U.id = ? LIMIT 1;";
    std::vector<DbQueryParam> params = {id};
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    return !queryResult.value().empty();
}

std::expected<std::vector<Teacher>, Error> SqlTeacherDao::findByFacultyId(const std::string& facultyId) const {
    std::string sql = "SELECT U.id as userId, U.firstName, U.lastName, U.birthDay, U.birthMonth, U.birthYear, "
                      "U.address, U.citizenId, U.email, U.phoneNumber, U.role, U.status, "
                      "T.facultyId, T.qualification, T.specializationSubjects, T.designation, T.experienceYears "
                      "FROM Users U JOIN Teachers T ON U.id = T.userId "
                      "WHERE T.facultyId = ?;";
    std::vector<DbQueryParam> params = {facultyId};
    
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<Teacher> teachers;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            teachers.push_back(parseResult.value());
        } else {
             return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more teachers for faculty " + facultyId});
        }
    }
    return teachers;
}

std::expected<std::vector<Teacher>, Error> SqlTeacherDao::findByDesignation(const std::string& designation) const {
     std::string sql = "SELECT U.id as userId, U.firstName, U.lastName, U.birthDay, U.birthMonth, U.birthYear, "
                      "U.address, U.citizenId, U.email, U.phoneNumber, U.role, U.status, "
                      "T.facultyId, T.qualification, T.specializationSubjects, T.designation, T.experienceYears "
                      "FROM Users U JOIN Teachers T ON U.id = T.userId "
                      "WHERE T.designation LIKE ?;"; // Dùng LIKE để tìm kiếm gần đúng
    std::vector<DbQueryParam> params = {"%" + designation + "%"};
    
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<Teacher> teachers;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            teachers.push_back(parseResult.value());
        } else {
             return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more teachers for designation."});
        }
    }
    return teachers;
}

std::expected<Teacher, Error> SqlTeacherDao::findByEmail(const std::string& email) const {
    std::string sql = "SELECT U.id as userId, U.firstName, U.lastName, U.birthDay, U.birthMonth, U.birthYear, "
                      "U.address, U.citizenId, U.email, U.phoneNumber, U.role, U.status, "
                      "T.facultyId, T.qualification, T.specializationSubjects, T.designation, T.experienceYears "
                      "FROM Users U JOIN Teachers T ON U.id = T.userId "
                      "WHERE U.email = ? AND U.role = ?;"; // Chỉ tìm teacher
    std::vector<DbQueryParam> params = {email, static_cast<int>(UserRole::TEACHER)};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Teacher with email " + email + " not found."});
    }
    return _parser->parse(table[0]);
}