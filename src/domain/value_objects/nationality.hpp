#pragma once
#include "common/result/result.hpp"
#include <string>
#include <string_view>

namespace domain::value_objects {

class Nationality {
  public:
    enum class Code {
        DE, // Deutschland
        US, // Vereinigte Staaten
        GB, // Vereinigtes Königreich
        FR, // Frankreich
        IT, // Italien
        ES, // Spanien
        CN, // China
        IN, // Indien
        JP, // Japan
        TR // Türkei
    };

    static constexpr Nationality fromCode(Code);
    static common::result::Result<Nationality> fromString(std::string_view);

    std::string toString() const;
    Code code() const noexcept;

  private:
    explicit Nationality(Code code);
    Code code_;
};

} // namespace domain::value_objects