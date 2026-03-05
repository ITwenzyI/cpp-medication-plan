#include "cli_app.hpp"
#include "common/result/result.hpp"
#include "error_renderer.hpp"
#include <iostream>
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
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
    }
}

void CliApp::showPatientsMenu() const {
    std::cout << "===== Patient Menu =====" << "\n";
    std::cout << "1. Create Patient" << "\n";
    std::cout << "2. List all Patients" << "\n";
    std::cout << "3. Find Patient by ID" << "\n";
    // std::cout << "4. Delete Patient by ID" << "\n";
    // std::cout << "5. Update Patient Name" << "\n";
    // std::cout << "6. Update Patient BirthDate" << "\n";
    // std::cout << "7. Update Patient Nationality" << "\n";
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
    // std::cout << "3. Find Medication by ID" << "\n";
    // std::cout << "4. Delete Medication by ID" << "\n";
    // std::cout << "5. Update Medication Name" << "\n";
    // std::cout << "6. Update Medication Strength" << "\n";
    // std::cout << "7. Update Medication Warnings" << "\n";
    std::cout << "0. Exit" << "\n";
}

common::result::Result<int> CliApp::readInt(std::string_view prompt, int min, int max) const {
    int choice;

    // If user input is not a int value
    if (!(std::cin >> choice)) {
        return common::result::Result<int>::fail(
            common::result::ErrorCode::InvalidArgument, "Invalid Choice", "CliApp::readInt");
    }

    // If user input is not correct for the menu
    if (choice < min || choice > max) {
        return common::result::Result<int>::fail(
            common::result::ErrorCode::InvalidArgument, "Invalid Choice", "CliApp::readInt");
    }
}

} // namespace ui::cli