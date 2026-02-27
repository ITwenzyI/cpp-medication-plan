#pragma once
#include "common/result/Result.hpp"
#include "domain/patient.hpp"
#include <string_view>
#include <vector>

namespace application::ports {
class IPatientRepository {
  public:
    virtual ~IPatientRepository() = default;

    virtual common::result::Result<domain::Patient> createPatient(const domain::Patient& p) = 0;

    virtual common::result::Result<std::vector<domain::Patient>> getAllPatients() const = 0;

    virtual common::result::Result<domain::Patient> findPatientById(int patient_id) const = 0;

    virtual common::result::Result<void> deletePatientById(int patient_id) = 0;

    virtual common::result::Result<void> updatePatientName(
        int patient_id, std::string_view name) = 0;

    virtual common::result::Result<void> updatePatientBirthdate(
        int patient_id, std::string_view new_birth_date) = 0;

    virtual common::result::Result<void> updatePatientNationality(
        int patient_id, std::string_view new_nationality) = 0;
};
} // namespace application::ports
