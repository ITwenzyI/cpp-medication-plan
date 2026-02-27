#include "intake_plan_repository_sqlite.hpp"
#include "common/validation/id_validation.hpp"
#include "common/validation/string_validation.hpp"
#include "time_of_day_mapper_sqlite.hpp"
#include <stdexcept>

namespace infrastructure::persistence::sqlite {
static domain::IntakePlan mapIntakePlan(const infrastructure::db::Statement& stmt) {
    domain::IntakePlan temp;
    temp.id = stmt.getInt(0);
    temp.patientId = stmt.getInt(1);
    temp.medicationId = stmt.getInt(2);
    temp.dose = stmt.getText(3);
    temp.timeOfDay = timeOfDayFromDbString(stmt.getText(4));
    temp.notes = stmt.getText(5);

    return temp;
}

IntakePlanRepositorySqlite::IntakePlanRepositorySqlite(infrastructure::db::Database& db) : db_(db) {
}

common::result::Result<domain::IntakePlan> IntakePlanRepositorySqlite::createIntakePlan(
    const domain::IntakePlan& plan) {

    auto stmt = db_.prepare("INSERT INTO intake_plans (patient_id, medication_id, dose, "
                            "time_of_day, notes) VALUES (?, ?, ?, ?, ?);");

    if (!common::validation::validateId(plan.patientId)) {
        return common::result::Result<domain::IntakePlan>::fail(
            common::result::ErrorCode::InvalidArgument, "patient_id must be positive",
            "IntakePlanRepositorySqlite::createIntakePlan");
    } else {
        stmt.bindInt(1, plan.patientId);
    }

    if (!common::validation::validateId(plan.medicationId)) {
        return common::result::Result<domain::IntakePlan>::fail(
            common::result::ErrorCode::InvalidArgument, "medication_id must be positive",
            "IntakePlanRepositorySqlite::createIntakePlan");
    } else {
        stmt.bindInt(2, plan.medicationId);
    }

    if (common::validation::isEmptyOrBlank(plan.dose)) {
        return common::result::Result<domain::IntakePlan>::fail(
            common::result::ErrorCode::InvalidArgument, "dose must not be empty",
            "IntakePlanRepositorySqlite::createIntakePlan");
    } else {
        stmt.bindText(3, plan.dose);
    }

    stmt.bindText(4, timeOfDayToDbString(plan.timeOfDay));

    if (common::validation::isEmptyOrBlank(plan.notes)) {
        stmt.bindNull(5);
    } else {
        stmt.bindText(5, plan.notes);
    }

    const int rc = stmt.step();
    switch (rc) {
        case SQLITE_DONE:
            break;

        case SQLITE_CONSTRAINT: {
            const int ext = sqlite3_extended_errcode(db_.get());

            switch (ext) {
                case SQLITE_CONSTRAINT_UNIQUE:
                case SQLITE_CONSTRAINT_PRIMARYKEY:
                    return common::result::Result<domain::IntakePlan>::fail(
                        common::result::ErrorCode::Conflict,
                        "intake plan already exists for patient, medication and time_of_day",
                        "IntakePlanRepositorySqlite::createIntakePlan");

                case SQLITE_CONSTRAINT_FOREIGNKEY:
                    return common::result::Result<domain::IntakePlan>::fail(
                        common::result::ErrorCode::ForeignKeyViolation,
                        "patient_id or medication_id does not exist",
                        "IntakePlanRepositorySqlite::createIntakePlan");

                case SQLITE_CONSTRAINT_NOTNULL:
                    return common::result::Result<domain::IntakePlan>::fail(
                        common::result::ErrorCode::Unexpected, "NOT NULL constraint failed",
                        "IntakePlanRepositorySqlite::createIntakePlan");

                default:
                    return common::result::Result<domain::IntakePlan>::fail(
                        common::result::ErrorCode::ConstraintViolation, "constraint violation",
                        "IntakePlanRepositorySqlite::createIntakePlan");
            }
        }

        case SQLITE_BUSY:
        case SQLITE_LOCKED:
            return common::result::Result<domain::IntakePlan>::fail(
                common::result::ErrorCode::DatabaseError, "database is busy or locked",
                "IntakePlanRepositorySqlite::createIntakePlan");

        default:
            return common::result::Result<domain::IntakePlan>::fail(
                common::result::ErrorCode::DatabaseError, "insert failed",
                "IntakePlanRepositorySqlite::createIntakePlan");
    }

    const int lastInsertRowID = static_cast<int>(sqlite3_last_insert_rowid(db_.get()));

    domain::IntakePlan newIntakePlan = plan;
    newIntakePlan.id = lastInsertRowID;

    return common::result::Result<domain::IntakePlan>::ok(newIntakePlan);
}

common::result::Result<std::vector<domain::IntakePlan>>
IntakePlanRepositorySqlite::getIntakePlansByPatientId(int patient_id) const {
}

common::result::Result<std::vector<domain::IntakePlan>>
IntakePlanRepositorySqlite::getIntakePlansByMedicationId(int medication_id) const {
}

common::result::Result<void> IntakePlanRepositorySqlite::deleteIntakePlanById(int intake_plan_id) {
}

common::result::Result<void> IntakePlanRepositorySqlite::updateIntakePlan(
    const domain::IntakePlan& plan) {
}

} // namespace infrastructure::persistence::sqlite