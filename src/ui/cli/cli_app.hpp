#pragma once
#include "common/result/result.hpp"
#include "infrastructure/db/database.hpp"
#include "infrastructure/persistence/sqlite/intake_plan_repository_sqlite.hpp"
#include "infrastructure/persistence/sqlite/medication_repository_sqlite.hpp"
#include "infrastructure/persistence/sqlite/patient_repository_sqlite.hpp"

namespace ui::cli {

class CliApp {
  public:
    explicit CliApp(infrastructure::db::Database& db);

    [[nodiscard]] int run();

  private:
    infrastructure::db::Database& db_;
    infrastructure::persistence::sqlite::PatientRepositorySqlite patientRepo_;
    infrastructure::persistence::sqlite::MedicationRepositorySqlite medicationRepo_;
    infrastructure::persistence::sqlite::IntakePlanRepositorySqlite intakePlanRepo_;

    // Menu
    void mainMenuLoop();
    void showMainMenu() const;
    void patientsMenuLoop();
    void showPatientsMenu() const;
    void medicationsMenuLoop();
    void showMedicationsMenu() const;
    void intakePlansMenuLoop();
    void showIntakePlansMenu() const;

    // Patient Commands
    void cmdListPatients();
    void cmdCreatePatient();
    void cmdFindPatientById();

    // Medications Commands
    void cmdListMedications();
    void cmdCreateMedication();

    // IntakePlans Commands
    void cmdListIntakePlans();
    void cmdCreateIntakePlan();

    // Utility
    void waitForEnter() const;
    common::result::Result<int> readInt(std::string_view prompt, int min, int max) const;

    // Error Message
    void printError(const common::result::AppError&) const;
};

} // namespace ui::cli