#pragma once

namespace common::result {
enum class ErrorCode {
    Ok,
    NotFound,
    InvalidArgument,
    DatabaseError,
    ConstraintViolation,
    InvalidNationalityCode,
    Unexpected
};
} // namespace common::result
