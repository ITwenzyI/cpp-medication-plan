#include "cli_app.hpp"
#include "common/result/result.hpp"
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
        printError(user_choice.error());
        user_choice = readInt(prompt, 0, 3);
    }

    switch (user_choice.value()) {
        case 0:
            return;
        case 1:
            patientsMenuLoop();
        case 2:
            medicationsMenuLoop();
        case 3:
            intakePlansMenuLoop();
    }
}

void CliApp::showMainMenu() const {
    std::cout << "===== Medication Main Menu =====" << "\n";
    std::cout << "1. Patients Menu" << "\n";
    std::cout << "2. Medications Menu" << "\n";
    std::cout << "3. IntakePlans Menu" << "\n";
    std::cout << "0. Exit" << "\n";
}

common::result::Result<int> CliApp::readInt(std::string_view prompt, int min, int max) const {
    int choice;

    // If user input is not a int value
    if (!(std::cin >> choice)) {
        return common::result::Result<int>::fail(
            common::result::ErrorCode::InvalidArgument, "Invalid Choice", "CliApp::showMainMenu");
    }

    // If user input is not correct for the menu
    if (choice < min || choice > max) {
        return common::result::Result<int>::fail(
            common::result::ErrorCode::InvalidArgument, "Invalid Choice", "CliApp::showMainMenu");
    }
}

void CliApp::printError(const common::result::AppError& error) const {
}

} // namespace ui::cli