#include "statement.hpp"

Statement::Statement(sqlite3_stmt* stmt)
    : stmt_(stmt) {
}

Statement::~Statement() {
    if (stmt_) {
        sqlite3_finalize(stmt_);
        stmt_ = nullptr;
    }
}

Statement::Statement(Statement&& other) noexcept
    : stmt_(other.stmt_) {
    other.stmt_ = nullptr;
}

Statement& Statement::operator=(Statement&& other) noexcept {
    if (this != &other) {
        if (stmt_) {
            sqlite3_finalize(stmt_);
        }
        stmt_ = other.stmt_;
        other.stmt_ = nullptr;
    }
    return *this;
}

[[nodiscard]] sqlite3_stmt* Statement::get() const {
    return stmt_;
}

int Statement::stepRaw() {
    if (stmt_ == nullptr) {
        throw std::logic_error("Step fehlgeschlagen.");
    }
    return sqlite3_step(stmt_);
}
