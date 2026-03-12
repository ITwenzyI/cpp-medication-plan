#include "domain/patient.hpp"
#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include "infrastructure/persistence/sqlite/intake_plan_repository_sqlite.hpp"
#include "infrastructure/persistence/sqlite/medication_repository_sqlite.hpp"
#include "infrastructure/persistence/sqlite/patient_repository_sqlite.hpp"
#include "ui/cli/cli_app.hpp"
#include "ui/cli/printer/intake_plan_printer.hpp"
#include "ui/cli/printer/medication_printer.hpp"
#include "ui/cli/printer/patient_printer.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    try {
        // prepare the first-run filesystem state before opening SQLite.
        const auto dbPath = infrastructure::db::ensureDatabasePath("data/medication.db");
        infrastructure::db::Database db(dbPath.string());

        // load the schema from either the source tree or the build directory.
        infrastructure::db::initDatabase(db, infrastructure::db::findSchemaPath());
        std::cout << "Database initalized\n";
        ui::cli::CliApp printer{db};

        auto run = printer.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
