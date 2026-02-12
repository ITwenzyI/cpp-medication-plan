/*
 * File: patient_repository
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//

#pragma once

#include <vector>
#include "../model/patient.hpp"
#include "../db/database.hpp"

class PatientRepository {
public:
    explicit PatientRepository(Database& db);

    Patient createPatient(const Patient& p);

    std::vector<Patient> getAllPatients();

private:
    Database& db_;
};