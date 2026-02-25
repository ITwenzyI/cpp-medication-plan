#pragma once

#include "common/result/result.hpp"
#include <chrono>
#include <string>
#include <string_view>

namespace domain::value_objects {

class BirthDate {
  public:
    // Factory: erzeugt nur ein valides BirthDate
    static common::result::Result<BirthDate> fromString(std::string_view iso8601);

    // Wandelt das interne Datum in das ISO Format YYYY-MM-DD um
    [[nodiscard]] std::string toString() const;

    // Zugriff auf einzelne Komponenten
    [[nodiscard]] int year() const noexcept;
    [[nodiscard]] unsigned month() const noexcept;
    [[nodiscard]] unsigned day() const noexcept;

    // Vergleichsoperatoren (wichtig für Value Objects)
    friend bool operator==(const BirthDate&, const BirthDate&) = default;
    friend auto operator<=>(const BirthDate&, const BirthDate&) = default;

  private:
    explicit BirthDate(std::chrono::year_month_day ymd);

    std::chrono::year_month_day date_;
};

} // namespace domain::value_objects