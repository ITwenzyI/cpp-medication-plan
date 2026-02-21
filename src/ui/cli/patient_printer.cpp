#include "patient_printer.hpp"
#include <iostream>

namespace ui::cli {
static std::string formatField(const std::string& field) {
    if (field.empty()) {
        return "-";
    }

    return field;
}

void printPatient(const domain::Patient& p) {
    std::cout << "ID: " << p.id << " Name: " << formatField(p.name)
              << " BirthDate: " << formatField(p.birth_date)
              << " Nationality: " << formatField(p.nationality) << "\n";
}

void printPatients(const std::vector<domain::Patient>& patients) {
    for (const domain::Patient& p : patients) {
        printPatient(p);
    }
}
} // namespace ui::cli
