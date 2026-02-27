#pragma once
#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>

namespace common::validation {

// true if string is empty or only whitespaces
inline bool isEmptyOrBlank(std::string_view sv) {
    return std::all_of(sv.begin(), sv.end(), [](unsigned char c) { return std::isspace(c); });
}

inline bool isEmptyOrBlank(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](unsigned char c) { return std::isspace(c); });
}

} // namespace common::validation