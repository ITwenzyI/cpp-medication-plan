#pragma once
#include "ErrorCode.hpp"
#include <string>

struct AppError {
    ErrorCode code;
    std::string message;
    std::string context;
};