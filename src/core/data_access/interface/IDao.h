#ifndef IDAO_H
#define IDAO_H

#include <vector>
#include <string> // Mặc định ID là string
#include <memory> // For std::shared_ptr in some cases (though usually not returned by DAO)
#include "../../../common/OperationResult.h" // For OperationResult
#include "../../entities/IEntity.h"         // Để đảm bảo TEntity là một IEntity

// Forward declarations cho TEntity (nếu cần thiết, thường không cần vì template)

template <typename TEntity, typename TId = std::string>
class IDao {
public:
    virtual ~IDao() = default;

    virtual OperationResult<TEntity> getById(TId id) const = 0;
    virtual OperationResult<std::vector<TEntity>> getAll() const = 0;
    // Optional: getAll với Paging và Sorting
    // virtual OperationResult<std::vector<TEntity>> getAll(PagingParams paging, SortParams sorting) const = 0;

    virtual OperationResult<TEntity> add(const TEntity& entity) = 0; // Trả về entity đã add (có thể có ID được generate) hoặc chỉ bool
    virtual OperationResult<bool> update(const TEntity& entity) = 0;
    virtual OperationResult<bool> remove(TId id) = 0;

    virtual OperationResult<bool> exists(TId id) const = 0; // (➕) Tiện ích kiểm tra tồn tại
};

#endif // IDAO_H