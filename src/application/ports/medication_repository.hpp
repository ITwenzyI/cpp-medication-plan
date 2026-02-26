#pragma once
#include "common/result/result.hpp"
#include "domain/medication.hpp"
#include <string_view>
#include <vector>

namespace application::ports {
class IMedicationRepository {
  public:
    virtual ~IMedicationRepository() = default;

    virtual common::result::Result<domain::Medication> createMedication(
        const domain::Medication& m) = 0;

    virtual common::result::Result<std::vector<domain::Medication>> getAllMedications() = 0;

    virtual common::result::Result<domain::Medication> findMedicationById(int medication_id) = 0;

    virtual common::result::Result<void> deleteMedicationById(int medication_id) = 0;

    virtual common::result::Result<void> updateMedicationName(
        int medication_id, std::string_view name) = 0;

    virtual common::result::Result<void> updateMedicationStrength(
        int medication_id, std::string_view new_strength) = 0;

    virtual common::result::Result<void> updateMedicationWarnings(
        int medication_id, std::string_view new_warnings) = 0;
};
} // namespace application::ports
