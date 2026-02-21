#pragma once

#include "app_error.hpp"
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

namespace common::result {
template <typename T> class [[nodiscard]] Result {
  public:
    static Result<T> ok(T value) {
        return Result<T>(std::move(value));
    }

    static Result<T> fail(AppError error) {
        return Result<T>(std::move(error));
    }

    static Result<T> fail(ErrorCode code, std::string message, std::string context = "") {
        AppError error{code, std::move(message), std::move(context)};
        return fail(std::move(error));
    }

    bool isOk() const {
        return std::holds_alternative<T>(data_);
    }

    bool isError() const {
        return !isOk();
    }

    const T& value() const {
        if (isError()) {
            throw std::logic_error("Result::value() called on error");
        }
        return std::get<T>(data_);
    }

    T& value() {
        if (isError()) {
            throw std::logic_error("Result::value() called on error");
        }
        return std::get<T>(data_);
    }

    const AppError& error() const {
        if (isOk()) {
            throw std::logic_error("Result::error() called on ok");
        }
        return std::get<AppError>(data_);
    }

  private:
    std::variant<T, AppError> data_;

    Result(T value) : data_(std::move(value)) {
    }
    Result(AppError error) : data_(std::move(error)) {
    }
};

template <> class [[nodiscard]] Result<void> {
  public:
    static Result<void> ok();

    static Result<void> fail(AppError error);

    static Result<void> fail(ErrorCode code, std::string message, std::string context = "");

    bool isOk() const;

    bool isError() const;

    const AppError& error() const;

  private:
    std::variant<std::monostate, AppError> data_;

    Result() : data_(std::monostate{}) {
    }

    Result(AppError error) : data_(std::move(error)) {
    }
};

} // namespace common::result