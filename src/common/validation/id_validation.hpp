#pragma once

namespace common::validation {

[[nodiscard]] inline bool validateId(int id) {
    return id > 0;
}
} // namespace common::validation