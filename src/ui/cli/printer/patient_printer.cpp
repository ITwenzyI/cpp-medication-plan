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

constexpr int title_width = 25;
constexpr int id_width = 8;
constexpr int name_width = 24;
constexpr int birth_date_width = 16;
constexpr int nationality_width = 12;
constexpr int seperation_line = 60;

} // namespace

namespace ui::cli {

void printPatientTableHeader() {
    for (int i = 0; i < title_width; i++) {
        std::cout << "=";
    }
    std::cout << " Patients ";
    for (int i = 0; i < title_width; i++) {
        std::cout << "=";
    }
    std::cout << "\n\n";
    std::cout << std::left << std::setw(id_width) << "ID" << std::setw(name_width) << "Name"
              << std::setw(birth_date_width) << "BirthDate" << std::setw(nationality_width)
              << "Nationality" << "\n";
    for (int i = 0; i < seperation_line; i++) {
        std::cout << "-";
    }
    std::cout << "\n";
}

void printPatientRow(const domain::Patient& patient) {
    const std::string nationality = patientNationalityToString(patient);

    std::cout << std::left << std::setw(id_width) << patient.id << std::setw(name_width)
              << patient.name << std::setw(birth_date_width) << formatField(patient.birth_date)
              << std::setw(nationality_width) << formatField(nationality) << "\n";
}

void printPatientsTable(const std::vector<domain::Patient>& patients) {
    printPatientTableHeader();
    for (auto patient : patients) {
        printPatientRow(patient);
    }
}

void printPatientDetails(const domain::Patient& patient) {
    const std::string nationality = patientNationalityToString(patient);

    std::cout << "ID: " << patient.id << "\nName: " << formatField(patient.name)
              << "\nBirthDate: " << formatField(patient.birth_date)
              << "\nNationality: " << formatField(nationality) << "\n";
}

} // namespace ui::cli
