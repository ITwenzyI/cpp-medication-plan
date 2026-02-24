#include "domain/patient.hpp"
#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include "infrastructure/persistence/sqlite/patient_repository_sqlite.hpp"
#include <iostream>

static void expect(bool condition, std::string_view message) {
    if (condition) {
        return;
    }
    std::cerr << "TEST FAILED: " << message << "\n";
    std::exit(1);
}

int main() {

    infrastructure::db::Database db(":memory:");
    infrastructure::db::initDatabase(db, "sql/schema.sql");

    infrastructure::persistence::sqlite::PatientRepositorySqlite repo(db);

    domain::Patient p1{0, "Steve Moro", "07.02.1998", "Deutsch"};

    auto created = repo.createPatient(p1);
    expect(created.id > 0, "created patient should have generated id");

    auto all = repo.getAllPatients();
    expect(!all.empty(), "getAllPatients should return at least one patient after create");
    expect(all.size() == 1, "expected exactly one patient in empty test database");
    expect(all[0].id == created.id, "expected stored patient id to equal created.id");
    expect(all[0].name == "Steve Moro", "stored name should match");

    auto found = repo.findPatientById(created.id);
    expect(found.isOk(), "findPatientById should succeed after creation");
    expect(found.value().name == created.name, "name should match");
    expect(found.value().birth_date == created.birth_date, "birth_date should match");
    expect(found.value().nationality == created.nationality, "nationality should match");

    auto update = repo.updatePatientName(created.id, "Bjarne Stroustrup");
    expect(update.isOk(), "updatePatientName should succeed");

    auto updated = repo.findPatientById(created.id);
    expect(updated.isOk(), "find after update should succeed");
    expect(updated.value().name == "Bjarne Stroustrup", "name should be updated");

    std::cout << "SMOKE TEST PASSED\n";

    return 0;
}