/*
 * File: init_db
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//

#ifndef MEDICATION_PLAN_INIT_DB_HPP
#define MEDICATION_PLAN_INIT_DB_HPP
#include "database.hpp"

namespace infrastructure::db {
void initDatabase(Database& db, const std::string& schemaPath);
} // namespace infrastructure::db

#endif //MEDICATION_PLAN_INIT_DB_HPP
