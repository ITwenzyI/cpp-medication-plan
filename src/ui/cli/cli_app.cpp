#include "cli_app.hpp"
#include "common/result/result.hpp"
#include "error_renderer.hpp"
#include <iostream>
#include <limits>
#include <string_view>

// CliApp ctor run()

//     mainMenuLoop() showMainMenu() patientsMenuLoop()...

//     cmdListPatients() cmdCreatePatient()...

//     readInt() waitForEnter()

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
    showMainMenu();

    std::string_view prompt = "Enter your Menu choice: ";
    auto user_choice = readInt(prompt, 0, 3);

    while (user_choice.isError()) {
        ErrorRenderer::printErrorMessage(user_choice.error(), "CliApp::mainMenuLoop");
        user_choice = readInt(prompt, 0, 3);
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

void CliApp::showMainMenu() const {
    std::cout << "===== Medication Main Menu =====" << "\n";
    std::cout << "1. Patients Menu" << "\n";
    std::cout << "2. Medications Menu" << "\n";
    std::cout << "3. IntakePlans Menu" << "\n";
    std::cout << "0. Exit" << "\n";
}

void CliApp::patientsMenuLoop() {
    showPatientsMenu();

    std::string_view prompt = "Choice: ";
    auto user_choice = readInt(prompt, 0, 3);

    while (user_choice.isError()) {
        ErrorRenderer::printErrorMessage(user_choice.error(), "CliApp::patientsMenuLoop");
        user_choice = readInt(prompt, 0, 7);
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

void CliApp::showPatientsMenu() const {
    std::cout << "===== Patient Menu =====" << "\n";
    std::cout << "1. Create Patient" << "\n";
    std::cout << "2. List all Patients" << "\n";
    std::cout << "3. Find Patient by ID" << "\n";
    std::cout << "4. Delete Patient by ID" << "\n";
    std::cout << "5. Update Patient Name" << "\n";
    std::cout << "6. Update Patient BirthDate" << "\n";
    std::cout << "7. Update Patient Nationality" << "\n";
    std::cout << "0. Exit" << "\n";
}

void CliApp::medicationsMenuLoop() {
    showMedicationsMenu();

    std::string_view prompt = "Choice: ";
    auto user_choice = readInt(prompt, 0, 3);

    while (user_choice.isError()) {
        ErrorRenderer::printErrorMessage(user_choice.error(), "CliApp::medicationsMenuLoop");
        user_choice = readInt(prompt, 0, 7);
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

void CliApp::showMedicationsMenu() const {
    std::cout << "===== Medication Menu =====" << "\n";
    std::cout << "1. Create Medication" << "\n";
    std::cout << "2. List all Medications" << "\n";
    std::cout << "3. Find Medication by ID" << "\n";
    std::cout << "4. Delete Medication by ID" << "\n";
    std::cout << "5. Update Medication Name" << "\n";
    std::cout << "6. Update Medication Strength" << "\n";
    std::cout << "7. Update Medication Warnings" << "\n";
    std::cout << "0. Exit" << "\n";
}

void CliApp::intakePlansMenuLoop() {
    showIntakePlansMenu();

    std::string_view prompt = "Choice: ";
    auto user_choice = readInt(prompt, 0, 3);

    while (user_choice.isError()) {
        ErrorRenderer::printErrorMessage(user_choice.error(), "CliApp::intakePlansMenuLoop");
        user_choice = readInt(prompt, 0, 5);
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
        case 5:
            cmdUpdateIntakePlan();
            break;
    }
}

void CliApp::showIntakePlansMenu() const {
    std::cout << "===== IntakePlan Menu =====" << "\n";
    std::cout << "1. Create IntakePlan" << "\n";
    std::cout << "2. List IntakePlans by PatientID" << "\n";
    std::cout << "3. List IntakePlans by MedicationID" << "\n";
    std::cout << "4. Delete IntakePlan by ID" << "\n";
    std::cout << "5. Update IntakePlan" << "\n";
    std::cout << "0. Exit" << "\n";
}

// Patient Commands

void CliApp::cmdCreatePatient() {
}

// Utility

void CliApp::waitForEnter() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << '\n';
}

common::result::Result<int> CliApp::readInt(std::string_view prompt, int min, int max) const {
    std::cout << prompt;

    int value;

    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return common::result::Result<int>::fail(common::result::ErrorCode::InvalidArgument,
            "Please enter a valid number", "CliApp::readInt");
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (value < min || value > max) {
        return common::result::Result<int>::fail(common::result::ErrorCode::InvalidArgument,
            "Choice is outside the allowed range", "CliApp::readInt");
    }

    return common::result::Result<int>::ok(value);
}

} // namespace ui::cli