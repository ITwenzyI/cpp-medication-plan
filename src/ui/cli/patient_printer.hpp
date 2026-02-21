#pragma once
#include "domain/patient.hpp"
#include <vector>

namespace ui::cli {
void printPatient(const domain::Patient& p);

void printPatients(const std::vector<domain::Patient>& patients);
} // namespace ui::cli
