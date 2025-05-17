#ifndef IENTITYPARSER_H
#define IENTITYPARSER_H

#include "../../../common/OperationResult.h"
#include <vector>
#include <string>
#include <map>
#include <any>

// Định nghĩa DbQueryResultRow ở đây nếu IDatabaseAdapter chưa định nghĩa,
// nhưng tốt hơn là IDatabaseAdapter định nghĩa nó và IEntityParser include.
// Giả sử IDatabaseAdapter.h đã định nghĩa:
// using DbQueryResultRow = std::map<std::string, std::any>;

template <typename TEntity, typename TRawDataFormat>
class IEntityParser {
public:
    virtual ~IEntityParser() = default;
    virtual OperationResult<TEntity> parse(const TRawDataFormat& rawData) const = 0;
};

#endif // IENTITYPARSER_H