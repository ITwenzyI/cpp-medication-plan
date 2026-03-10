#include "cli_app.hpp"
#include "common/result/result.hpp"
#include "error_renderer.hpp"
#include "input.hpp"
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
        auto user_choice = input::readMenuChoice(prompt, 0, 3);

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
                // case 5:
                //     cmdUpdatePatientName();
                //     break;
                // case 6:
                //     cmdUpdatePatientBirthDate();
                //     break;
                // case 7:
                //     cmdUpdatePatientNationality();
                //     break;
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
        auto user_choice = input::readMenuChoice(prompt, 0, 3);

        while (user_choice.isError()) {
            ErrorRenderer::printErrorMessage(user_choice.error(), "CliApp::medicationsMenuLoop");
            user_choice = input::readMenuChoice(prompt, 0, 7);
        }

        switch (user_choice.value()) {
            case 0:
                return;
            // case 1:
            //     cmdCreateMedication();
            //     break;
            // case 2:
            //     cmdListMedications();
            //     break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
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
        auto user_choice = ui::cli::input::readMenuChoice(prompt, 0, 3);

        while (user_choice.isError()) {
            ErrorRenderer::printErrorMessage(user_choice.error(), "CliApp::intakePlansMenuLoop");
            user_choice = input::readMenuChoice(prompt, 0, 5);
        }

        switch (user_choice.value()) {
            case 0:
                return;
            case 1:
                //     cmdCreateIntakePlan();
                //     break;
                // case 2:
                //     cmdListIntakePlansByPatientId();
                //     break;
                // case 3:
                //     cmdListIntakePlansByMedicationId();
                //     break;
                // case 4:
                //     cmdDeleteIntakePlanById();
                //     break;
                // case 5:
                //     cmdUpdateIntakePlan();
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
    std::cout << "5. Update IntakePlan" << "\n";
    std::cout << "0. Back" << "\n";
}

// Patient Commands

void CliApp::cmdCreatePatient() {
    std::cout << "===== Create Patient =====" << "\n\n";

    auto name = input::readNonEmpty("Enter the Name of the Patient: ");
    if (handleResultError(name, "CliApp::cmdCreatePatient"))
        return;

    auto birth_date = input::readOptionalBirthDate("Enter the BirthDate of the Patient: ");
    if (handleResultError(birth_date, "CliApp::cmdCreatePatient"))
        return;

    auto nationality = input::readOptionalNationality("Enter the Nationality of the Patient: ");
    if (handleResultError(nationality, "CliApp::cmdCreatePatient"))
        return;

    domain::Patient patient;

    patient.id = 0;
    patient.name = name.value();
    auto birthDateOpt = birth_date.value();
    if (birthDateOpt) {
        patient.birth_date = *birthDateOpt;
    }
    auto nationalityOpt = nationality.value();
    if (nationalityOpt) {
        patient.nationality = *nationalityOpt;
    }

    auto result = patientRepo_.createPatient(patient);
    if (handleResultError(result, "CliApp::cmdCreatePatient"))
        return;
    std::cout << "Patient created successfully (ID: " << std::to_string(result.value().id)
              << ").\n";
    waitForEnter();
}

void CliApp::cmdListPatients() {

    auto all_patients = patientRepo_.getAllPatients();

    if (handleResultError(all_patients, "CliApp::cmdListPatients"))
        return;

    if (all_patients.value().empty()) {
        std::cout << "No patients found.";
        return;
    }

    printPatientsTable(all_patients.value());
    waitForEnter();
}

void CliApp::cmdFindPatientById() {
    std::cout << "===== Find Patient By ID =====" << "\n\n";

    auto id = input::readInt("Enter the ID of the Patient: ");
    if (handleResultError(id, "CliApp::cmdFindPatientById"))
        return;

    auto found_patient = patientRepo_.findPatientById(id.value());

    if (handleResultError(found_patient, "CliApp::cmdFindPatientById"))
        return;

    printPatientDetails(found_patient.value());
    waitForEnter();
}

void CliApp::cmdDeletePatientById() {
    std::cout << "===== Delete Patient By ID =====" << "\n\n";

    auto id = input::readInt("Enter the ID of the Patient: ");
    if (handleResultError(id, "CliApp::cmdDeletePatientById"))
        return;

    auto found_patient = patientRepo_.findPatientById(id.value());

    if (handleResultError(found_patient, "CliApp::cmdDeletePatientById"))
        return;

    auto user_confirm = input::confirm("Delete patient with ID " + std::to_string(id.value()));

    if (handleResultError(user_confirm, "CliApp::cmdDeletePatientById"))
        return;

    if (!user_confirm.value()) {
        std::cout << "Patient with ID: " + std::to_string(id.value()) << " was not deleted.\n";
        waitForEnter();
        return;
    }

    auto deleted_patient = patientRepo_.deletePatientById(id.value());
    if (handleResultError(deleted_patient, "CliApp::cmdDeletePatientById"))
        return;

    std::cout << "Successfully deleted Patient with ID: " + std::to_string(id.value()) << ".\n";
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

// Utility

void CliApp::waitForEnter() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << '\n';
}

} // namespace ui::cli