#pragma once

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
} // namespace common::result
