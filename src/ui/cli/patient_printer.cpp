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

constexpr int idWidth = 2;
constexpr int nameWidth = 12;
constexpr int birthDateWidth = 10;
constexpr int nationalityWidth = 5;

} // namespace

namespace ui::cli {

void printPatientTableHeader() {
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
