#ifndef IENTITY_H
#define IENTITY_H

#include <string>

class IEntity {
public:
    virtual ~IEntity() = default;

    // Tất cả các entity chính đều có ID (dù là string hay int, ở đây thống nhất string trước)
    virtual std::string getId() const = 0;

    // Phương thức để biểu diễn entity dưới dạng chuỗi (cho việc hiển thị, logging)
    virtual std::string toString() const = 0;
};

#endif // IENTITY_H