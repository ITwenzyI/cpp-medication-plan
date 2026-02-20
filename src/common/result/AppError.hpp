#pragma once
#include "src/common/result/ErrorCode.hpp"
#include <string>

struct AppError {
    ErrorCode code;
    std::string message;
    std::string context;
};