#pragma once
#include "domain/patient.hpp"
#include "infrastructure/db/database.hpp"
#include <vector>

class IPatientRepository {
  public:
    virtual ~IPatientRepository() = default;

    virtual Patient createPatient(const Patient& p) = 0;

    [[nodiscard]] virtual std::vector<Patient> getAllPatients() = 0;
};