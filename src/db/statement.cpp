#include "statement.hpp"
#include <stdexcept>

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

int Statement::stepRaw() const {
    if (stmt_ == nullptr) {
        throw std::logic_error("Step fehlgeschlagen.");
    }
    return sqlite3_step(stmt_);
}

void Statement::reset() const {
    if (stmt_ == nullptr) {
        throw std::logic_error("Reset fehlgeschlagen.");
    }

    sqlite3_reset(stmt_);
}

void Statement::bindText(int index, const std::string& value){
    int rc = sqlite3_bind_text(stmt_, index, value.c_str(), -1, SQLITE_TRANSIENT);
    if(rc != SQLITE_OK) {
        throw std::runtime_error("Bind fehlgeschlagen.");
    }
}

int Statement::step(){
    if (!stmt_) {
        throw std::logic_error("Statement ist null.");
    }

    int rc = sqlite3_step(stmt_);
    if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
        return rc;
    }

    throw std::runtime_error("Step fehlgeschlagen.");
}