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
    infrastructure::db::initDatabase(db, "sql/schema.sql");

    infrastructure::persistence::sqlite::MedicationRepositorySqlite repo(db);

    domain::Medication m1{0, "Ibuprofen", "400mg",
        "Do not use if you have stomach ulcers or severe kidney problems."};

    // ======= CREATE Medication ======

    // Create Medication
    auto created = repo.createMedication(m1);
    expect(created.isOk(), "created medication should succeed");
    expect(created.value().id > 0, "created medication should have generated id");

    // ======= GET ALL PATIENTS ======

    // Get All Medications
    auto all = repo.getAllMedications();
    expect(!all.value().empty(),
        "getAllMedications should return at least one medication after create");
    expect(all.value().size() == 1, "expected exactly one medication in empty test database");
    expect(all.value()[0].id == created.value().id,
        "expected stored medication id to equal created.id");
    expect(all.value()[0].name == "Ibuprofen", "stored name should match");

    // ======= FIND MEDICATION BY ID ======

    // Find Medication by ID
    auto found = repo.findMedicationById(created.value().id);
    expect(found.isOk(), "findMedicationById should succeed after creation");
    expect(found.value().name == created.value().name, "name should match");
    expect(found.value().strength == created.value().strength, "strength should match");
    expect(found.value().warnings == created.value().warnings, "warnings should match");

    // ======= UPDATE MEDICATION NAME ======

    // Update Medication Name
    auto update = repo.updateMedicationName(created.value().id, "Metformin");
    expect(update.isOk(), "updateMedicationName should succeed");

    // Find Medication by ID after Update Medication Name
    auto updated = repo.findMedicationById(created.value().id);
    expect(updated.isOk(), "find after update should succeed");
    expect(updated.value().name == "Metformin", "name should be updated");

    // Update Medication Name with same Name
    auto upd_same = repo.updateMedicationName(created.value().id, "Metformin");
    expect(upd_same.isOk(), "updateMedicationName with same value should still succeed");

    // Update Medication Name with invalid ID=0
    auto upd_bad = repo.updateMedicationName(0, "X");
    expect(upd_bad.isError(), "update with invalid id should fail");
    expect(upd_bad.error().code == common::result::ErrorCode::InvalidArgument,
        "invalid id should return InvalidArgument");

    // ======= UPDATE MEDICATION STRENGTH ======

    // Update Medication Strength
    auto update_strength = repo.updateMedicationStrength(created.value().id, "600mg");
    expect(update_strength.isOk(), "updateMedicationtStrength should succeed");

    // Find Medication by ID after Update Medication Strength
    auto updated_strength = repo.findMedicationById(created.value().id);
    expect(updated_strength.isOk(), "find after update should succeed");
    expect(updated_strength.value().strength == "600mg", "strength should be updated");

    // ======= UPDATE MEDICATION WARNINGS ======

    // Update Medication Warnings
    auto update_warning = repo.updateMedicationWarnings(
        created.value().id, "Do not use in cases of severe renal insufficiency.");
    expect(update_warning.isOk(), "updateMedicationWarning should succeed");

    // Find Medication by ID after Update Medication Warnings
    auto updated_warnings = repo.findMedicationById(created.value().id);
    expect(updated_warnings.isOk(), "find after update should succeed");
    expect(
        updated_warnings.value().warnings == "Do not use in cases of severe renal insufficiency.",
        "warnings should be updated");

    // ======= DELETE MEDICATION ======

    auto delete1 = repo.deleteMedicationById(created.value().id);
    expect(delete1.isOk(), "deleteMedicationById should succeed");

    auto after_delete = repo.findMedicationById(created.value().id);
    expect(after_delete.isError(), "find after delete should fail");
    expect(after_delete.error().code == common::result::ErrorCode::NotFound,
        "find after delete should return NotFound");

    auto delete2 = repo.deleteMedicationById(created.value().id);
    expect(delete2.isError(), "delete missing medication should fail");
    expect(delete2.error().code == common::result::ErrorCode::NotFound,
        "delete missing medication should return NotFound");

    auto delete_bad = repo.deleteMedicationById(0);
    expect(delete_bad.isError(), "update with invalid id should fail");
    expect(delete_bad.error().code == common::result::ErrorCode::InvalidArgument,
        "invalid id should return InvalidArgument");

    std::cout << "MEDICATION SMOKE TEST PASSED\n";

    return 0;
}