#include "cli_app.hpp"
#include "common/result/result.hpp"
#include "domain/intake_plan.hpp"
#include "domain/medication.hpp"
#include "domain/patient.hpp"
#include "error_renderer.hpp"
#include "infrastructure/persistence/sqlite/nationality_mapper_sqlite.hpp"
#include "input.hpp"
#include "printer/intake_plan_printer.hpp"
#include "printer/medication_printer.hpp"
#include "printer/patient_printer.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <string_view>

namespace ui::cli {
CliApp::CliApp(infrastructure::db::Database& db)
    : db_(db), patientRepo_(db), medicationRepo_(db), intakePlanRepo_(db) {
}

int CliApp::run() {
    mainMenuLoop();
    return 0;
}

// Menu

void CliApp::mainMenuLoop() {
    while (true) {
        showMainMenu();

        std::string_view prompt = "Enter your Menu choice: ";
        auto user_choice = input::readMenuChoice(prompt, 0, 3);

        while (user_choice.isError()) {
            ErrorRenderer::printErrorMessage(user_choice.error(), "CliApp::mainMenuLoop");
            user_choice = input::readMenuChoice(prompt, 0, 3);
        }

        switch (user_choice.value()) {
            case 0:
                return;
            case 1:
                patientsMenuLoop();
                break;
            case 2:
                medicationsMenuLoop();
                break;
            case 3:
                intakePlansMenuLoop();
                break;
        }
    }
}

void CliApp::showMainMenu() const {
    std::cout << "===== Medication Main Menu =====" << "\n";
    std::cout << "1. Patients Menu" << "\n";
    std::cout << "2. Medications Menu" << "\n";
    std::cout << "3. IntakePlans Menu" << "\n";
    std::cout << "0. Exit" << "\n";
}

void CliApp::patientsMenuLoop() {
    while (true) {
        showPatientsMenu();

        std::string_view prompt = "Choice: ";
        auto user_choice = input::readMenuChoice(prompt, 0, 7);

        while (user_choice.isError()) {
            ErrorRenderer::printErrorMessage(user_choice.error(), "CliApp::patientsMenuLoop");
            user_choice = input::readMenuChoice(prompt, 0, 7);
        }

        switch (user_choice.value()) {
            case 0:
                return;
            case 1:
                cmdCreatePatient();
                break;
            case 2:
                cmdListPatients();
                break;
            case 3:
                cmdFindPatientById();
                break;
            case 4:
                cmdDeletePatientById();
                break;
            case 5:
                cmdUpdatePatientName();
                break;
            case 6:
                cmdUpdatePatientBirthDate();
                break;
            case 7:
                cmdUpdatePatientNationality();
                break;
        }
    }
}

void CliApp::showPatientsMenu() const {
    std::cout << "===== Patient Menu =====" << "\n";
    std::cout << "1. Create Patient" << "\n";
    std::cout << "2. List all Patients" << "\n";
    std::cout << "3. Find Patient by ID" << "\n";
    std::cout << "4. Delete Patient by ID" << "\n";
    std::cout << "5. Update Patient Name" << "\n";
    std::cout << "6. Update Patient BirthDate" << "\n";
    std::cout << "7. Update Patient Nationality" << "\n";
    std::cout << "0. Back" << "\n";
}

void CliApp::medicationsMenuLoop() {
    while (true) {
        showMedicationsMenu();

        std::string_view prompt = "Choice: ";
        auto user_choice = input::readMenuChoice(prompt, 0, 7);

        while (user_choice.isError()) {
            ErrorRenderer::printErrorMessage(user_choice.error(), "CliApp::medicationsMenuLoop");
            user_choice = input::readMenuChoice(prompt, 0, 7);
        }

        switch (user_choice.value()) {
            case 0:
                return;
            case 1:
                cmdCreateMedication();
                break;
            case 2:
                cmdListMedications();
                break;
            case 3:
                cmdFindMedicationById();
                break;
            case 4:
                cmdDeleteMedicationById();
                break;
            case 5:
                cmdUpdateMedicationName();
                break;
            case 6:
                cmdUpdateMedicationStrength();
                break;
            case 7:
                cmdUpdateMedicationWarnings();
                break;
        }
    }
}

void CliApp::showMedicationsMenu() const {
    std::cout << "===== Medication Menu =====" << "\n";
    std::cout << "1. Create Medication" << "\n";
    std::cout << "2. List all Medications" << "\n";
    std::cout << "3. Find Medication by ID" << "\n";
    std::cout << "4. Delete Medication by ID" << "\n";
    std::cout << "5. Update Medication Name" << "\n";
    std::cout << "6. Update Medication Strength" << "\n";
    std::cout << "7. Update Medication Warnings" << "\n";
    std::cout << "0. Back" << "\n";
}

void CliApp::intakePlansMenuLoop() {
    while (true) {
        showIntakePlansMenu();

        std::string_view prompt = "Choice: ";
        auto user_choice = ui::cli::input::readMenuChoice(prompt, 0, 4);

        while (user_choice.isError()) {
            ErrorRenderer::printErrorMessage(user_choice.error(), "CliApp::intakePlansMenuLoop");
            user_choice = input::readMenuChoice(prompt, 0, 4);
        }

        switch (user_choice.value()) {
            case 0:
                return;
            case 1:
                cmdCreateIntakePlan();
                break;
            case 2:
                cmdListIntakePlansByPatientId();
                break;
            case 3:
                cmdListIntakePlansByMedicationId();
                break;
            case 4:
                cmdDeleteIntakePlanById();
                break;
        }
    }
}

void CliApp::showIntakePlansMenu() const {
    std::cout << "===== IntakePlan Menu =====" << "\n";
    std::cout << "1. Create IntakePlan" << "\n";
    std::cout << "2. List IntakePlans by PatientID" << "\n";
    std::cout << "3. List IntakePlans by MedicationID" << "\n";
    std::cout << "4. Delete IntakePlan by ID" << "\n";
    std::cout << "0. Back" << "\n";
}

// Patient Commands

void CliApp::cmdCreatePatient() {
    std::cout << "===== Create Patient =====" << "\n\n";

    auto name = input::readNonEmpty("Enter patient name: ");
    if (handleResultError(name, "CliApp::cmdCreatePatient"))
        return;

    auto birth_date = input::readOptionalBirthDate("Enter patient birthdate (YYYY-MM-DD): ");
    if (handleResultError(birth_date, "CliApp::cmdCreatePatient"))
        return;

    auto nationality =
        input::readOptionalNationality("Enter patient nationality (DE, US, GB, FR, ...): ");
    if (handleResultError(nationality, "CliApp::cmdCreatePatient"))
        return;

    domain::Patient patient;

    patient.id = 0;
    patient.name = name.value();
    auto birthDateOpt = birth_date.value();
    if (birthDateOpt) {
        patient.birth_date = birthDateOpt.value();
    }
    auto nationalityOpt = nationality.value();
    if (nationalityOpt) {
        patient.nationality = nationalityOpt.value();
    }

    auto result = patientRepo_.createPatient(patient);
    if (handleResultError(result, "CliApp::cmdCreatePatient"))
        return;
    std::cout << "Patient created successfully (ID: " << result.value().id << ").\n";
    waitForEnter();
}

void CliApp::cmdListPatients() {

    auto all_patients = patientRepo_.getAllPatients();

    if (handleResultError(all_patients, "CliApp::cmdListPatients"))
        return;

    if (all_patients.value().empty()) {
        std::cout << "No patients found.";
        waitForEnter();
        return;
    }

    printer::printPatientsTable(all_patients.value());
    waitForEnter();
}

void CliApp::cmdFindPatientById() {
    std::cout << "===== Find Patient By ID =====" << "\n\n";

    auto id = input::readInt("Enter patient ID: ");
    if (handleResultError(id, "CliApp::cmdFindPatientById"))
        return;

    auto found_patient = patientRepo_.findPatientById(id.value());

    if (handleResultError(found_patient, "CliApp::cmdFindPatientById"))
        return;

    printer::printPatientDetails(found_patient.value());
    waitForEnter();
}

void CliApp::cmdDeletePatientById() {
    std::cout << "===== Delete Patient By ID =====" << "\n\n";

    auto id = input::readInt("Enter patient ID: ");
    if (handleResultError(id, "CliApp::cmdDeletePatientById"))
        return;

    auto found_patient = patientRepo_.findPatientById(id.value());

    if (handleResultError(found_patient, "CliApp::cmdDeletePatientById"))
        return;

    auto user_confirm = input::confirm("Delete patient with ID " + std::to_string(id.value()));

    if (handleResultError(user_confirm, "CliApp::cmdDeletePatientById"))
        return;

    if (!user_confirm.value()) {
        std::cout << "Patient with ID: " << id.value() << " was not deleted.\n";
        waitForEnter();
        return;
    }

    auto deleted_patient = patientRepo_.deletePatientById(id.value());
    if (handleResultError(deleted_patient, "CliApp::cmdDeletePatientById"))
        return;

    std::cout << "Deleted patient with ID: " << id.value() << ".\n";
    waitForEnter();
}

void CliApp::cmdUpdatePatientName() {
    std::cout << "===== Update Patient Name =====" << "\n\n";

    auto id = input::readInt("Enter patient ID: ");
    if (handleResultError(id, "CliApp:cmdUpdatePatientName"))
        return;

    auto found_patient = patientRepo_.findPatientById(id.value());

    if (handleResultError(found_patient, "CliApp:cmdUpdatePatientName"))
        return;

    auto old_name_patient = found_patient.value().name;
    auto new_name_patient = input::readNonEmpty("New patient name: ");
    if (handleResultError(new_name_patient, "CliApp:cmdUpdateMedicationStrength"))
        return;

    auto user_confirm = input::confirm("Update patient name with ID " + std::to_string(id.value()));

    if (handleResultError(user_confirm, "CliApp:cmdUpdatePatientName"))
        return;

    if (!user_confirm.value()) {
        std::cout << "Patient with ID " << id.value() << " was not updated.\n";
        waitForEnter();
        return;
    }

    auto updated_patient = patientRepo_.updatePatientName(id.value(), new_name_patient.value());
    if (handleResultError(updated_patient, "CliApp:cmdUpdatePatientName"))
        return;

    std::cout << "Patient " << id.value() << " updated.\n"
              << "Old name: " << old_name_patient << "\n"
              << "New name: " << new_name_patient.value() << "\n";
    waitForEnter();
}

void CliApp::cmdUpdatePatientBirthDate() {
    std::cout << "===== Update Patient BirthDate =====" << "\n\n";

    auto id = input::readInt("Enter patient ID: ");
    if (handleResultError(id, "CliApp::cmdUpdatePatientBirthDate"))
        return;

    auto found_patient = patientRepo_.findPatientById(id.value());

    if (handleResultError(found_patient, "CliApp::cmdUpdatePatientBirthDate"))
        return;

    auto old_birth_date_patient = found_patient.value().birth_date;
    auto input_new_birth_date_patient =
        input::readOptionalBirthDate("New patient birthdate (YYYY-MM-DD): ");
    if (handleResultError(input_new_birth_date_patient, "CliApp::cmdUpdatePatientBirthDate"))
        return;
    auto new_birth_date_patient = input_new_birth_date_patient.value();

    auto user_confirm =
        input::confirm("Update patient birthdate with ID " + std::to_string(id.value()));

    if (handleResultError(user_confirm, "CliApp::cmdUpdatePatientBirthDate"))
        return;

    if (!user_confirm.value()) {
        std::cout << "Patient with ID " << id.value() << " was not updated.\n";
        waitForEnter();
        return;
    }

    const std::string new_birth_date_value = new_birth_date_patient.value_or("");
    auto updated_patient = patientRepo_.updatePatientBirthdate(id.value(), new_birth_date_value);
    if (handleResultError(updated_patient, "CliApp::cmdUpdatePatientBirthDate"))
        return;

    std::cout << "Patient " << id.value() << " updated.\n"
              << "Old birthdate: "
              << (old_birth_date_patient.empty() ? "-" : old_birth_date_patient) << "\n"
              << "New birthdate: " << (new_birth_date_value.empty() ? "-" : new_birth_date_value)
              << "\n";
    waitForEnter();
}

void CliApp::cmdUpdatePatientNationality() {
    std::cout << "===== Update Patient Nationality =====" << "\n\n";

    auto id = input::readInt("Enter patient ID: ");
    if (handleResultError(id, "CliApp::cmdUpdatePatientNationality"))
        return;

    auto found_patient = patientRepo_.findPatientById(id.value());

    if (handleResultError(found_patient, "CliApp::cmdUpdatePatientNationality"))
        return;

    auto old_nationality_patient = found_patient.value().nationality;

    auto input_new_nationality_patient =
        input::readOptionalNationality("New patient nationality (DE, US, GB, FR, ...): ");
    if (handleResultError(input_new_nationality_patient, "CliApp::cmdUpdatePatientNationality"))
        return;
    auto new_nationality_patient = input_new_nationality_patient.value();

    auto user_confirm =
        input::confirm("Update patient nationality with ID " + std::to_string(id.value()));

    if (handleResultError(user_confirm, "CliApp::cmdUpdatePatientNationality"))
        return;

    if (!user_confirm.value()) {
        std::cout << "Patient with ID " << id.value() << " was not updated.\n";
        waitForEnter();
        return;
    }

    const std::string new_nationality_value = new_nationality_patient.has_value()
        ? infrastructure::persistence::sqlite::nationalityToDbString(
              new_nationality_patient.value())
        : "";
    const std::string old_nationality_value = old_nationality_patient.has_value()
        ? infrastructure::persistence::sqlite::nationalityToDbString(
              old_nationality_patient.value())
        : "";

    auto updated_patient = patientRepo_.updatePatientNationality(id.value(), new_nationality_value);
    if (handleResultError(updated_patient, "CliApp::cmdUpdatePatientNationality"))
        return;

    std::cout << "Patient " << id.value() << " updated.\n"
              << "Old nationality: "
              << (old_nationality_value.empty() ? "-" : old_nationality_value) << "\n"
              << "New nationality: "
              << (new_nationality_value.empty() ? "-" : new_nationality_value) << "\n";
    waitForEnter();
}

// Medication Commands

void CliApp::cmdCreateMedication() {
    std::cout << "===== Create Medication =====" << "\n\n";

    auto name = input::readNonEmpty("Enter medication name: ");
    if (handleResultError(name, "CliApp::cmdCreateMedication"))
        return;

    auto strength = input::readNonEmpty("Enter medication strength: ");
    if (handleResultError(strength, "CliApp::cmdCreateMedication"))
        return;

    auto warnings = input::readOptionalString("Enter medication warnings (optional): ");

    domain::Medication medication;

    medication.id = 0;
    medication.name = name.value();
    medication.strength = strength.value();
    if (warnings.has_value()) {
        medication.warnings = warnings.value();
    }

    auto result = medicationRepo_.createMedication(medication);
    if (handleResultError(result, "CliApp::cmdCreateMedication"))
        return;
    std::cout << "Medication created successfully (ID: " << result.value().id << ").\n";
    waitForEnter();
}

void CliApp::cmdListMedications() {

    auto all_medications = medicationRepo_.getAllMedications();

    if (handleResultError(all_medications, "CliApp::cmdListMedications"))
        return;

    if (all_medications.value().empty()) {
        std::cout << "No medications found.";
        waitForEnter();
        return;
    }

    printer::printMedicationsTable(all_medications.value());
    waitForEnter();
}

void CliApp::cmdFindMedicationById() {
    std::cout << "===== Find Medication By ID =====" << "\n\n";

    auto id = input::readInt("Enter medication ID: ");
    if (handleResultError(id, "CliApp::cmdFindMedicationById"))
        return;

    auto found_medication = medicationRepo_.findMedicationById(id.value());

    if (handleResultError(found_medication, "CliApp::cmdFindMedicationById"))
        return;

    printer::printMedicationDetails(found_medication.value());
    waitForEnter();
}

void CliApp::cmdDeleteMedicationById() {
    std::cout << "===== Delete Medication By ID =====" << "\n\n";

    auto id = input::readInt("Enter medication ID: ");
    if (handleResultError(id, "CliApp::cmdDeleteMedicationById"))
        return;

    auto found_medication = medicationRepo_.findMedicationById(id.value());

    if (handleResultError(found_medication, "CliApp::cmdDeleteMedicationById"))
        return;

    auto user_confirm = input::confirm("Delete medication with ID " + std::to_string(id.value()));

    if (handleResultError(user_confirm, "CliApp::cmdDeleteMedicationById"))
        return;

    if (!user_confirm.value()) {
        std::cout << "Medication with ID: " << id.value() << " was not deleted.\n";
        waitForEnter();
        return;
    }

    auto deleted_medication = medicationRepo_.deleteMedicationById(id.value());
    if (handleResultError(deleted_medication, "CliApp::cmdDeleteMedicationById"))
        return;

    std::cout << "Deleted medication with ID: " << id.value() << ".\n";
    waitForEnter();
}

void CliApp::cmdUpdateMedicationName() {
    std::cout << "===== Update Medication Name =====" << "\n\n";

    auto id = input::readInt("Enter medication ID: ");
    if (handleResultError(id, "CliApp:cmdUpdateMedicationName"))
        return;

    auto found_medication = medicationRepo_.findMedicationById(id.value());

    if (handleResultError(found_medication, "CliApp:cmdUpdateMedicationName"))
        return;

    auto old_name_medication = found_medication.value().name;
    auto new_name_medication = input::readNonEmpty("New medication name: ");
    if (handleResultError(new_name_medication, "CliApp:cmdUpdateMedicationStrength"))
        return;

    auto user_confirm =
        input::confirm("Update medication name with ID " + std::to_string(id.value()));

    if (handleResultError(user_confirm, "CliApp:cmdUpdateMedicationName"))
        return;

    if (!user_confirm.value()) {
        std::cout << "Medication with ID " << id.value() << " was not updated.\n";
        waitForEnter();
        return;
    }

    auto updated_medication =
        medicationRepo_.updateMedicationName(id.value(), new_name_medication.value());
    if (handleResultError(updated_medication, "CliApp:cmdUpdateMedicationName"))
        return;

    std::cout << "Medication " << id.value() << " updated.\n"
              << "Old name: " << old_name_medication << "\n"
              << "New name: " << new_name_medication.value() << "\n";
    waitForEnter();
}

void CliApp::cmdUpdateMedicationStrength() {
    std::cout << "===== Update Medication Strength =====" << "\n\n";

    auto id = input::readInt("Enter medication ID: ");
    if (handleResultError(id, "CliApp:cmdUpdateMedicationStrength"))
        return;

    auto found_medication = medicationRepo_.findMedicationById(id.value());

    if (handleResultError(found_medication, "CliApp:cmdUpdateMedicationStrength"))
        return;

    auto old_strength_medication = found_medication.value().strength;
    auto new_strength_medication = input::readNonEmpty("New medication strength: ");
    if (handleResultError(new_strength_medication, "CliApp:cmdUpdateMedicationStrength"))
        return;

    auto user_confirm =
        input::confirm("Update medication strength with ID " + std::to_string(id.value()));

    if (handleResultError(user_confirm, "CliApp:cmdUpdateMedicationStrength"))
        return;

    if (!user_confirm.value()) {
        std::cout << "Medication with ID " << id.value() << " was not updated.\n";
        waitForEnter();
        return;
    }

    auto updated_medication =
        medicationRepo_.updateMedicationStrength(id.value(), new_strength_medication.value());
    if (handleResultError(updated_medication, "CliApp:cmdUpdateMedicationStrength"))
        return;

    std::cout << "Medication " << id.value() << " updated.\n"
              << "Old strength: " << old_strength_medication << "\n"
              << "New strength: " << new_strength_medication.value() << "\n";
    waitForEnter();
}

void CliApp::cmdUpdateMedicationWarnings() {
    std::cout << "===== Update Medication Warnings =====" << "\n\n";

    auto id = input::readInt("Enter medication ID: ");
    if (handleResultError(id, "CliApp:cmdUpdateMedicationWarnings"))
        return;

    auto found_medication = medicationRepo_.findMedicationById(id.value());

    if (handleResultError(found_medication, "CliApp:cmdUpdateMedicationWarnings"))
        return;

    auto old_warnings_medication = found_medication.value().warnings;
    auto new_warnings_medication =
        input::readOptionalString("New medication warnings (optional): ");

    auto user_confirm =
        input::confirm("Update medication warnings with ID " + std::to_string(id.value()));

    if (handleResultError(user_confirm, "CliApp:cmdUpdateMedicationWarnings"))
        return;

    if (!user_confirm.value()) {
        std::cout << "Medication with ID " << id.value() << " was not updated.\n";
        waitForEnter();
        return;
    }

    if (new_warnings_medication.has_value()) {
        auto updated_medication =
            medicationRepo_.updateMedicationWarnings(id.value(), new_warnings_medication.value());
        if (handleResultError(updated_medication, "CliApp:cmdUpdateMedicationWarnings"))
            return;
    } else {
        auto updated_medication = medicationRepo_.updateMedicationWarnings(id.value(), "");
        if (handleResultError(updated_medication, "CliApp:cmdUpdateMedicationWarnings"))
            return;
    }

    if (!new_warnings_medication.has_value()) {
        new_warnings_medication = "-";
    }
    if (old_warnings_medication.empty()) {
        old_warnings_medication = "-";
    }

    std::cout << "Medication " << id.value() << " updated.\n"
              << "Old warnings: " << old_warnings_medication << "\n"
              << "New warnings: " << new_warnings_medication.value() << "\n";
    waitForEnter();
}

// IntakePlans Commands

void CliApp::cmdCreateIntakePlan() {
    std::cout << "===== Create IntakePlan =====" << "\n\n";

    auto user_confirm = input::confirm("Print out all patients?");
    if (handleResultError(user_confirm, "CliApp::cmdCreateIntakePlan"))
        return;
    if (user_confirm.value()) {
        cmdListPatients();
    }
    auto patient_id = input::readInt("Enter patient ID: ");
    if (handleResultError(patient_id, "CliApp::cmdCreateIntakePlan"))
        return;
    auto found_patient = patientRepo_.findPatientById(patient_id.value());
    if (handleResultError(found_patient, "CliApp::cmdCreateIntakePlan"))
        return;

    user_confirm = input::confirm("Print out all medications?");
    if (handleResultError(user_confirm, "CliApp::cmdCreateIntakePlan"))
        return;
    if (user_confirm.value()) {
        cmdListMedications();
    }
    auto medication_id = input::readInt("Enter medication ID: ");
    if (handleResultError(medication_id, "CliApp::cmdCreateIntakePlan"))
        return;
    auto found_medication = medicationRepo_.findMedicationById(medication_id.value());
    if (handleResultError(found_medication, "CliApp::cmdCreateIntakePlan"))
        return;

    auto dose = input::readNonEmpty("Enter dose: ");
    if (handleResultError(dose, "CliApp::cmdCreateIntakePlan"))
        return;

    auto time_of_day = input::readTimeOfDay("Enter time of day (Morning, Noon, Evening, Night): ");
    if (handleResultError(time_of_day, "CliApp::cmdCreateIntakePlan"))
        return;

    auto notes = input::readOptionalString("Enter notes (optional): ");

    domain::IntakePlan intake_plan;

    intake_plan.id = 0;
    intake_plan.patient_id = patient_id.value();
    intake_plan.medication_id = medication_id.value();
    intake_plan.dose = dose.value();
    intake_plan.time_of_day = time_of_day.value();

    if (notes.has_value()) {
        intake_plan.notes = notes.value();
    }

    auto result = intakePlanRepo_.createIntakePlan(intake_plan);
    if (handleResultError(result, "CliApp::cmdCreateIntakePlan"))
        return;
    std::cout << "IntakePlan created successfully (ID: " << result.value().id << ").\n";
    waitForEnter();
}

void CliApp::cmdListIntakePlansByPatientId() {
    std::cout << "===== List IntakePlans By Patient ID =====" << "\n\n";

    auto user_confirm = input::confirm("Print out all patients?");
    if (handleResultError(user_confirm, "CliApp::cmdListIntakePlansByPatientId"))
        return;
    if (user_confirm.value()) {
        cmdListPatients();
    }

    auto patient_id = input::readInt("Enter patient ID: ");
    if (handleResultError(patient_id, "CliApp::cmdListIntakePlansByPatientId"))
        return;
    auto found_patient = patientRepo_.findPatientById(patient_id.value());
    if (handleResultError(found_patient, "CliApp::cmdListIntakePlansByPatientId"))
        return;

    auto intake_plans_patient = intakePlanRepo_.getIntakePlansByPatientId(patient_id.value());
    if (handleResultError(intake_plans_patient, "CliApp::cmdListIntakePlansByPatientId"))
        return;

    if (intake_plans_patient.value().empty()) {
        std::cout << "No IntakePlans found for PatientID " << patient_id.value();
        waitForEnter();
        return;
    }

    printer::printIntakePlansTable(intake_plans_patient.value());
    waitForEnter();
}

void CliApp::cmdListIntakePlansByMedicationId() {
    std::cout << "===== List IntakePlans By Medication ID =====" << "\n\n";

    auto user_confirm = input::confirm("Print out all medications?");
    if (handleResultError(user_confirm, "CliApp::cmdListIntakePlansByMedicationId"))
        return;
    if (user_confirm.value()) {
        cmdListMedications();
    }

    auto medication_id = input::readInt("Enter medication ID: ");
    if (handleResultError(medication_id, "CliApp::cmdListIntakePlansByMedicationId"))
        return;
    auto found_medication = medicationRepo_.findMedicationById(medication_id.value());
    if (handleResultError(found_medication, "CliApp::cmdListIntakePlansByMedicationId"))
        return;

    auto intake_plans_medication =
        intakePlanRepo_.getIntakePlansByMedicationId(medication_id.value());
    if (handleResultError(intake_plans_medication, "CliApp::cmdListIntakePlansByMedicationId"))
        return;

    if (intake_plans_medication.value().empty()) {
        std::cout << "No IntakePlans found for MedicationID " << medication_id.value();
        waitForEnter();
        return;
    }

    printer::printIntakePlansTable(intake_plans_medication.value());
    waitForEnter();
}

void CliApp::cmdDeleteIntakePlanById() {
    std::cout << "===== Delete IntakePlan By ID =====" << "\n\n";

    auto id = input::readInt("Enter IntakePlan ID: ");
    if (handleResultError(id, "CliApp::cmdDeleteIntakePlanById"))
        return;

    auto user_confirm = input::confirm("Delete IntakePlan with ID " + std::to_string(id.value()));

    if (handleResultError(user_confirm, "CliApp::cmdDeleteIntakePlanById"))
        return;

    if (!user_confirm.value()) {
        std::cout << "IntakePlan with ID: " << id.value() << " was not deleted.\n";
        waitForEnter();
        return;
    }

    auto deleted_intake_plan = intakePlanRepo_.deleteIntakePlanById(id.value());
    if (handleResultError(deleted_intake_plan, "CliApp::cmdDeleteIntakePlanById"))
        return;

    std::cout << "Deleted IntakePlan with ID: " << id.value() << ".\n";
    waitForEnter();
}

// Utility

void CliApp::waitForEnter() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << '\n';
}

} // namespace ui::cli
