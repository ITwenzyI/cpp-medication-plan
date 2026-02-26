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
    std::cerr << "TEST FAILED: " << message << "\n";
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

    std::cout << "MEDICATION SMOKE TEST PASSED\n";

    return 0;
}