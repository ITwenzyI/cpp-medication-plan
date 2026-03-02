/*
 * File: init_db
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//

#include "init_db.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

namespace infrastructure::db {
void initDatabase(Database& db, const std::string& schemaPath) {
    std::ifstream file(schemaPath);
    if (!file) {
        throw std::runtime_error("Schema file not found.");
    }

    std::string sql;

    std::string line;
    while (std::getline(file, line)) {
        sql.append(line);
        // To ensure that no line breaks are lost
        sql.push_back('\n');
    }
    db.execute(sql.c_str());
}
} // namespace infrastructure::db
