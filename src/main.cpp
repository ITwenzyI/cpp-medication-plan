#include <iostream>
#include "db/database.hpp"
#include "db/init_db.hpp"

int main() {
    try {
        Database db("data/medication.db");

        initDatabase(db, "sql/schema.sql");
        std::cout << "Datenbank initialisiert\n";





    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }


    return 0;
}
