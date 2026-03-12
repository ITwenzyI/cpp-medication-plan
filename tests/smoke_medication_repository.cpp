#include "common/result/result.hpp"
#include "domain/medication.hpp"
#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include "infrastructure/persistence/sqlite/medication_repository_sqlite.hpp"
#include <iostream>

static void expect(bool condition, std::string_view message) {
    if (condition) {
        return;
    }
    std::cerr << "MEDICATION SMOKE TEST FAILED: " << message << "\n";
    std::exit(1);
}

int main() {

    infrastructure::db::Database db(":memory:");
    infrastructure::db::initDatabase(db, infrastructure::db::findSchemaPath());

    infrastructure::persistence::sqlite::MedicationRepositorySqlite medication_repository(db);

    domain::Medication seed_medication{0, "Ibuprofen", "400mg",
        "Do not use if you have stomach ulcers or severe kidney problems."};

    // ======= CREATE Medication ======

    // Create Medication
    auto created_medication = medication_repository.createMedication(seed_medication);
    expect(created_medication.isOk(), "created medication should succeed");
    expect(created_medication.value().id > 0, "created medication should have generated id");

    // ======= GET ALL PATIENTS ======

    // Get All Medications
    auto all_medications = medication_repository.getAllMedications();
    expect(!all_medications.value().empty(),
        "getAllMedications should return at least one medication after create");
    expect(all_medications.value().size() == 1, "expected exactly one medication in empty test database");
    expect(all_medications.value()[0].id == created_medication.value().id,
        "expected stored medication id to equal created.id");
    expect(all_medications.value()[0].name == "Ibuprofen", "stored name should match");

    // ======= FIND MEDICATION BY ID ======

    // Find Medication by ID
    auto found_medication = medication_repository.findMedicationById(created_medication.value().id);
    expect(found_medication.isOk(), "findMedicationById should succeed after creation");
    expect(found_medication.value().name == created_medication.value().name, "name should match");
    expect(found_medication.value().strength == created_medication.value().strength, "strength should match");
    expect(found_medication.value().warnings == created_medication.value().warnings, "warnings should match");

    // ======= UPDATE MEDICATION NAME ======

    // Update Medication Name
    auto update_name_result = medication_repository.updateMedicationName(created_medication.value().id, "Metformin");
    expect(update_name_result.isOk(), "updateMedicationName should succeed");

    // Find Medication by ID after Update Medication Name
    auto medication_after_name_update =
        medication_repository.findMedicationById(created_medication.value().id);
    expect(medication_after_name_update.isOk(), "find after update should succeed");
    expect(medication_after_name_update.value().name == "Metformin", "name should be updated");

    // Update Medication Name with same Name
    auto update_name_same_value_result =
        medication_repository.updateMedicationName(created_medication.value().id, "Metformin");
    expect(update_name_same_value_result.isOk(), "updateMedicationName with same value should still succeed");

    // Update Medication Name with invalid ID=0
    auto update_name_invalid_id_result = medication_repository.updateMedicationName(0, "X");
    expect(update_name_invalid_id_result.isError(), "update with invalid id should fail");
    expect(update_name_invalid_id_result.error().code == common::result::ErrorCode::InvalidArgument,
        "invalid id should return InvalidArgument");

    // ======= UPDATE MEDICATION STRENGTH ======

    // Update Medication Strength
    auto update_strength_result =
        medication_repository.updateMedicationStrength(created_medication.value().id, "600mg");
    expect(update_strength_result.isOk(), "updateMedicationtStrength should succeed");

    // Find Medication by ID after Update Medication Strength
    auto medication_after_strength_update =
        medication_repository.findMedicationById(created_medication.value().id);
    expect(medication_after_strength_update.isOk(), "find after update should succeed");
    expect(medication_after_strength_update.value().strength == "600mg", "strength should be updated");

    // ======= UPDATE MEDICATION WARNINGS ======

    // Update Medication Warnings
    auto update_warnings_result = medication_repository.updateMedicationWarnings(
        created_medication.value().id, "Do not use in cases of severe renal insufficiency.");
    expect(update_warnings_result.isOk(), "updateMedicationWarning should succeed");

    // Find Medication by ID after Update Medication Warnings
    auto medication_after_warnings_update =
        medication_repository.findMedicationById(created_medication.value().id);
    expect(medication_after_warnings_update.isOk(), "find after update should succeed");
    expect(
        medication_after_warnings_update.value().warnings == "Do not use in cases of severe renal insufficiency.",
        "warnings should be updated");

    // ======= DELETE MEDICATION ======

    auto delete_medication_result = medication_repository.deleteMedicationById(created_medication.value().id);
    expect(delete_medication_result.isOk(), "deleteMedicationById should succeed");

    auto find_after_delete_result = medication_repository.findMedicationById(created_medication.value().id);
    expect(find_after_delete_result.isError(), "find after delete should fail");
    expect(find_after_delete_result.error().code == common::result::ErrorCode::NotFound,
        "find after delete should return NotFound");

    auto delete_missing_medication_result =
        medication_repository.deleteMedicationById(created_medication.value().id);
    expect(delete_missing_medication_result.isError(), "delete missing medication should fail");
    expect(delete_missing_medication_result.error().code == common::result::ErrorCode::NotFound,
        "delete missing medication should return NotFound");

    auto delete_invalid_id_result = medication_repository.deleteMedicationById(0);
    expect(delete_invalid_id_result.isError(), "update with invalid id should fail");
    expect(delete_invalid_id_result.error().code == common::result::ErrorCode::InvalidArgument,
        "invalid id should return InvalidArgument");

    std::cout << "MEDICATION SMOKE TEST PASSED\n";

    return 0;
}
