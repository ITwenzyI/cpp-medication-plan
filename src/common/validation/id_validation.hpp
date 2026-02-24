#pragma once

#include "common/result/result.hpp"

namespace common::validation {

[[nodiscard]] inline common::result::Result<void> validateId(int id) {
    if (id <= 0) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "id must be positive", "common::validation::validateId");
    }
    return common::result::Result<void>::ok();
}
} // namespace common::validation