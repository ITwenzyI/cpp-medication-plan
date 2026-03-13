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
        const domain::IntakePlan& new_plan) override;

    [[nodiscard]] common::result::Result<std::vector<domain::IntakePlan>> getIntakePlansByPatientId(
        int new_patient_id) const override;

    [[nodiscard]] common::result::Result<std::vector<domain::IntakePlan>>
    getIntakePlansByMedicationId(int new_medication_id) const override;

    [[nodiscard]] common::result::Result<domain::IntakePlan> findIntakePlanById(
        int id) const override;

    [[nodiscard]] common::result::Result<void> deleteIntakePlanById(
        int new_intake_plan_id) override;

    [[nodiscard]] common::result::Result<void> updateIntakePlan(
        const domain::IntakePlan& new_plan) override;

  private:
    infrastructure::db::Database& db_;
};
} // namespace infrastructure::persistence::sqlite
