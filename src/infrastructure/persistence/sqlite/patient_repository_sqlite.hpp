#pragma once
#include "application/ports/patient_repository.hpp"
#include "common/result/result.hpp"
#include "infrastructure/db/database.hpp"
#include <string_view>
#include <vector>

namespace infrastructure::persistence::sqlite {
class PatientRepositorySqlite : public application::ports::IPatientRepository {
  public:
    explicit PatientRepositorySqlite(infrastructure::db::Database& db);

    [[nodiscard]] common::result::Result<domain::Patient> createPatient(
        const domain::Patient& p) override;

    [[nodiscard]] common::result::Result<std::vector<domain::Patient>> getAllPatients() override;

    [[nodiscard]] common::result::Result<domain::Patient> findPatientById(int patient_id) override;

    [[nodiscard]] common::result::Result<void> deletePatientById(int patient_id) override;

    [[nodiscard]] common::result::Result<void> updatePatientName(
        int patient_id, std::string_view name);

  private:
    infrastructure::db::Database& db_;
};
} // namespace infrastructure::persistence::sqlite
