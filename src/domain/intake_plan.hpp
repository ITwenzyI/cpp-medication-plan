#pragma once
#include <string>

namespace domain {

enum class TimeOfDay { Morning, Noon, Evening, Night };

struct IntakePlan {
    int id;
    int patient_id;
    int medication_id;
    std::string dose;
    TimeOfDay time_of_day;
    std::string notes;
};

} // namespace domain