#include "medication_repository_sqlite.hpp"
#include "common/validation/birth_date_validation.hpp"
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
} // namespace infrastructure::persistence::sqlite