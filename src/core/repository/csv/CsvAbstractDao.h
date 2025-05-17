#pragma once
#include "../interface/IDao.h"
#include "../../parsing/interface/IEntityParser.h" // Sẽ dùng parser
#include "../../../utils/IFileHandler.h"         // Sẽ dùng file handler
#include <vector>
#include <string>
#include <memory>

template <typename TEntity, typename TId = std::string>
class CsvAbstractDao : public IDao<TEntity, TId> {
protected:
    std::shared_ptr<IFileHandler> _fileHandler;
    std::filesystem::path _filePath;
    // Parser sẽ chuyên trách việc chuyển đổi dòng CSV thành Entity
    std::shared_ptr<IEntityParser<TEntity, std::vector<std::string>>> _csvRowParser;

    // Helper để đọc tất cả entities, được sử dụng bởi getAll, getById, exists, ...
    virtual OperationResult<std::vector<TEntity>> loadAllEntities() const;
    // Helper để ghi tất cả entities (dùng cho add, update, remove)
    virtual OperationResult<bool> saveAllEntities(const std::vector<TEntity>& entities);

public:
    CsvAbstractDao(std::shared_ptr<IFileHandler> fileHandler,
                   const std::filesystem::path& filePath,
                   std::shared_ptr<IEntityParser<TEntity, std::vector<std::string>>> csvRowParser)
        : _fileHandler(fileHandler), _filePath(filePath), _csvRowParser(csvRowParser) {}

    // Các phương thức của IDao sẽ sử dụng loadAllEntities và saveAllEntities
    OperationResult<TEntity> getById(TId id) const override;
    OperationResult<std::vector<TEntity>> getAll() const override;
    OperationResult<TEntity> add(const TEntity& entity) override; // Có thể cần trả về TEntity thay vì bool
    OperationResult<bool> update(const TEntity& entity) override;
    OperationResult<bool> remove(TId id) override;
    OperationResult<bool> exists(TId id) const override;
};