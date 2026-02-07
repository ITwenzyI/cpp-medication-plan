#include <iostream>
#include "db/database.hpp"

int main() {
    try {
        Database db("data/medication.db");

        sqlite3* handle = db.get();
        auto stmt = db.prepare("SELECT 1;");
        int rc = stmt.stepRaw();

        // Little test
        if (rc == SQLITE_ROW) {
            std::cout << "ROW\n";
        }
        rc = stmt.stepRaw();
        if (rc == SQLITE_DONE) {
            std::cout << "DONE";
        }


    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }


    return 0;
}
