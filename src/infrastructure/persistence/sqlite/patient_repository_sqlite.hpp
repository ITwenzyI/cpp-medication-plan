#pragma once
#include "application/ports/patient_repository.hpp"

class PatientRepositorySqlite : public IPatientRepository {
  public:
    explicit PatientRepositorySqlite(Database& db);

    Patient createPatient(const Patient& p) override;

    [[nodiscard]] std::vector<Patient> getAllPatients() override;

  private:
    Database& db_;
};