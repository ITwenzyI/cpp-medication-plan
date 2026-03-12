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
    infrastructure::db::initDatabase(db, infrastructure::db::findSchemaPath());

    infrastructure::persistence::sqlite::PatientRepositorySqlite repo_patient(db);
    infrastructure::persistence::sqlite::MedicationRepositorySqlite repo_med(db);
    infrastructure::persistence::sqlite::IntakePlanRepositorySqlite repo_plan(db);

    domain::Patient primary_patient{0, "Steve Moro", "1998-02-07", domain::Nationality::DE};
    domain::Medication primary_medication{0, "Ibuprofen", "400mg",
        "Do not use if you have stomach ulcers or severe kidney problems."};

    // ======= CREATE PATIENT AND MEDICATION ======

    // Create Patient
    auto created_patient = repo_patient.createPatient(primary_patient);
    expect(created_patient.isOk(), "created patient should succeed");
    expect(created_patient.value().id > 0, "created patient should have generated id");

    // Create Medication
    auto created_medication = repo_med.createMedication(primary_medication);
    expect(created_medication.isOk(), "created medication should succeed");
    expect(created_medication.value().id > 0, "created medication should have generated id");

    // ======= CREATE INTAKE_PLAN ======
    domain::IntakePlan primary_intake_plan{0, created_patient.value().id,
        created_medication.value().id, "400mg", domain::TimeOfDay::Noon, "Take with food!"};
    auto created_intake_plan = repo_plan.createIntakePlan(primary_intake_plan);
    expect(created_intake_plan.isOk(), "created intake_plan should succeed");
    expect(created_intake_plan.value().id > 0, "created intake_plan should have generated id");

    // ======= GET INTAKE_PLAN BY PATIENT_ID ======
    auto plans_by_patient_id = repo_plan.getIntakePlansByPatientId(created_patient.value().id);
    expect(plans_by_patient_id.isOk(), "get intake_plans by patient_id should succeed");
    expect(plans_by_patient_id.value().size() == 1,
        "expected exactly one intake_plan in test database");
    expect(plans_by_patient_id.value()[0].id == created_intake_plan.value().id, "id should match");
    expect(plans_by_patient_id.value()[0].patient_id == created_intake_plan.value().patient_id,
        "patient_id should match");
    expect(
        plans_by_patient_id.value()[0].medication_id == created_intake_plan.value().medication_id,
        "medication_id should match");

    expect(plans_by_patient_id.value()[0].dose == created_intake_plan.value().dose,
        "dose should match with created");
    expect(plans_by_patient_id.value()[0].dose == "400mg", "dose should match with string");

    expect(plans_by_patient_id.value()[0].time_of_day == created_intake_plan.value().time_of_day,
        "time_of_day should match with created");
    expect(plans_by_patient_id.value()[0].time_of_day == domain::TimeOfDay::Noon,
        "time_of_day should match with string");

    expect(plans_by_patient_id.value()[0].notes == created_intake_plan.value().notes,
        "notes should match with created");
    expect(plans_by_patient_id.value()[0].notes == "Take with food!",
        "notes should match with string");

    // ======= TEST UNIQUE CONSTRAINT ======
    auto duplicate_create_result = repo_plan.createIntakePlan(primary_intake_plan);
    expect(duplicate_create_result.isError(), "create with the same intake_plan should fail");
    expect(duplicate_create_result.error().code == common::result::ErrorCode::Conflict,
        "same intake_plan should return conflict");

    // ======= TEST FOREIGN KEY CONSTRAINT ======
    domain::IntakePlan invalid_fk_intake_plan = primary_intake_plan;
    invalid_fk_intake_plan.patient_id = 999; // Invalid PatientID
    auto created_invalid_fk_plan = repo_plan.createIntakePlan(invalid_fk_intake_plan);
    expect(created_invalid_fk_plan.isError(), "create with invalid patient_id should fail");
    expect(created_invalid_fk_plan.error().code == common::result::ErrorCode::ForeignKeyViolation,
        "same intake_plan should return ForeignKeyViolation");

    // ======= TEST NOTES NULL MAPPING ======
    domain::IntakePlan no_notes_intake_plan{0, created_patient.value().id,
        created_medication.value().id, "600mg", domain::TimeOfDay::Morning};

    auto created_no_notes_plan = repo_plan.createIntakePlan(no_notes_intake_plan);
    expect(created_no_notes_plan.isOk(), "created intake_plan2 should succeed");
    expect(created_no_notes_plan.value().id > 0, "created intake_plan2 should have generated id");

    plans_by_patient_id = repo_plan.getIntakePlansByPatientId(created_patient.value().id);
    expect(plans_by_patient_id.isOk(), "get intake_plans2 by patient_id should succeed");
    expect(plans_by_patient_id.value().size() == 2, "expected two intake_plans in test database");

    // finds the correct intake_plan of the two existing in database
    for (auto no_notes_plan : plans_by_patient_id.value()) {
        if (no_notes_plan.id == created_no_notes_plan.value().id) {
            expect(no_notes_plan.notes == "", "notes should be empty");
        }
    }

    // ======= GET INTAKE_PLAN BY MEDICATION_ID ======
    domain::Patient secondary_patient{0, "Kilian Cpp", "2000-12-12", domain::Nationality::DE};
    auto created_secondary_patient = repo_patient.createPatient(secondary_patient);
    expect(created_secondary_patient.isOk(), "create new patient should succeed.");
    domain::Medication secondary_medication{
        0, "Paracetamol", "500 mg", "Do not exceed 4,000 mg in 24 hours"};

    auto created_secondary_medication = repo_med.createMedication(secondary_medication);
    expect(created_secondary_medication.isOk(), "create new medication should succeed");

    domain::IntakePlan secondary_intake_plan{0, created_secondary_patient.value().id,
        created_secondary_medication.value().id, "500 mg", domain::TimeOfDay::Night};
    auto created_secondary_plan = repo_plan.createIntakePlan(secondary_intake_plan);
    expect(created_secondary_plan.isOk(), "create new plan should succeed");

    auto plans_by_secondary_medication_id =
        repo_plan.getIntakePlansByMedicationId(created_secondary_medication.value().id);
    expect(plans_by_secondary_medication_id.isOk(),
        "get new_intake_plans by medication_id should succeed");
    expect(plans_by_secondary_medication_id.value().size() == 1,
        "expected one intake_plan in test database with this new medication_id");
    expect(plans_by_secondary_medication_id.value()[0].id == created_secondary_plan.value().id,
        "id should match");
    expect(plans_by_secondary_medication_id.value()[0].patient_id ==
            created_secondary_plan.value().patient_id,
        "patient_id should match");
    expect(plans_by_secondary_medication_id.value()[0].medication_id ==
            created_secondary_plan.value().medication_id,
        "medication_id should match");

    // ======= DELETE INTAKE_PLAN BY ID ======
    auto delete_secondary_plan_result =
        repo_plan.deleteIntakePlanById(created_secondary_plan.value().id);
    expect(delete_secondary_plan_result.isOk(), "delete new intake_plan should succeed");
    auto plans_by_secondary_medication_id_after_delete =
        repo_plan.getIntakePlansByMedicationId(created_secondary_medication.value().id);
    expect(plans_by_secondary_medication_id_after_delete.isOk(),
        "get intake_plans by medication_id after delete should succeed");
    expect(plans_by_secondary_medication_id_after_delete.value().size() == 0,
        "expected zero intake_plans in test database with this medication_id after delete");

    // ======= UPDATE INTAKE_PLAN ======
    domain::Patient update_patient{0, "Before Update", "1999-01-01", domain::Nationality::FR};
    auto created_update_patient = repo_patient.createPatient(update_patient);
    expect(created_update_patient.isOk(), "create patient before update should succeed.");

    domain::Medication update_medication{
        0, "Paracetamol", "1000 mg", "Do not exceed 4,000 mg in 24 hours"};
    auto created_update_medication = repo_med.createMedication(update_medication);
    expect(created_update_medication.isOk(), "create medication before update should succeed");

    domain::IntakePlan intake_plan_before_update{0, created_update_patient.value().id,
        created_update_medication.value().id, "1000 mg", domain::TimeOfDay::Morning};
    auto created_plan_before_update = repo_plan.createIntakePlan(intake_plan_before_update);
    expect(created_plan_before_update.isOk(), "create intake_plan before update should succeed");

    auto plans_by_medication_id_before_update =
        repo_plan.getIntakePlansByMedicationId(created_update_medication.value().id);
    expect(plans_by_medication_id_before_update.isOk(),
        "get intake_plans before update by medication_id should succeed");
    expect(plans_by_medication_id_before_update.value().size() == 1,
        "expected one intake_plan in test database with the update medication_id");
    auto updated_plan = plans_by_medication_id_before_update.value()[0];
    expect(updated_plan.id == created_plan_before_update.value().id,
        "fetched intake_plan before update should match created plan");
    updated_plan.dose = "1mg";
    updated_plan.time_of_day = domain::TimeOfDay::Night;

    auto update_intake_plan_result = repo_plan.updateIntakePlan(updated_plan);
    expect(update_intake_plan_result.isOk(), "update intake_plan should succeed");
    auto plans_by_medication_id_after_update =
        repo_plan.getIntakePlansByMedicationId(created_update_medication.value().id);
    expect(plans_by_medication_id_after_update.isOk(),
        "get intake_plans after update by medication_id should succeed");
    expect(plans_by_medication_id_after_update.value().size() == 1,
        "expected one intake_plan after update for the update medication_id");
    expect(updated_plan.dose == "1mg", "dose should match after update");
    expect(updated_plan.dose == plans_by_medication_id_after_update.value()[0].dose,
        "dose should match after update");
    expect(updated_plan.time_of_day == domain::TimeOfDay::Night,
        "time_of_day should match after update");
    expect(updated_plan.time_of_day == plans_by_medication_id_after_update.value()[0].time_of_day,
        "time_of_day should match after update");

    // ======= TEST NOTFOUND UPDATE ======
    updated_plan.id = 999999;
    auto updated_not_found_plan = repo_plan.updateIntakePlan(updated_plan);
    expect(updated_not_found_plan.isError(), "update intake_plan with invalid id should fail");
    expect(updated_not_found_plan.error().code == common::result::ErrorCode::NotFound,
        "update with invalid id should return NotFound");

    // ======= TEST UNIQUE CONFLICT UPDATE ======
    domain::Patient unique_conflict_patient{
        0, "Unique Conflict", "1999-01-01", domain::Nationality::FR};
    auto created_unique_conflict_patient = repo_patient.createPatient(unique_conflict_patient);
    expect(created_unique_conflict_patient.isOk(), "create patient should succeed.");

    domain::Medication unique_conflict_medication{
        0, "Paracetamol", "1000 mg", "Do not exceed 4,000 mg in 24 hours"};
    auto created_unique_conflict_medication = repo_med.createMedication(secondary_medication);
    expect(created_unique_conflict_medication.isOk(), "create medication should succeed");

    // Plan 1 with Morning
    domain::IntakePlan unique_conflict_plan_morning{0, created_unique_conflict_patient.value().id,
        created_unique_conflict_medication.value().id, "1000 mg", domain::TimeOfDay::Morning};
    auto created_unique_conflict_plan_morning =
        repo_plan.createIntakePlan(unique_conflict_plan_morning);
    expect(created_unique_conflict_plan_morning.isOk(), "create intake_plan should succeed");
    // Plan 2 with Noon
    domain::IntakePlan unique_conflict_plan_noon{0, created_unique_conflict_patient.value().id,
        created_unique_conflict_medication.value().id, "1000 mg", domain::TimeOfDay::Noon};
    auto created_unique_conflict_plan_noon = repo_plan.createIntakePlan(unique_conflict_plan_noon);
    expect(created_unique_conflict_plan_noon.isOk(), "create intake_plan should succeed");

    // Update Plan 2 to Morning
    auto unique_conflict_plan_after_create = created_unique_conflict_plan_noon.value();
    unique_conflict_plan_after_create.time_of_day = domain::TimeOfDay::Morning;
    auto update_unique_conflict_result =
        repo_plan.updateIntakePlan(unique_conflict_plan_after_create);
    expect(update_unique_conflict_result.isError(),
        "update plan with plan already existing should fail");
    expect(update_unique_conflict_result.error().code == common::result::ErrorCode::Conflict,
        "update with same plan should return Conflict");

    // ======= TEST FOREIGN KEY VIOLATION UPDATE ======
    domain::IntakePlan fk_valid_intake_plan{0, created_unique_conflict_patient.value().id,
        created_unique_conflict_medication.value().id, "90 mg", domain::TimeOfDay::Evening};
    auto created_fk_valid_plan = repo_plan.createIntakePlan(fk_valid_intake_plan);
    expect(created_fk_valid_plan.isOk(), "create intake_plan should succeed");
    auto fk_invalid_plan = created_fk_valid_plan.value();
    fk_invalid_plan.patient_id = 9999999;
    auto update_fk_invalid_result = repo_plan.updateIntakePlan(fk_invalid_plan);
    expect(update_fk_invalid_result.isError(), "update plan with invalid patient_id should fail");
    expect(update_fk_invalid_result.error().code == common::result::ErrorCode::ForeignKeyViolation,
        "update with invalid patient_id should return ForeignKeyViolation");

    // ========================================
    // ========================================
    // ======= TEST CASCADE DELETE ======
    auto delete_patient1 = repo_patient.deletePatientById(created_patient.value().id);
    auto intake_plans_after_delete =
        repo_plan.getIntakePlansByPatientId(created_patient.value().id);
    expect(intake_plans_after_delete.isOk(),
        "get intake_plans_after_delete by patient_id should succeed");
    expect(intake_plans_after_delete.value().size() == 0,
        "expected zero intake_plans in test database after delete of patient");

    std::cout << "INTAKE_PLAN SMOKE TEST PASSED\n";

    return 0;
}
