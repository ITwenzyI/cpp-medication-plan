#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include <iostream>

void expect(bool condition, std::string_view message) {
    if (condition) {
        return;
    }
    std::cerr << "TEST FAILED: " << message << "\n";
    std::exit(1);
}

int main() {

    infrastructure::db::Database db(":memory:");
    infrastructure::db::initDatabase(db, "schema.sql");
    std::cout << "Datenbank initialisiert\n";

    std::cout << "smoke test";

    return 0;
}