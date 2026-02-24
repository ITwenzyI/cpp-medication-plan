/*
 * File: init_db
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//
#pragma once
#include "database.hpp"

namespace infrastructure::db {
void initDatabase(Database& db, const std::string& schemaPath);
} // namespace infrastructure::db
