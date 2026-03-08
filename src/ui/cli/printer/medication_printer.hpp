#pragma once
#include "domain/medication.hpp"
#include <vector>

namespace ui::cli::printer {
void printMedicationTableHeader();

void printMedicationRow(const domain::Medication& medication);

void printMedicationsTable(const std::vector<domain::Medication>& medications);

void printMedicationDetails(const domain::Medication& medication);
}