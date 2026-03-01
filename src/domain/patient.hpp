/*
 * File: patient
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//

#pragma once
#include <string>

namespace domain {

enum class Nationality { DE, US, GB, FR, IT, ES, CN, IN, JP, TR };
struct Patient {
    int id;
    std::string name;
    std::string birth_date;
    Nationality nationality;
};
} // namespace domain
