#include "common/result/result.hpp"
#include "domain/intake_plan.hpp"
#include "infrastructure/db/database.hpp"
#include "infrastructure/db/init_db.hpp"
#include "infrastructure/persistence/sqlite/intake_plan_repository_sqlite.hpp"
#include <iostream>

static void expect(bool condition, std::string_view message) {
    if (condition) {
        return;
    }
    std::cerr << "INTAKE_PLAN SMOKE TEST FAILED: " << message << "\n";
    std::exit(1);
}