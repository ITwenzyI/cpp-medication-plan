#pragma once
#include "application/ports/patient_repository.hpp"

namespace infrastructure::persistence::sqlite {
class PatientRepositorySqlite : public application::ports::IPatientRepository {
  public:
    explicit PatientRepositorySqlite(db::Database& db);

    domain::Patient createPatient(const domain::Patient& p) override;

    [[nodiscard]] std::vector<domain::Patient> getAllPatients() override;

  private:
    db::Database& db_;
};
} // namespace infrastructure::persistence::sqlite
