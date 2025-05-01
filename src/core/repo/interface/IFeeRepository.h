#ifndef IFEEREPOSITORY_H
#define IFEEREPOSITORY_H

#include <optional>
#include <string>
#include "../../entities/FeeRecord.h"

// Interface cho Fee Repository
class IFeeRepository {
private:

protected:

public:
    virtual ~IFeeRepository() = default;

    // CRUD operations
    virtual std::optional<FeeRecord> findByStudentId(const std::string& studentId) = 0;
    virtual bool add(const FeeRecord& feeRecord) = 0;
    virtual bool update(const FeeRecord& feeRecord) = 0;
    virtual bool remove(const std::string& studentId) = 0;
};

#endif // IFEEREPOSITORY_H