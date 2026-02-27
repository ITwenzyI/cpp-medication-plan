#include "medication_repository_sqlite.hpp"
#include "common/validation/id_validation.hpp"
#include "common/validation/string_validation.hpp"
#include <stdexcept>

namespace infrastructure::persistence::sqlite {
static domain::Medication mapMedication(const infrastructure::db::Statement& stmt) {
    domain::Medication temp;
    temp.id = stmt.getInt(0);
    temp.name = stmt.getText(1);
    temp.strength = stmt.getText(2);
    temp.warnings = stmt.getText(3);

    return temp;
}

MedicationRepositorySqlite::MedicationRepositorySqlite(infrastructure::db::Database& db) : db_(db) {
}

common::result::Result<domain::Medication> MedicationRepositorySqlite::createMedication(
    const domain::Medication& m) {

    auto stmt = db_.prepare("INSERT INTO medications (name, strength, warnings) VALUES (?, ?, ?);");

    if (common::validation::isEmptyOrBlank(m.name)) {
        return common::result::Result<domain::Medication>::fail(
            common::result::ErrorCode::InvalidArgument, "name must not be empty",
            "MedicationRepositorySqlite::createMedication");
    } else {
        stmt.bindText(1, m.name);
    }

    if (common::validation::isEmptyOrBlank(m.strength)) {
        return common::result::Result<domain::Medication>::fail(
            common::result::ErrorCode::InvalidArgument, "strength must not be empty",
            "MedicationRepositorySqlite::createMedication");
    } else {
        stmt.bindText(2, m.strength);
    }

    if (!common::validation::isEmptyOrBlank(m.warnings)) {
        stmt.bindText(3, m.warnings);
    } else {
        stmt.bindNull(3);
    }

    int rc = stmt.step();
    if (rc != SQLITE_DONE) {
        return common::result::Result<domain::Medication>::fail(
            common::result::ErrorCode::DatabaseError, "INSERT failed",
            "MedicationRepositorySqlite::createMedication");
    }

    int lastInsertRowID = static_cast<int>(sqlite3_last_insert_rowid(db_.get()));

    domain::Medication newMedication = m;
    newMedication.id = lastInsertRowID;

    return common::result::Result<domain::Medication>::ok(newMedication);
}

common::result::Result<std::vector<domain::Medication>>
MedicationRepositorySqlite::getAllMedications() const {
    auto stmt =
        db_.prepare("SELECT id, name, strength, warnings FROM medications ORDER BY id ASC;");

    std::vector<domain::Medication> result;

    while (true) {
        int rc = stmt.step();

        if (rc == SQLITE_ROW) {
            result.push_back(mapMedication(stmt));
            continue;
        } else if (rc == SQLITE_DONE) {
            break;
        } else {
            return common::result::Result<std::vector<domain::Medication>>::fail(
                common::result::ErrorCode::DatabaseError, "SELECT failed",
                "MedicationRepositorySqlite::getAllMedications");
        }
    }

    return common::result::Result<std::vector<domain::Medication>>::ok(result);
}

common::result::Result<domain::Medication> MedicationRepositorySqlite::findMedicationById(
    int medication_id) const {
    if (!common::validation::validateId(medication_id)) {
        return common::result::Result<domain::Medication>::fail(
            common::result::ErrorCode::InvalidArgument, "medication_id must be positive",
            "MedicationRepositorySqlite::findMedicationById");
    }

    auto stmt = db_.prepare("SELECT id, name, strength, warnings FROM medications WHERE id = ?;");

    stmt.bindInt(1, medication_id);

    int rc = stmt.step();

    if (rc == SQLITE_ROW) {
        return common::result::Result<domain::Medication>::ok(mapMedication(stmt));
    } else if (rc == SQLITE_DONE) {
        return common::result::Result<domain::Medication>::fail(common::result::ErrorCode::NotFound,
            "No medication with id: " + std::to_string(medication_id),
            "MedicationRepositorySqlite::findMedicationById");
    }
    throw std::logic_error("unreachable");
}

common::result::Result<void> MedicationRepositorySqlite::deleteMedicationById(int medication_id) {
    if (!common::validation::validateId(medication_id)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "medication_id must be positive", "MedicationRepositorySqlite::deleteMedicationById");
    }

    auto stmt = db_.prepare("DELETE FROM medications WHERE id = ?;");

    stmt.bindInt(1, medication_id);

    int rc = stmt.step();

    if (db_.changes() == 0) {
        return common::result::Result<void>::fail(common::result::ErrorCode::NotFound,
            "No medication with id: " + std::to_string(medication_id),
            "MedicationRepositorySqlite::deleteMedicationById");
    }
    return common::result::Result<void>::ok();
}

common::result::Result<void> MedicationRepositorySqlite::updateMedicationName(
    int medication_id, std::string_view name) {

    if (!common::validation::validateId(medication_id)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "medication_id must be positive", "MedicationRepositorySqlite::updateMedicationName");
    }
    if (common::validation::isEmptyOrBlank(name)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "name must not be empty", "MedicationRepositorySqlite::updateMedicationName");
    }

    auto stmt = db_.prepare("UPDATE medications SET name = ? WHERE id = ?;");

    stmt.bindText(1, name);
    stmt.bindInt(2, medication_id);

    int rc = stmt.step();

    if (db_.changes() == 0) {
        auto existing = findMedicationById(medication_id);
        if (existing.isError()) {
            if (existing.error().code == common::result::ErrorCode::NotFound) {
                return common::result::Result<void>::fail(common::result::ErrorCode::NotFound,
                    "No medication with id: " + std::to_string(medication_id),
                    "MedicationRepositorySqlite::updateMedicationName");
            }
            return common::result::Result<void>::fail(existing.error());
        }
    }
    return common::result::Result<void>::ok();
}

common::result::Result<void> MedicationRepositorySqlite::updateMedicationStrength(
    int medication_id, std::string_view new_strength) {

    if (!common::validation::validateId(medication_id)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "medication_id must be positive",
            "MedicationRepositorySqlite::updateMedicationStrength");
    }
    if (common::validation::isEmptyOrBlank(new_strength)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "new_strength must not be empty",
            "MedicationRepositorySqlite::updateMedicationStrength");
    }

    auto stmt = db_.prepare("UPDATE medications SET strength = ? WHERE id = ?;");

    stmt.bindText(1, new_strength);
    stmt.bindInt(2, medication_id);

    int rc = stmt.step();

    if (db_.changes() == 0) {
        auto existing = findMedicationById(medication_id);
        if (existing.isError()) {
            if (existing.error().code == common::result::ErrorCode::NotFound) {
                return common::result::Result<void>::fail(common::result::ErrorCode::NotFound,
                    "No medication with id: " + std::to_string(medication_id),
                    "MedicationRepositorySqlite::updateMedicationStrength");
            }
            return common::result::Result<void>::fail(existing.error());
        }
    }
    return common::result::Result<void>::ok();
}

common::result::Result<void> MedicationRepositorySqlite::updateMedicationWarnings(
    int medication_id, std::string_view new_warnings) {

    if (!common::validation::validateId(medication_id)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "medication_id must be positive",
            "MedicationRepositorySqlite::updateMedicationWarnings");
    }
    if (common::validation::isEmptyOrBlank(new_warnings)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "new_warnings must not be empty",
            "MedicationRepositorySqlite::updateMedicationWarnings");
    }

    auto stmt = db_.prepare("UPDATE medications SET warnings = ? WHERE id = ?;");

    stmt.bindText(1, new_warnings);
    stmt.bindInt(2, medication_id);

    int rc = stmt.step();

    if (db_.changes() == 0) {
        auto existing = findMedicationById(medication_id);
        if (existing.isError()) {
            if (existing.error().code == common::result::ErrorCode::NotFound) {
                return common::result::Result<void>::fail(common::result::ErrorCode::NotFound,
                    "No medication with id: " + std::to_string(medication_id),
                    "MedicationRepositorySqlite::updateMedicationWarnings");
            }
            return common::result::Result<void>::fail(existing.error());
        }
    }
    return common::result::Result<void>::ok();
}

} // namespace infrastructure::persistence::sqlite