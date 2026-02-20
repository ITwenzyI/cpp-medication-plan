#pragma once

namespace common::result {
enum class ErrorCode {
    Ok,
    NotFound,
    InvalidArgument,
    DatabaseError,
    ConstraintViolation,
    Unexpected
};
} // namespace common::result
