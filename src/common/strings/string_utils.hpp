#pragma once
#include <string>
#include <string_view>

namespace common::strings {

std::string trim(std::string_view input);

std::string toUpper(std::string_view input);

std::string normalize(std::string_view input);
}