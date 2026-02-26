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
}

common::result::Result<std::vector<domain::Medication>>
MedicationRepositorySqlite::getAllMedications() {
}

common::result::Result<domain::Medication> MedicationRepositorySqlite::findMedicationById(
    int medication_id) {
}

common::result::Result<void> MedicationRepositorySqlite::deleteMedicationById(int medication_id) {
}

common::result::Result<void> MedicationRepositorySqlite::updateMedicationName(
    int medication_id, std::string_view name) {
}

common::result::Result<void> MedicationRepositorySqlite::updateMedicationStrength(
    int medication_id, std::string_view new_strength) {
}

common::result::Result<void> MedicationRepositorySqlite::updateMedicationWarnings(
    int medication_id, std::string_view new_warnings) {
}

} // namespace infrastructure::persistence::sqlite