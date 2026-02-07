#include <iostream>
#include "db/database.hpp"

int main() {
    try {
        Database db("data/medication.db");

        sqlite3* handle = db.get();
        auto stmt = db.prepare("SELECT 1;");
        int rc = stmt.stepRaw();

        db.execute("TEST;");


    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }


    return 0;
}
