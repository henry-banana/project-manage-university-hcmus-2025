#ifndef IENTITYPARSER_H
#define IENTITYPARSER_H

#include "../../../common/OperationResult.h"
#include <vector> // For TRawData as vector<string>
#include <string> // For TRawData as string (single line)
#include <map>    // For TRawData as map<string, any> (DB row)
#include <any>    // For DB row value type

template <typename TEntity, typename TRawDataFormat>
class IEntityParser {
public:
    virtual ~IEntityParser() = default;
    // Parse raw data (e.g., a CSV row as vector<string>, or a DB row as map<string, any>)
    // into an entity object.
    virtual OperationResult<TEntity> parse(const TRawDataFormat& rawData) const = 0;

    // (Optional) Format an entity object back into a raw data format (e.g., for saving to CSV).
    // virtual OperationResult<TRawDataFormat> format(const TEntity& entity) const = 0;
};

#endif // IENTITYPARSER_H