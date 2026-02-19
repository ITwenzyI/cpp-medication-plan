#include "patient_printer.hpp"
#include <iostream>

static std::string formatField(const std::string& field) {
    if (field.empty()) {
        return "-";
    }

    return field;
}

void printPatient(const Patient& p) {
    std::cout << "ID: " << p.id << " Name: " << formatField(p.name)
              << " BirthDate: " << formatField(p.birth_date)
              << " Nationality: " << formatField(p.nationality) << "\n";
}

void printPatients(const std::vector<Patient>& patients) {
    for (const Patient& p : patients) {
        printPatient(p);
    }
}