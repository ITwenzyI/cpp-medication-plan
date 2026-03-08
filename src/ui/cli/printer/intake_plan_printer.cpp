#include "intake_plan_printer.hpp"
#include "infrastructure/persistence/sqlite/time_of_day_mapper_sqlite.hpp"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace {
std::string formatField(const std::string& field) {
    if (field.empty()) {
        return "-";
    }

    return field;
}

std::string intakePlanTimeOfDayToString(const domain::IntakePlan& intake_plan) {
    return infrastructure::persistence::sqlite::timeOfDayToDbString(intake_plan.time_of_day);
}

constexpr int title_width = 26;
constexpr int id_width = 8;
constexpr int patient_id_width = 15;
constexpr int medication_id_width = 18;
constexpr int dose_width = 14;
constexpr int time_of_day_width = 16;
constexpr int seperation_line = 66;
} // namespace

namespace ui::cli::printer {

void printIntakePlanTableHeader() {
    for (int i = 0; i < title_width; i++) {
        std::cout << "=";
    }
    std::cout << " IntakePlans ";
    for (int i = 0; i < title_width; i++) {
        std::cout << "=";
    }
    std::cout << "\n\n";
    std::cout << std::left << std::setw(id_width) << "ID" << std::setw(patient_id_width)
              << "PatientID" << std::setw(medication_id_width) << "MedicationID"
              << std::setw(dose_width) << "Dose" << std::setw(time_of_day_width) << "TimeOfDay"
              << "\n";
    for (int i = 0; i < seperation_line; i++) {
        std::cout << "-";
    }
    std::cout << "\n";
}

void printIntakePlanRow(const domain::IntakePlan& intake_plan) {
}

void printIntakePlansTable(const std::vector<domain::IntakePlan>& intake_plans) {
}

void printIntakePlanDetails(const domain::IntakePlan& intake_plan) {
    const std::string time_of_day = intakePlanTimeOfDayToString(intake_plan);

    std::cout << "ID: " << intake_plan.id << "\nPatientID: " << intake_plan.patient_id
              << "\nMedicationID: " << intake_plan.medication_id << "\nDose: " << intake_plan.dose
              << "\nTimeOfDay: " << formatField(time_of_day)
              << "\nNotes: " << formatField(intake_plan.notes) << "\n";
}

} // namespace ui::cli::printer