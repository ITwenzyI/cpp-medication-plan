#pragma once
#include "common/result/result.hpp"
#include <string_view>

namespace ui::cli {

class ErrorRenderer {
  public:
    static std::string_view mapErrorCodeLabel(common::result::ErrorCode code);
    static void printErrorMessage(const common::result::AppError& error);

  private:
    static bool shouldPrintContext(common::result::ErrorCode code);
};

} // namespace ui::cli
