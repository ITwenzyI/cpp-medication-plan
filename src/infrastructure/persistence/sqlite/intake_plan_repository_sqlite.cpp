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