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

constexpr int title_width = 25;
constexpr int id_width = 8;
constexpr int name_width = 18;
constexpr int strength_width = 14;
constexpr int warnings_width = 25;
constexpr int seperation_line = 64;
} // namespace

namespace ui::cli::printer {

void printMedicationTableHeader() {
    for (int i = 0; i < title_width; i++) {
        std::cout << "=";
    }
    std::cout << " Medications ";
    for (int i = 0; i < title_width; i++) {
        std::cout << "=";
    }
    std::cout << "\n\n";
    std::cout << std::left << std::setw(id_width) << "ID" << std::setw(name_width) << "Name"
              << std::setw(strength_width) << "Strength" << std::setw(warnings_width) << "Warnings"
              << "\n";
    for (int i = 0; i < seperation_line; i++) {
        std::cout << "-";
    }
    std::cout << "\n";
}

void printMedicationRow(const domain::Medication& medication) {
    std::cout << std::left << std::setw(id_width) << medication.id << std::setw(name_width)
              << medication.name << std::setw(strength_width) << medication.strength
              << std::setw(warnings_width) << medication.warnings << "\n";
}

void printMedicationsTable(const std::vector<domain::Medication>& medications) {
    printMedicationTableHeader();

    for (auto medication : medications) {
        printMedicationRow(medication);
    }
}

void printMedicationDetails(const domain::Medication& medication) {
    std::cout << "ID: " << medication.id << "\nName: " << medication.name
              << "\nStrength: " << formatField(medication.strength)
              << "\nWarnings: " << formatField(medication.warnings) << "\n";
}
} // namespace ui::cli::printer