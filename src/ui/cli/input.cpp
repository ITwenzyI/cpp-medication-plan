#include "input.hpp"
#include "common/result/result.hpp"
#include "common/strings/string_utils.hpp"
#include "domain/patient.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

namespace ui::cli::input {

std::string readLine(std::string_view prompt) {
    std::cout << prompt << std::flush;

    std::string line;
    std::getline(std::cin, line);

    return line;
}

common::result::Result<std::string> readNonEmpty(std::string_view prompt) {
    auto input = readLine(prompt);
    auto trimmed = common::strings::trim(input);

    if (trimmed.empty()) {
        return common::result::Result<std::string>::fail(common::result::ErrorCode::InvalidArgument,
            "Input must not be empty", "ui::cli::input::readNonEmpty");
    }
    return common::result::Result<std::string>::ok(trimmed);
}

std::optional<std::string> readOptionalString(std::string_view prompt) {
    auto input = readLine(prompt);
    auto trimmed = common::strings::trim(input);

    if (trimmed.empty()) {
        return std::nullopt;
    }
    return trimmed;
}

common::result::Result<int> readInt(std::string_view prompt, int min, int max) {
}

common::result::Result<bool> confirm(std::string_view prompt) {
}

common::result::Result<domain::Nationality> readNationality(std::string_view prompt) {
}
} // namespace ui::cli::input
