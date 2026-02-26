#pragma once
#include <string>

namespace domain {

enum class TimeOfDay { Morning, Noon, Evening, Night };

struct IntakePlan {
    int id;
    int patientId;
    int medicationId;
    std::string dose;
    TimeOfDay timeOfDay;
    std::string notes;
};

} // namespace domain