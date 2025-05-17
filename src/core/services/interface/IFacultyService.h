#ifndef IFACULTYSERVICE_H
#define IFACULTYSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include "../../../common/OperationResult.h"
#include "../../entities/Faculty.h"

class IFacultyService {
public:
    virtual ~IFacultyService() = default;
    virtual OperationResult<Faculty> getFacultyById(const std::string& facultyId) const = 0;
    virtual OperationResult<Faculty> getFacultyByName(const std::string& name) const = 0;
    virtual OperationResult<std::vector<Faculty>> getAllFaculties() const = 0;

    // Các hàm sau thường chỉ Admin mới được gọi
    virtual OperationResult<Faculty> addFaculty(const std::string& id, const std::string& name) = 0;
    virtual OperationResult<bool> updateFaculty(const std::string& facultyId, const std::string& newName) = 0;
    virtual OperationResult<bool> removeFaculty(const std::string& facultyId) = 0; // Cần kiểm tra ràng buộc
};

#endif // IFACULTYSERVICE_H