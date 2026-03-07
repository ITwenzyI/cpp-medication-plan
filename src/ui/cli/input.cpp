#include "input.hpp"
#include "common/result/result.hpp"
#include "common/strings/string_utils.hpp"
#include "common/validation/birth_date_validation.hpp"
#include "domain/patient.hpp"
#include "infrastructure/persistence/sqlite/nationality_mapper_sqlite.hpp"
#include "infrastructure/persistence/sqlite/time_of_day_mapper_sqlite.hpp"
#include <charconv>
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

common::result::Result<int> readInt(std::string_view prompt) {
    auto input = readLine(prompt);
    auto trimmed = common::strings::trim(input);

    if (trimmed.empty()) {
        return common::result::Result<int>::fail(common::result::ErrorCode::InvalidArgument,
            "Please enter a valid number", "ui::cli::input::readInt");
    }

    int value = 0;

    auto [ptr, ec] = std::from_chars(trimmed.data(), trimmed.data() + trimmed.size(), value);

    if (ec == std::errc::invalid_argument) {
        return common::result::Result<int>::fail(common::result::ErrorCode::InvalidArgument,
            "Please enter a valid number", "ui::cli::input::readInt");
    } else if (ec == std::errc::result_out_of_range) {
        return common::result::Result<int>::fail(common::result::ErrorCode::InvalidArgument,
            "This number is too large", "ui::cli::input::readInt");
    }

    if (ptr != trimmed.data() + trimmed.size()) {
        return common::result::Result<int>::fail(common::result::ErrorCode::InvalidArgument,
            "Please enter a valid number", "ui::cli::input::readInt");
    }

    return common::result::Result<int>::ok(value);
}

common::result::Result<int> readMenuChoice(std::string_view prompt, int min, int max) {
    auto choice = readInt(prompt);

    if (choice.isError()) {
        return choice;
    }

    auto value = choice.value();

    if (value < min || value > max) {
        return common::result::Result<int>::fail(common::result::ErrorCode::InvalidArgument,
            "Please enter a number between " + std::to_string(min) + " and " + std::to_string(max),
            "ui::cli::input::readMenuChoice");
    }

    return common::result::Result<int>::ok(value);
}

common::result::Result<bool> confirm(std::string_view prompt) {
    auto new_prompt = std::string(prompt);
    new_prompt.append(" (y/n): ");

    while (true) {
        auto input = readLine(new_prompt);
        auto normalized = common::strings::normalize(input);

        if (normalized == "Y" || normalized == "YES") {
            return common::result::Result<bool>::ok(true);
        }
        if (normalized == "N" || normalized == "NO") {
            return common::result::Result<bool>::ok(false);
        }
        std::cout << "Please enter 'y' or 'n'.\n";
    }
}

common::result::Result<std::optional<std::string>> readOptionalBirthDate(std::string_view prompt) {
    auto input = readLine(prompt);
    auto trimmed = common::strings::trim(input);

    if (trimmed.empty()) {
        return common::result::Result<std::optional<std::string>>::ok(std::nullopt);
    }

    if (!common::validation::isValidBirthDate(trimmed)) {
        return common::result::Result<std::optional<std::string>>::fail(
            common::result::ErrorCode::InvalidArgument, "Invalid birth date. Format: YYYY-MM-DD.",
            "ui::cli::input::readOptionalBirthDate");
    }
    return common::result::Result<std::optional<std::string>>::ok(trimmed);
}

common::result::Result<std::optional<domain::Nationality>> readOptionalNationality(
    std::string_view prompt) {
    auto input = readLine(prompt);
    auto normalized = common::strings::normalize(input);

    if (normalized.empty()) {
        return common::result::Result<std::optional<domain::Nationality>>::ok(std::nullopt);
    }

    auto mapped = infrastructure::persistence::sqlite::nationalityFromDbString(normalized);
    if (mapped.isError()) {
        return common::result::Result<std::optional<domain::Nationality>>::fail(mapped.error().code,
            "Invalid nationality code. Use e.g. DE, US, GB.",
            "ui::cli::input::readOptionalNationality");
    }
    return common::result::Result<std::optional<domain::Nationality>>::ok(mapped.value());
}

common::result::Result<domain::TimeOfDay> readTimeOfDay(std::string_view prompt) {
    auto input = readLine(prompt);
    auto trimmed = common::strings::trim(input);

    if (trimmed.empty()) {
        return common::result::Result<domain::TimeOfDay>::fail(
            common::result::ErrorCode::InvalidArgument,
            "Time must not be empty. Use: Morning, Noon, Evening, Night.",
            "ui::cli::input::readTimeOfDay");
    }

    auto mapped = infrastructure::persistence::sqlite::timeOfDayFromDbString(trimmed);
    if (mapped.isError()) {
        return common::result::Result<domain::TimeOfDay>::fail(mapped.error().code,
            "Invalid time of day. Use: Morning, Noon, Evening, Night.",
            "ui::cli::input::readTimeOfDay");
    }
    return common::result::Result<domain::TimeOfDay>::ok(mapped.value());
}
} // namespace ui::cli::input
