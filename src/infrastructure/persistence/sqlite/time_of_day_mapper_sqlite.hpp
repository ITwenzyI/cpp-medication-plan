#pragma once
#include "domain/intake_plan.hpp"
#include <stdexcept>
#include <string>

namespace infrastructure::persistence::sqlite {
inline std::string timeOfDayToDbString(domain::TimeOfDay timeOfDay) {
    switch (timeOfDay) {
        case domain::TimeOfDay::Morning:
            return "morning";
        case domain::TimeOfDay::Noon:
            return "noon";
        case domain::TimeOfDay::Evening:
            return "evening";
        case domain::TimeOfDay::Night:
            return "night";
    }
    throw std::logic_error("Invalid TimeOfDay enum value.");
}

inline common::result::Result<domain::TimeOfDay> timeOfDayFromDbString(std::string_view value) {
    if (value == "morning") {
        return common::result::Result<domain::TimeOfDay>::ok(domain::TimeOfDay::Morning);
    }
    if (value == "noon") {
        return common::result::Result<domain::TimeOfDay>::ok(domain::TimeOfDay::Noon);
    }
    if (value == "evening") {
        return common::result::Result<domain::TimeOfDay>::ok(domain::TimeOfDay::Evening);
    }
    if (value == "night") {
        return common::result::Result<domain::TimeOfDay>::ok(domain::TimeOfDay::Night);
    }

    return common::result::Result<domain::TimeOfDay>::fail(
        common::result::ErrorCode::DataCorruption, "Invalid time_of_day value in database",
        "infrastructure::persistence::sqlite::timeOfDayFromDbString");
}
} // namespace infrastructure::persistence::sqlite