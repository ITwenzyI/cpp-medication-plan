#pragma once
#include "common/result/result.hpp"
#include "domain/intake_plan.hpp"
#include <vector>

namespace application::ports {
class IIntakePlanRepository {
  public:
    virtual ~IIntakePlanRepository() = default;

    virtual common::result::Result<domain::IntakePlan> createIntakePlan(
        const domain::IntakePlan& new_plan) = 0;

    virtual common::result::Result<std::vector<domain::IntakePlan>> getIntakePlansByPatientId(
        int new_patient_id) const = 0;

    virtual common::result::Result<std::vector<domain::IntakePlan>> getIntakePlansByMedicationId(
        int new_medication_id) const = 0;

    virtual common::result::Result<domain::IntakePlan> findIntakePlanById(int id) const = 0;

    virtual common::result::Result<void> deleteIntakePlanById(int new_intake_plan_id) = 0;

    virtual common::result::Result<void> updateIntakePlan(const domain::IntakePlan& new_plan) = 0;
};
} // namespace application::ports
