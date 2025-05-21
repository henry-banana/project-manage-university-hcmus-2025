#ifndef MOCKFEERECORDDAO_H
#define MOCKFEERECORDDAO_H

#include "../interface/IFeeRecordDao.h"
// FeeRecord đã được IFeeRecordDao.h include (vì IFeeRecordDao kế thừa IDao<FeeRecord, std::string>)

class MockFeeRecordDao : public IFeeRecordDao {
private:
    // std::map<std::string, FeeRecord> _mockData; // Key là studentId
public:
    MockFeeRecordDao();
    ~MockFeeRecordDao() override = default;

    // Các phương thức từ IDao<FeeRecord, std::string>
    std::expected<FeeRecord, Error> getById(const std::string& studentId) const override;
    std::expected<std::vector<FeeRecord>, Error> getAll() const override;
    std::expected<FeeRecord, Error> add(const FeeRecord& feeRecord) override;
    std::expected<bool, Error> update(const FeeRecord& feeRecord) override;
    std::expected<bool, Error> remove(const std::string& studentId) override;
    std::expected<bool, Error> exists(const std::string& studentId) const override;
};

#endif // MOCKFEERECORDDAO_H