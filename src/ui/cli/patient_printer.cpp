#include "patient_printer.hpp"
#include "infrastructure/persistence/sqlite/nationality_mapper_sqlite.hpp"
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

std::string patientNationalityToString(const domain::Patient& patient) {
    if (patient.nationality.has_value()) {
        return infrastructure::persistence::sqlite::nationalityToDbString(
            patient.nationality.value());
    } else {
        return "";
    }
}

constexpr int titleWidth = 25;
constexpr int idWidth = 6;
constexpr int nameWidth = 20;
constexpr int birthDateWidth = 14;
constexpr int nationalityWidth = 12;
constexpr int seperation_line = 60;

} // namespace

namespace ui::cli {

void printPatientTableHeader() {
    for (int i = 0; i < titleWidth; i++) {
        std::cout << "=";
    }
    std::cout << " Patients ";
    for (int i = 0; i < titleWidth; i++) {
        std::cout << "=";
    }
    std::cout << "\n\n";
    std::cout << std::left << std::setw(idWidth) << "ID" << std::setw(nameWidth) << "Name"
              << std::setw(birthDateWidth) << "BirthDate" << std::setw(nationalityWidth)
              << "Nationality" << "\n";
    for (int i = 0; i < seperation_line; i++) {
        std::cout << "-";
    }
    std::cout << "\n";
}

void printPatientRow(const domain::Patient& patient) {
}

void printPatientsTable(const std::vector<domain::Patient>& patients) {
}

void printPatientDetails(const domain::Patient& patient) {
    const std::string nationality = patientNationalityToString(patient);

    std::cout << "ID: " << patient.id << "\nName: " << formatField(patient.name)
              << "\nBirthDate: " << formatField(patient.birth_date)
              << "\nNationality: " << formatField(nationality) << "\n";
}

} // namespace ui::cli
