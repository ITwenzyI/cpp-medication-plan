#pragma once
#include "domain/patient.hpp"
#include <vector>

namespace application::ports {
class IPatientRepository {
  public:
    virtual ~IPatientRepository() = default;

    virtual domain::Patient createPatient(const domain::Patient& p) = 0;

    [[nodiscard]] virtual std::vector<domain::Patient> getAllPatients() = 0;
};
} // namespace application::ports
