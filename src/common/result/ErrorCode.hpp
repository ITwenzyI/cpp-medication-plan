#pragma once

enum class ErrorCode {
    Ok,
    NotFound,
    InvalidArgument,
    DatabaseError,
    ConstraintViolation,
    Unexpected
};