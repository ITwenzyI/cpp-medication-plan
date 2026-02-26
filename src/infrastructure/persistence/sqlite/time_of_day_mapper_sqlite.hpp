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

inline domain::TimeOfDay timeOfDayFromDbString(std::string_view value) {
    if (value == "morning") {
        return domain::TimeOfDay::Morning;
    }
    if (value == "noon") {
        return domain::TimeOfDay::Noon;
    }
    if (value == "evening") {
        return domain::TimeOfDay::Evening;
    }
    if (value == "night") {
        return domain::TimeOfDay::Night;
    }

    throw std::runtime_error("Invalid time_of_day value in database.");
}
} // namespace infrastructure::persistence::sqlite