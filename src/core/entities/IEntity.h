#ifndef IENTITY_H
#define IENTITY_H

#include <string>
#include <vector>
#include "../../common/ErrorType.h" // For Error struct
#include "../../common/ValidationResult.h" // For ValidationResult

class IEntity {
public:
    virtual ~IEntity() = default;

    // Bắt buộc phải có ID, nhưng kiểu ID có thể khác nhau, nên tạm để là string
    // Các lớp con cụ thể có thể có getter ID với kiểu mạnh hơn
    virtual std::string getStringId() const = 0;

    virtual std::string toString() const = 0; // Để debug và hiển thị cơ bản

    // (Optional but recommended) Mỗi entity có thể tự validate các thuộc tính cơ bản của nó.
    // Validator bên ngoài sẽ validate các rule phức tạp hơn (liên quan DB, logic nghiệp vụ)
    virtual ValidationResult validateBasic() const = 0;
};

#endif // IENTITY_H