#ifndef STUDENTSQLPARSER_H
#define STUDENTSQLPARSER_H

#include "../interface/IEntityParser.h"
#include "../../entities/Student.h"
#include "../../entities/User.h" // Vì Student kế thừa User
#include "../../entities/Birthday.h"
#include "../../../common/LoginStatus.h"
#include "../../../common/UserRole.h"
#include <any> // Cho std::any_cast

namespace SqlParserUtils {
    // (➕) Thêm inline
    template <typename T>
    inline T getOptional(const DbQueryResultRow& row, const std::string& columnName, T defaultValue = T{}) {
        auto it = row.find(columnName);
        if (it != row.end() && it->second.has_value()) {
            try {
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast& e) {
                // LOG_WARN("Bad any_cast for column: " + columnName + " to expected type. Error: " + e.what());
                // std::cerr << "Bad any_cast for column: " << columnName << " - " << e.what() << std::endl;
                return defaultValue;
            }
        }
        return defaultValue;
    }

    // (➕) Thêm inline
    template <>
    inline std::string getOptional<std::string>(const DbQueryResultRow& row, const std::string& columnName, std::string defaultValue) {
        auto it = row.find(columnName);
        if (it != row.end() && it->second.has_value()) {
            try {
                return std::any_cast<std::string>(it->second);
            } catch (const std::bad_any_cast&) {
                 try { 
                    return std::string(std::any_cast<const char*>(it->second));
                } catch (const std::bad_any_cast& e) {
                    // LOG_WARN("Bad any_cast for string column: " + columnName + ". Error: " + e.what());
                    // std::cerr << "Bad any_cast for string column: " << columnName << " - " << e.what() << std::endl;
                    return defaultValue;
                }
            }
        }
        return defaultValue;
    }
} // namespace SqlParserUtils

class StudentSqlParser : public IEntityParser<Student, DbQueryResultRow> {
public:
    StudentSqlParser() = default;

    std::expected<Student, Error> parse(const DbQueryResultRow& row) const override;

    // Serialize Student to DbQueryResultRow (map column_name -> value)
    // This might be useful for some generic operations, but toQueryParams is more common for SQL DAOs.
    std::expected<DbQueryResultRow, Error> serialize(const Student& student) const override;

    // Convert Student to a vector of std::any for INSERT statements
    // Order: id, firstName, lastName, birthDay, birthMonth, birthYear, address, citizenId, email, phoneNumber, role, status, facultyId
    // (ID của User, các trường của User, sau đó các trường của Student)
    std::expected<std::vector<std::any>, Error> toQueryInsertParams(const Student& student) const override;

    // Convert Student to a vector of std::any for UPDATE statements
    // Order: firstName, lastName, birthDay, birthMonth, birthYear, address, citizenId, email, phoneNumber, role, status, facultyId, id (for WHERE clause)
    std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const Student& student) const override;
};

#endif // STUDENTSQLPARSER_H