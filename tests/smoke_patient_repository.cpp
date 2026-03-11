#include "common/result/result.hpp"
#include "domain/patient.hpp"
#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include "infrastructure/persistence/sqlite/patient_repository_sqlite.hpp"
#include <iostream>

static void expect(bool condition, std::string_view message) {
    if (condition) {
        return;
    }
    std::cerr << "PATIENT SMOKE TEST FAILED: " << message << "\n";
    std::exit(1);
}

int main() {

    infrastructure::db::Database db(":memory:");
    infrastructure::db::initDatabase(db, "sql/schema.sql");

    infrastructure::persistence::sqlite::PatientRepositorySqlite patient_repository(db);

    domain::Patient seed_patient{0, "Steve Moro", "1998-02-07", domain::Nationality::DE};

    // ======= CREATE PATIENT ======

    // Create Patient
    auto created_patient = patient_repository.createPatient(seed_patient);
    expect(created_patient.isOk(), "created patient should succeed");
    expect(created_patient.value().id > 0, "created patient should have generated id");

    // ======= GET ALL PATIENTS ======

    // Get All Patients
    auto all_patients = patient_repository.getAllPatients();
    expect(!all_patients.value().empty(),
        "getAllPatients should return at least one patient after create");
    expect(all_patients.value().size() == 1, "expected exactly one patient in empty test database");
    expect(all_patients.value()[0].id == created_patient.value().id,
        "expected stored patient id to equal created.id");
    expect(all_patients.value()[0].name == "Steve Moro", "stored name should match");

    // ======= FIND PATIENT BY ID ======

    // Find Patient by ID
    auto found_patient = patient_repository.findPatientById(created_patient.value().id);
    expect(found_patient.isOk(), "findPatientById should succeed after creation");
    expect(found_patient.value().name == created_patient.value().name, "name should match");
    expect(found_patient.value().birth_date == created_patient.value().birth_date,
        "birth_date should match");
    expect(found_patient.value().nationality == created_patient.value().nationality,
        "nationality should match");

    // ======= UPDATE PATIENT NAME ======

    // Update Patient Name
    auto update_name_result =
        patient_repository.updatePatientName(created_patient.value().id, "Bjarne Stroustrup");
    expect(update_name_result.isOk(), "updatePatientName should succeed");

    // Find Patient by ID after Update Patient Name
    auto patient_after_name_update = patient_repository.findPatientById(created_patient.value().id);
    expect(patient_after_name_update.isOk(), "find after update should succeed");
    expect(patient_after_name_update.value().name == "Bjarne Stroustrup", "name should be updated");

    // Update Patient Name with same Name
    auto update_name_same_value_result =
        patient_repository.updatePatientName(created_patient.value().id, "Bjarne Stroustrup");
    expect(update_name_same_value_result.isOk(),
        "updatePatientName with same value should still succeed");

    // Update Patient Name with invalid ID=0
    auto update_name_invalid_id_result = patient_repository.updatePatientName(0, "X");
    expect(update_name_invalid_id_result.isError(), "update with invalid id should fail");
    expect(update_name_invalid_id_result.error().code == common::result::ErrorCode::InvalidArgument,
        "invalid id should return InvalidArgument");

    // ======= UPDATE PATIENT BIRTHDATE ======

    // Update Patient BirthDate
    auto update_birthdate_result =
        patient_repository.updatePatientBirthdate(created_patient.value().id, "1999-06-06");
    expect(update_birthdate_result.isOk(), "updatePatientBirthdate should succeed");

    // Find Patient by ID after Update Patient BirthDate
    auto patient_after_birthdate_update =
        patient_repository.findPatientById(created_patient.value().id);
    expect(patient_after_birthdate_update.isOk(), "find after update should succeed");
    expect(patient_after_birthdate_update.value().birth_date == "1999-06-06",
        "birthdate should be updated");

    // Clear Patient BirthDate
    auto clear_birthdate_result =
        patient_repository.updatePatientBirthdate(created_patient.value().id, "");
    expect(clear_birthdate_result.isOk(), "clearing birthdate should succeed");

    auto patient_after_clearing_birthdate =
        patient_repository.findPatientById(created_patient.value().id);
    expect(patient_after_clearing_birthdate.isOk(), "find after clear birthdate should succeed");
    expect(patient_after_clearing_birthdate.value().birth_date.empty(),
        "birthdate should be empty after clear");

    // Update Patient BirthDate with invalid BirthDate
    auto update_birthdate_invalid_format_result =
        patient_repository.updatePatientBirthdate(created_patient.value().id, "1999.06.06");
    expect(update_birthdate_invalid_format_result.isError(),
        "updatePatientBirthDate with invalid birth_date should fail");
    expect(update_birthdate_invalid_format_result.error().code ==
            common::result::ErrorCode::InvalidArgument,
        "invalid birth_date should return InvalidArgument");

    // ======= UPDATE PATIENT NATIONALITY ======

    // Update Patient Nationality
    auto update_nationality_result =
        patient_repository.updatePatientNationality(created_patient.value().id, "US");
    expect(update_nationality_result.isOk(), "updatePatientNationality should succeed");

    // Find Patient by ID after Update Patient Nationality
    auto patient_after_nationality_update =
        patient_repository.findPatientById(created_patient.value().id);
    expect(patient_after_nationality_update.isOk(), "find after update should succeed");
    expect(patient_after_nationality_update.value().nationality.has_value(),
        "nationality should be present after update");
    expect(patient_after_nationality_update.value().nationality.value() == domain::Nationality::US,
        "nationality should be updated");

    // Clear Nationality (optional)
    auto clear_nationality_result =
        patient_repository.updatePatientNationality(created_patient.value().id, "");
    expect(clear_nationality_result.isOk(), "clearing nationality should succeed");

    auto patient_after_clearing_nationality =
        patient_repository.findPatientById(created_patient.value().id);
    expect(
        patient_after_clearing_nationality.isOk(), "find after clear nationality should succeed");
    expect(!patient_after_clearing_nationality.value().nationality.has_value(),
        "nationality should be empty after clear");

    // ======= DELETE PATIENT ======

    auto delete_patient_result = patient_repository.deletePatientById(created_patient.value().id);
    expect(delete_patient_result.isOk(), "deletePatientById should succeed");

    auto find_after_delete_result = patient_repository.findPatientById(created_patient.value().id);
    expect(find_after_delete_result.isError(), "find after delete should fail");
    expect(find_after_delete_result.error().code == common::result::ErrorCode::NotFound,
        "find after delete should return NotFound");

    auto delete_missing_patient_result =
        patient_repository.deletePatientById(created_patient.value().id);
    expect(delete_missing_patient_result.isError(), "delete missing patient should fail");
    expect(delete_missing_patient_result.error().code == common::result::ErrorCode::NotFound,
        "delete missing patient should return NotFound");

    auto delete_invalid_id_result = patient_repository.deletePatientById(0);
    expect(delete_invalid_id_result.isError(), "update with invalid id should fail");
    expect(delete_invalid_id_result.error().code == common::result::ErrorCode::InvalidArgument,
        "invalid id should return InvalidArgument");

    std::cout << "PATIENT SMOKE TEST PASSED\n";

    return 0;
}
