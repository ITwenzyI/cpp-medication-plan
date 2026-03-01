#include "patient_printer.hpp"
#include "infrastructure/persistence/sqlite/nationality_mapper_sqlite.hpp"
#include <iostream>

namespace ui::cli {
static std::string formatField(const std::string& field) {
    if (field.empty()) {
        return "-";
    }

    return field;
}

void printPatient(const domain::Patient& p) {
    const std::string nationality = p.nationality.has_value()
        ? infrastructure::persistence::sqlite::nationalityToDbString(p.nationality.value())
        : "";

    std::cout << "ID: " << p.id << " Name: " << formatField(p.name)
              << " BirthDate: " << formatField(p.birth_date)
              << " Nationality: " << formatField(nationality) << "\n";
}

void printPatients(const std::vector<domain::Patient>& patients) {
    for (const domain::Patient& p : patients) {
        printPatient(p);
    }
}
} // namespace ui::cli
