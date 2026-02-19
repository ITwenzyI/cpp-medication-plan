/*
 * File: patient_repository
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//

#pragma once

#include "../db/database.hpp"
#include "../model/patient.hpp"
#include <vector>

class PatientRepository {
  public:
    explicit PatientRepository(Database& db);

    Patient createPatient(const Patient& p);

    [[nodiscard]] std::vector<Patient> getAllPatients();

  private:
    Database& db_;
};