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

        domain::Patient p1{1, "James Walker", "1994-03-18", domain::Nationality::GB};
        domain::Patient p2{2, "Olivia Smith", "2000-09-05", domain::Nationality::US};
        domain::Patient p3{2, "Daniel Brown", "1987-12-27", domain::Nationality::CN};
        domain::Patient p4{2, "Emily Johnson", "2001-11-23"};

        // auto p1_created = repo.createPatient(p1);
        // auto p2_created = repo.createPatient(p2);
        // auto p3_created = repo.createPatient(p3);
        // auto p4_created = repo.createPatient(p4);
        auto allPatients = repo.getAllPatients();

        // ui::cli::printPatientTableHeader();
        // ui::cli::printPatientRow(p1);
        // ui::cli::printPatientRow(p2);
        ui::cli::printPatientsTable(allPatients.value());

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
