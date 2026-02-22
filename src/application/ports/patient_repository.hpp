#pragma once
#include "common/result/Result.hpp"
#include "domain/patient.hpp"
#include <string_view>
#include <vector>

namespace application::ports {
class IPatientRepository {
  public:
    virtual ~IPatientRepository() = default;

    virtual domain::Patient createPatient(const domain::Patient& p) = 0;

    [[nodiscard]] virtual std::vector<domain::Patient> getAllPatients() = 0;

    virtual common::result::Result<domain::Patient> findPatientById(int patient_id) = 0;

    virtual common::result::Result<void> deletePatientById(int patient_id) = 0;

    virtual common::result::Result<void> updatePatientName(
        int patient_id, std::string_view name) = 0;
};
} // namespace application::ports
