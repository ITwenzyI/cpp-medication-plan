#include "intake_plan_repository_sqlite.hpp"
#include "common/result/result.hpp"
#include "common/validation/id_validation.hpp"
#include "common/validation/string_validation.hpp"
#include "domain/intake_plan.hpp"
#include "time_of_day_mapper_sqlite.hpp"
#include <vector>

namespace infrastructure::persistence::sqlite {
static common::result::Result<domain::IntakePlan> mapIntakePlan(
    const infrastructure::db::Statement& stmt) {
    domain::IntakePlan temp;
    temp.id = stmt.getInt(0);
    temp.patient_id = stmt.getInt(1);
    temp.medication_id = stmt.getInt(2);
    temp.dose = stmt.getText(3);

    auto todResult = timeOfDayFromDbString(stmt.getText(4));
    if (todResult.isError()) {
        return common::result::Result<domain::IntakePlan>::fail(todResult.error().code,
            todResult.error().message, "infrastructure::persistence::sqlite::mapIntakePlan");
    }
    temp.time_of_day = todResult.value();

    temp.notes = stmt.getText(5);

    return common::result::Result<domain::IntakePlan>::ok(temp);
}

static common::result::Result<domain::IntakePlan> validateIntakePlan(
    const domain::IntakePlan& plan) {

    if (!common::validation::validateId(plan.patient_id)) {
        return common::result::Result<domain::IntakePlan>::fail(
            common::result::ErrorCode::InvalidArgument, "patient_id must be positive",
            "IntakePlanRepositorySqlite::validateIntakePlan");
    }

    if (!common::validation::validateId(plan.medication_id)) {
        return common::result::Result<domain::IntakePlan>::fail(
            common::result::ErrorCode::InvalidArgument, "medication_id must be positive",
            "IntakePlanRepositorySqlite::validateIntakePlan");
    }

    if (common::validation::isEmptyOrBlank(plan.dose)) {
        return common::result::Result<domain::IntakePlan>::fail(
            common::result::ErrorCode::InvalidArgument, "dose must not be empty",
            "IntakePlanRepositorySqlite::validateIntakePlan");
    }

    return common::result::Result<domain::IntakePlan>::ok(plan);
}

static common::result::Result<domain::IntakePlan> validateIntakePlanForCreate(
    const domain::IntakePlan& plan) {

    auto base = validateIntakePlan(plan);
    if (base.isError())
        return base;

    auto normalizedPlan = base.value();
    normalizedPlan.id = 0;

    return common::result::Result<domain::IntakePlan>::ok(normalizedPlan);
}

static common::result::Result<domain::IntakePlan> validateIntakePlanForUpdate(
    const domain::IntakePlan& plan) {

    auto base = validateIntakePlan(plan);

    if (base.isError())
        return base;

    if (!common::validation::validateId(plan.id)) {
        return common::result::Result<domain::IntakePlan>::fail(
            common::result::ErrorCode::InvalidArgument, "intake_plan_id must be positive",
            "IntakePlanRepositorySqlite::validateIntakePlanForUpdate");
    }

    return common::result::Result<domain::IntakePlan>::ok(plan);
}

IntakePlanRepositorySqlite::IntakePlanRepositorySqlite(infrastructure::db::Database& db) : db_(db) {
}

common::result::Result<domain::IntakePlan> IntakePlanRepositorySqlite::createIntakePlan(
    const domain::IntakePlan& plan) {

    auto validated = validateIntakePlanForCreate(plan);
    if (validated.isError()) {
        return common::result::Result<domain::IntakePlan>::fail(validated.error().code,
            validated.error().message, "IntakePlanRepositorySqlite::createIntakePlan");
    }

    auto stmt = db_.prepare("INSERT INTO intake_plans (patient_id, medication_id, dose, "
                            "time_of_day, notes) VALUES (?, ?, ?, ?, ?);");

    const domain::IntakePlan normalized = validated.value();

    stmt.bindInt(1, normalized.patient_id);
    stmt.bindInt(2, normalized.medication_id);
    stmt.bindText(3, normalized.dose);
    stmt.bindText(4, timeOfDayToDbString(normalized.time_of_day));

    if (common::validation::isEmptyOrBlank(normalized.notes)) {
        stmt.bindNull(5);
    } else {
        stmt.bindText(5, normalized.notes);
    }

    const int rc = stmt.step();
    switch (rc) {
        case SQLITE_DONE:
            break;

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

        case SQLITE_CONSTRAINT:
            return common::result::Result<domain::IntakePlan>::fail(
                common::result::ErrorCode::ConstraintViolation, "constraint violation",
                "IntakePlanRepositorySqlite::createIntakePlan");

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

    domain::IntakePlan newIntakePlan = normalized;
    newIntakePlan.id = lastInsertRowID;

    return common::result::Result<domain::IntakePlan>::ok(newIntakePlan);
}

common::result::Result<std::vector<domain::IntakePlan>>
IntakePlanRepositorySqlite::getIntakePlansByPatientId(int patient_id) const {

    if (!common::validation::validateId(patient_id)) {
        return common::result::Result<std::vector<domain::IntakePlan>>::fail(
            common::result::ErrorCode::InvalidArgument, "patient_id must be positive",
            "IntakePlanRepositorySqlite::getIntakePlansByPatientId");
    }

    auto stmt = db_.prepare("SELECT id, patient_id, medication_id, dose, "
                            "time_of_day, notes FROM intake_plans WHERE patient_id = ? ORDER BY "
                            "time_of_day, medication_id, id;");

    stmt.bindInt(1, patient_id);

    std::vector<domain::IntakePlan> result;

    while (true) {
        const int rc = stmt.step();

        switch (rc) {
            case SQLITE_DONE:
                return common::result::Result<std::vector<domain::IntakePlan>>::ok(result);

            case SQLITE_ROW: {
                auto mapped = mapIntakePlan(stmt);

                if (mapped.isError()) {
                    return common::result::Result<std::vector<domain::IntakePlan>>::fail(
                        mapped.error().code, mapped.error().message,
                        "IntakePlanRepositorySqlite::getIntakePlansByPatientId");
                }

                result.push_back(mapped.value());
                continue;
            }

            default:
                return common::result::Result<std::vector<domain::IntakePlan>>::fail(
                    common::result::ErrorCode::DatabaseError, "SELECT failed",
                    "IntakePlanRepositorySqlite::getIntakePlansByPatientId");
        }
    }
}

common::result::Result<std::vector<domain::IntakePlan>>
IntakePlanRepositorySqlite::getIntakePlansByMedicationId(int medication_id) const {

    if (!common::validation::validateId(medication_id)) {
        return common::result::Result<std::vector<domain::IntakePlan>>::fail(
            common::result::ErrorCode::InvalidArgument, "medication_id must be positive",
            "IntakePlanRepositorySqlite::getIntakePlansByMedicationId");
    }

    auto stmt = db_.prepare("SELECT id, patient_id, medication_id, dose, "
                            "time_of_day, notes FROM intake_plans WHERE medication_id = ? ORDER BY "
                            "time_of_day, medication_id, id;");

    stmt.bindInt(1, medication_id);

    std::vector<domain::IntakePlan> result;

    while (true) {
        const int rc = stmt.step();

        switch (rc) {
            case SQLITE_DONE:
                return common::result::Result<std::vector<domain::IntakePlan>>::ok(result);

            case SQLITE_ROW: {
                auto mapped = mapIntakePlan(stmt);

                if (mapped.isError()) {
                    return common::result::Result<std::vector<domain::IntakePlan>>::fail(
                        mapped.error().code, mapped.error().message,
                        "IntakePlanRepositorySqlite::getIntakePlansByMedicationId");
                }

                result.push_back(mapped.value());
                continue;
            }

            default:
                return common::result::Result<std::vector<domain::IntakePlan>>::fail(
                    common::result::ErrorCode::DatabaseError, "SELECT failed",
                    "IntakePlanRepositorySqlite::getIntakePlansByMedicationId");
        }
    }
}

common::result::Result<domain::IntakePlan> IntakePlanRepositorySqlite::findIntakePlanById(
    int id) const {
    if (!common::validation::validateId(id)) {
        return common::result::Result<domain::IntakePlan>::fail(
            common::result::ErrorCode::InvalidArgument, "id must be positive",
            "IntakePlanRepositorySqlite::findIntakePlanById");
    }

    auto stmt = db_.prepare("SELECT id, patient_id, medication_id, dose, "
                            "time_of_day, notes FROM intake_plans WHERE id = ?;");

    stmt.bindInt(1, id);

    domain::IntakePlan result;
    const int rc = stmt.step();

    switch (rc) {
        case SQLITE_DONE:
            return common::result::Result<domain::IntakePlan>::fail(
                common::result::ErrorCode::NotFound, "No IntakePlan with id: " + std::to_string(id),
                "IntakePlanRepositorySqlite::findIntakePlanById");

        case SQLITE_ROW: {
            auto mapped = mapIntakePlan(stmt);

            if (mapped.isError()) {
                return common::result::Result<domain::IntakePlan>::fail(mapped.error().code,
                    mapped.error().message, "IntakePlanRepositorySqlite::findIntakePlanById");
            }

            result = mapped.value();
            return common::result::Result<domain::IntakePlan>::ok(result);
        }

        default:
            return common::result::Result<domain::IntakePlan>::fail(
                common::result::ErrorCode::DatabaseError, "SELECT failed",
                "IntakePlanRepositorySqlite::findIntakePlanById");
    }
}

common::result::Result<void> IntakePlanRepositorySqlite::deleteIntakePlanById(int intake_plan_id) {
    if (!common::validation::validateId(intake_plan_id)) {
        return common::result::Result<void>::fail(common::result::ErrorCode::InvalidArgument,
            "intake_plan_id must be positive", "IntakePlanRepositorySqlite::deleteIntakePlanById");
    }

    auto stmt = db_.prepare("DELETE FROM intake_plans WHERE id = ?;");

    stmt.bindInt(1, intake_plan_id);

    int rc = stmt.step();

    if (db_.changes() == 0) {
        return common::result::Result<void>::fail(common::result::ErrorCode::NotFound,
            "No IntakePlan with id: " + std::to_string(intake_plan_id),
            "IntakePlanRepositorySqlite::deleteIntakePlanById");
    }
    return common::result::Result<void>::ok();
}

common::result::Result<void> IntakePlanRepositorySqlite::updateIntakePlan(
    const domain::IntakePlan& plan) {

    auto validated = validateIntakePlanForUpdate(plan);
    if (validated.isError()) {
        return common::result::Result<void>::fail(validated.error().code, validated.error().message,
            "IntakePlanRepositorySqlite::updateIntakePlan");
    }

    auto stmt = db_.prepare("UPDATE intake_plans SET patient_id = ?, medication_id = ?, dose = "
                            "?, time_of_day = ?, notes = ?  WHERE id = ?;");

    auto normalized = validated.value();

    stmt.bindInt(1, normalized.patient_id);
    stmt.bindInt(2, normalized.medication_id);
    stmt.bindText(3, normalized.dose);
    stmt.bindText(4, timeOfDayToDbString(normalized.time_of_day));
    if (common::validation::isEmptyOrBlank(normalized.notes)) {
        stmt.bindNull(5);
    } else {
        stmt.bindText(5, normalized.notes);
    }

    stmt.bindInt(6, normalized.id);

    int rc = stmt.step();

    switch (rc) {
        case SQLITE_DONE: {
            if (db_.changes() == 0) {
                return common::result::Result<void>::fail(common::result::ErrorCode::NotFound,
                    "No intake_plan with id: " + std::to_string(normalized.id),
                    "IntakePlanRepositorySqlite::updateIntakePlan");
            }
            return common::result::Result<void>::ok();
        }
        case SQLITE_CONSTRAINT_UNIQUE:
        case SQLITE_CONSTRAINT_PRIMARYKEY: {
            return common::result::Result<void>::fail(common::result::ErrorCode::Conflict,
                "duplicate (patient, medication, time_of_day)",
                "IntakePlanRepositorySqlite::updateIntakePlan");
        }
        case SQLITE_CONSTRAINT_FOREIGNKEY: {
            return common::result::Result<void>::fail(
                common::result::ErrorCode::ForeignKeyViolation,
                "referenced patient or medication missing",
                "IntakePlanRepositorySqlite::updateIntakePlan");
        }
        default: {
            return common::result::Result<void>::fail(common::result::ErrorCode::DatabaseError,
                "UPDATE failed", "IntakePlanRepositorySqlite::updateIntakePlan");
        }
    }
}

} // namespace infrastructure::persistence::sqlite
