#include "domain/patient.hpp"
#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include "infrastructure/persistence/sqlite/intake_plan_repository_sqlite.hpp"
#include "infrastructure/persistence/sqlite/medication_repository_sqlite.hpp"
#include "infrastructure/persistence/sqlite/patient_repository_sqlite.hpp"
#include "ui/cli/printer/intake_plan_printer.hpp"
#include "ui/cli/printer/medication_printer.hpp"
#include "ui/cli/printer/patient_printer.hpp"

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

        // ui::cli::printer::printPatientTableHeader();
        // ui::cli::printer::printPatientRow(p1);
        // ui::cli::printer::printPatientRow(p2);
        // ui::cli::printer::printPatientsTable(allPatients.value());

        infrastructure::persistence::sqlite::MedicationRepositorySqlite medication_repository(db);

        domain::Medication seed_medication{0, "Ibuprofen", "400mg",
            "Do not use if you have stomach ulcers or severe kidney problems."};
        domain::Medication secondary_medication{
            0, "Paracetamol", "500 mg", "Do not exceed 4,000 mg in 24 hours"};

        // auto m1_created = medication_repository.createMedication(seed_medication);
        // auto m2_created = medication_repository.createMedication(secondary_medication);

        auto allMedications = medication_repository.getAllMedications();

        // ui::cli::printer::printMedicationDetails(seed_medication);
        // ui::cli::printer::printMedicationTableHeader();
        // ui::cli::printer::printMedicationRow(seed_medication);
        // ui::cli::printer::printMedicationRow(secondary_medication);
        // ui::cli::printer::printMedicationsTable(allMedications.value());

        // ui::cli::printer::printIntakePlanTableHeader();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
