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
struct Patient {
    int id;
    std::string name;
    std::string birth_date;
    std::string nationality;
};
} // namespace domain
