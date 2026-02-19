/*
 * File: patient_repository
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//

#pragma once

#include "domain/patient.hpp"
#include "infrastructure/db/database.hpp"
#include <vector>

class PatientRepository {
  public:
    explicit PatientRepository(Database& db);

    Patient createPatient(const Patient& p);

    [[nodiscard]] std::vector<Patient> getAllPatients();

  private:
    Database& db_;
};
