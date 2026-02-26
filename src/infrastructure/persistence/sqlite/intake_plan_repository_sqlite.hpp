#pragma once
#include "application/ports/intake_plan_repository.hpp"
#include "common/result/result.hpp"
#include "infrastructure/db/database.hpp"
#include <vector>

namespace infrastructure::persistence::sqlite {
class IntakePlanRepositorySqlite : public application::ports::IIntakePlanRepository {
  public:
    explicit IntakePlanRepositorySqlite(infrastructure::db::Database& db);

    [[nodiscard]] common::result::Result<domain::IntakePlan> createIntakePlan(
        const domain::IntakePlan& plan) override;

    [[nodiscard]] common::result::Result<std::vector<domain::IntakePlan>> getIntakePlansByPatientId(
        int patient_id) const override;

    [[nodiscard]] common::result::Result<std::vector<domain::IntakePlan>>
    getIntakePlansByMedicationId(int medication_id) const override;

    [[nodiscard]] common::result::Result<void> deleteIntakePlanById(int intake_plan_id) override;

    [[nodiscard]] common::result::Result<void> updateIntakePlan(
        const domain::IntakePlan& plan) override;

  private:
    infrastructure::db::Database& db_;
};
} // namespace infrastructure::persistence::sqlite
