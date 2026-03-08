#include "medication_printer.hpp"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace {
std::string formatField(const std::string& field) {
    if (field.empty()) {
        return "-";
    }

    return field;
}

constexpr int titleWidth = 25;
constexpr int idWidth = 8;
constexpr int nameWidth = 18;
constexpr int strengthWidth = 14;
constexpr int warningsWidth = 25;
constexpr int seperation_line = 60;
} // namespace

namespace ui::cli::printer {

void printMedicationTableHeader() {
}

void printMedicationRow(const domain::Medication& medication) {
}

void printMedicationsTable(const std::vector<domain::Medication>& medications) {
}

void printMedicationDetails(const domain::Medication& medication) {
    std::cout << "ID: " << medication.id << "\nName: " << medication.name
              << "\nStrength: " << formatField(medication.strength)
              << "\nWarnings: " << formatField(medication.warnings) << "\n";
}
} // namespace ui::cli::printer