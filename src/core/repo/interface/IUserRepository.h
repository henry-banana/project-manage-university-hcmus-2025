#ifndef IUSERREPOSITORY_H
#define IUSERREPOSITORY_H

#include <string>
#include <vector>
#include <memory> // For std::unique_ptr or std::shared_ptr
#include <optional> // For returning optional objects

// Forward declaration
namespace Core { namespace Entities { class User; } } // Giả sử namespace

// Interface chung cho các thao tác cơ bản với User (nếu cần)
// Thường thì sẽ dùng IStudentRepository và IFacultyRepository cụ thể hơn.
class IUserRepository {
public:
    virtual ~IUserRepository() = default;

    // Tìm user theo ID (có thể trả về Student hoặc Faculty)
    virtual std::optional<std::unique_ptr<Core::Entities::User>> findById(const std::string& userId) = 0;

    // Thêm user mới (có thể cần tách ra Student/Faculty)
    // virtual bool addUser(const Core::Entities::User& user) = 0;

    // Cập nhật thông tin user
    // virtual bool updateUser(const Core::Entities::User& user) = 0;

    // Xóa user
    // virtual bool deleteUser(const std::string& userId) = 0;
};

#endif // IUSERREPOSITORY_H