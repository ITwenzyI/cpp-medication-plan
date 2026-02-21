#pragma once
#include "application/ports/patient_repository.hpp"
#include "infrastructure/db/database.hpp"

namespace infrastructure::persistence::sqlite {
class PatientRepositorySqlite : public application::ports::IPatientRepository {
  public:
    explicit PatientRepositorySqlite(infrastructure::db::Database& db);

    domain::Patient createPatient(const domain::Patient& p) override;

    [[nodiscard]] std::vector<domain::Patient> getAllPatients() override;

    [[nodiscard]] common::result::Result<domain::Patient> findPatientById(int patient_id) override;

  private:
    infrastructure::db::Database& db_;
};
} // namespace infrastructure::persistence::sqlite
