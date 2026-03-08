#pragma once
#include "domain/intake_plan.hpp"
#include <vector>

namespace ui::cli::printer {
void printIntakePlanTableHeader();

void printIntakePlanRow(const domain::IntakePlan& intake_plan);

void printIntakePlansTable(const std::vector<domain::IntakePlan>& intake_plans);

void printIntakePlanDetails(const domain::IntakePlan& intake_plan);
}