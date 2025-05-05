#ifndef IUSERREPOSITORY_H
#define IUSERREPOSITORY_H

#include <string>
#include <vector>
#include <memory> // For std::unique_ptr or std::shared_ptr
#include <optional> // For returning optional objects

// Interface chung cho các thao tác cơ bản với User (nếu cần)
// Thường thì sẽ dùng IStudentRepository và IFacultyRepository cụ thể hơn.
class IUserRepository {
public:
    virtual ~IUserRepository() = default;
};

#endif // IUSERREPOSITORY_H