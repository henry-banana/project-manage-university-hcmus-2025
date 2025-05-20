#ifndef IDAO_H
#define IDAO_H

#include <vector>
#include <string>
#include <memory>
#include <expected> // (➕) Thêm include này
#include "../../../common/ErrorType.h" // Để dùng Error struct
#include "../../entities/IEntity.h"

template <typename TEntity, typename TId = std::string>
class IDao {
public:
    virtual ~IDao() = default;

    virtual std::expected<TEntity, Error> getById(const TId& id) const = 0;
    virtual std::expected<std::vector<TEntity>, Error> getAll() const = 0;
    // virtual std::expected<std::vector<TEntity>, Error> getAll(PagingParams paging, SortParams sorting) const = 0;

    virtual std::expected<TEntity, Error> add(const TEntity& entity) = 0;
    virtual std::expected<bool, Error> update(const TEntity& entity) = 0;
    virtual std::expected<bool, Error> remove(const TId& id) = 0;

    virtual std::expected<bool, Error> exists(const TId& id) const = 0;
};

#endif // IDAO_H