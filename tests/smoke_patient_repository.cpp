#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include <iostream>

int main() {

    infrastructure::db::Database db(":memory:");
    infrastructure::db::initDatabase(db, "schema.sql");
    std::cout << "Datenbank initialisiert\n";

    std::cout << "smoke test";

    return 0;
}