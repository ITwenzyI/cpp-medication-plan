#pragma once
#include <string>

namespace domain {
struct Medication {
    int id;
    std::string name;
    std::string strength;
    std::string warnings;
};
} // namespace domain