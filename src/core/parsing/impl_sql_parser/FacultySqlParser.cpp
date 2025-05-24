#include "FacultySqlParser.h"

std::expected<Faculty, Error> FacultySqlParser::parse(const DbQueryResultRow& row) const {
    try {
        std::string id = SqlParserUtils::getOptional<std::string>(row, "id");
        if (id.empty()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Faculty ID not found or empty in SQL row."});
        }
        std::string name = SqlParserUtils::getOptional<std::string>(row, "name");
        if (name.empty()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Faculty name not found or empty in SQL row."});
        }
        return Faculty(id, name);
    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse Faculty from SQL row: " + std::string(e.what())});
    } catch (const std::exception& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Generic error parsing Faculty from SQL row: " + std::string(e.what())});
    }
}

std::expected<DbQueryResultRow, Error> FacultySqlParser::serialize(const Faculty& faculty) const {
    DbQueryResultRow row;
    row.emplace("id", faculty.getId());
    row.emplace("name", faculty.getName());
    return row;
}

std::expected<std::vector<std::any>, Error> FacultySqlParser::toQueryInsertParams(const Faculty& faculty) const {
    std::vector<std::any> params;
    params.push_back(faculty.getId());
    params.push_back(faculty.getName());
    return params;
}

std::expected<std::vector<std::any>, Error> FacultySqlParser::toQueryUpdateParams(const Faculty& faculty) const {
    std::vector<std::any> params;
    params.push_back(faculty.getName());
    params.push_back(faculty.getId()); // For WHERE clause
    return params;
}