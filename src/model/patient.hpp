/*
 * File: patient
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//

#ifndef MEDICATION_PLAN_PATIENT_HPP
#define MEDICATION_PLAN_PATIENT_HPP
#include <string>

struct Patient {
    int id;
    std::string name;
    std::string birth_date;
    std::string nationality;

};

#endif //MEDICATION_PLAN_PATIENT_HPP