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
    InvalidNationalityCode,
    InvalidBirthDate,
    Unexpected
};
} // namespace common::result
