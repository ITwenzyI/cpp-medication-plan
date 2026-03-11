#pragma once
#include "common/validation/string_validation.hpp"
#include <cctype>
#include <charconv>
#include <string_view>

namespace common::validation {

inline bool isLeapYear(int year) {
    if (year % 400 == 0) {
        return true;
    }
    if (year % 100 == 0) {
        return false;
    }
    return year % 4 == 0;
}

// Format: YYYY-MM-DD
inline bool isValidBirthDate(std::string_view sv) {
    if (isEmptyOrBlank(sv)) {
        return false;
    }
    if (sv.size() != 10) {
        return false;
    }
    if (sv[4] != '-' || sv[7] != '-') {
        return false;
    }
    for (size_t i = 0; i < sv.size(); i++) {
        if (i == 4 || i == 7) {
            continue;
        }
        if (!std::isdigit(static_cast<unsigned char>(sv[i]))) {
            return false;
        }
    }

    auto year_sv = sv.substr(0, 4);
    auto month_sv = sv.substr(5, 2);
    auto day_sv = sv.substr(8, 2);
    int year = 0, month = 0, day = 0;

    auto res_year = std::from_chars(year_sv.data(), year_sv.data() + year_sv.size(), year);
    if (res_year.ec != std::errc{} || res_year.ptr != (year_sv.data() + year_sv.size())) {
        return false;
    }
    auto res_month = std::from_chars(month_sv.data(), month_sv.data() + month_sv.size(), month);
    if (res_month.ec != std::errc{} || res_month.ptr != (month_sv.data() + month_sv.size())) {
        return false;
    }
    auto res_day = std::from_chars(day_sv.data(), day_sv.data() + day_sv.size(), day);
    if (res_day.ec != std::errc{} || res_day.ptr != (day_sv.data() + day_sv.size())) {
        return false;
    }

    if (year < 1900 || year > 2027) {
        return false;
    }

    if (month < 1 || month > 12) {
        return false;
    }

    if (day < 1) {
        return false;
    }

    int max_day = 31;
    switch (month) {
        case 2:
            max_day = isLeapYear(year) ? 29 : 28;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            max_day = 30;
            break;
        default:
            break;
    }

    return day <= max_day;
}
} // namespace common::validation
