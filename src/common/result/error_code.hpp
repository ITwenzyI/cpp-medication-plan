#pragma once

namespace common::result {
enum class ErrorCode {
    Ok,
    NotFound,
    InvalidArgument,
    Conflict,
    ForeignKeyViolation,
    DatabaseError,
    ConstraintViolation,
    AlreadyExists,
    DataCorruption,
    Unexpected
};
} // namespace common::result
