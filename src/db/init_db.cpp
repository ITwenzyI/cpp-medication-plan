/*
 * File: init_db
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//

#include "init_db.hpp"
#include <fstream>

void initDatabase(Database& db, const std::string& schemaPath) {
    std::ifstream file(schemaPath);
    if (!file) {
        throw std::runtime_error("Schema Datei nicht gefunden.");
    }

    std::string sql;

    std::string line;
    while (std::getline(file, line)) {
        sql.append(line);
        // Damit keine Zeilenumbrüche verloren gehen
        sql.push_back('\n');
    }
    db.execute(sql.c_str());
}