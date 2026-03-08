#pragma once
#include "domain/patient.hpp"
#include <vector>

namespace ui::cli {

void printPatientTableHeader();

void printPatientRow(const domain::Patient& patient);

void printPatientsTable(const std::vector<domain::Patient>& patients);

void printPatientDetails(const domain::Patient& patient);
} // namespace ui::cli