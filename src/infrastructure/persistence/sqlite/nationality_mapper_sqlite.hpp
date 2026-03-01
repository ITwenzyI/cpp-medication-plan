#pragma once
#include "common/result/result.hpp"
#include "domain/patient.hpp"
#include <stdexcept>
#include <string>

namespace infrastructure::persistence::sqlite {
inline std::string nationalityToDbString(domain::Nationality nationality) {
    switch (nationality) {
        case domain::Nationality::DE:
            return "DE";
        case domain::Nationality::US:
            return "US";
        case domain::Nationality::GB:
            return "GB";
        case domain::Nationality::FR:
            return "FR";
        case domain::Nationality::IT:
            return "IT";
        case domain::Nationality::ES:
            return "ES";
        case domain::Nationality::CN:
            return "CN";
        case domain::Nationality::IN:
            return "IN";
        case domain::Nationality::JP:
            return "JP";
        case domain::Nationality::TR:
            return "TR";
    }
    throw std::logic_error("Invalid Nationality enum value.");
}

inline common::result::Result<domain::Nationality> nationalityFromDbString(std::string_view value) {
    if (value == "DE") {
        return common::result::Result<domain::Nationality>::ok(domain::Nationality::DE);
    }
    if (value == "US") {
        return common::result::Result<domain::Nationality>::ok(domain::Nationality::US);
    }
    if (value == "GB") {
        return common::result::Result<domain::Nationality>::ok(domain::Nationality::GB);
    }
    if (value == "FR") {
        return common::result::Result<domain::Nationality>::ok(domain::Nationality::FR);
    }
    if (value == "IT") {
        return common::result::Result<domain::Nationality>::ok(domain::Nationality::IT);
    }
    if (value == "ES") {
        return common::result::Result<domain::Nationality>::ok(domain::Nationality::ES);
    }
    if (value == "CN") {
        return common::result::Result<domain::Nationality>::ok(domain::Nationality::CN);
    }
    if (value == "IN") {
        return common::result::Result<domain::Nationality>::ok(domain::Nationality::IN);
    }
    if (value == "JP") {
        return common::result::Result<domain::Nationality>::ok(domain::Nationality::JP);
    }
    if (value == "TR") {
        return common::result::Result<domain::Nationality>::ok(domain::Nationality::TR);
    }

    return common::result::Result<domain::Nationality>::fail(
        common::result::ErrorCode::InvalidArgument, "Invalid nationality code in database.");
}
} // namespace infrastructure::persistence::sqlite