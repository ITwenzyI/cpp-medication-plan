#pragma once
#include "application/ports/medication_repository.hpp"
#include "common/result/result.hpp"
#include "infrastructure/db/database.hpp"
#include <string_view>
#include <vector>

namespace infrastructure::persistence::sqlite {
class MedicationRepositorySqlite : public application::ports::IMedicationRepository {
  public:
    explicit MedicationRepositorySqlite(infrastructure::db::Database& db);

    [[nodiscard]] common::result::Result<domain::Medication> createMedication(
        const domain::Medication& m) override;

    [[nodiscard]] common::result::Result<std::vector<domain::Medication>>
    getAllMedications() const override;

    [[nodiscard]] common::result::Result<domain::Medication> findMedicationById(
        int medication_id) const override;

    [[nodiscard]] common::result::Result<void> deleteMedicationById(int medication_id) override;

    [[nodiscard]] common::result::Result<void> updateMedicationName(
        int medication_id, std::string_view name) override;

    [[nodiscard]] common::result::Result<void> updateMedicationStrength(
        int medication_id, std::string_view new_strength) override;

    [[nodiscard]] common::result::Result<void> updateMedicationWarnings(
        int medication_id, std::string_view new_warnings) override;

  private:
    infrastructure::db::Database& db_;
};
} // namespace infrastructure::persistence::sqlite
