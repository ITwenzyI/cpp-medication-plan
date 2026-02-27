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
    expect(created_patient.isOk(), "created intake_plan should succeed");
    expect(created_patient.value().id > 0, "created patient should have generated id");

    // Create Medication
    auto created_med = repo_med.createMedication(m1);
    expect(created_med.isOk(), "created medication should succeed");
    expect(created_med.value().id > 0, "created medication should have generated id");

    // ======= CREATE INTAKE_PLAN ======
    domain::IntakePlan plan1{0, created_patient.value().id, created_med.value().id, "400mg",
        domain::TimeOfDay::Noon, "Take with food!"};
    auto created_plan = repo_plan.createIntakePlan(plan1);
    expect(created_plan.isOk(), "created intake_plan should succeed");
    expect(created_plan.value().id > 0, "created intake_plan should have generated id");

    // ======= GET INTAKE_PLAN BY PATIENT_ID ======
    auto plans_by_patient_id = repo_plan.getIntakePlansByPatientId(created_patient.value().id);
    expect(plans_by_patient_id.isOk(), "get intake_plans by patient_id should succeed");
    expect(plans_by_patient_id.value().size() == 1,
        "expected exactly one intake_plan in test database");
    expect(plans_by_patient_id.value()[0].id == created_plan.value().id, "id should match");
    expect(plans_by_patient_id.value()[0].patientId == created_plan.value().patientId,
        "patient_id should match");
    expect(plans_by_patient_id.value()[0].medicationId == created_plan.value().medicationId,
        "medication_id should match");

    expect(plans_by_patient_id.value()[0].dose == created_plan.value().dose,
        "dose should match with created");
    expect(plans_by_patient_id.value()[0].dose == "400mg", "dose should match with string");

    expect(plans_by_patient_id.value()[0].timeOfDay == created_plan.value().timeOfDay,
        "time_of_day should match with created");
    expect(plans_by_patient_id.value()[0].timeOfDay == domain::TimeOfDay::Noon,
        "time_of_day should match with string");

    expect(plans_by_patient_id.value()[0].notes == created_plan.value().notes,
        "notes should match with created");
    expect(plans_by_patient_id.value()[0].notes == "Take with food!",
        "notes should match with string");

    std::cout << "INTAKE_PLAN SMOKE TEST PASSED\n";

    return 0;
}