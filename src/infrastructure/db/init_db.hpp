/*
 * File: init_db
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//
#pragma once
#include "database.hpp"
#include <filesystem>
#include <string>

namespace infrastructure::db {
std::filesystem::path ensureDatabasePath(const std::filesystem::path& dbPath);

std::filesystem::path findSchemaPath();

void initDatabase(Database& db, const std::filesystem::path& schemaPath);
} // namespace infrastructure::db
