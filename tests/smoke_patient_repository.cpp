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

    infrastructure::persistence::sqlite::PatientRepositorySqlite repo(db);

    domain::Patient p1{0, "Steve Moro", "1998-02-07", "DE"};

    // ======= CREATE PATIENT ======

    // Create Patient
    auto created = repo.createPatient(p1);
    expect(created.isOk(), "created patient should succeed");
    expect(created.value().id > 0, "created patient should have generated id");

    // ======= GET ALL PATIENTS ======

    // Get All Patients
    auto all = repo.getAllPatients();
    expect(!all.value().empty(), "getAllPatients should return at least one patient after create");
    expect(all.value().size() == 1, "expected exactly one patient in empty test database");
    expect(
        all.value()[0].id == created.value().id, "expected stored patient id to equal created.id");
    expect(all.value()[0].name == "Steve Moro", "stored name should match");

    // ======= FIND PATIENT BY ID ======

    // Find Patient by ID
    auto found = repo.findPatientById(created.value().id);
    expect(found.isOk(), "findPatientById should succeed after creation");
    expect(found.value().name == created.value().name, "name should match");
    expect(found.value().birth_date == created.value().birth_date, "birth_date should match");
    expect(found.value().nationality == created.value().nationality, "nationality should match");

    // ======= UPDATE PATIENT NAME ======

    // Update Patient Name
    auto update = repo.updatePatientName(created.value().id, "Bjarne Stroustrup");
    expect(update.isOk(), "updatePatientName should succeed");

    // Find Patient by ID after Update Patient Name
    auto updated = repo.findPatientById(created.value().id);
    expect(updated.isOk(), "find after update should succeed");
    expect(updated.value().name == "Bjarne Stroustrup", "name should be updated");

    // Update Patient Name with same Name
    auto upd_same = repo.updatePatientName(created.value().id, "Bjarne Stroustrup");
    expect(upd_same.isOk(), "updatePatientName with same value should still succeed");

    // Update Patient Name with invalid ID=0
    auto upd_bad = repo.updatePatientName(0, "X");
    expect(upd_bad.isError(), "update with invalid id should fail");
    expect(upd_bad.error().code == common::result::ErrorCode::InvalidArgument,
        "invalid id should return InvalidArgument");

    // ======= UPDATE PATIENT BIRTHDATE ======

    // Update Patient BirthDate
    auto update_birthdate = repo.updatePatientBirthdate(created.value().id, "1999-06-06");
    expect(update_birthdate.isOk(), "updatePatientBirthdate should succeed");

    // Find Patient by ID after Update Patient BirthDate
    auto updated_birthdate = repo.findPatientById(created.value().id);
    expect(updated_birthdate.isOk(), "find after update should succeed");
    expect(updated_birthdate.value().birth_date == "1999-06-06", "birthdate should be updated");

    // Update Patient BirthDate with invalid BirthDate
    auto update_birthdate_invalid = repo.updatePatientBirthdate(created.value().id, "1999.06.06");
    expect(update_birthdate_invalid.isError(),
        "updatePatientBirthDate with invalid birth_date should fail");
    expect(update_birthdate_invalid.error().code == common::result::ErrorCode::InvalidArgument,
        "invalid birth_date should return InvalidArgument");

    // ======= UPDATE PATIENT NATIONALITY ======

    // Update Patient Nationality
    auto update_nationality = repo.updatePatientNationality(created.value().id, "EN");
    expect(update_nationality.isOk(), "updatePatientNationality should succeed");

    // Find Patient by ID after Update Patient Nationality
    auto updated_nationality = repo.findPatientById(created.value().id);
    expect(updated_nationality.isOk(), "find after update should succeed");
    expect(updated_nationality.value().nationality == "EN", "nationality should be updated");

    // ======= DELETE PATIENT ======

    auto delete1 = repo.deletePatientById(created.value().id);
    expect(delete1.isOk(), "deletePatientById should succeed");

    auto after_delete = repo.findPatientById(created.value().id);
    expect(after_delete.isError(), "find after delete should fail");
    expect(after_delete.error().code == common::result::ErrorCode::NotFound,
        "find after delete should return NotFound");

    auto delete2 = repo.deletePatientById(created.value().id);
    expect(delete2.isError(), "delete missing patient should fail");
    expect(delete2.error().code == common::result::ErrorCode::NotFound,
        "delete missing patient should return NotFound");

    auto delete_bad = repo.deletePatientById(0);
    expect(delete_bad.isError(), "update with invalid id should fail");
    expect(delete_bad.error().code == common::result::ErrorCode::InvalidArgument,
        "invalid id should return InvalidArgument");

    std::cout << "PATIENT SMOKE TEST PASSED\n";

    return 0;
}