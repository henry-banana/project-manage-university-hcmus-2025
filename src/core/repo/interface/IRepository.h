#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include <string>
#include <vector>

template<typename T>

class IRepository {
public:
    virtual ~IRepository() = default;

    // Course operations
    virtual std::optional<T> findById(const std::string& id) const = 0;
    virtual std::vector<T> findAll() const = 0;
    virtual std::vector<T> findByFacultyId(const std::string& id) const = 0; // Courses offered by a Faculty
    virtual bool add(const T& obj) = 0;
    virtual bool update(const T& obj) = 0;
    virtual bool remove(const std::string& obj_id) = 0;
    virtual bool exists(const std::string& obj_id) const = 0;
};

#endif // IREPOSITORY_H