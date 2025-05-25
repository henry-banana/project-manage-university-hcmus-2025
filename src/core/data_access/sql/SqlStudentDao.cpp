#include "SqlStudentDao.h"
#include <vector> // For std::vector
#include <stdexcept> // For std::invalid_argument

SqlStudentDao::SqlStudentDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                             std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> parser)
    : _dbAdapter(std::move(dbAdapter)), _parser(std::move(parser)) {
    if (!_dbAdapter) {
        throw std::invalid_argument("Database adapter cannot be null for SqlStudentDao.");
    }
    if (!_parser) {
        throw std::invalid_argument("Student parser cannot be null for SqlStudentDao.");
    }
}

std::expected<Student, Error> SqlStudentDao::getById(const std::string& id) const {
    // JOIN Users và Students tables
    std::string sql = "SELECT U.id as userId, U.firstName, U.lastName, U.birthDay, U.birthMonth, U.birthYear, "
                      "U.address, U.citizenId, U.email, U.phoneNumber, U.role, U.status, S.facultyId "
                      "FROM Users U JOIN Students S ON U.id = S.userId "
                      "WHERE U.id = ?;";
    std::vector<DbQueryParam> params = {id};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Student with ID " + id + " not found."});
    }
    return _parser->parse(table[0]);
}

std::expected<std::vector<Student>, Error> SqlStudentDao::getAll() const {
    std::string sql = "SELECT U.id as userId, U.firstName, U.lastName, U.birthDay, U.birthMonth, U.birthYear, "
                      "U.address, U.citizenId, U.email, U.phoneNumber, U.role, U.status, S.facultyId "
                      "FROM Users U JOIN Students S ON U.id = S.userId;";
    
    auto queryResult = _dbAdapter->executeQuery(sql);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<Student> students;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            students.push_back(parseResult.value());
        } else {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more students."});
        }
    }
    return students;
}

std::expected<Student, Error> SqlStudentDao::add(const Student& student) {
    ValidationResult vr = student.validateBasic();
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid Student data for add: " + vr.getErrorMessagesCombined()});
    }
    // Kiểm tra xem email hoặc citizenId đã tồn tại chưa (trong bảng Users)
    auto emailCheck = findByEmail(student.getEmail());
    if(emailCheck.has_value()){
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Email " + student.getEmail() + " already exists for another user."});
    }
    // Thêm các kiểm tra khác nếu cần (ví dụ citizenId)

    auto transResult = _dbAdapter->beginTransaction();
    if (!transResult.has_value()) {
        return std::unexpected(transResult.error());
    }

    // 1. Insert vào bảng Users
    std::string userSql = "INSERT INTO Users (id, firstName, lastName, birthDay, birthMonth, birthYear, address, citizenId, email, phoneNumber, role, status) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    auto userParamsResult = _parser->toQueryInsertParams(student); // Parser sẽ trả về đủ params cho cả Users và Students
    if (!userParamsResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(userParamsResult.error());
    }
    const auto& allParams = userParamsResult.value();
    // Lấy các tham số cho bảng Users (12 tham số đầu tiên theo StudentSqlParser::toQueryInsertParams)
    std::vector<DbQueryParam> userInsertParams(allParams.begin(), allParams.begin() + 12);


    auto userExecResult = _dbAdapter->executeUpdate(userSql, userInsertParams);
    if (!userExecResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(userExecResult.error());
    }
    if (userExecResult.value() == 0) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to add user part of student, no rows affected."});
    }

    // 2. Insert vào bảng Students
    std::string studentSql = "INSERT INTO Students (userId, facultyId) VALUES (?, ?);";
    // Lấy tham số cho bảng Students (tham số cuối từ allParams là facultyId, userId là student.getId())
    std::vector<DbQueryParam> studentInsertParams = {student.getId(), allParams.back()};


    auto studentExecResult = _dbAdapter->executeUpdate(studentSql, studentInsertParams);
    if (!studentExecResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(studentExecResult.error());
    }
     if (studentExecResult.value() == 0) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to add student-specific part, no rows affected."});
    }

    auto commitResult = _dbAdapter->commitTransaction();
    if (!commitResult.has_value()) {
        // Dù commit thất bại, cố gắng rollback (dù có thể không cần nếu DB tự xử lý)
        _dbAdapter->rollbackTransaction(); 
        return std::unexpected(commitResult.error());
    }

    return student;
}

std::expected<bool, Error> SqlStudentDao::update(const Student& student) {
    ValidationResult vr = student.validateBasic();
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid Student data for update: " + vr.getErrorMessagesCombined()});
    }
    // Kiểm tra email mới có trùng với user khác không
    auto byEmail = findByEmail(student.getEmail());
    if(byEmail.has_value() && byEmail.value().getId() != student.getId()){
         return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Email " + student.getEmail() + " already exists for another user."});
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
    
    auto paramsResult = _parser->toQueryUpdateParams(student);
    if (!paramsResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(paramsResult.error());
    }
    const auto& allParams = paramsResult.value();
    // Lấy các tham số cho Users (11 tham số đầu + id ở cuối)
    // firstName, lastName, birthDay, birthMonth, birthYear, address, citizenId, email, phoneNumber, role, status, id
    std::vector<DbQueryParam> userUpdateParams(allParams.begin(), allParams.begin() + 11);
    userUpdateParams.push_back(allParams.back()); // id

    auto userExecResult = _dbAdapter->executeUpdate(userSql, userUpdateParams);
    if (!userExecResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(userExecResult.error());
    }
    // Không cần check rows affected chặt chẽ ở đây vì có thể chỉ update 1 trong 2 bảng

    // 2. Update bảng Students
    std::string studentSql = "UPDATE Students SET facultyId = ? WHERE userId = ?;";
    // facultyId là tham số thứ 12 (index 11), id là tham số cuối cùng (index 12) trong allParams
    std::vector<DbQueryParam> studentUpdateParams = {allParams[11], allParams.back()};


    auto studentExecResult = _dbAdapter->executeUpdate(studentSql, studentUpdateParams);
     if (!studentExecResult.has_value()) {
        _dbAdapter->rollbackTransaction();
        return std::unexpected(studentExecResult.error());
    }

    if (userExecResult.value() == 0 && studentExecResult.value() == 0) {
        // Nếu không có dòng nào được update ở cả 2 bảng (có thể do ID không tồn tại)
        _dbAdapter->rollbackTransaction(); // Rollback để đảm bảo
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Student with ID " + student.getId() + " not found for update, or no changes made."});
    }

    auto commitResult = _dbAdapter->commitTransaction();
    if (!commitResult.has_value()) {
         _dbAdapter->rollbackTransaction();
        return std::unexpected(commitResult.error());
    }
    return true;
}

std::expected<bool, Error> SqlStudentDao::remove(const std::string& id) {
    // Xóa User sẽ tự động cascade xóa Student, Login, Enrollments, CourseResults, FeeRecords (theo schema)
    std::string sql = "DELETE FROM Users WHERE id = ? AND role = ?;"; // Chỉ xóa nếu là Student
    std::vector<DbQueryParam> params = {id, static_cast<int>(UserRole::STUDENT)};

    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Student with ID " + id + " not found for removal."});
    }
    return true;
}

std::expected<bool, Error> SqlStudentDao::exists(const std::string& id) const {
    std::string sql = "SELECT 1 FROM Users U JOIN Students S ON U.id = S.userId WHERE U.id = ? LIMIT 1;";
    std::vector<DbQueryParam> params = {id};
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    return !queryResult.value().empty();
}

std::expected<std::vector<Student>, Error> SqlStudentDao::findByFacultyId(const std::string& facultyId) const {
    std::string sql = "SELECT U.id as userId, U.firstName, U.lastName, U.birthDay, U.birthMonth, U.birthYear, "
                      "U.address, U.citizenId, U.email, U.phoneNumber, U.role, U.status, S.facultyId "
                      "FROM Users U JOIN Students S ON U.id = S.userId "
                      "WHERE S.facultyId = ?;";
    std::vector<DbQueryParam> params = {facultyId};
    
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<Student> students;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            students.push_back(parseResult.value());
        } else {
             return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more students for faculty " + facultyId});
        }
    }
    return students;
}

std::expected<Student, Error> SqlStudentDao::findByEmail(const std::string& email) const {
    std::string sql = "SELECT U.id as userId, U.firstName, U.lastName, U.birthDay, U.birthMonth, U.birthYear, "
                      "U.address, U.citizenId, U.email, U.phoneNumber, U.role, U.status, S.facultyId "
                      "FROM Users U JOIN Students S ON U.id = S.userId "
                      "WHERE U.email = ? AND U.role = ?;"; // Chỉ tìm student
    std::vector<DbQueryParam> params = {email, static_cast<int>(UserRole::STUDENT)};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Student with email " + email + " not found."});
    }
    return _parser->parse(table[0]);
}

std::expected<std::vector<Student>, Error> SqlStudentDao::findByStatus(LoginStatus status) const {
     std::string sql = "SELECT U.id as userId, U.firstName, U.lastName, U.birthDay, U.birthMonth, U.birthYear, "
                      "U.address, U.citizenId, U.email, U.phoneNumber, U.role, U.status, S.facultyId "
                      "FROM Users U JOIN Students S ON U.id = S.userId "
                      "WHERE U.status = ? AND U.role = ?;"; // Chỉ tìm student
    std::vector<DbQueryParam> params = {static_cast<int>(status), static_cast<int>(UserRole::STUDENT)};
    
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<Student> students;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            students.push_back(parseResult.value());
        } else {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more students for status."});
        }
    }
    return students;
}

std::expected<bool, Error> SqlStudentDao::updateStatus(const std::string& studentId, LoginStatus newStatus) {
    // Chỉ update status trong bảng Users, role vẫn là Student
    std::string sql = "UPDATE Users SET status = ? WHERE id = ? AND role = ?;";
    std::vector<DbQueryParam> params = {static_cast<int>(newStatus), studentId, static_cast<int>(UserRole::STUDENT)};

    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Student with ID " + studentId + " not found for status update."});
    }
    return true;
}