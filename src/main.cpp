#include <iostream>
#include "db/database.hpp"
#include "db/init_db.hpp"

int main() {
    try {
        Database db("data/medication.db");

        initDatabase(db, "sql/schema.sql");




    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }


    return 0;
}
