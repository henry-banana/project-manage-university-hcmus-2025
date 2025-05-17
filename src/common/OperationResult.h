// src/common/OperationResult.h
#ifndef OPERATION_RESULT_H
#define OPERATION_RESULT_H

#include <expected> 
#include "ErrorType.h"

template <typename TSuccess>
using OperationResult = std::expected<TSuccess, Error>;

// Dùng cho các thao tác thành công không cần trả về giá trị cụ thể (ngoài việc báo thành công/thất bại)
// Ví dụ: add, update, remove thường trả về bool
using SuccessStatus = OperationResult<bool>; // true nếu thành công, Error object nếu thất bại


#endif // OPERATION_RESULT_H