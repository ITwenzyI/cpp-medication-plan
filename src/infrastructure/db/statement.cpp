#include "statement.hpp"
#include <stdexcept>
#include <string>

namespace infrastructure::db {
Statement::Statement(sqlite3_stmt* stmt) : stmt_(stmt) {
}

Statement::~Statement() {
    if (stmt_) {
        sqlite3_finalize(stmt_);
        stmt_ = nullptr;
    }
}

Statement::Statement(Statement&& other) noexcept : stmt_(other.stmt_) {
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

int Statement::step() {
    if (!stmt_) {
        throw std::logic_error("Statement ist null.");
    }

    int rc = sqlite3_step(stmt_);
    if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
        return rc;
    }

    throw std::runtime_error("Step fehlgeschlagen.");
}

void Statement::reset() {
    if (stmt_ == nullptr) {
        throw std::logic_error("Reset fehlgeschlagen.");
    }

    sqlite3_reset(stmt_);
}

void Statement::bindText(int index, std::string_view value) {
    if (!stmt_) {
        throw std::logic_error("Statement ist Null.");
    }

    int rc = sqlite3_bind_text(
        stmt_, index, value.data(), static_cast<int>(value.size()), SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Statement::bindText failed");
    }
}

void Statement::bindText(int index, const std::string& value) {
    bindText(index, std::string_view{value});
}

void Statement::bindNull(int index) {
    if (!stmt_) {
        throw std::logic_error("Statement ist Null.");
    }

    int rc = sqlite3_bind_null(stmt_, index);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Bind fehlgeschlagen.");
    }
}

void Statement::bindInt(int index, int value) {
    if (!stmt_) {
        throw std::logic_error("Statement ist Null.");
    }

    int rc = sqlite3_bind_int(stmt_, index, value);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Bind fehlgeschlagen.");
    }
}

void Statement::bindDouble(int index, double value) {
    if (!stmt_) {
        throw std::logic_error("Statement ist Null.");
    }

    int rc = sqlite3_bind_double(stmt_, index, value);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Bind fehlgeschlagen.");
    }
}

int Statement::getInt(int column) const {
    if (!stmt_) {
        throw std::logic_error("Statement ist Null.");
    }

    return sqlite3_column_int(stmt_, column);
}

double Statement::getDouble(int column) const {
    if (!stmt_) {
        throw std::logic_error("Statement ist Null.");
    }

    return sqlite3_column_double(stmt_, column);
}

std::string Statement::getText(int column) const {
    if (!stmt_) {
        throw std::logic_error("Statement ist Null.");
    }

    if (sqlite3_column_type(stmt_, column) == SQLITE_NULL) {
        return "";
    }

    const unsigned char* text = sqlite3_column_text(stmt_, column);
    if (text != nullptr) {
        return reinterpret_cast<const char*>(text);
    }

    return "";
}
} // namespace infrastructure::db
