#include "application/repository/patient_repository.hpp"
#include "domain/patient.hpp"
#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include "ui/cli/patient_printer.hpp"

#include <iostream>
#include <vector>

int main() {
    try {
        Database db("data/medication.db");

        //initDatabase(db, "sql/schema.sql");
        //std::cout << "Datenbank initialisiert\n";

        PatientRepository repo(db);

        //Patient p1{1, "Kilian", "01.01.2000", "Deutsch"};

        //repo.createPatient(p1);
        std::vector<Patient> allPatients;
        allPatients = repo.getAllPatients();

        printPatients(allPatients);

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
