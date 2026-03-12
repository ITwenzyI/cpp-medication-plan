/*
 * File: init_db
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//

#include "init_db.hpp"
#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace infrastructure::db {
std::filesystem::path ensureDatabasePath(const std::filesystem::path& dbPath) {
    if (dbPath.empty()) {
        throw std::runtime_error("Database path must not be empty.");
    }

    // handles separators such as '/' and '\' correctly on different operating systems.
    const auto parentPath = dbPath.parent_path();
    if (parentPath.empty()) {
        return dbPath;
    }

    // SQLite can create the database file itself, but not missing parent directories.
    // use of std::error_code here instead of letting create_directories throw directly.
    // use is, to convert filesystem errors into own runtime_error with custom message.
    std::error_code ec;
    std::filesystem::create_directories(parentPath, ec);
    if (ec) {
        throw std::runtime_error(
            "Unable to create database directory '" + parentPath.string() + "': " + ec.message());
    }

    return dbPath;
}

std::filesystem::path findSchemaPath() {
    // current_path() returns the process working directory, which may be the repo root
    // or the build directory depending on how the executable was started.
    std::error_code ec;
    const auto currentPath = std::filesystem::current_path(ec);
    if (ec) {
        throw std::runtime_error("Unable to determine current working directory: " + ec.message());
    }

    // support both runs from the repository root and from the build directory.
    // checks a small set of likely locations instead of hard-coding a single relative path.
    const std::array<std::filesystem::path, 4> candidates = {
        currentPath / "schema.sql",
        currentPath / "sql" / "schema.sql",
        currentPath.parent_path() / "schema.sql",
        currentPath.parent_path() / "sql" / "schema.sql",
    };

    for (const auto& candidate : candidates) {
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
    }

    // if none of the candidates exists, include every checked path in the error message.
    std::ostringstream error;
    error << "Schema file not found. Checked:";
    for (const auto& candidate : candidates) {
        error << "\n - " << candidate.string();
    }

    throw std::runtime_error(error.str());
}

void initDatabase(Database& db, const std::filesystem::path& schemaPath) {
    std::ifstream file(schemaPath);
    if (!file) {
        throw std::runtime_error("Schema file not found: " + schemaPath.string());
    }

    std::string sql;

    std::string line;
    while (std::getline(file, line)) {
        sql.append(line);
        // preserve line breaks so the schema is executed exactly as authored.
        sql.push_back('\n');
    }
    db.execute(sql.c_str());
}
} // namespace infrastructure::db
