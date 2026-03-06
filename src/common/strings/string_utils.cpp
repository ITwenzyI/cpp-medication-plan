#include "string_utils.hpp"
#include <cctype>
#include <string>
#include <string_view>

namespace common::strings {

std::string trim(std::string_view input) {

    auto start = input.find_first_not_of(" \t\n\r\f\v");
    auto end = input.find_last_not_of(" \t\n\r\f\v");

    if (start == std::string_view::npos) {
        return "";
    }

    auto sub_size = (end - start) + 1;

    std::string_view sub = input.substr(start, sub_size);

    return std::string(sub);
}

std::string toUpper(std::string_view input) {
    std::string result;
    result.reserve(input.size());

    for (char c : input) {
        result.push_back(std::toupper(static_cast<unsigned char>(c)));
    }

    return result;
}

std::string normalize(std::string_view input) {

    auto trimmed = trim(input);
    auto normalized = toUpper(trimmed);

    return normalized;
}

} // namespace common::strings