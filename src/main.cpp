#include "domain/patient.hpp"
#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include "infrastructure/persistence/sqlite/patient_repository_sqlite.hpp"
#include "ui/cli/patient_printer.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    try {
        infrastructure::db::Database db("data/medication.db");

        //infrastructure::db::initDatabase(db, "sql/schema.sql");
        //std::cout << "Datenbank initialisiert\n";

        infrastructure::persistence::sqlite::PatientRepositorySqlite repo(db);

        //domain::Patient p1{1, "Kilian", "01.01.2000", "Deutsch"};

        //repo.createPatient(p1);
        //std::vector<domain::Patient> allPatients;
        auto allPatients = repo.getAllPatients();

        ui::cli::printPatients(allPatients.value());

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
