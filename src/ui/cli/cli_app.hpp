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
    void cmdCreatePatient();
    void cmdListPatients();
    void cmdFindPatientById();
    void cmdDeletePatientById();
    void cmdUpdatePatientName();
    void cmdUpdatePatientBirthDate();
    void cmdUpdatePatientNationality();

    // Medications Commands
    void cmdCreateMedication();
    void cmdListMedications();
    void cmdFindMedicationById();
    void cmdDeleteMedicationById();
    void cmdUpdateMedicationName();
    void cmdUpdateMedicationStrength();
    void cmdUpdateMedicationWarnings();

    // IntakePlans Commands
    void cmdCreateIntakePlan();
    void cmdListIntakePlansByPatientId();
    void cmdListIntakePlansByMedicationId();
    void cmdDeleteIntakePlanById();
    void cmdUpdateIntakePlan();

    // Utility
    void waitForEnter() const;
};

} // namespace ui::cli