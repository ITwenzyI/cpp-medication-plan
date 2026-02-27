#include "common/result/result.hpp"
#include "domain/intake_plan.hpp"
#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include "infrastructure/persistence/sqlite/intake_plan_repository_sqlite.hpp"
#include "infrastructure/persistence/sqlite/medication_repository_sqlite.hpp"
#include "infrastructure/persistence/sqlite/patient_repository_sqlite.hpp"
#include <iostream>
#include <string_view>

static void expect(bool condition, std::string_view message) {
    if (condition) {
        return;
    }
    std::cerr << "INTAKE_PLAN SMOKE TEST FAILED: " << message << "\n";
    std::exit(1);
}

int main() {

    infrastructure::db::Database db(":memory:");
    infrastructure::db::initDatabase(db, "sql/schema.sql");

    infrastructure::persistence::sqlite::PatientRepositorySqlite repo_patient(db);
    infrastructure::persistence::sqlite::MedicationRepositorySqlite repo_med(db);
    infrastructure::persistence::sqlite::IntakePlanRepositorySqlite repo_plan(db);

    domain::Patient p1{0, "Steve Moro", "1998-02-07", "DE"};
    domain::Medication m1{0, "Ibuprofen", "400mg",
        "Do not use if you have stomach ulcers or severe kidney problems."};

    // ======= CREATE PATIENT AND MEDICATION ======

    // Create Patient
    auto created_patient = repo_patient.createPatient(p1);
    expect(created_patient.value().id > 0, "created patient should have generated id");

    // Create Medication
    auto created_med = repo_med.createMedication(m1);
    expect(created_med.value().id > 0, "created medication should have generated id");

    // ======= CREATE INTAKE_PLAN ======
    domain::IntakePlan plan1{0, created_patient.value().id, created_med.value().id, "400mg",
        domain::TimeOfDay::Noon, "Take with food!"};
    auto created_plan = repo_plan.createIntakePlan(plan1);
    expect(created_plan.value().id > 0, "created intake_plan should have generated id");

    std::cout << "INTAKE_PLAN SMOKE TEST PASSED\n";

    return 0;
}