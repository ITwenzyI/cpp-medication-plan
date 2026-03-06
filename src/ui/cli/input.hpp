#pragma once

#include "common/result/result.hpp"
#include "domain/patient.hpp"
#include <optional>
#include <string>
#include <string_view>

namespace ui::cli::input {

std::string readLine(std::string_view prompt);

common::result::Result<std::string> readNonEmpty(std::string_view prompt);

std::optional<std::string> readOptionalString(std::string_view prompt);

common::result::Result<int> readInt(std::string_view prompt);
common::result::Result<int> readMenuChoice(std::string_view prompt, int min, int max);

common::result::Result<bool> confirm(std::string_view prompt);

common::result::Result<domain::Nationality> readNationality(std::string_view prompt);
} // namespace ui::cli::input