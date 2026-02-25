#pragma once

namespace common::result {
enum class ErrorCode {
    Ok,
    NotFound,
    InvalidArgument,
    DatabaseError,
    ConstraintViolation,
    InvalidNationalityCode,
    InvalidBirthDate,
    Unexpected
};
} // namespace common::result
