#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include "ui/cli/cli_app.hpp"

#include <iostream>
#include <stdexcept>

int main() {
    try {
        // prepare the first-run filesystem state before opening SQLite.
        const auto dbPath = infrastructure::db::ensureDatabasePath("data/medication.db");
        infrastructure::db::Database db(dbPath.string());

        // load the schema from either the source tree or the build directory.
        infrastructure::db::initDatabase(db, infrastructure::db::findSchemaPath());
        ui::cli::CliApp printer{db};

        auto run = printer.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
