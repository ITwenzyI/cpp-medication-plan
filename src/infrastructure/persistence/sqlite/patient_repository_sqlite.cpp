#include "patient_repository_sqlite.hpp"
#include "common/result/result.hpp"
#include "common/validation/birth_date_validation.hpp"
#include "common/validation/id_validation.hpp"
#include "common/validation/string_validation.hpp"
#include "nationality_mapper_sqlite.hpp"
#include <stdexcept>

namespace infrastructure::persistence::sqlite {
static common::result::Result<domain::Patient> mapPatient(
    const infrastructure::db::Statement& stmt) {
    domain::Patient temp;
    temp.id = stmt.getInt(0);
    temp.name = stmt.getText(1);
    temp.birth_date = stmt.getText(2);
    auto nationality_result = nationalityFromDbNullableString(stmt.getText(3));
    if (nationality_result.isError()) {
        return common::result::Result<domain::Patient>::fail(nationality_result.error().code,
            nationality_result.error().message, "infrastructure::persistence::sqlite::mapPatient");
    }
    temp.nationality = nationality_result.value();

    return common::result::Result<domain::Patient>::ok(temp);
}

PatientRepositorySqlite::PatientRepositorySqlite(infrastructure::db::Database& db) : db_(db) {
}

common::result::Result<domain::Patient> PatientRepositorySqlite::createPatient(
    const domain::Patient& p) {

    auto stmt =
        db_.prepare("INSERT INTO patients (name, birth_date, nationality) VALUES (?, ?, ?);");

    if (common::validation::isEmptyOrBlank(p.name)) {
        return common::result::Result<domain::Patient>::fail(
            common::result::ErrorCode::InvalidArgument, "name must not be empty",
            "PatientRepositorySqlite::createPatient");
    } else {
        stmt.bindText(1, p.name);
    }

    if (!common::validation::isEmptyOrBlank(p.birth_date)) {
        if (!common::validation::isValidBirthDate(p.birth_date)) {
            return common::result::Result<domain::Patient>::fail(
                common::result::ErrorCode::InvalidArgument,
                "birth_date format is invalid (YYYY-MM-DD)",
                "PatientRepositorySqlite::createPatient");
        }
        stmt.bindText(2, p.birth_date);
    } else {
        stmt.bindNull(2);
    }

    if (p.nationality.has_value()) {
        stmt.bindText(3, nationalityToDbString(p.nationality.value()));
    } else {
        stmt.bindNull(3);
    }

    int rc = stmt.step();
    if (rc != SQLITE_DONE) {
        return common::result::Result<domain::Patient>::fail(
            common::result::ErrorCode::DatabaseError, "INSERT failed",
            "PatientRepositorySqlite::createPatient");
    }

    int lastInsertRowID = static_cast<int>(sqlite3_last_insert_rowid(db_.get()));

    domain::Patient newPatient = p;
    newPatient.id = lastInsertRowID;

    return common::result::Result<domain::Patient>::ok(newPatient);
}

common::result::Result<std::vector<domain::Patient>>
PatientRepositorySqlite::getAllPatients() const {
    auto stmt =
        db_.prepare("SELECT id, name, birth_date, nationality FROM patients ORDER BY id ASC;");

    std::vector<domain::Patient> result;

    while (true) {
        int rc = stmt.step();

        if (rc == SQLITE_ROW) {
            auto mapped = mapPatient(stmt);
            if (mapped.isError()) {
                return common::result::Result<std::vector<domain::Patient>>::fail(
                    mapped.error().code, mapped.error().message,
                    "PatientRepositorySqlite::getAllPatients");
            }
            result.push_back(mapped.value());
            continue;
        } else if (rc == SQLITE_DONE) {
            break;
        } else {
            return common::result::Result<std::vector<domain::Patient>>::fail(
                common::result::ErrorCode::DatabaseError, "SELECT failed",
                "PatientRepositorySqlite::getAllPatients");
        }
    }

    return common::result::Result<std::vector<domain::Patient>>::ok(result);
}

common::result::Result<domain::Patient> PatientRepositorySqlite::findPatientById(
    int patient_id) const {
    if (!common::validation::validateId(patient_id)) {
        return common::result::Result<domain::Patient>::fail(
            common::result::ErrorCode::InvalidArgument, "patient_id must be positive",
            "PatientRepositorySqlite::findPatientById");
    }

    auto stmt = db_.prepare("SELECT id, name, birth_date, nationality FROM patients WHERE id = ?;");

    stmt.bindInt(1, patient_id);

    int rc = stmt.step();

    if (rc == SQLITE_ROW) {
        auto mapped = mapPatient(stmt);
        if (mapped.isError()) {
            return common::result::Result<domain::Patient>::fail(mapped.error().code,
                mapped.error().message, "PatientRepositorySqlite::findPatientById");
        }
        return common::result::Result<domain::Patient>::ok(mapped.value());
    } else if (rc == SQLITE_DONE) {
        return common::result::Result<domain::Patient>::fail(common::result::ErrorCode::NotFound,
            "No patient with id: " + std::to_string(patient_id),
            "PatientRepositorySqlite::findPatientById");
    }
    throw std::logic_error("unreachable");
}

common::result::Result<void> PatientRepositorySqlite::deletePatientById(int patient_id) {
    if (!common::validation::validateId(patient_id)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "patient_id must be positive", "PatientRepositorySqlite::deletePatientById");
    }

    auto stmt = db_.prepare("DELETE FROM patients WHERE id = ?;");

    stmt.bindInt(1, patient_id);

    int rc = stmt.step();

    if (db_.changes() == 0) {
        return common::result::Result<void>::fail(common::result::ErrorCode::NotFound,
            "No patient with id: " + std::to_string(patient_id),
            "PatientRepositorySqlite::deletePatientById");
    }
    return common::result::Result<void>::ok();
}

common::result::Result<void> PatientRepositorySqlite::updatePatientName(
    int patient_id, std::string_view name) {

    if (!common::validation::validateId(patient_id)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "patient_id must be positive", "PatientRepositorySqlite::updatePatientName");
    }
    if (common::validation::isEmptyOrBlank(name)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "name must not be empty", "PatientRepositorySqlite::updatePatientName");
    }

    auto stmt = db_.prepare("UPDATE patients SET name = ? WHERE id = ?;");

    stmt.bindText(1, name);
    stmt.bindInt(2, patient_id);

    int rc = stmt.step();

    if (db_.changes() == 0) {
        auto existing = findPatientById(patient_id);
        if (existing.isError()) {
            if (existing.error().code == common::result::ErrorCode::NotFound) {
                return common::result::Result<void>::fail(common::result::ErrorCode::NotFound,
                    "No patient with id: " + std::to_string(patient_id),
                    "PatientRepositorySqlite::updatePatientName");
            }
            return common::result::Result<void>::fail(existing.error());
        }
    }
    return common::result::Result<void>::ok();
}

common::result::Result<void> PatientRepositorySqlite::updatePatientBirthdate(
    int patient_id, std::string_view new_birth_date) {

    if (!common::validation::validateId(patient_id)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "patient_id must be positive", "PatientRepositorySqlite::updatePatientBirthdate");
    }
    if (!common::validation::isEmptyOrBlank(new_birth_date) &&
        !common::validation::isValidBirthDate(new_birth_date)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "birth_date format is invalid (YYYY-MM-DD)",
            "PatientRepositorySqlite::updatePatientBirthdate");
    }

    auto stmt = db_.prepare("UPDATE patients SET birth_date = ? WHERE id = ?;");

    if (common::validation::isEmptyOrBlank(new_birth_date)) {
        stmt.bindNull(1);
    } else {
        stmt.bindText(1, new_birth_date);
    }
    stmt.bindInt(2, patient_id);

    int rc = stmt.step();

    if (db_.changes() == 0) {
        auto existing = findPatientById(patient_id);
        if (existing.isError()) {
            if (existing.error().code == common::result::ErrorCode::NotFound) {
                return common::result::Result<void>::fail(common::result::ErrorCode::NotFound,
                    "No patient with id: " + std::to_string(patient_id),
                    "PatientRepositorySqlite::updatePatientBirthdate");
            }
            return common::result::Result<void>::fail(existing.error());
        }
    }
    return common::result::Result<void>::ok();
}

common::result::Result<void> PatientRepositorySqlite::updatePatientNationality(
    int patient_id, std::string_view new_nationality) {

    if (!common::validation::validateId(patient_id)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "patient_id must be positive", "PatientRepositorySqlite::updatePatientNationality");
    }
    auto stmt = db_.prepare("UPDATE patients SET nationality = ? WHERE id = ?;");

    if (common::validation::isEmptyOrBlank(new_nationality)) {
        stmt.bindNull(1);
    } else {
        auto parsed = nationalityFromDbString(new_nationality);
        if (parsed.isError()) {
            return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
                "nationality has invalid country code",
                "PatientRepositorySqlite::updatePatientNationality");
        }

        stmt.bindText(1, nationalityToDbString(parsed.value()));
    }
    stmt.bindInt(2, patient_id);

    int rc = stmt.step();

    if (db_.changes() == 0) {
        auto existing = findPatientById(patient_id);
        if (existing.isError()) {
            if (existing.error().code == common::result::ErrorCode::NotFound) {
                return common::result::Result<void>::fail(common::result::ErrorCode::NotFound,
                    "No patient with id: " + std::to_string(patient_id),
                    "PatientRepositorySqlite::updatePatientNationality");
            }
            return common::result::Result<void>::fail(existing.error());
        }
    }
    return common::result::Result<void>::ok();
}

} // namespace infrastructure::persistence::sqlite
