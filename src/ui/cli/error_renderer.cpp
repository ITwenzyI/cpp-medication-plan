#include "error_renderer.hpp"
#include "common/result/result.hpp"
#include <iostream>
#include <string_view>

namespace ui::cli {

std::string_view ErrorRenderer::mapErrorCodeLabel(common::result::ErrorCode code) {
    switch (code) {
        case common::result::ErrorCode::Ok:
            return "No error";

        case common::result::ErrorCode::NotFound:
            return "Requested item was not found";

        case common::result::ErrorCode::InvalidArgument:
            return "Invalid input provided";

        case common::result::ErrorCode::Conflict:
            return "Operation conflicts with existing data";

        case common::result::ErrorCode::ForeignKeyViolation:
            return "Referenced item does not exist";

        case common::result::ErrorCode::DatabaseError:
            return "A database error occurred";

        case common::result::ErrorCode::ConstraintViolation:
            return "Data violates a database constraint";

        case common::result::ErrorCode::AlreadyExists:
            return "Item already exists";

        case common::result::ErrorCode::DataCorruption:
            return "Stored data appears to be corrupted";

        case common::result::ErrorCode::Unexpected:
            return "An unexpected error occurred";
    }

    return "Unknown error";
}

void ErrorRenderer::printErrorMessage(const common::result::AppError& error) {

    std::cout << "\nError: " << mapErrorCodeLabel(error.code) << '\n';
    if (error.message.empty()) {
        std::cout << "Message: " << "---" << '\n';
    } else {
        std::cout << "Message: " << error.message << '\n';
    }

    if (shouldPrintContext(error.code)) {
        std::cout << "Details:\n";
        std::cout << "  Code: " << common::result::errorCodeName(error.code) << '\n';
        std::cout << "  Where: " << error.context << '\n';
    }

    std::cout << '\n';
}

bool ErrorRenderer::shouldPrintContext(common::result::ErrorCode code) {
    switch (code) {
        case common::result::ErrorCode::DatabaseError:
        case common::result::ErrorCode::ForeignKeyViolation:
        case common::result::ErrorCode::ConstraintViolation:
        case common::result::ErrorCode::DataCorruption:
        case common::result::ErrorCode::Unexpected:
            return true;
        default:
            return false;
    }
}

} // namespace ui::cli
