#pragma once
#include "error_code.hpp"
#include <string>

namespace common::result {
struct AppError {
    ErrorCode code;
    std::string message;
    std::string context = "";
};
} // namespace common::result