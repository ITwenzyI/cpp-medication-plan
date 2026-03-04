#pragma once
#include <string_view>

namespace common::result {
enum class ErrorCode {
    Ok, // Operation succeeded

    NotFound, // No entity found (e.g. UPDATE/DELETE changes == 0)

    InvalidArgument, // Validation failure (invalid input data)

    Conflict, // State conflict (e.g. UNIQUE constraint violation)

    ForeignKeyViolation, // Referenced entity does not exist (FK constraint)

    DatabaseError, // Low-level database error (I/O, prepare/step failure)

    ConstraintViolation, // Generic constraint failure (fallback if not more specific)

    AlreadyExists, // Entity with same identity already exists (mainly for CREATE)

    DataCorruption, // Persisted data is inconsistent or invalid (e.g. invalid enum in DB)

    Unexpected // Unhandled or logically impossible state
};

inline std::string_view errorCodeName(ErrorCode code) noexcept {
    switch (code) {
        case ErrorCode::Ok:
            return "Ok";
        case ErrorCode::NotFound:
            return "NotFound";
        case ErrorCode::InvalidArgument:
            return "InvalidArgument";
        case ErrorCode::Conflict:
            return "Conflict";
        case ErrorCode::ForeignKeyViolation:
            return "ForeignKeyViolation";
        case ErrorCode::DatabaseError:
            return "DatabaseError";
        case ErrorCode::ConstraintViolation:
            return "ConstraintViolation";
        case ErrorCode::AlreadyExists:
            return "AlreadyExists";
        case ErrorCode::DataCorruption:
            return "DataCorruption";
        case ErrorCode::Unexpected:
            return "Unexpected";
    }

    return "UnknownErrorCode";
}

} // namespace common::result
