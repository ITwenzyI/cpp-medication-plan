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
        //std::cout << "Database initalized\n";

        infrastructure::persistence::sqlite::PatientRepositorySqlite repo(db);

        //domain::Patient p1{1, "Kilian Cpp", "2000-01-01", "DE"};
        //domain::Patient p2{2, "Test Patient", "1998-12-24", "ES"};

        //auto all_patients = repo.createPatient(p1);
        auto allPatients = repo.getAllPatients();

        ui::cli::printPatients(allPatients.value());

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
