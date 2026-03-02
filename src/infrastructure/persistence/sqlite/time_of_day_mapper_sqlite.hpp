#pragma once
#include "common/result/result.hpp"
#include "domain/intake_plan.hpp"
#include <stdexcept>
#include <string>

namespace infrastructure::persistence::sqlite {
inline std::string timeOfDayToDbString(domain::TimeOfDay time_of_day) {
    switch (time_of_day) {
        case domain::TimeOfDay::Morning:
            return "Morning";
        case domain::TimeOfDay::Noon:
            return "Noon";
        case domain::TimeOfDay::Evening:
            return "Evening";
        case domain::TimeOfDay::Night:
            return "Night";
    }
    throw std::logic_error("Invalid TimeOfDay enum value.");
}

inline common::result::Result<domain::TimeOfDay> timeOfDayFromDbString(std::string_view value) {
    if (value == "Morning") {
        return common::result::Result<domain::TimeOfDay>::ok(domain::TimeOfDay::Morning);
    }
    if (value == "Noon") {
        return common::result::Result<domain::TimeOfDay>::ok(domain::TimeOfDay::Noon);
    }
    if (value == "Evening") {
        return common::result::Result<domain::TimeOfDay>::ok(domain::TimeOfDay::Evening);
    }
    if (value == "Night") {
        return common::result::Result<domain::TimeOfDay>::ok(domain::TimeOfDay::Night);
    }

    return common::result::Result<domain::TimeOfDay>::fail(
        common::result::ErrorCode::DataCorruption, "Invalid time_of_day value in database",
        "infrastructure::persistence::sqlite::timeOfDayFromDbString");
}
} // namespace infrastructure::persistence::sqlite