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
constexpr int nameWidth = 15;
constexpr int strengthWidth = 14;
constexpr int warningsWidth = 25;
constexpr int seperation_line = 64;
} // namespace

namespace ui::cli::printer {

void printMedicationTableHeader() {
    for (int i = 0; i < titleWidth; i++) {
        std::cout << "=";
    }
    std::cout << " Medications ";
    for (int i = 0; i < titleWidth; i++) {
        std::cout << "=";
    }
    std::cout << "\n\n";
    std::cout << std::left << std::setw(idWidth) << "ID" << std::setw(nameWidth) << "Name"
              << std::setw(strengthWidth) << "Strength" << std::setw(warningsWidth) << "Warnings"
              << "\n";
    for (int i = 0; i < seperation_line; i++) {
        std::cout << "-";
    }
    std::cout << "\n";
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