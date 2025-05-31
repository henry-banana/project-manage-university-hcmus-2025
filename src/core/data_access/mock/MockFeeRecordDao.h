// --- START OF MODIFIED FILE src/core/data_access/mock/MockFeeRecordDao.h ---
#ifndef MOCKFEERECORDDAO_H
#define MOCKFEERECORDDAO_H

#include "../interface/IFeeRecordDao.h"
#include <map>      // (➕)
#include <string>   // (➕)

class MockFeeRecordDao : public IFeeRecordDao {
public:
    MockFeeRecordDao();
    ~MockFeeRecordDao() override = default;

    std::expected<FeeRecord, Error> getById(const std::string& studentId) const override;
    std::expected<std::vector<FeeRecord>, Error> getAll() const override;
    std::expected<FeeRecord, Error> add(const FeeRecord& feeRecord) override;
    std::expected<bool, Error> update(const FeeRecord& feeRecord) override;
    std::expected<bool, Error> remove(const std::string& studentId) override;
    std::expected<bool, Error> exists(const std::string& studentId) const override;

    static void initializeDefaultMockData();
    static void clearMockData();
};

#endif // MOCKFEERECORDDAO_H
// --- END OF MODIFIED FILE src/core/data_access/mock/MockFeeRecordDao.h ---